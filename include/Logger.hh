#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <chrono>

class Logger {

  public:

    enum class Level { INFO, WARNING, ERROR, DEBUG, FATAL};

    static void Log(const std::string& message, Level level = Level::INFO);

    // RAII Scoped Timer
    class ScopedTimer{

      public:
        ScopedTimer(const std::string& label, Level level = Level::INFO);
        ~ScopedTimer();

      private:
        std::string label;
        Level level;
        std::chrono::high_resolution_clock::time_point startTime;

    };

  private:

    static std::string GetCurrentTime();
    static std::string LevelToString(Level level);

};

#endif