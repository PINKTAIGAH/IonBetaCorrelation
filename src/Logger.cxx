#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

#include "Logger.hh"
#include "ArgumentParser.hh"

// Logger Methods

void Logger::Log(const std::string& message, Level level){

  if (ArgumentParser::Instance().GetBoolValue("noansi")){
    std::cout << '[' << GetCurrentTime() << ']' 
              << '[' << LevelToString(level) << "] "
              << message << std::endl;
  }else{
    std::cout << '[' << Colours::CYAN << GetCurrentTime() << Colours::RESET << ']' 
              << '[' << GetLevelColour(level) << LevelToString(level) << Colours::RESET << "] "
              << message << std::endl;
  }
}

std::string Logger::GetCurrentTime(){
  auto now = std::chrono::system_clock::now();
  auto timeT = std::chrono::system_clock::to_time_t(now);
  auto millisecond = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

  std::stringstream ss;
  ss << std::put_time(std::localtime(&timeT), "%Y-%m-%d %H:%M:%S")
     << "." << std::setfill('0') << std::setw(3) << millisecond.count();
  
  return ss.str();
}

std::string Logger::LevelToString(Level level){
  switch (level){
    case Level::INFO: return "INFO";
    case Level::WARNING: return "WARNING";
    case Level::ERROR: return "ERROR";
    case Level::DEBUG: return "DEBUG";
    case Level::FATAL: return "FATAL";
    default: return "UNKNOWN";
  }
}

std::string Logger::GetLevelColour(Level level){
  switch (level){
    case Level::INFO: return Colours::GREEN;
    case Level::WARNING: return Colours::YELLOW;
    case Level::ERROR: return Colours::RED;
    case Level::DEBUG: return Colours::BLUE;
    case Level::FATAL: return Colours::RED;
    default: return Colours::WHITE;
  }
}

// Scoped Timer implementation
Logger::ScopedTimer::ScopedTimer(const std::string& label, Level level)
  : label(label), level(level), startTime(std::chrono::high_resolution_clock::now()) {}

Logger::ScopedTimer::~ScopedTimer(){
  auto endTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration<double>(endTime-startTime).count();
  Logger::Log(label + " took " + std::to_string(duration) + " seconds", level);
}

