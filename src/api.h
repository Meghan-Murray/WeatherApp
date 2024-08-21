#ifndef API_H
#define API_H

#define APIKEY "ce1818f7f25a4b46bee141825242008"
#define URL "api.weatherapi.com"
#define BUFFER_SIZE 2048
#include <string>

std::string getInfoOfCity(const char* name);

#endif 