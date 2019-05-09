//
// Created by bibhash on 4/16/19.
//

#include <iostream>
#include "Database.h"

Database* Database::inst_db = NULL;
std::vector<std::string> Database::m_tickerNames;
std::map<std::string, TickerBlock*> Database::m_db;
std::vector<DateTime> Database::m_referenceDates;

Database::Database(std::string &a_universe, std::string &a_directory) {
    LoadUniverseData(a_universe);
    LoadTickerData(a_directory);
}

Database& Database::GetInstanceOfDb(std::string a_universe, std::string a_directory) {
    if (inst_db == NULL)
        inst_db = new Database(a_universe, a_directory);
    return *inst_db;
}

void Database::LoadUniverseData(std::string a_universe) {
    std::ifstream in(a_universe);
    std::string line;

    if (!in){
        std::cerr << "ERROR! Could not open Universe file." << std::endl;
        exit(1);
    }

    while(!in.eof()){
        getline(in, line);
        m_tickerNames.push_back(line);
    }
}

void Database::LoadTickerData(std::string a_directory) {
    std::string refDateDirectory = a_directory + "/" + m_refDateTicker + "_.csv";
    LoadReferenceDates(refDateDirectory);

    //std::cout << m_referenceDates.size() << std::endl;
    //std::vector<std::string> failedFiles;

    for (auto ticker : m_tickerNames){
        std::string currTickerFile = a_directory + "/" + ticker + "_.csv";

        std::ifstream in(currTickerFile);
        if (in.fail()){ // since the universe file does not correspond with the ticker data directory files
            //failedFiles.push_back(currTickerFile);
            //std::cout << currTickerFile << std::endl;
            continue;
        }
        //std::cout << "Existing Ticker Files: " << ticker << std::endl;

        TickerBlock *currTicker = new TickerBlock(ticker, m_referenceDates, currTickerFile);
        currTicker->PrintParsedData();
        m_db[ticker] = currTicker;
        //TODO: remove the break
        break;
    }

    //std::cout << failedFiles.size() << std::endl;
    //std::cout << m_db.size() << std::endl;
    //std::cout << m_tickerNames.size() << std::endl;
}

void Database::LoadReferenceDates(std::string a_directory) {
    std::ifstream in(a_directory);
    std::string line;
    if(!in){
        std::cerr << "ERROR! Could not open IBM file" << std::endl;
        exit(1);
    }
    getline(in, line);  // get the first line which has the info of the file
    getline(in, line);  // get the second line which has the headers
    //iterate through the lines
    while(getline(in, line)){   // !in.eof() had issues, IBM file included garbage data towards the end
        int pos = line.find(',');   // get the position of the first comma
        std::string date = line.substr(0, pos);  // get the date
        m_referenceDates.emplace_back(date);
        line.clear();
    }
}