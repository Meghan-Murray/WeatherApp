#include "widgetCity.h"
#include "api.h"
#include "json.h"
#include <iostream>

using namespace JsonParser;

WidgetCity::WidgetCity(const char* city)
{
    std::string jsonText = getInfoOfCity(city);

    text_it start = jsonText.begin();
	JsonValue parsed = ParseJsonHelper(jsonText, start);
	JsonValue location =  (*parsed.json)["location"];

	std::cout << (*location.json)["country"].str << std::endl;

    std::string name;
    std::string country;
    double temp_c;
    std::string condition;
    double wind_kph; // km in hour... i want metr in sec
    double precip_mm; 
}

void WidgetCity::widgetCity()
{

}