#include <algorithm> 
#include <fstream>  
#include <cassert>   
#include <cctype>    
#include <iostream>

#include "json.h"

using namespace JsonParser;

JsonValue JsonParser::ParseJson(const std::string & filepath)
{
    std::string text;
    ReadFile(filepath, text);

    text_it start = text.begin();
    return ParseJsonHelper(text, start);
}

void JsonParser::ReadFile(const std::string & filepath, std::string & output)
{
    std::ifstream file(filepath);
    std::string line;

    while(std::getline(file, line)) {
    }
}

JsonValue JsonParser::ParsePrimitive(const std::string & text, text_it start, text_it end)
{
    std::string substr       = text.substr(start - text.begin(), end - start);
    if (substr[0] == '"') {
        char *str = new char[substr.size()-2 + 1];
        for (int i = 1; i < substr.size()-1; ++i) {
            str[i-1]=substr[i];
        }
        str[substr.size()-2]=0;
        return {.str = str};
    }
    size_t float_point_index = substr.find(".");

    if(float_point_index >= (end - start)) { 
        return {.i = std::stoi(substr)};
    } else { 
        return {.d = std::stod(substr)};
    }
}

std::pair<std::string, JsonValue> JsonParser::RetriveKeyValuePair(const std::string & text, text_it & it)
{
    assert(it != text.end());

    while(*it == ' ' || *it == '\n') {
        it++;
    }

    text_it curr_it;
    std::string key;
    JsonValue value;
    if(*it == '\"') {
        curr_it = ++it;
        while(*it != '\"') {
            it++;
        }

        key = text.substr(curr_it - text.begin(), it - curr_it);
        assert(*(++it) == ':'); 
        it++;
    }

    while(*it == ' ' || *it == '\n') {
        it++;
    }

    if(*it == '{') {
        value = ParseJsonHelper(text, it);
    } else {

        curr_it = it;
        bool column_bracket = false;
        while(isdigit(*it) || *it == '-' || *it == '.' || *it == '"' || column_bracket) {
            if (*it == '"') {
                column_bracket = !column_bracket;
            }
            it++;
        }
        value = ParsePrimitive(text, curr_it, it);
    }

    if(*it == ',') {
        it++;
    }

    return std::make_pair(key, value);
}

JsonValue JsonParser::ParseJsonHelper(const std::string & text, text_it & it)
{
    assert(*it == '{'); 
    it++;

    std::map<std::string, JsonValue> * json_map = new std::map<std::string, JsonValue>;

    do {
        const auto [key, value] = RetriveKeyValuePair(text, it);
        (*json_map)[key]        = value;

        while(*it == ' ' || *it == '\n') {
            it++;
        }
    } while(*it != '}');

    it++; 

    return {.json = json_map};
}