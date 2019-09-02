/*
 * This class will be responsible for holding the database that
 * is to be used while in simulation. This class follows the
 * singleton pattern to avoid multiple copies and instances. This
 * is done because the database can be of large memory and we do
 * not want unnecessary copies of such.
 */

#include <iostream>
#include "Database.h"

// declaration of static variables
Database* Database::inst_db = NULL;
std::vector<std::string> Database::m_tickerNames;
std::map<std::string, TickerBlock*> Database::m_db;
std::vector<DateTime> Database::m_referenceDates;


/**/
/*

 Database::Database()

 NAME

    Database::Database - constructs the in-memory database,
    loads the universe and ticker files

 SYNOPSIS

    Database::Database(std::string &a_universe, std::string &a_directory);

        a_universe  --> the name of file that contains tickers for the
                        universe
        a_directory --> the path to the directory for price data of each
                        ticker to load

 DESCRIPTION

    This is the constructor of the Database object. It simply calls functions
    to load the universe file and price data files.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    4/16/2019

 */
/**/
Database::Database(std::string &a_universe, std::string &a_directory) {
    LoadUniverseData(a_universe);
    LoadTickerData(a_directory);
}


/**/
/*

 Database::GetInstanceOfDb()

 NAME

    Database::GetInstanceOfDb - returns the instance of Database object

 SYNOPSIS

    Database::GetInstanceOfDb(std::string a_universe, std::string a_directory);

        a_universe  --> the string of path to universe file

        a_directory --> the string of path to price data directory

 DESCRIPTION

    This function returns the single instance of the Database object.

    If the Database object has not yet been defined, then this function
    creates a new instance of the Database by calling its constructor.

 RETURNS

    Returns pointer to the single Database object.

 AUTHOR

    Bibhash Mulmi

 DATE

 4/16/2019

 */
/**/
Database& Database::GetInstanceOfDb(std::string a_universe, std::string a_directory) {
    if (inst_db == nullptr)
        inst_db = new Database(a_universe, a_directory);
    return *inst_db;
}


/**/
/*
 Database::LoadUniverseData()

 NAME

    Database::LoadUniverseData - loads the ticker names from the universe
    file

 SYNOPSIS

    void Database::LoadUniverseData(std::string a_universe);

        a_universe  --> the path to the universe file

 DESCRIPTION

    This function opens the universe file and pushes each of the ticker
    defined in this file into the m_tickerNames vector which will later
    be used to load the price data from the respective directory.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    4/16/2019
 */
/**/
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


/**/
/*

 Database::LoadTickerData()

 NAME

    Database::LoadTickerData - loads price data of each of the ticker
    defined in the universe file.

 SYNOPSIS

    void Database::LoadTickerData(std::string a_directory);

        a_directory --> path to the directory of price data

 DESCRIPTION

    This function first loads the reference dates to get the
    active trading dates. Then it creates a new instance of
    TickerBlock object for each ticker in the universe that
    has the price data in the data directory.

    Those tickers whose data were loaded are stored in a vector
    for available tickers for later use in simulation.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    4/16/2019

 */
/**/
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
        //break;
    }

    std::cout << "Ticker Data Loaded #: " << m_availableTickers.size() << "/" << m_tickerNames.size() << std::endl;
    //std::cout << m_db.size() << std::endl;
    //std::cout << m_tickerNames.size() << std::endl;
}


/**/
/*

 Database::LoadReferenceDates()

 NAME

    Database::LoadReferenceDates - loads the dates from the reference
    ticker's price data

 SYNOPSIS

    void Database::LoadReferenceDates(const std::string &a_directory);

        a_directory --> the path to the reference ticker file

 DESCRIPTION

    This function opens the reference ticker's price data file and loads
    the first column of the file which is the date. Then it stores these
    dates into a vector of reference dates.

 RETURNS

    Nothing

 AUTHOR

    Bibhash Mulmi

 DATE

    4/17/2019

 */
/**/
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


/**/
/*

 Database::operator[]()

 NAME

    Database::operator[] - overloads [] operator for Database object

 SYNOPSIS

    TickerBlock& Database::operator[](std::string const &a_ticker);

        a_ticker    --> name of the ticker

 DESCRIPTION

    This overloads the '[]' operator for Database object. When '[]' is
    used on the database object, the pointer to the TickerBlock object
    is returned.

 RETURNS

    Returns the pointer to the TickerBlock object that is passed in as
    argument.

 AUTHOR

    Bibhash Mulmi

 DATE

    4/18/2019

 */
/**/
TickerBlock& Database::operator[](std::string const &a_ticker) {
    return *m_db[a_ticker];
}


/**/
/*

 Database::getTickers()

 NAME

    Database::getTickers - returns the vector of tickers whose
    price data are available

 SYNOPSIS

    std::vector<std::string> Database::getTickers()

 DESCRIPTION

    This function returns the vector of available tickers.

 RETURNS

    Returns a vector

 AUTHOR

    Bibhash Mulmi

 DATE

    8/8/2019

 */
/**/
std::vector<std::string> Database::getTickers() {
    return m_availableTickers;
}


/**/
/*

 Database::getTradingDates()

 NAME
    Database::getTradingDates - returns the vector of trading
    dates

 SYNOPSIS

     Database::getTradingDates();

 DESCRIPTION

    This is an accessor for the reference trading dates.

 RETURNS

    Returns vector of DateTime object.

 AUTHOR

    Bibhash Mulmi

 DATE

    8/8/2019

 */
/**/
std::vector<DateTime> Database::getTradingDates() {
    return m_referenceDates;
}
