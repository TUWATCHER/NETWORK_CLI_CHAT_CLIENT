#include "Logger.hpp"

Logger::Logger(std::string filename)
{    
    try
    {
        logfile.open(filename, std::ios::trunc | std::ios_base::out | std::ios_base::in);        
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
    logfile.seekp(0, std::ios::end);
    shared_mutex.unlock_shared();
}

void Logger::writeLog(std::string message)
{
    shared_mutex.lock();
    if (logfile.is_open())
    {
        logfile << message;
        logfile.seekg(0, std::ios::beg);      
    }
    shared_mutex.unlock();
}
