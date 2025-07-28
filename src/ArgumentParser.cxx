#include "ArgumentParser.hh"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

// Singleton Methods
ArgumentParser::ArgumentParser() {}

ArgumentParser& ArgumentParser::Instance() {
  static ArgumentParser instance;
  return instance;
}

// Helper Methods
std::string ArgumentParser::normalizeFlagName(const std::string& flag) const {
  // Remove leading dashes and convert to lowercase for consistency
  std::string normalized = flag;
  while (!normalized.empty() && normalized[0] == '-') {
    normalized = normalized.substr(1);
  }
  return normalized;
}

bool ArgumentParser::isShortFlag(const std::string& arg) const {
  return arg.length() == 2 && arg[0] == '-' && arg[1] != '-';
}

bool ArgumentParser::isLongFlag(const std::string& arg) const {
  return arg.length() > 2 && arg[0] == '-' && arg[1] == '-';
}

std::string ArgumentParser::extractFlagName(const std::string& arg) const {
  if (isLongFlag(arg)) {
    return arg.substr(2);
  } else if (isShortFlag(arg)) {
    std::string shortFlag = arg.substr(1);
    auto it = shortToLongFlag.find(shortFlag);
    return (it != shortToLongFlag.end()) ? it->second : shortFlag;
  }
  return arg;
}

// Configuration Methods
void ArgumentParser::SetProgramDescription(const std::string& description) {
  programDescription = description;
}

void ArgumentParser::AddFlag(const std::string& longFlag, const std::string& description, bool required, const std::string& defaultValue, const std::string& shortFlag) {
  std::string normalizedLong = normalizeFlagName(longFlag);
    
  flagInfos[normalizedLong] = FlagInfo(description, defaultValue, required, false, shortFlag);
    
  // Set default value if provided
  if (!defaultValue.empty()) {
    arguments[normalizedLong] = defaultValue;
  }
    
  // Map short flag to long flag
  if (!shortFlag.empty()) {
    shortToLongFlag[shortFlag] = normalizedLong;
  }
}

void ArgumentParser::AddBooleanFlag(const std::string& longFlag, const std::string& description, const std::string& shortFlag) {
  std::string normalizedLong = normalizeFlagName(longFlag);
    
  flagInfos[normalizedLong] = FlagInfo(description, "false", false, true, shortFlag);
  arguments[normalizedLong] = "false"; // Default to false
    
  // Map short flag to long flag
  if (!shortFlag.empty()) {
    shortToLongFlag[shortFlag] = normalizedLong;
  }
}

bool ArgumentParser::Parse(int argc, char* argv[]) {
  parseErrors.clear();
    
  if (argc > 0) {
    programName = argv[0];
  }
    
  for (int idx = 1; idx < argc; ++idx) {
    std::string arg = argv[idx];
        
    // Handle help flag specially
    if (arg == "-h" || arg == "--help") {
      PrintHelp();
      return false; // Don't continue parsing
    }
        
    // Handle flags starting with - or --
    if (arg[0] == '-') {
      std::string flagName = extractFlagName(arg);
            
      // Check if this is a known flag
      if (flagInfos.find(flagName) == flagInfos.end()) {
        parseErrors.push_back("Unknown flag: " + arg);
        continue;
      }
            
      const FlagInfo& info = flagInfos[flagName];
            
      if (info.isBooleanFlag) {
        // Boolean flag - just set to true, don't consume next argument
          arguments[flagName] = "true";
      } 
      else {
        // Flag that expects a value
        if (idx + 1 < argc && argv[idx + 1][0] != '-') {
          // Next argument is the value
          arguments[flagName] = argv[idx + 1];
          ++idx; // Skip the value argument
        } 
        else {
          parseErrors.push_back("Flag " + arg + " requires a value");
        }
      }
    } 
    else {
      // Handle positional arguments
      positionalArgs.push_back(arg);
    }
  }
    
  // Validate all required flags are present
  return ValidateArguments();
}

std::string ArgumentParser::GetValue(const std::string& flag) const {
  std::string normalizedFlag = normalizeFlagName(flag);
  auto it = arguments.find(normalizedFlag);
  return (it != arguments.end()) ? it->second : "";
}

std::string ArgumentParser::GetValue(const std::string& flag, const std::string& defaultValue) const {
  std::string value = GetValue(flag);
  return value.empty() ? defaultValue : value;
}

bool ArgumentParser::GetBoolValue(const std::string& flag) const {
  std::string value = GetValue(flag);
  return value == "true" || value == "1" || value == "yes" || value == "on";
}

std::optional<std::string> ArgumentParser::GetOptionalValue(const std::string& flag) const {
  std::string normalizedFlag = normalizeFlagName(flag);
  auto it = arguments.find(normalizedFlag);
  if (it != arguments.end()) return it->second; 
  return std::nullopt;
}

bool ArgumentParser::HasFlag(const std::string& flag) const {
  std::string normalizedFlag = normalizeFlagName(flag);
  return arguments.find(normalizedFlag) != arguments.end();
}

const std::vector<std::string>& ArgumentParser::GetPositionalArgs() const {
  return positionalArgs;
}

std::string ArgumentParser::GetPositionalArg(size_t index) const {
  return (index < positionalArgs.size()) ? positionalArgs[index] : "";
}

std::string ArgumentParser::GetPositionalArg(size_t index, const std::string& defaultValue) const {
  return (index < positionalArgs.size()) ? positionalArgs[index] : defaultValue;
}

size_t ArgumentParser::GetPositionalArgCount() const {
  return positionalArgs.size();
}

void ArgumentParser::PrintHelp() const {
  std::cout << "====================================================================\n";
  if (!programDescription.empty()) {
    std::cout << programDescription << "\n\n";
  }
  std::cout << "Usage: " << programName << " [options] [positional arguments]\n\n";
    
  if (!flagInfos.empty()) {
    std::cout << "Options:\n";
        
    // Calculate max width for alignment
    size_t maxWidth = 0;
    for (const auto& pair : flagInfos) {
      std::string flagDisplay = "  --" + pair.first;
      if (!pair.second.shortFlag.empty()) {
        flagDisplay += ", -" + pair.second.shortFlag;
      }
      maxWidth = std::max(maxWidth, flagDisplay.length());
    }
        
    for (const auto& pair : flagInfos) {
      std::string flagDisplay = "  --" + pair.first;
      if (!pair.second.shortFlag.empty()) {
        flagDisplay += ", -" + pair.second.shortFlag;
      }
            
      std::cout << std::left << std::setw(maxWidth + 4) << flagDisplay;
      std::cout << pair.second.description;
            
      if (pair.second.required) {
        std::cout << " (required)";
      } 
      else if (!pair.second.defaultValue.empty() && !pair.second.isBooleanFlag) {
        std::cout << " (default: " << pair.second.defaultValue << ")";
        }
      std::cout << "\n";
    }
  }
    
  std::cout << "  -h, --help              Show this help message\n";
  std::cout << "====================================================================\n";
}

void ArgumentParser::PrintParsedArgs() const {
  std::cout << "Parsed arguments:\n";
  for (const auto& pair : arguments) {
     std::cout << "  " << pair.first << " = " << pair.second << "\n";
  }
    
  if (!positionalArgs.empty()) {
    std::cout << "Positional arguments:\n";
    for (size_t i = 0; i < positionalArgs.size(); ++i) {
      std::cout << "  [" << i << "] = " << positionalArgs[i] << "\n";
    }
  }
}

const std::vector<std::string>& ArgumentParser::GetParseErrors() const { return parseErrors; }

void ArgumentParser::Clear() {
  arguments.clear();
  positionalArgs.clear();
  flagInfos.clear();
  shortToLongFlag.clear();
  parseErrors.clear();
  programName.clear();
  programDescription.clear();
}

bool ArgumentParser::ValidateArguments() const {
  bool valid = true;
  
  for (const auto& pair : flagInfos) {
    if (pair.second.required && arguments.find(pair.first) == arguments.end()) {
      std::cerr << "Error: Required flag --" << pair.first << " is missing\n";
      valid = false;
    }
  }
  
  return valid;
}

void ArgumentParser::TestParsing() const {
    std::cout << "\n=== PARSING TEST EXAMPLES ===\n";
    std::cout << "Command: program -v input.txt -o output.txt\n";
    std::cout << "Expected behavior:\n";
    std::cout << "  -v (boolean flag): true\n";
    std::cout << "  -o (value flag): output.txt\n";
    std::cout << "  positional[0]: input.txt\n\n";
    
    std::cout << "Command: program input.txt -v\n";
    std::cout << "Expected behavior:\n";
    std::cout << "  -v (boolean flag): true\n";
    std::cout << "  positional[0]: input.txt\n\n";
    
    std::cout << "Command: program -v -o output.txt input.txt\n";
    std::cout << "Expected behavior:\n";
    std::cout << "  -v (boolean flag): true\n";
    std::cout << "  -o (value flag): output.txt\n";
    std::cout << "  positional[0]: input.txt\n";
    std::cout << "==============================\n\n";
}