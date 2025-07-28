#ifndef ARGUMENT_PARSER_H
#define ARGUMENT_PARSER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <optional>
#include <set>

class ArgumentParser {
private:
    // Singleton pattern
    explicit ArgumentParser();
    
    struct FlagInfo {
        std::string description;
        std::string defaultValue;
        bool required;
        bool isBooleanFlag;
        std::string shortFlag;  // Single character alternative
        
        FlagInfo(const std::string& desc = "", const std::string& def = "", 
                bool req = false, bool isBool = false, const std::string& shortF = "")
            : description(desc), defaultValue(def), required(req), 
              isBooleanFlag(isBool), shortFlag(shortF) {}
    };
    
    std::unordered_map<std::string, std::string> arguments;
    std::vector<std::string> positionalArgs;
    std::unordered_map<std::string, FlagInfo> flagInfos;
    std::unordered_map<std::string, std::string> shortToLongFlag; // Maps short flags to long flags
    std::string programName;
    std::string programDescription;
    std::vector<std::string> parseErrors;
    
    // Helper methods
    std::string normalizeFlagName(const std::string& flag) const;
    bool isShortFlag(const std::string& arg) const;
    bool isLongFlag(const std::string& arg) const;
    std::string extractFlagName(const std::string& arg) const;
    
public:
    // Singleton Methods
    static ArgumentParser& Instance();
    
    // Delete move/copy semantics
    ArgumentParser(const ArgumentParser&) = delete;
    ArgumentParser& operator=(const ArgumentParser&) = delete;
    ArgumentParser(ArgumentParser&&) = delete;
    ArgumentParser& operator=(ArgumentParser&&) = delete;
    
    // Configuration Methods
    void SetProgramDescription(const std::string& description);
    
    // Add a flag definition with optional short flag
    void AddFlag(const std::string& longFlag, const std::string& description = "", 
                 bool required = false, const std::string& defaultValue = "",
                 const std::string& shortFlag = "");
    
    // Add a boolean flag (doesn't require a value)
    void AddBooleanFlag(const std::string& longFlag, const std::string& description = "",
                        const std::string& shortFlag = "");
    
    // Parse command line arguments
    bool Parse(int argc, char* argv[]);
    
    // Get value for a specific flag
    std::string GetValue(const std::string& flag) const;
    
    // Get value with default if not found
    std::string GetValue(const std::string& flag, const std::string& defaultValue) const;
    
    // Get boolean value (useful for boolean flags)
    bool GetBoolValue(const std::string& flag) const;
    
    // Get optional value (returns std::nullopt if not found)
    std::optional<std::string> GetOptionalValue(const std::string& flag) const;
    
    // Check if flag exists
    bool HasFlag(const std::string& flag) const;
    
    // Get all positional arguments
    const std::vector<std::string>& GetPositionalArgs() const;
    
    // Get specific positional argument by index
    std::string GetPositionalArg(size_t index) const;
    
    // Get specific positional argument with default
    std::string GetPositionalArg(size_t index, const std::string& defaultValue) const;
    
    // Get number of positional arguments
    size_t GetPositionalArgCount() const;
    
    // Print help/usage information
    void PrintHelp() const;
    
    // Print all parsed arguments (for debugging)
    void PrintParsedArgs() const;
    
    // Get parse errors
    const std::vector<std::string>& GetParseErrors() const;
    
    // Clear all data (useful for testing)
    void Clear();
    
    // Validate that all required arguments are present
    bool ValidateArguments() const;
    
    // Test method to demonstrate parsing behavior
    void TestParsing() const;
};

#endif