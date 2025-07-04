#include <iostream>
#include <stdexcept>

#include "Logger.hh"
#include "ArgumentParser.hh"

// Singleton Methods
ArgumentParser::ArgumentParser() {}

ArgumentParser& ArgumentParser::Instance(){
  static ArgumentParser instance;
  return instance;
}

// Class Methods

void ArgumentParser::AddFlag(const std::string& flag, const std::string& description, bool required, const std::string& defaultValue) {
    flagDescriptions[flag] = description;
    requiredFlags[flag] = required;
    if (!defaultValue.empty()) {
        arguments[flag] = defaultValue;
    }
}

bool ArgumentParser::Parse(int argc, char* argv[]) {
  if (argc > 0) {
    programName = argv[0];
  }

  for (int idx = 1; idx < argc; ++idx) {
    std::string arg = argv[idx];
      
    // Handle flags starting with - or --
    if (arg[0] == '-') {
      std::string flag;
          
      // Handle long flags (--flag)
      if (arg.length() > 1 && arg[1] == '-') {
        flag = arg.substr(2);
      }else {
        // Handle short flags (-f)
        flag = arg.substr(1);
      }
          
      // Check if this flag expects a value
      if (idx + 1 < argc && argv[idx + 1][0] != '-') {
        // Next argument is the value
        arguments[flag] = argv[idx + 1];
        ++idx; // Skip the value argument
      } else {
        // Flag without value (boolean flag)
        arguments[flag] = "true";
      }
    }else {
      // Handle positional arguments
      positionalArgs.push_back(arg);
    }
  }

  // Check if all required flags are present
  bool missingFlag = false;
  for (const auto& pair : requiredFlags) {
    if (pair.second && arguments.find(pair.first) == arguments.end()) {
      Logger::Log("Required flag -" + pair.first + " is missing", Logger::Level::ERROR);
      missingFlag = true;
    }
  }

  if (missingFlag) return false;
  else return true;
}

std::string ArgumentParser::GetValue(const std::string& flag) const {
  auto it = arguments.find(flag);
  if (it != arguments.end()) {
    return it->second;
  }
  return "";
}

bool ArgumentParser::HasFlag(const std::string& flag) const {
  return arguments.find(flag) != arguments.end();
}

const std::vector<std::string>& ArgumentParser::GetPositionalArgs() const {
    return positionalArgs;
}

std::string ArgumentParser::GetPositionalArg(size_t index) const {
  if (index < positionalArgs.size()) {
    return positionalArgs[index];
  }
  return "";
}

void ArgumentParser::PrintHelp() const {
  std::cout << "/////////////////////// How to use Program ///////////////////////" << std::endl;
  std::cout << "Usage: " << programName << " [options] [positional arguments]" << std::endl;
  std::cout << "Options:" << std::endl;
    
  for (const auto& pair : flagDescriptions) {
    std::string required = requiredFlags.at(pair.first) ? " (required)" : "";
    std::cout << "  -" << pair.first << ", --" << pair.first 
              << "\t" << pair.second << required << std::endl;
  }
  std::cout << "//////////////////////////////////////////////////////////////////" << std::endl;
}

void ArgumentParser::PrintParsedArgs() const {
  Logger::Log(" Parsed arguments:");
  for (const auto& pair : arguments) {
    std::cout << "  " << pair.first << " = " << pair.second << std::endl;
  }
  
  if (!positionalArgs.empty()) {
    std::cout << "Positional arguments:" << std::endl;
    for (size_t i = 0; i < positionalArgs.size(); ++i) {
      std::cout << "  [" << i << "] = " << positionalArgs[i] << std::endl;
    }
  }
}