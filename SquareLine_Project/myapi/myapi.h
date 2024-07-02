// #ifndef RANDOM_H
// #define RANDOM_H

// // Function to get a random value between 1 and 100
// int random_value();

// #endif // RANDOM_H

#ifndef MY_API_H
#define MY_API_H

#include <curl/curl.h>
#include "refresh_token.h"
#include <stdbool.h>

typedef struct {
    float temperature;
    float humidity;
    float sound;
    float co2;
    float co;
    float tvocs;
    float pm25;
    float pm10;
    float nh3;
    float ch4;
    float IAQ;
    //char status[10];
} SensorData;
size_t WriteCallback(void *ptr, size_t size, size_t nmemb, void *userdata);
// Function to make API request and return response
CURLcode ApiRequest(const char *url, const char *authorization_header, char *response, long *response_code);

// Function to check JSON response
int CheckJsonResponse(const char *response);
int fetch_lights(const char *response_electronic, bool *L1, bool *L2);
// Function to fetch and return humidity value
int fetch_all_sensor_data(const char *response, SensorData *sensorData);
// Trong myapi.c hoặc một file header được include bởi myapi.c
void RefreshTokenApi(char **access_token, char **refresh_token_str);
int fetch_lights_shared(const char *response_electronic_shared, bool *L1, bool *L2);

char* AIot();
char* Electronic();
char* Electronic_shared(); 
void SendButton1On();
void SendButton1Off();
void SendButton2On();
void SendButton2Off();
#endif // MY_API_H



