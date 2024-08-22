#ifndef WIDGETCITY_H
#define WIDGETCITY_H

#include "../lvgl/lvgl.h"
#include "lvgl/src/font/lv_font.h"
#include <string>

class WidgetCity {
    const char * name;
    const char * country;
    int temp_c;
    const char * text;
    int wind_msec;
    int precip_mm;
    const char * last_updated;
    std::string query;

  public:
    WidgetCity(const char * city, lv_obj_t * parent);
    void widgetCity();
    void updateWidget();
   // const char * name;

  private:
    int widgetInit(const char * city);
    lv_obj_t * tempL;
    lv_obj_t * textL;
    lv_obj_t * windL;
    lv_obj_t * precipL;
};

#endif