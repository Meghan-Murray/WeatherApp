#ifndef WIDGETCITY_H
#define WIDGETCITY_H

#include <string>
class WidgetCity 
{
    std::string name;
    std::string country;
    double temp_c;
    std::string condition;
    double wind_kph; // km in hour... i want metr in sec
    double precip_mm; 

public:
    WidgetCity(const char* city);
    void widgetCity();
};

#endif