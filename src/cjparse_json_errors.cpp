#include <stdexcept>
#include <string>

class cjparse_error : public std::runtime_error
{
public:
  enum class ERROR_CODE
  {
    JSON_SYNTAX_ERROR = 0,
    JSON_INVALID_JSON_VALUE = 1,
  };

public:
  cjparse_error (int line, int char_number, ERROR_CODE cd)
      : std::runtime_error (error_code_to_str (cd, line, char_number))
  {
    this->line = line;
    this->char_number = char_number;
    this->code = cd;
  };

public:
  int line, char_number;
  ERROR_CODE code;

private:
  std::string
  error_code_to_str (const ERROR_CODE &cd, const int &line,
                     const int &char_number)
  {
    if (cd == ERROR_CODE::JSON_SYNTAX_ERROR)
      return "CJPARSE: JSON syntax error in line: " + std::to_string (line)
             + " character: " + std::to_string (char_number) + "\n";
    else if (cd == ERROR_CODE::JSON_INVALID_JSON_VALUE)
      return "CJPARSE: invalid JSON Value in line: " + std::to_string (line)
             + " character: " + std::to_string (char_number) + "\n";
    else if (cd == ERROR_CODE::JSON_INVALID_JSON_VALUE)
      return "CJPARSE: invalid JSON Value in line: " + std::to_string (line)
             + " character: " + std::to_string (char_number) + "\n";

    return "CJPARSE: UNKNOWN ERROR\n";
  }
};
