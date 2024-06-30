

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <time.h>  // Thêm thư viện time.h để sử dụng time() và srand()
#include "weatherapi.h"

// Callback function to write the data received to a string
size_t write_callback_weather(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;
    strncat((char *)userdata, (char *)ptr, total_size);
    return total_size;
}

CURLcode ApiRequestWeather(const char *url, char *response, long *response_code) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_weather);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

        memset(response, 0, 4096);  // Reset response buffer

        res = curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, response_code);

        curl_easy_cleanup(curl);
    }
    return res;
}

void GetDataWeather(float *temperature, float *humidity, char *description) {
    // Khởi tạo bộ sinh số ngẫu nhiên
    srand(time(NULL));

    // Random giữa hai giá trị chuỗi "Hanoi" và "Danang"
    const char *city_name = "Danang";
    // int random_value = rand() % 3;
    // if (random_value == 0) {
    //     city_name = "Hanoi";
    // } else if (random_value == 1) {
    //     city_name = "Danang";
    // } else {                
    //     city_name = "Dubai";
    // }

    const char *api_key = "bef36c479a90e4b5db397e502d54a360";

    char url[256];
    snprintf(url, sizeof(url), "http://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s", city_name, api_key);

    char response[4096];
    long response_code;

    CURLcode res = ApiRequestWeather(url, response, &response_code);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return;
    } else {
        cJSON *json = cJSON_Parse(response);
        if (json == NULL) {
            fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
            return;
        }

        cJSON *main = cJSON_GetObjectItemCaseSensitive(json, "main");
        cJSON *weather = cJSON_GetObjectItemCaseSensitive(json, "weather");

        if (cJSON_IsObject(main) && cJSON_IsArray(weather)) {
            cJSON *item = cJSON_GetArrayItem(weather, 0); // Take the first item in the "weather" array
            cJSON *main_weather = cJSON_GetObjectItemCaseSensitive(item, "main");

            cJSON *temp = cJSON_GetObjectItemCaseSensitive(main, "temp");
            cJSON *hum = cJSON_GetObjectItemCaseSensitive(main, "humidity");

            if (cJSON_IsNumber(temp) && cJSON_IsNumber(hum) && cJSON_IsString(main_weather)) {
                *temperature = temp->valuedouble - 273.15; // Convert from Kelvin to Celsius
                *humidity = hum->valuedouble;
                strcpy(description, main_weather->valuestring);
                cJSON_Delete(json);

                return; // Successfully retrieved temperature, humidity, and main weather
            }
        }
        cJSON_Delete(json);
    }
    fprintf(stderr, "Failed to get weather data\n");
    fprintf(stderr, "%s\n", response);
}
