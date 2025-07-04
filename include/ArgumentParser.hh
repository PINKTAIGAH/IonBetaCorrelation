// ArgumentParser.h
#ifndef ARGUMENT_PARSER_H
#define ARGUMENT_PARSER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

class ArgumentParser {
private:
  // Singleton pattern
  explicit ArgumentParser();

  std::unordered_map<std::string, std::string> arguments;
  std::vector<std::string> positionalArgs;
  std::unordered_map<std::string, bool> requiredFlags;
  std::unordered_map<std::string, std::string> flagDescriptions;
  std::string programName;

public:

  // Singleton Methods
  static ArgumentParser& Instance();

  //Delete move/copy semantics
  ArgumentParser(const ArgumentParser&) = delete;
  ArgumentParser& operator=(const ArgumentParser&) = delete;
  ArgumentParser(const ArgumentParser&&) = delete;
  ArgumentParser& operator=(const ArgumentParser&&) = delete;

  // Methods    
  // Add a flag definition (optional with default value)
  void AddFlag(const std::string& flag, const std::string& description = "", 
               bool required = false, const std::string& defaultValue = "");

  // Parse command line arguments
  bool Parse(int argc, char* argv[]);

  // Get value for a specific flag
  std::string GetValue(const std::string& flag) const;

  // Check if flag exists
  bool HasFlag(const std::string& flag) const;

  // Get all positional arguments
  const std::vector<std::string>& GetPositionalArgs() const;

  // Get specific positional argument by index
  std::string GetPositionalArg(size_t index) const;

  // Print help/usage information
  void PrintHelp() const;

  // Print all parsed arguments (for debugging)
  void PrintParsedArgs() const;
};

#endif // ARGUMENT_PARSER_H