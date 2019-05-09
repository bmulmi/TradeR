//
// Created by bibhash on 4/15/19.
//

#include <iostream>
#include "ConfigParser.h"

ConfigParser::ConfigParser(std::string a_filename) {
    m_fileName = a_filename;
    HEADER.assign(headerStr);
    PARAMS.assign(paramStr);
    COMMENT.assign(commentStr);
}

void ConfigParser::ParseData() {
    // open the file
    std::ifstream file(m_fileName);
    if (!file){
        std::cerr << "ERROR! Failed to open Configurations file." << std::endl;
        exit(1);
    }
    std::cout << "Parsing Configurations.ini..." << std::endl;
    // iterate through each line
    while (!file.eof()){
        std::string line;
        std::getline(file, line);
        // get line type
        TYPE lineType = Identify(line);

        switch (lineType) {
            case header:
                StoreHeader(line);
                break;

            case params:
                StoreParams(line);
                break;

            case comment:
                continue;

            default:
                continue;
        }
    }
    std::cout << "Parsing Configurations.ini COMPLETED! " << std::endl;
}

ConfigParser::TYPE ConfigParser::Identify(std::string a_line) {
    if (std::regex_match(a_line, HEADER))
        return TYPE::header;
    else if (std::regex_match(a_line, PARAMS))
        return TYPE::params;
    else
        return TYPE::comment;
}

void ConfigParser::StoreHeader(std::string a_line) {
    std::string headerName;

    for (char c : a_line){
        if (c == '[' || c == ']') continue;
        headerName.push_back(c);
    }

    m_currHeader = Utilities::toLowerCase(headerName);
}

void ConfigParser::StoreParams(std::string a_line) {
    auto pos = a_line.find('=');
    std::string key = a_line.substr(0, pos);
    std::string val = a_line.substr(pos+1);
    key = Utilities::toLowerCase(key);
    m_configData[m_currHeader][key] = val;
}

std::map<std::string, std::unordered_map<std::string, std::string>> ConfigParser::GetConfigData() {
    return m_configData;
}

std::string ConfigParser::GetConfigValues(std::string a_header, std::string a_key) {
    return m_configData[a_header][a_key];
}

void ConfigParser::PrintConfigs() {
    std::map<std::string, std::unordered_map<std::string, std::string>> data = m_configData;

    for (auto headerSection : data){
        std::cout << "SECTION " << headerSection.first << ": " << std::endl;
        for (auto parameterPair : headerSection.second)
            std::cout << parameterPair.first << " = " << parameterPair.second << std::endl;
        std::cout << std::endl;
    }
}