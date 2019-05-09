cmake_minimum_required(VERSION 3.13)
project(TradeR)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(
        TradeR main.cpp
        ConfigParser.cpp
        ConfigParser.h
        Utilities.cpp
        Utilities.h
        Database.cpp
        Database.h
        TickerBlock.cpp
        TickerBlock.h
        DateTime.cpp DateTime.h)