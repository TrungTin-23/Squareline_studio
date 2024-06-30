// #ifndef RANDOM_H
// #define RANDOM_H

// // Function to get a random value between 1 and 100
// int random_value();

// #endif // RANDOM_H

#ifndef CONTROL_BUTTON_H
#define CONTROL_BUTTON_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <lvgl/lvgl.h>
#include "ui/ui.h"
#include "myapi/myapi.h"
#include <stdbool.h>
#include "showmyapi.h"
// extern bool L1;
// extern bool L2;

void ChangeButton1(lv_event_t * e);
void ChangeButton2(lv_event_t * e);


#endif // CONTROL_BUTTON_H
