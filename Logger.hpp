#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <shared_mutex>

class Logger
{
    public:
        Logger(std::string filename);
        ~Logger();
        void readLog();
        void writeLog(std::string message);
    private:
        std::fstream logfile;
        int lastMessageID;
        std::shared_mutex shared_mutex;
};