#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <unistd.h>  // For sleep function
#include <jwt.h>
#include <time.h>


// Định nghĩa struct MemoryStruct
// char *access_token = NULL;
// char *refresh_token_str = NULL;
struct MemoryStruct {
    char *memory;
    size_t size;
};

size_t WriteCallbackRefreshtoken(void *ptr, size_t size, size_t nmemb, struct MemoryStruct *mem) {
        size_t real_size = size * nmemb;
        char *ptr_reallocated = realloc(mem->memory, mem->size + real_size + 1);
        if (ptr_reallocated == NULL) {
            fprintf(stderr, "Failed to reallocate memory.\n");
            return 0;
        }
        mem->memory = ptr_reallocated;
        memcpy(&(mem->memory[mem->size]), ptr, real_size);
        mem->size += real_size;
        mem->memory[mem->size] = 0;
        return real_size;
    }

time_t GetTimeExp(const char *token) {
        jwt_t *jwt = NULL;
        int ret = 0;
        time_t exp = -1;

        // Giải mã token JWT
        ret = jwt_decode(&jwt, token, NULL, 0);
        if (ret != 0) {
            fprintf(stderr, "Error decoding JWT: %d\n", ret);
            return -1;
        }

        // Lấy giá trị thời gian hết hạn
        exp = jwt_get_grant_int(jwt, "exp");
        if (exp == -1) {
            fprintf(stderr, "Error getting exp\n");
        }

        // Giải phóng JWT
        jwt_free(jwt);

        return exp;
    }

void RefreshTokenApi(char **access_token, char **refresh_token_str) {
    // Hàm lấy thời gian hết hạn của token


    // Free old tokens if they exist
    if (*access_token) {
        free(*access_token);
        *access_token = NULL;
    }
    if (*refresh_token_str) {
        free(*refresh_token_str);
        *refresh_token_str = NULL;
    }

    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    char *url = "https://dev-dboard.d-soft.tech/api/auth/login";
    char *data = "{\"username\": \"smo2024@d-soft.com.vn\", \"password\": \"Dsoft#1234\"}";

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallbackRefreshtoken);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            cJSON *json_response = cJSON_Parse(chunk.memory);
            if (json_response != NULL) {
                cJSON *token = cJSON_GetObjectItemCaseSensitive(json_response, "token");
                cJSON *refresh_token = cJSON_GetObjectItemCaseSensitive(json_response, "refreshToken");
                if (cJSON_IsString(token) && (token->valuestring != NULL)) {
                    *access_token = strdup(token->valuestring);
                }
                if (cJSON_IsString(refresh_token) && (refresh_token->valuestring != NULL)) {
                    *refresh_token_str = strdup(refresh_token->valuestring);
                }
                cJSON_Delete(json_response);
            }
        } else {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        free(chunk.memory);
    }

    // Kiểm tra thời gian hết hạn của token
    if (*access_token != NULL) {
        time_t exp = GetTimeExp(*access_token);

        // Kiểm tra và in giá trị thời gian hết hạn
        if (exp != -1) {
            //printf("Token expiration time: %ld\n", exp);

            time_t now = time(NULL);
            if (difftime(exp, now) <= 0) {
                printf("Token has expired. Refreshing token...\n");
                RefreshTokenApi(access_token, refresh_token_str);  // Gọi lại hàm refresh_token_api nếu token đã hết hạn
            }
        } else {
            fprintf(stderr, "Failed to get token expiration time\n");
        }
    } else {
        fprintf(stderr, "Access token is NULL\n");
    }
}


