#include "widgetCity.h"
// #include "../lv_conf.h"
#include "api.h"
#include "json.h"
#include <iostream>

using namespace JsonParser;

static void btnDelete_event_handler(lv_event_t * e)
{
    lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_delete(lv_obj_get_parent(obj));
}

static void btnUpdate_event_handler(lv_event_t * e)
{
    WidgetCity * ta = (WidgetCity *)lv_event_get_user_data(e);
    ta->updateWidget();
}

WidgetCity::WidgetCity(const char * city, lv_obj_t * parent)
{
    if(widgetInit(city) == 1) return;

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_font(&style, &lv_font_montserrat_18);

    static lv_style_t styleB;
    lv_style_init(&styleB);
    lv_style_set_text_font(&styleB, &lv_font_montserrat_28);
    lv_style_set_text_color(&styleB, lv_palette_darken(LV_PALETTE_RED, 1));
    lv_style_set_shadow_width(&styleB, 0);
    lv_style_set_bg_opa(&styleB, 0);

    static lv_style_t styleB2;
    lv_style_init(&styleB2);
    lv_style_set_text_font(&styleB2, &lv_font_montserrat_22);
    lv_style_set_text_color(&styleB2, lv_palette_darken(LV_PALETTE_BLUE, 1));
    lv_style_set_shadow_width(&styleB2, 0);
    lv_style_set_bg_opa(&styleB2, 0);

    lv_obj_t * weatherWidget = lv_obj_create(parent);
    lv_obj_set_width(weatherWidget, lv_pct(100));
    lv_obj_remove_flag(weatherWidget, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * nameL = lv_label_create(weatherWidget);
    lv_label_set_text(nameL, name);
    lv_obj_align(nameL, LV_ALIGN_TOP_LEFT, 5, -10);
    lv_obj_add_style(nameL, &styleB, 0);

    lv_obj_t * countryL = lv_label_create(weatherWidget);
    lv_label_set_text(countryL, country);
    lv_obj_align(countryL, LV_ALIGN_TOP_LEFT, 5, 25);
    lv_obj_add_style(countryL, &style, 0);

    tempL            = lv_label_create(weatherWidget);
    std::string temp = std::to_string(temp_c);
    if(temp_c > 0) temp = '+' + temp + " C°";
    lv_label_set_text(tempL, temp.c_str());
    lv_obj_align(tempL, LV_ALIGN_BOTTOM_LEFT, 5, 5);
    lv_obj_add_style(tempL, &styleB, 0);

    textL = lv_label_create(weatherWidget);
    lv_label_set_text(textL, text);
    lv_obj_align(textL, LV_ALIGN_BOTTOM_RIGHT, 5, -40);
    lv_obj_add_style(textL, &style, 0);

    windL            = lv_label_create(weatherWidget);
    std::string wind = "Wind: " + std::to_string(wind_msec) + " m/s";
    lv_label_set_text(windL, wind.c_str());
    lv_obj_align(windL, LV_ALIGN_BOTTOM_RIGHT, 5, -15);
    lv_obj_add_style(windL, &style, 0);

    precipL            = lv_label_create(weatherWidget);
    std::string precip = "Precipitation: " + std::to_string(precip_mm) + " mm";
    lv_label_set_text(precipL, precip.c_str());
    lv_obj_align(precipL, LV_ALIGN_BOTTOM_RIGHT, 5, 10);
    lv_obj_add_style(precipL, &style, 0);

    lv_obj_t * btnUpdate = lv_button_create(weatherWidget);
    lv_obj_align(btnUpdate, LV_ALIGN_TOP_RIGHT, -20, -15);
    lv_obj_set_size(btnUpdate, 30, 30);
    lv_obj_add_event_cb(btnUpdate, btnUpdate_event_handler, LV_EVENT_CLICKED, this);
    lv_obj_t * btnL = lv_label_create(btnUpdate);
    lv_label_set_text(btnL, LV_SYMBOL_REFRESH);
    lv_obj_add_style(btnUpdate, &styleB2, 0);
    lv_obj_center(btnL);

    lv_obj_t * btnClose = lv_button_create(weatherWidget);
    lv_obj_align(btnClose, LV_ALIGN_TOP_RIGHT, 10, -15);
    lv_obj_set_size(btnClose, 30, 30);
    lv_obj_add_event_cb(btnClose, btnDelete_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label = lv_label_create(btnClose);
    lv_label_set_text(label, LV_SYMBOL_CLOSE);
    lv_obj_add_style(btnClose, &styleB, 0);
    lv_obj_center(label);
}

int WidgetCity::widgetInit(const char * city)
{
    std::string jsonText = getInfoOfCity(city);
    query = city;

    text_it start    = jsonText.begin();
    JsonValue parsed = ParseJsonHelper(jsonText, start);

    JsonValue error = (*parsed.json)["error"];
    if(error.json) return 1;

    JsonValue location  = (*parsed.json)["location"];
    JsonValue current   = (*parsed.json)["current"];
    JsonValue condition = (*current.json)["condition"];

    name         = (*location.json)["name"].str;
    country      = (*location.json)["country"].str;
    last_updated = (*current.json)["last_updated"].str;
    temp_c       = (int)(*current.json)["temp_c"].d;
    text         = (*condition.json)["text"].str;
    wind_msec    = (int)((*current.json)["wind_kph"].d) / 3.6;
    precip_mm    = (int)(*current.json)["precip_mm"].d;

    return 0;
}

void WidgetCity::updateWidget()
{
    std::string jsonText = getInfoOfCity(query.c_str());
    text_it start        = jsonText.begin();
    JsonValue parsed     = ParseJsonHelper(jsonText, start);
    JsonValue current   = (*parsed.json)["current"];
    JsonValue condition = (*current.json)["condition"];
    temp_c    = (int)(*current.json)["temp_c"].d;
    text      = (*condition.json)["text"].str;
    wind_msec = (int)((*current.json)["wind_kph"].d) / 3.6;
    precip_mm = (int)(*current.json)["precip_mm"].d;

    std::string temp = std::to_string(temp_c);
    if(temp_c > 0) temp = '+' + temp + " C°";
    lv_label_set_text(tempL, temp.c_str());
    lv_label_set_text(textL, text);
    std::string wind = "Wind: " + std::to_string(wind_msec) + " m/s";
    lv_label_set_text(windL, wind.c_str());
    std::string precip = "Precipitation: " + std::to_string(precip_mm) + " mm";
    lv_label_set_text(precipL, precip.c_str());
}