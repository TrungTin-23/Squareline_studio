#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <unistd.h>
#include "myapi.h"
#include <stdbool.h>

// Callback function to write the data received to a string

char *access_token = NULL;
char *refresh_token_str = NULL;


size_t WriteCallback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;
    strncat((char *)userdata, (char *)ptr, total_size);
    return total_size;
}

CURLcode ApiRequest(const char *url, const char *authorization_header, char *response, long *response_code) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();            
    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, authorization_header);
        headers = curl_slist_append(headers, "Accept: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

        memset(response, 0, 4096);  // Reset response buffer

        res = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, response_code);

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return res;
}


int CheckJsonResponse(const char *response) {
    cJSON *json = cJSON_Parse(response);
    if (json == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return 0;
    }
    cJSON_Delete(json);
    return 1;
}

char* AIot() {
   
    RefreshTokenApi(&access_token, &refresh_token_str);
    const char *url =  "https://dev-dboard.d-soft.tech/api/plugins/telemetry/DEVICE/271d0ac0-0c26-11ef-9d9c-a5b33d7c1216/values/timeseries";
    //const char *authorization_header = "X-Authorization: Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJzbW8yMDI0QGQtc29mdC5jb20udm4iLCJ1c2VySWQiOiJhNWQ2M2VjMC0zMzAxLTExZWYtYTMwMC01M2ZmMzFmNjEzOWMiLCJzY29wZXMiOlsiVEVOQU5UX0FETUlOIl0sInNlc3Npb25JZCI6IjBjNzBkYTEzLTgyZGYtNGRkNi05ODJlLWQ5MjIwY2M2ZDE2ZSIsImV4cCI6MTcxOTM5MzcwNiwiaXNzIjoidGhpbmdzYm9hcmQuaW8iLCJpYXQiOjE3MTkzODQ3MDYsImVuYWJsZWQiOnRydWUsImlzUHVibGljIjpmYWxzZSwidGVuYW50SWQiOiJhN2RhMzlkMC1lYmVjLTExZWUtOWQ5Yy1hNWIzM2Q3YzEyMTYiLCJjdXN0b21lcklkIjoiMTM4MTQwMDAtMWRkMi0xMWIyLTgwODAtODA4MDgwODA4MDgwIn0.TH0LAlCQvzgvvnQH3cbFwzJslwqR02bOqHcGAm9i-OS2ydRadcgbmAQTorVLiANV-oQzX-UGfW6R4cUsvCeDiA";
    char authorization_header[4096]; // Đảm bảo rằng mảng này đủ lớn
    snprintf(authorization_header, sizeof(authorization_header), "X-Authorization: Bearer %s", access_token); 
    static char response[4096];    //
    long response_code;

    CURLcode res = ApiRequest(url, authorization_header, response, &response_code);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return NULL;
    } else {
        if (CheckJsonResponse(response)) {
           // printf("JSON Response: %s\n", response);
            return response;
        }
    }
    return NULL;
}


int fetch_all_sensor_data(const char *response, SensorData *sensorData) {
    cJSON *json = cJSON_Parse(response); // Phân tích chuỗi JSON
    if (json == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr()); // Báo lỗi nếu không phân tích được JSON
        return 0;
    }

    // char *json_string = cJSON_Print(json); // Chuyển đối tượng JSON thành chuỗi
    // if (json_string != NULL) {
    //     printf("JSON Response: %s\n", json_string); // In chuỗi JSON ra terminal
    //     free(json_string); // Giải phóng bộ nhớ cho chuỗi JSON
    // }

    // Lấy các giá trị từ JSON và cập nhật vào SensorData
    cJSON *temperature = cJSON_GetObjectItemCaseSensitive(json, "Temperature");
    if (temperature != NULL && cJSON_IsArray(temperature)) {
        cJSON *temperatureItem = cJSON_GetArrayItem(temperature, 0);
        if (temperatureItem != NULL) {
            cJSON *valueItem = cJSON_GetObjectItemCaseSensitive(temperatureItem, "value");
            if (valueItem != NULL && cJSON_IsString(valueItem)) {
                sensorData->temperature = atof(valueItem->valuestring);
                //printf("Temperature: %.2f\n", sensorData->temperature);
            }
        }
    }

    cJSON *humidity = cJSON_GetObjectItemCaseSensitive(json, "Humidity");
    if (humidity != NULL && cJSON_IsArray(humidity)) {
        cJSON *humidityItem = cJSON_GetArrayItem(humidity, 0);
        if (humidityItem != NULL) {
            cJSON *valueItem = cJSON_GetObjectItemCaseSensitive(humidityItem, "value");
            if (valueItem != NULL && cJSON_IsString(valueItem)) {
                sensorData->humidity = atof(valueItem->valuestring);
                //printf("Humidity: %.2f\n", sensorData->humidity);
            }
        }
    }

   
    cJSON *co2 = cJSON_GetObjectItemCaseSensitive(json, "CO2");
    if (co2 != NULL && cJSON_IsArray(co2)) {
        cJSON *co2Item = cJSON_GetArrayItem(co2, 0);
        if (co2Item != NULL) {
            cJSON *valueItem = cJSON_GetObjectItemCaseSensitive(co2Item, "value");
            if (valueItem != NULL && cJSON_IsString(valueItem)) {
                sensorData->co2 = atof(valueItem->valuestring);
                //printf("CO2: %.2f\n", sensorData->co2);
            }
        }
    }

    cJSON *co = cJSON_GetObjectItemCaseSensitive(json, "CO");
    if (co != NULL && cJSON_IsArray(co)) {
        cJSON *coItem = cJSON_GetArrayItem(co, 0);
        if (coItem != NULL) {
            cJSON *valueItem = cJSON_GetObjectItemCaseSensitive(coItem, "value");
            if (valueItem != NULL && cJSON_IsString(valueItem)) {
                sensorData->co = atof(valueItem->valuestring);
                //printf("CO: %.2f\n", sensorData->co);
            }
        }
    }

    cJSON *tvocs = cJSON_GetObjectItemCaseSensitive(json, "TVOCs");
    if (tvocs != NULL && cJSON_IsArray(tvocs)) {
        cJSON *tvocsItem = cJSON_GetArrayItem(tvocs, 0);
        if (tvocsItem != NULL) {
            cJSON *valueItem = cJSON_GetObjectItemCaseSensitive(tvocsItem, "value");
            if (valueItem != NULL && cJSON_IsString(valueItem)) {
                sensorData->tvocs = atof(valueItem->valuestring);
                //printf("TVOCs: %.2f\n", sensorData->tvocs);
            }
        }
    }

    cJSON *ch4 = cJSON_GetObjectItemCaseSensitive(json, "CH4");
    if (ch4 != NULL && cJSON_IsArray(ch4)) {
        cJSON *ch4Item = cJSON_GetArrayItem(ch4, 0);
        if (ch4Item != NULL) {
            cJSON *valueItem = cJSON_GetObjectItemCaseSensitive(ch4Item, "value");
            if (valueItem != NULL && cJSON_IsString(valueItem)) {
                sensorData->ch4 = atof(valueItem->valuestring);
                //printf("CH4: %.2f\n", sensorData->ch4);
            }
        }
    }
cJSON *nh3 = cJSON_GetObjectItemCaseSensitive(json, "NH3");
    if (nh3 != NULL && cJSON_IsArray(nh3)) {
        cJSON *nh3Item = cJSON_GetArrayItem(nh3, 0);
        if (nh3Item != NULL) {
            cJSON *valueItem = cJSON_GetObjectItemCaseSensitive(nh3Item, "value");
            if (valueItem != NULL && cJSON_IsString(valueItem)) {
                sensorData->nh3 = atof(valueItem->valuestring);
                //printf("NH3: %.2f\n", sensorData->nh3);
            }
        }
    }

    cJSON *pm25 = cJSON_GetObjectItemCaseSensitive(json, "PM2.5");
    if (pm25 != NULL && cJSON_IsArray(pm25)) {
        cJSON *pm25Item = cJSON_GetArrayItem(pm25, 0);
        if (pm25Item != NULL) {
            cJSON *valueItem = cJSON_GetObjectItemCaseSensitive(pm25Item, "value");
            if (valueItem != NULL && cJSON_IsString(valueItem)) {
                sensorData->pm25 = atof(valueItem->valuestring);
                //printf("PM2.5: %.2f\n", sensorData->pm25);
            }
        }
    }

    cJSON *pm10 = cJSON_GetObjectItemCaseSensitive(json, "PM10");
    if (pm10 != NULL && cJSON_IsArray(pm10)) {
        cJSON *pm10Item = cJSON_GetArrayItem(pm10, 0);
        if (pm10Item != NULL) {
            cJSON *valueItem = cJSON_GetObjectItemCaseSensitive(pm10Item, "value");
            if (valueItem != NULL && cJSON_IsString(valueItem)) {
                sensorData->pm10 = atof(valueItem->valuestring);
                //printf("PM10: %.2f\n", sensorData->pm10);
            }
        }
    }

    

 


    cJSON *iaq = cJSON_GetObjectItemCaseSensitive(json, "IAQ");
    if (iaq != NULL && cJSON_IsArray(iaq)) {
        cJSON *iaqItem = cJSON_GetArrayItem(iaq, 0);
        if (iaqItem != NULL) {
            cJSON *valueItem = cJSON_GetObjectItemCaseSensitive(iaqItem, "value");
            if (valueItem != NULL && cJSON_IsString(valueItem)) {
                sensorData->IAQ = atof(valueItem->valuestring);
                //printf("IAQ: %.2f\n", sensorData->IAQ);
                 printf("4");
            }
        }
    }

    // cJSON *status = cJSON_GetObjectItemCaseSensitive(json, "Status");
    // if (status != NULL && cJSON_IsArray(status)) {
    //     cJSON *statusItem = cJSON_GetArrayItem(status, 0);
    //     if (statusItem != NULL) {
    //         cJSON *valueItem = cJSON_GetObjectItemCaseSensitive(statusItem, "value");
    //         if (valueItem != NULL && cJSON_IsString(valueItem)) {
    //             strncpy(sensorData->status, valueItem->valuestring, sizeof(sensorData->status) - 1);
    //             sensorData->status[sizeof(sensorData->status) - 1] = '\0'; // Đảm bảo chuỗi được kết thúc đúng
    //             printf("Status: %s\n", sensorData->status);
    //         }
    //     }
    // }
        //printf("H");
        
    cJSON_Delete(json); // Giải phóng bộ nhớ JSON
    return 1; // Trả về số lượng dữ liệu trích xuất thành công
}

char* Electronic() {
    
    RefreshTokenApi(&access_token, &refresh_token_str);
    const char *url = "https://dev-dboard.d-soft.tech/api/plugins/telemetry/DEVICE/4e1a2900-2de6-11ef-a300-53ff31f6139c/values/attributes/CLIENT_SCOPE";
    char authorization_header[4096]; // Đảm bảo rằng mảng này đủ lớn
    snprintf(authorization_header, sizeof(authorization_header), "X-Authorization: Bearer %s", access_token); 
    static char response_electronic[4096];
    long response_code_electronic;

    CURLcode res = ApiRequest(url, authorization_header, response_electronic, &response_code_electronic);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return NULL;
    } else {
        if (CheckJsonResponse(response_electronic)) {
            //printf("JSON Response: %s\n", response_electronic);
            return response_electronic;
            
        }
    }
    return NULL;
}

char* Electronic_shared() {
    
    RefreshTokenApi(&access_token, &refresh_token_str);
    const char *url = "https://dev-dboard.d-soft.tech/api/plugins/telemetry/DEVICE/4e1a2900-2de6-11ef-a300-53ff31f6139c/values/attributes/SHARED_SCOPE";
    char authorization_header[4096]; // Đảm bảo rằng mảng này đủ lớn
    snprintf(authorization_header, sizeof(authorization_header), "X-Authorization: Bearer %s", access_token);  
    static char response_electronic_shared[4096];
    long response_code_electronic;

    CURLcode res = ApiRequest(url, authorization_header, response_electronic_shared, &response_code_electronic);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return NULL;
    } else {
        if (CheckJsonResponse(response_electronic_shared)) {
            //printf("JSON Response: %s\n", response_electronic_shared);
            return response_electronic_shared;
        }
    }
    return NULL;
}

int fetch_lights_shared(const char *response_electronic_shared, bool *L1, bool *L2) {
    cJSON *json = cJSON_Parse(response_electronic_shared);
    if (json == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return 0;
    }

    cJSON *item = NULL;
    cJSON_ArrayForEach(item, json) {
        cJSON *key = cJSON_GetObjectItemCaseSensitive(item, "key");
        cJSON *value = cJSON_GetObjectItemCaseSensitive(item, "value");

        if (cJSON_IsString(key) && (key->valuestring != NULL)) {
            if (strcmp(key->valuestring, "ss_L1") == 0 && cJSON_IsBool(value)) {
                *L1 = cJSON_IsTrue(value);
                 //printf("\ntrang thai L1: %d",*L1);
            } else if (strcmp(key->valuestring, "ss_L2") == 0 && cJSON_IsBool(value)) {
                *L2 = cJSON_IsTrue(value);
                 //printf("\ntrang thai L2: %d",*L2);
            }
        }
    }

    cJSON_Delete(json);
    return 0;
}


int fetch_lights(const char *response_electronic, bool *L1, bool *L2) {
    cJSON *json = cJSON_Parse(response_electronic);
    if (json == NULL) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return 0;
    }

    cJSON *item = NULL;
    cJSON_ArrayForEach(item, json) {
        cJSON *key = cJSON_GetObjectItemCaseSensitive(item, "key");
        cJSON *value = cJSON_GetObjectItemCaseSensitive(item, "value");

        if (cJSON_IsString(key) && (key->valuestring != NULL)) {
            if (strcmp(key->valuestring, "L1") == 0 && cJSON_IsBool(value)) {
                *L1 = cJSON_IsTrue(value);
                 printf("\ntrang thai L1: %d",*L1);
            } else if (strcmp(key->valuestring, "L2") == 0 && cJSON_IsBool(value)) {
                *L2 = cJSON_IsTrue(value);
                 printf("\ntrang thai L2: %d",*L2);
            }
        }
    }

    cJSON_Delete(json);
    return 0;
}

// Function to send a POST request with JSON data
CURLcode send_post_request(const char *url, const char *authorization_header, const char *post_data, char *response, long *response_code) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Accept: */*");
        headers = curl_slist_append(headers, authorization_header);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

        memset(response, 0, 4096); // Reset response buffer

        res = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, response_code);

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return res;
}

// You can call this function like this:
void SendButton1On() {
    RefreshTokenApi(&access_token, &refresh_token_str);
    char authorization_header[4096]; 
    const char *url = "https://dev-dboard.d-soft.tech/api/plugins/telemetry/DEVICE/4e1a2900-2de6-11ef-a300-53ff31f6139c/attributes/SHARED_SCOPE";
    snprintf(authorization_header, sizeof(authorization_header), "X-Authorization: Bearer %s", access_token); 
    // const char *authorization_header = "X-Authorization: Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJpbnRlcm50cmFubmluZ0BkLXNvZnQuY29tLnZuIiwic2NvcGVzIjpbIlRFTkFOVF9BRE1JTiJdLCJ1c2VySWQiOiJjZGM2N2E1MC1lYmVjLTExZWUtOWQ5Yy1hNWIzM2Q3YzEyMTYiLCJlbmFibGVkIjp0cnVlLCJpc1B1YmxpYyI6ZmFsc2UsInRlbmFudElkIjoiYTdkYTM5ZDAtZWJlYy0xMWVlLTlkOWMtYTViMzNkN2MxMjE2IiwiY3VzdG9tZXJJZCI6IjEzODE0MDAwLTFkZDItMTFiMi04MDgwLTgwODA4MDgwODA4MCIsImlzcyI6InRoaW5nc2JvYXJkLmlvIiwiaWF0IjoxNzE3Mzc4NjQ2LCJleHAiOjE3MTczODc2NDZ9.JQQdytz0b3L4Ddf93LEzfUVmnXUH4FIibTaZIXXB0ivkBt_f2A4ZfpYnBMszkx9J_jWGu9XMh1JDo1JFo_QMpg";
    const char *post_data = "{ \"ss_L1\": true }";

    char response[4096];
    long response_code;

    CURLcode res = send_post_request(url, authorization_header, post_data, response, &response_code);

    // if (res != CURLE_OK) {
    //     fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    // } else {
    //     printf("POST request sent. Response Code: %ld\n", response_code);
    //     printf("Response: %s\n", response);
    // }
}


void SendButton1Off() {
    RefreshTokenApi(&access_token, &refresh_token_str);
    char authorization_header[4096]; 
    const char *url = "https://dev-dboard.d-soft.tech/api/plugins/telemetry/DEVICE/4e1a2900-2de6-11ef-a300-53ff31f6139c/attributes/SHARED_SCOPE";
    snprintf(authorization_header, sizeof(authorization_header), "X-Authorization: Bearer %s", access_token); 
    // const char *authorization_header = "X-Authorization: Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJpbnRlcm50cmFubmluZ0BkLXNvZnQuY29tLnZuIiwic2NvcGVzIjpbIlRFTkFOVF9BRE1JTiJdLCJ1c2VySWQiOiJjZGM2N2E1MC1lYmVjLTExZWUtOWQ5Yy1hNWIzM2Q3YzEyMTYiLCJlbmFibGVkIjp0cnVlLCJpc1B1YmxpYyI6ZmFsc2UsInRlbmFudElkIjoiYTdkYTM5ZDAtZWJlYy0xMWVlLTlkOWMtYTViMzNkN2MxMjE2IiwiY3VzdG9tZXJJZCI6IjEzODE0MDAwLTFkZDItMTFiMi04MDgwLTgwODA4MDgwODA4MCIsImlzcyI6InRoaW5nc2JvYXJkLmlvIiwiaWF0IjoxNzE3Mzc4NjQ2LCJleHAiOjE3MTczODc2NDZ9.JQQdytz0b3L4Ddf93LEzfUVmnXUH4FIibTaZIXXB0ivkBt_f2A4ZfpYnBMszkx9J_jWGu9XMh1JDo1JFo_QMpg";
    const char *post_data = "{ \"ss_L1\": false }";

    char response[4096];
    long response_code;

    CURLcode res = send_post_request(url, authorization_header, post_data, response, &response_code);

    // if (res != CURLE_OK) {
    //     fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    // } else {
    //     printf("POST request sent. Response Code: %ld\n", response_code);
    //     printf("Response: %s\n", response);
    // }
}

void SendButton2On() {
    RefreshTokenApi(&access_token, &refresh_token_str);
    char authorization_header[4096]; 
    const char *url = "https://dev-dboard.d-soft.tech/api/plugins/telemetry/DEVICE/4e1a2900-2de6-11ef-a300-53ff31f6139c/attributes/SHARED_SCOPE";
    snprintf(authorization_header, sizeof(authorization_header), "X-Authorization: Bearer %s", access_token); 
    // const char *authorization_header = "X-Authorization: Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJpbnRlcm50cmFubmluZ0BkLXNvZnQuY29tLnZuIiwic2NvcGVzIjpbIlRFTkFOVF9BRE1JTiJdLCJ1c2VySWQiOiJjZGM2N2E1MC1lYmVjLTExZWUtOWQ5Yy1hNWIzM2Q3YzEyMTYiLCJlbmFibGVkIjp0cnVlLCJpc1B1YmxpYyI6ZmFsc2UsInRlbmFudElkIjoiYTdkYTM5ZDAtZWJlYy0xMWVlLTlkOWMtYTViMzNkN2MxMjE2IiwiY3VzdG9tZXJJZCI6IjEzODE0MDAwLTFkZDItMTFiMi04MDgwLTgwODA4MDgwODA4MCIsImlzcyI6InRoaW5nc2JvYXJkLmlvIiwiaWF0IjoxNzE3Mzc4NjQ2LCJleHAiOjE3MTczODc2NDZ9.JQQdytz0b3L4Ddf93LEzfUVmnXUH4FIibTaZIXXB0ivkBt_f2A4ZfpYnBMszkx9J_jWGu9XMh1JDo1JFo_QMpg";
    const char *post_data = "{ \"ss_L2\": true }";

    char response[4096];
    long response_code;

    CURLcode res = send_post_request(url, authorization_header, post_data, response, &response_code);

    // if (res != CURLE_OK) {
    //     fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    // } else {
    //     printf("POST request sent. Response Code: %ld\n", response_code);
    //     printf("Response: %s\n", response);
    // }
}


void SendButton2Off() {
    RefreshTokenApi(&access_token, &refresh_token_str);
    char authorization_header[4096]; 
    const char *url = "https://dev-dboard.d-soft.tech/api/plugins/telemetry/DEVICE/4e1a2900-2de6-11ef-a300-53ff31f6139c/attributes/SHARED_SCOPE";
    snprintf(authorization_header, sizeof(authorization_header), "X-Authorization: Bearer %s", access_token); 
    // const char *authorization_header = "X-Authorization: Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJpbnRlcm50cmFubmluZ0BkLXNvZnQuY29tLnZuIiwic2NvcGVzIjpbIlRFTkFOVF9BRE1JTiJdLCJ1c2VySWQiOiJjZGM2N2E1MC1lYmVjLTExZWUtOWQ5Yy1hNWIzM2Q3YzEyMTYiLCJlbmFibGVkIjp0cnVlLCJpc1B1YmxpYyI6ZmFsc2UsInRlbmFudElkIjoiYTdkYTM5ZDAtZWJlYy0xMWVlLTlkOWMtYTViMzNkN2MxMjE2IiwiY3VzdG9tZXJJZCI6IjEzODE0MDAwLTFkZDItMTFiMi04MDgwLTgwODA4MDgwODA4MCIsImlzcyI6InRoaW5nc2JvYXJkLmlvIiwiaWF0IjoxNzE3Mzc4NjQ2LCJleHAiOjE3MTczODc2NDZ9.JQQdytz0b3L4Ddf93LEzfUVmnXUH4FIibTaZIXXB0ivkBt_f2A4ZfpYnBMszkx9J_jWGu9XMh1JDo1JFo_QMpg";
    const char *post_data = "{ \"ss_L2\": false }";

    char response[4096];
    long response_code;

    CURLcode res = send_post_request(url, authorization_header, post_data, response, &response_code);

    // if (res != CURLE_OK) {
    //     fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    // } else {
    //     printf("POST request sent. Response Code: %ld\n", response_code);
    //     printf("Response: %s\n", response);
    // }
}



