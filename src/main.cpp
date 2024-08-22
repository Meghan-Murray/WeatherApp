#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "../lv_conf.h"
#include "api.h"
#include "widgetCity.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <iostream>

#define MY_DISP_HOR_RES "800"
#define MY_DISP_VER_RES "600"

bool checkLine = false;
lv_obj_t * tv;

static const char * getenv_default(const char * name, const char * dflt)
{
    return getenv(name) ?: dflt;
}

static void disp_init(void)
{
    const int width  = atoi(getenv("LV_SDL_VIDEO_WIDTH") ?: MY_DISP_HOR_RES);
    const int height = atoi(getenv("LV_SDL_VIDEO_HEIGHT") ?: MY_DISP_VER_RES);

    lv_sdl_window_create(width, height);
}

static void textarea_event_handler(lv_event_t * e)
{
    lv_obj_t * ta = (lv_obj_t *)lv_event_get_target(e);
}

static void btnm_event_handler(lv_event_t * e)
{
    lv_obj_t * obj    = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t * ta     = (lv_obj_t *)lv_event_get_user_data(e);
    const char * text = lv_textarea_get_text(ta);

    if(text != "" && lv_obj_get_child_count(tv) < 5) {
        WidgetCity *widgetCity = new WidgetCity(text, tv);
    }
}

void drawTitle()
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_font(&style, &lv_font_montserrat_18);

    lv_obj_set_flex_flow(lv_screen_active(), LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(lv_screen_active(), LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    tv = lv_obj_create(lv_screen_active());
    lv_obj_set_flex_flow(tv, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(tv, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_flex_grow(tv, 10);
    lv_obj_set_width(tv, lv_pct(90));

    lv_obj_t * th = lv_obj_create(lv_screen_active());
    lv_obj_set_height(th, 90);
    lv_obj_set_flex_flow(th, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(th, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_width(th, lv_pct(90));
    lv_obj_remove_flag(th, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_t * ta = lv_textarea_create(th);

    lv_textarea_set_one_line(ta, true);
    lv_obj_add_event_cb(ta, textarea_event_handler, LV_EVENT_READY, ta);
    lv_textarea_set_max_length(ta, 60);
    lv_textarea_set_placeholder_text(ta, "city's name");
    lv_obj_set_width(ta, lv_pct(80));
    lv_group_add_obj(lv_group_get_default(), ta);
    lv_obj_add_style(ta, &style, 0);

    lv_obj_t * btnSearch = lv_button_create(th);
    lv_obj_add_event_cb(btnSearch, btnm_event_handler, LV_EVENT_CLICKED, ta);
    lv_obj_t * label = lv_label_create(btnSearch);
    lv_label_set_text(label, "Add City");
    lv_obj_add_style(label, &style, 0);
    lv_obj_center(label);

    lv_obj_remove_flag(btnSearch, LV_OBJ_FLAG_CLICK_FOCUSABLE); 
}

int main(void)
{
    lv_init();

    disp_init();
    lv_group_t * g = lv_group_create();
    lv_group_set_default(g);

    lv_indev_t * mouse      = lv_sdl_mouse_create();
    lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
    lv_indev_t * keyboard   = lv_sdl_keyboard_create();
    lv_indev_set_group(keyboard, lv_group_get_default());
    lv_indev_set_mode(keyboard, LV_INDEV_MODE_NONE);

    drawTitle();

    while(1) {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}
