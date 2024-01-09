#include "Logger.hpp"

Logger::Logger(std::string filename)
{    
    try
    {
        logfile.open(filename, std::ios_base::in | std::ios_base::out | std::ios::app);        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }    
    
}
Logger::~Logger()
{
    if(logfile.is_open())
    {
        logfile.close();
    }
}

void Logger::readLog()
{
    shared_mutex.lock_shared();
    logfile.seekp(0);
    std::list<std::string> loglines;
    std::string line;
    if (logfile.is_open())
    {
        while (getline(logfile, line))
        {
            loglines.push_back(line);
        }
        std::cout << loglines.back() << std::endl;
    }
    logfile.seekg(0, std::ios::end);
    shared_mutex.unlock_shared();
}

void Logger::writeLog(std::string message)
{
    shared_mutex.lock();
    logfile.seekg(0, std::ios::end);
    if (logfile.is_open())
    {
        logfile << message;      
    }
    logfile.seekp(0);
    shared_mutex.unlock();
}
