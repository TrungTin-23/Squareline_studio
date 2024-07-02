
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <unistd.h>
#define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>
#include "lvgl/lvgl.h"
#include "lv_drivers/sdl/sdl.h"
#include "ui/ui.h"
#include "myapi/myapi.h"
#include "myapi/weatherapi.h"
#include <stdbool.h> 
#include "myapi/showmyapi.h"
#include "myapi/control_button.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void update_sensor_data_task(lv_timer_t * timer) ;
int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/
 
  /*Initialize LVGL*/
  lv_init();

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  hal_init();

  ui_init();
  // lv_obj_add_event_cb(ui_Switchlight1, ChangeButton1, LV_EVENT_CLICKED, NULL);
   //lv_obj_add_event_cb(ui_Switch1, ChangeButton1, LV_EVENT_VALUE_CHANGED, NULL);   
   //lv_obj_add_event_cb(ui_Switch2, ChangeButton2, LV_EVENT_VALUE_CHANGED, NULL);
  lv_timer_create(update_sensor_data_task, 500, NULL);  // 1000ms = 1 giây
  //lv_timer_create(update_weather, 1000, NULL);  // 1000ms = 1 giây
  //lv_timer_t * timer = lv_timer_create(update_sensor_data_task, 500, NULL);
    
  while(1) {

     // printf("%d",hum);
      //update_icon_weather();
      //printf("hello");
      lv_timer_handler();

        usleep(5 * 1000);
     // usleep(5 * 1000);
      //sleep(5);
  
      
  }

  return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for LVGL
 */
static void hal_init(void)
{
  /* Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
  sdl_init();

  /*Create a display buffer*/
  static lv_color_t buf[SDL_HOR_RES * SDL_VER_RES];
  static lv_disp_draw_buf_t disp_draw_buf;
  lv_disp_draw_buf_init(&disp_draw_buf, buf, NULL, SDL_HOR_RES * SDL_VER_RES);

  /*Create a display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv); /*Basic initialization*/
  disp_drv.draw_buf = &disp_draw_buf;
  disp_drv.flush_cb = sdl_display_flush;
  disp_drv.hor_res = SDL_HOR_RES;
  disp_drv.ver_res = SDL_VER_RES;
  lv_disp_drv_register(&disp_drv);

  /* Add a mouse as input device */
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv); /*Basic initialization*/
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = sdl_mouse_read;
  lv_indev_drv_register(&indev_drv);
}

