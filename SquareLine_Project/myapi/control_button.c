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
#include "control_button.h"
#include "myapi/myapi.h"
#include <stdbool.h>

bool button1 = false, button2 = false;

void ChangeButton1(lv_event_t * e) {
    lv_obj_t * obj = lv_event_get_target(e);
    bool state = lv_obj_has_state(obj, LV_STATE_CHECKED); // Kiểm tra trạng thái của switch

    button1 = state; // Cập nhật trạng thái button1 dựa trên trạng thái của switch

    if(button1) {
       printf("Button1 is ON %d\n", button1);
       SendButton1On();
    } else {
        printf("Button1 is OFF %d\n", button1); 
        SendButton1Off();     
    }
    lv_timer_t * timer = lv_timer_create(update_button1, 10000, NULL);
    lv_timer_set_repeat_count(timer, 1); // Đảm bảo timer chỉ chạy một lần
}


void ChangeButton2(lv_event_t * e) {
    lv_obj_t * obj = lv_event_get_target(e);
    bool state = lv_obj_has_state(obj, LV_STATE_CHECKED); // Kiểm tra trạng thái của switch

    button2 = state; // Cập nhật trạng thái button1 dựa trên trạng thái của switch

    if(button2) {
       printf("Button2 is ON %d\n", button2);
       SendButton2On();
    } else {
        printf("Button2 is OFF %d\n", button2);
        SendButton2Off();    
    }
    lv_timer_t * timer = lv_timer_create(update_button2, 10000, NULL);
    lv_timer_set_repeat_count(timer, 1); // Đảm bảo timer chỉ chạy một lần
}


