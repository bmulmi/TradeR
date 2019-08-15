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
    if (inst_db == nullptr)
        inst_db = new Database(a_universe, a_directory);
    return *inst_db;
}

void Database::LoadUniverseData(std::string a_universe) {
    std::ifstream in(a_universe);
    std::string line;

    if (!in){
        std::cerr << "ERROR! Could not open Universe file." << a_universe << std::endl;
        exit(1);
    }

    while(!in.eof()){
        getline(in, line);
        if (line.empty()) continue;
        m_tickerNames.push_back(line);
    }
}

void Database::LoadTickerData(std::string a_directory) {
    std::string refDateDirectory = a_directory + "/" + m_refDateTicker + "_.csv";
    LoadReferenceDates(refDateDirectory);

    //std::cout << m_referenceDates.size() << std::endl;
    //std::vector<std::string> failedFiles;

    std::cout << "Loading Ticker Data..." << std::endl;

    for (std::string ticker : m_tickerNames){
        //std::cout<<"Loading " << ticker << std::endl;
        std::string currTickerFile = a_directory + "/" + ticker + "_.csv";

        std::ifstream in(currTickerFile);
        if (in.fail()){ // since the universe file does not correspond with the ticker data directory files
            //failedFiles.push_back(currTickerFile);
            //std::cerr << "FILE NOT FOUND: " << currTickerFile << std::endl;
            continue;
        }
        //std::cout << "Existing Ticker Files: " << ticker << std::endl;

        TickerBlock *currTicker = new TickerBlock(ticker, m_referenceDates, currTickerFile);
        //currTicker->PrintParsedData();
        m_db[ticker] = currTicker;

        m_availableTickers.push_back(ticker);
        //TODO: remove the break
        break;
    }

    std::cout << "Ticker Data Loaded #: " << m_availableTickers.size() << std::endl;
    //std::cout << m_db.size() << std::endl;
    //std::cout << m_tickerNames.size() << std::endl;
}

void Database::LoadReferenceDates(const std::string &a_directory) {
    std::ifstream in(a_directory);
    std::string line;
    if(!in){
        std::cerr << "ERROR! Could not open IBM file" << std::endl;
        exit(1);
    }

    // get the first line which has the info of the file
    getline(in, line);

    // get the second line which has the headers
    getline(in, line);

    // iterate through the lines
    while(getline(in, line)){
        // !in.eof() had issues, IBM file included garbage data towards the end
        // get the position of the first comma to get the date
        int pos = line.find(',');
        std::string date = line.substr(0, pos);
        m_referenceDates.insert(m_referenceDates.begin(), date);
        line.clear();
    }
}

TickerBlock& Database::operator[](std::string ticker) {
    return *m_db[ticker];
}

std::vector<std::string> Database::getTickers() {
    return m_availableTickers;
}

std::vector<DateTime> Database::getTradingDates() {
    return m_referenceDates;
}

std::map<std::string, TickerBlock*> Database::getDatabase() {
    return m_db;
}