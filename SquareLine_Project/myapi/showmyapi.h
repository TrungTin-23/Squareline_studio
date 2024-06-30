// sensor_data.h

#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <lvgl/lvgl.h>
#include "ui/ui.h"
#include "myapi.h"
#include "weatherapi.h"
#include "control_button.h"
// Declare LVGL objects
// extern lv_obj_t * ui_ValueHum;
// extern lv_obj_t * ui_ValueTem;
// extern lv_obj_t * ui_TemAPI;

// Declare sensor data variables
extern float hum;
extern float tem;
extern float temw;
extern float humw;
extern char desw[256];
// extern bool L1 ;
// extern bool L2 ;
extern bool button1;
extern bool button2;

// Function prototypes
// int fetch_humidity(int *humidity);
// int fetch_temperature(int *temperature);
// void GetDataWeather(float *temperature, float *humidity, char *description);
int getCurrentSec(int*tg); 
int GetHourNow(int*hour);
void update_sensor_data_task(lv_timer_t * timer);
void update_button1(lv_timer_t * timer);
void update_button2(lv_timer_t * timer);
void update_iaq(lv_timer_t * timer);
void update_weather(lv_timer_t * timer);
void update_icon_weather();
void send_on_off(lv_timer_t * timer);
void update_data_chart(lv_timer_t * timer);

#endif // SENSOR_DATA_H
