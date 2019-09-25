/**
 * This class will be responsible for parsing the configurations
 * ini file. It uses regular expressions to identify header,
 * parameter values and comments in the text file and stores
 * them into a map with header as key and parameter=values as
 * unordered map as value.
 */

#include "ConfigParser.h"

/**/
/*
 ConfigParser::ConfigParser()

 NAME

    ConfigParser::ConfigParser - constructs the ConfigParser object

 SYNOPSIS

    ConfigParser::ConfigParser(std::string a_filename);

        a_filename  --> the configurations.ini file name to be parsed

 DESCRIPTION

    The constructor sets the filename variable and assigns the raw
    string to the regular expression defined in the header file.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    4/15/2019

*/
/**/
ConfigParser::ConfigParser(std::string a_filename) {
    m_fileName = a_filename;
    HEADER.assign(headerStr);
    PARAMS.assign(paramStr);
    COMMENT.assign(commentStr);
}


/**/
/*
ConfigParse::ParseData()

 NAME

    void ConfigParser::ParseData - opens and parses the file

 SYNOPSIS

    void ConfigParser::ParseData();

 DESCRIPTION

    This function will open the ini file and read each line
    while end of file is not reached. It throws a standard
    error if the file cannot be opened and exits with signal 1.

    In the loop, it attempts to recognize each line type by
    comparing it with each of the three regex defined. Then, it
    stores the line by calling other functions.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    4/15/2019

 */
/**/
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

    std::cout << "Parsing Configurations.ini COMPLETED! " << "\n\n";
}


/**/
/*
 ConfigParser::TYPE ConfigParser::Identify()

 NAME

    ConfigParser::TYPE ConfigParser::Identify - matches regex and returns
    regex type

 SYNOPSIS

    ConfigParser::TYPE ConfigParser(std::string a_line);

        a_line  --> the line to be identified

 DESCRIPTION

    This function matches the line passed to the regular expression
    and returns the enum type it matches to

 RETURNS

    Returns ConfigParse::TYPE

 AUTHOR

    Bibhash Mulmi

 DATE

    4/15/2019

 */
/**/
ConfigParser::TYPE ConfigParser::Identify(std::string a_line) {
    if (std::regex_match(a_line, HEADER))
        return TYPE::header;
    else if (std::regex_match(a_line, PARAMS))
        return TYPE::params;
    else
        return TYPE::comment;
}


/**/
/*

 ConfigParser::StoreHeader()

 NAME

    ConfigParser::StoreHeader - stores the line as header

 SYNOPSIS

    void ConfigParser::StoreHeader(std::string a_line);

        a_line  --> the line to be stored as header

 DESCRIPTION

    This function stores the line to a global variable
    called m_currHeader which is used by other functions
    to assign the parameter values under that header name.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    4/15/2019

 */
/**/
void ConfigParser::StoreHeader(std::string a_line) {
    std::string headerName;

    for (char c : a_line){
        if (c == '[' || c == ']') continue;
        headerName.push_back(c);
    }

    m_currHeader = Utilities::toLowerCase(headerName);
}


/**/
/*

 ConfigParser::StoreParams()

 NAME

    ConfigParser::StoreParams - stores parameter values from the
    line passed in as arguement

 SYNOPSIS

    void ConfigParser::StoreParams(std::string a_line);

        a_line  --> the line to be stored as key = value

 DESCRIPTION

    This function finds the position of '=' in the line
    passed in as argument. Then it takes the substring
    before the '=', makes that the 'key' and the substring
    after the '=', makes that the 'value' of the unordered
    map.

    The unordered map is the value of the map 'm_configData'.
    The key of this map is the header of the parameter value.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    4/15/2019

 */
/**/
void ConfigParser::StoreParams(std::string a_line) {
    auto pos = a_line.find('=');
    std::string key = a_line.substr(0, pos);
    std::string val = a_line.substr(pos+1);
    key = Utilities::toLowerCase(key);
    m_configData[m_currHeader][key] = val;
}


/**/
/*

 ConfigParser::GetConfigData()

 NAME

    ConfigParser::GetConfigData - returns the map of Configurations built from
    parsing the file

 SYNOPSIS

    std::map<std::string, std::unordered_map<std::string, std::string>>
    ConfigParser::GetConfigData()

 DESCRIPTION

    This function simply returns the m_configData map that
    stores all the necessary details from the Configurations.ini
    file. The map has the following structure:

    map < Header, unordered_map < key, value > >
        where,  Header is the header of the section that is inside '[ ]'
                key is the parameter's string which is before '='
                value is the paramter's string which is after '='
 RETURNS

    Returns the Map with the Header as key and Unordered Map of
    parameter = value as the value

 AUTHOR

    Bibhash Mulmi

 DATE

    4/16/2019

 */
/**/
std::map<std::string, std::unordered_map<std::string, std::string>> ConfigParser::GetConfigData() {
    return m_configData;
}


/**/
/*

 ConfigParser::GetConfigValues()

 NAME

    ConfigParser::GetConfigValues - returns the value of a parameter
    under the header section

 SYNOPSIS

    std::string ConfigParser::GetConfigValues( std::string a_header,
                                                std::string a_key)

        a_header    --> the header section under which the value
                        is defined

        a_key       --> the key under which the value is assigned

 DESCRIPTION

    This function returns the value defined by the key which is
    assigned under the header section

 RETURNS

    Returns the value assigned under the key defined under the header
    as a string.

 AUTHOR

    Bibhash Mulmi

 DATE

    4/16/2019
 */
/**/
std::string ConfigParser::GetConfigValues(std::string a_header, std::string a_key) {
    return m_configData[a_header][a_key];
}


/**/
/*

 ConfigParser::PrintConfigs()

 NAME

    ConfigParser::PrintConfigs - prints the configuration file details
    from internal storage to the console

 SYNOPSIS

    void ConfigParser::PrintConfigs();

 DESCRIPTION

    This function simply prints the configuration file details stored in
    the map m_configData into the console window. This function is useful
    for debugging and checking whether the configuration file is read
    correctly or not.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    4/15/2019
 */
/**/
void ConfigParser::PrintConfigs() {
    std::map<std::string, std::unordered_map<std::string, std::string>> data = m_configData;

    for (auto const& headerSection : data){
        std::cout << "SECTION " << headerSection.first << ": " << std::endl;
        for (auto const& parameterPair : headerSection.second)
            std::cout << parameterPair.first << " = " << parameterPair.second << std::endl;
        std::cout << std::endl;
    }
}
