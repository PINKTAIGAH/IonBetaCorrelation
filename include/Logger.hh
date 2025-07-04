#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <chrono>

namespace Colours {
    // Text Colours
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";
    const std::string BLACK = "\033[30m";
    
    // Bright Colours
    const std::string BRIGHT_RED = "\033[91m";
    const std::string BRIGHT_GREEN = "\033[92m";
    const std::string BRIGHT_YELLOW = "\033[93m";
    const std::string BRIGHT_BLUE = "\033[94m";
    const std::string BRIGHT_MAGENTA = "\033[95m";
    const std::string BRIGHT_CYAN = "\033[96m";
    const std::string BRIGHT_WHITE = "\033[97m";
    
    // Background Colours
    const std::string BG_RED = "\033[41m";
    const std::string BG_GREEN = "\033[42m";
    const std::string BG_YELLOW = "\033[43m";
    const std::string BG_BLUE = "\033[44m";
    const std::string BG_MAGENTA = "\033[45m";
    const std::string BG_CYAN = "\033[46m";
    const std::string BG_WHITE = "\033[47m";
    
    // Text Styles
    const std::string BOLD = "\033[1m";
    const std::string UNDERLINE = "\033[4m";
    const std::string ITALIC = "\033[3m";
    const std::string RESET = "\033[0m";
}

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
    static std::string GetLevelColour(Level level);

};

#endif