#include <map>
#include <string>
#include <vector>

namespace raymarcher {
class Arguments
{
  //-------------------------------------------------------------------------
public:
  Arguments();
  // Type of argument
  enum argType
  {
    FIXED,   // cannot be skipped, must be specified by user
    OPTIONAL // may not be specified by user
  };
  enum parseResult
  {
    OK = 0,
    ARGUMENT_INVALID_FORMAT // one of arguments was specified without complying
                            // to provided format
  };
  enum paramLogic
  {
    WITH_PARAM = true,
    NO_PARAM = false
  };
  // Set which parameters (FIXED or OPTIONAL are expected firs)
  void First(argType type);

  // Specify next fixed argument 'name' with regular language 'format'
  // NOTE: fixed arguments are expected in the order they were added using
  // addFixed
  void AddFixed(std::string name, std::string format = "");

  // Add optionial argument, which is expected as -K, where K = shortKey
  // Example: AddOpt(--output-file, o);
  void AddOpt(std::string name,
              std::string shortKey,
              bool hasParam = WITH_PARAM,
              std::string format = "");

  // Add optionial argument, which is expected as -K, where K = shortKey or key
  void AddLongOpt(std::string name,
                  std::string key,
                  std::string shortKey,
                  bool hasParam = WITH_PARAM,
                  std::string format = "");

  // Parse provided command-line
  int Parse(int argc, const char** argv);

  // Has user entered any argument with name 'name'
  bool HasArgument(std::string name);
  bool AllowRedefinition(bool allow);
  std::string Value_or(std::string argument, std::string defaultValue);

  // Returns value of argument, specified with name 'arg'
  std::string operator[](const std::string arg);

  std::string GetErrorMessage() { return this->errorMessage; }
  const std::map<std::string, std::string>& getMap() { return this->arguments; }

  //-------------------------------------------------------------------------
private:
  // Stores which kind of argument (FIXED/OPT) is expected to appear first
  // DEFAULT: OPTIONAL
  argType argumentPrecedence;

  // map argumentName to its value
  std::map<std::string, std::string> arguments;
  std::vector<std::pair<std::string, std::string>> fixedArguments;

  // option => (option name, hasParam)
  std::map<std::string, std::pair<std::string, bool>> options;
  //
  bool HasFormat(std::string format, std::string testedWord);

  // Do we have -s (s = shortOption) in our database ?
  bool HasOption(std::string option);

  bool HasOptionParam(std::string longOpt);
  std::string GetOptionName(std::string opt);

  std::string errorMessage;

  bool isRedefinitionAllowed;
};
}
