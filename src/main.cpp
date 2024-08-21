#include "lvgl/lvgl.h"
// #include "lvgl/demos/lv_demos.h"
#include "api.h"
#include "widgetCity.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <iostream>

static const char * getenv_default(const char * name, const char * dflt)
{
    return getenv(name) ?: dflt;
}

static void lv_linux_disp_init(void)
{
    const int width  = atoi(getenv("LV_SDL_VIDEO_WIDTH") ?: "800");
    const int height = atoi(getenv("LV_SDL_VIDEO_HEIGHT") ?: "480");

    //  lv_sdl_window_create(width, height);
}

int main(void)
{
    lv_init();
    WidgetCity widgetCity("London");
    /*Linux display device init*/
    // lv_linux_disp_init();

    /*Create a Demo*/
    // lv_demo_widgets();
    // lv_demo_widgets_start_slideshow();

    /*Handle LVGL tasks*/
    /*
    while(1) {
        lv_timer_handler();
        usleep(5000);
    }
    */
    return 0;
}
