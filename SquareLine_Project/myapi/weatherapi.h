// #ifndef RANDOM_H
// #define RANDOM_H

// // Function to get a random value between 1 and 100
// int random_value();

// #endif // RANDOM_H

#ifndef WEATHER_API_H
#define WEATHER_API_H

#include <curl/curl.h>

// Function to make API request and return response
CURLcode ApiRequestWeather(const char *url, char *response, long *response_code);

void GetDataWeather(float *temperature, float *humidity, char *description);

#endif // WEATHER_API_H

