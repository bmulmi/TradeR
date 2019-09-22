#include <iostream>
#include "Simulator.h"
#include "TradingModel.h"

int main(int argc, char** argv) {
    TradingModel test(argc, argv);
    test.runSim();
    return 0;
}