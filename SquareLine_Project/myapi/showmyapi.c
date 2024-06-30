#include <unistd.h>
#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <unistd.h>
#include "lvgl/lvgl.h"
#include "ui/ui.h"
#include "showmyapi.h"
#include <time.h>
#include "control_button.h"
#include <unistd.h>

#include <stdbool.h> 


lv_coord_t ui_ChartDust_series_1_array[] = { 0, 10, 20, 40, 80, 80 };

lv_coord_t ui_ChartNoise_series_1_array[] = { 0, 10, 20, 40, 80, 80 };


float  hum = 0;
float tem = 0;
float co2 = 0;
float ch4 = 0;
float tovcs = 0;
float co = 0;
float dust = 0;
float sound = 0;
float energyy = 0;
int aiq=0;
bool L1 = false;
bool L2 = false;
bool ssL1 = false;
bool ssL2 = false;
float  temw =0,humw=0;
char desw[256] = {0};
int indextime=0;
int hour = 0;
int getCurrentSec(int*tg) { 
    time_t now;
    struct tm *tm_now;
    time(&now);
    tm_now = localtime(&now);  
    return *tg = tm_now->tm_sec+1;
}

int GetHourNow(int *hour) { 
    time_t now;
    struct tm *tm_now;
    time(&now);
    tm_now = localtime(&now);  
    return *hour = tm_now->tm_hour;
}


void update_sensor_data_task(lv_timer_t * timer) {
    GetHourNow(&hour);
    printf("\n%d",hour);
    getCurrentSec(&indextime);

    //  lv_chart_series_t * ui_ChartDust_series_1 = lv_chart_add_series(ui_Image68, lv_color_hex(0x297AC6),
    //                                                                  LV_CHART_AXIS_PRIMARY_Y);

    //  lv_chart_series_t * ui_ChartNoise_series_1 = lv_chart_add_series(ui_Image67, lv_color_hex(0x297AC6),
    //                                                                   LV_CHART_AXIS_PRIMARY_Y);
    SensorData sensorData;
    if(indextime%20==0){
    char*response = AIot();
    fetch_all_sensor_data(response, &sensorData);
    printf("L");
            // Cập nhật các nhãn với giá trị mới
    //dust=sensorData.dust;
    sound=sensorData.sound;
    lv_label_set_text_fmt(ui_NumHumdity, "%.1f", sensorData.humidity);
    lv_label_set_text_fmt(ui_NumTemp, " %.1f", sensorData.temperature);
    lv_label_set_text_fmt(ui_NumCo2, " %.1f", sensorData.co2);
    lv_label_set_text_fmt(ui_NumTVOCs, " %.1f", sensorData.tvocs);
    lv_label_set_text_fmt(ui_NumCH4, " %.1f", sensorData.ch4);
    lv_label_set_text_fmt(ui_NumCO, " %.1f", sensorData.co);
    lv_label_set_text_fmt(ui_NumPM25, "%.3f", sensorData.pm25);
    lv_label_set_text_fmt(ui_NumPM10, "%.3f", sensorData.pm10);
    lv_label_set_text_fmt(ui_NumIAQ, " %d", sensorData.iaq);
    if (sensorData.iaq >= 0 && sensorData.iaq <= 50)
    {
    lv_label_set_text(ui_Good, "GOOD");
    }   
    else if (sensorData.iaq >= 51 && sensorData.iaq <= 100)
    {
    lv_label_set_text(ui_Good, "MODERATE");
    }
    else if (sensorData.iaq >= 101 && sensorData.iaq <= 150)
    {
    lv_label_set_text(ui_Good, "UNSAFE FOR SENSITIVE");
    }
    else if (sensorData.iaq >= 151 && sensorData.iaq <= 200)
    {
    lv_label_set_text(ui_Good, "UNHEALTHY");
    }
    else if (sensorData.iaq >= 201 && sensorData.iaq <= 300)
    {
    lv_label_set_text(ui_Good, "VERY UNHEALTHY");
    }
    else if (sensorData.iaq >= 301 && sensorData.iaq <= 500)
    {
    lv_label_set_text(ui_Good, "HARZARDOUS");
    }
     

    }
    switch(hour)
    {
        // case 10: 
        case 0:
   
        ui_ChartDust_series_1_array[0]=dust;
        ui_ChartNoise_series_1_array[0]=sound;
        printf("\n1");
        break;  
        // case 20: 
        case 4:

        ui_ChartDust_series_1_array[1]=dust;
        ui_ChartNoise_series_1_array[1]=sound;
        printf("\n2");
        break;  
        // case 30: 
        case 8:

        ui_ChartDust_series_1_array[2]=dust;
        ui_ChartNoise_series_1_array[2]=sound;
        printf("\n3");
        break;  
        // case 40: 
        case 12:

        ui_ChartDust_series_1_array[3]=dust;
        ui_ChartNoise_series_1_array[3]=sound;
        printf("\n4");
        break;  
        // case 50: 
        case 16:

        ui_ChartDust_series_1_array[4]=dust;
        ui_ChartNoise_series_1_array[4]=sound;
        printf("\n5");
        break;  
        // case 60: 
        case 20:

        ui_ChartDust_series_1_array[5]=dust;
        ui_ChartNoise_series_1_array[5]=sound;
        printf("\n6");
        break;  

    }

    //lv_chart_set_ext_y_array(ui_Image68, ui_ChartDust_series_1, ui_ChartDust_series_1_array);
    //lv_chart_set_ext_y_array(ui_Image67, ui_ChartNoise_series_1, ui_ChartNoise_series_1_array);
}

void update_button1(lv_timer_t * timer) 

{

    char* data =  Electronic();

    fetch_lights(data,&L1,&L2);
    char* data_shared =  Electronic_shared();
    fetch_lights_shared(data_shared,&ssL1,&ssL2);
    // printf("\ntrang thai L1:  %d", L1);
    // printf("\ntrang thai ssL1:  %d", ssL1);
    // printf("\ntrang thai buttton1:  %d", button1);
                if (ssL1==L1)
                {
                    //    lv_obj_add_state( ui_Switchlight1, LV_STATE_CHECKED);
                        // Đánh dấu button là "checked" hoặc "bật"
                        printf ("\nOK");

                }
                else if(ssL1 ==true&&L1==false)
                {   
                    printf("\nxoa icon");
                    // button1=false;
                    lv_obj_clear_state(ui_Switch1, LV_STATE_CHECKED); 
                    lv_event_send(ui_Switch1, LV_EVENT_VALUE_CHANGED, NULL); // Gửi sự kiện VALUE CHANGED sau khi thay đổi trạng thái
                }

                else if(ssL1==false&&L1==true)
                {   
                    printf("\n set lại icon");
                    lv_obj_add_state(ui_Switch1, LV_STATE_CHECKED); 
                    lv_event_send(ui_Switch1, LV_EVENT_VALUE_CHANGED, NULL); // Gửi sự kiện VALUE CHANGED sau khi thay đổi trạng thái
                
                }

                
}


void update_button2(lv_timer_t * timer) 

{
    printf("hello");
    char* data =  Electronic();

    fetch_lights(data,&L1,&L2);
    char* data_shared =  Electronic_shared();
    fetch_lights_shared(data_shared,&ssL1,&ssL2);
    // printf("\ntrang thai L1:  %d", L1);
    // printf("\ntrang thai ssL1:  %d", ssL1);
    // printf("\ntrang thai buttton1:  %d", button1);
                if (ssL2==L2)
                {
                    //    lv_obj_add_state( ui_Switchlight1, LV_STATE_CHECKED);
                        // Đánh dấu button là "checked" hoặc "bật"
                        printf ("\nOK");

                }
                else if(ssL2 ==true&&L2==false)
                {   
                    printf("\nxoa icon");

                    // button1=false;
                    lv_obj_clear_state(ui_Switch2, LV_STATE_CHECKED); 
                    lv_event_send(ui_Switch2, LV_EVENT_VALUE_CHANGED, NULL); // Gửi sự kiện VALUE CHANGED sau khi thay đổi trạng thái
                }

                else if(ssL2==false&&L2==true)
                {   
                    printf("\n set lại icon");
                    lv_obj_add_state(ui_Switch2, LV_STATE_CHECKED); 
                    lv_event_send(ui_Switch2, LV_EVENT_VALUE_CHANGED, NULL); // Gửi sự kiện VALUE CHANGED sau khi thay đổi trạng thái
                
                }

                
}


// void update_weather(lv_timer_t * timer)
// {
//     int count_weather =0;
//     getCurrentSec(&count_weather);
//     if(count_weather%59==0)
//     {
//         GetDataWeather(&temw, &humw, desw);
//         lv_label_set_text_fmt(ui_TemAPI, "%.1f", (int)(temw * 10) / 10.0);
//     //      if (strcmp(desw, "Clouds") == 0) {
//         if(strcmp(desw, "Clouds") == 0){
//         lv_img_set_src(ui_IconWeather, &ui_img_03n);
//         }
//         else if (strcmp(desw, "Rain") == 0) { 
//             lv_img_set_src(ui_IconWeather, &ui_img_10n);
//         }
//         else if (strcmp(desw, "Clear") == 0) {
//             lv_img_set_src(ui_IconWeather, &ui_img_01d);
//         } 
//         else if (strcmp(desw, "Thunderstorm") == 0) {
            
//             lv_img_set_src(ui_IconWeather, &ui_img_11d);
//         }
//         else if (strcmp(desw, "Snow") == 0) {
            
//             lv_img_set_src(ui_IconWeather, &ui_img_13d);
//         }
//         else if (strcmp(desw, "Drizzle") == 0) {
//             lv_img_set_src(ui_IconWeather, &ui_img_09d);
//         }
//         else if (strcmp(desw, "Mist") == 0) {
            
//             lv_img_set_src(ui_IconWeather, &ui_img_50d);
//         }


//     }

// }

