//
// Created by bibhash on 4/16/19.
//

#ifndef TRADER_DATABASE_H
#define TRADER_DATABASE_H


#include <string>
#include "TickerBlock.h"
#include "DateTime.h"
#include <map>
#include <fstream>

class Database {
public:
    Database& operator = (Database const&) = delete;
    ~Database() = default;
    static Database& GetInstanceOfDb(std::string a_universe, std::string a_directory);
    TickerBlock & operator [](std::string ticker);
    std::vector<std::string> getTickers();
    std::vector<DateTime> getTradingDates();
    std::map<std::string, TickerBlock*> getDatabase();

private:
    Database(std::string &a_universe, std::string &a_directory);
    static Database *inst_db;                           // The only instance of the database

    void LoadTickerData(std::string a_directory);
    void LoadUniverseData(std::string a_universe);
    void LoadReferenceDates(const std::string &a_directory);

    static std::map<std::string, TickerBlock*> m_db;    // The main structure that stores the data
    static std::vector<DateTime> m_referenceDates;      // stores all the dates where trading happened
    static std::vector<std::string> m_tickerNames;      // stores all the ticker names from the universe

    std::vector<std::string> m_availableTickers;        // stores all the ticker names for which data is available

    std::string m_refDateTicker = "IBM";                // Ticker Name for which to refer for dates
};


#endif //TRADER_DATABASE_H
