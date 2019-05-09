#include <iostream>
#include "ConfigParser.h"
#include "Database.h"

int main(int argc, char** argv) {
    ConfigParser a(argv[1]);
    a.ParseData();
    Database db = Database::GetInstanceOfDb(a.GetConfigValues("universe", "universe_file"), a.GetConfigValues("universe", "price_data"));

    return 0;
}