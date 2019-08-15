//
// Created by bibhash on 4/15/19.
//

#ifndef TRADER_CONFIGPARSER_H
#define TRADER_CONFIGPARSER_H

#include <string>
#include <unordered_map>
#include <map>
#include <regex>
#include <fstream>
#include "Utilities.h"


class ConfigParser {
public:
    explicit ConfigParser(std::string a_filename);
    ~ConfigParser() = default;
    void ParseData();
    std::string GetConfigValues(std::string a_header, std::string a_key);
    std::map<std::string, std::unordered_map<std::string, std::string>> GetConfigData();
    enum TYPE{
        header,
        params,
        comment
    };
    void PrintConfigs();


private:
    std::string m_fileName;
    std::string m_currHeader;
    std::map<std::string, std::unordered_map<std::string, std::string>> m_configData;

    TYPE Identify(std::string a_line);
    void StoreHeader(std::string a_line);
    void StoreParams(std::string a_line);

    // instantiate regex objects
    std::regex HEADER;
    std::regex PARAMS;
    std::regex COMMENT;

    // regex string to match the config data type
    // uses raw string literals to include the escape characters
    const std::string headerStr = R"(^\[\w+\]$)";
    const std::string paramStr = R"(^.+=.+$)";
    const std::string commentStr = R"(^;.*$)";
};


#endif //TRADER_CONFIGPARSER_H
