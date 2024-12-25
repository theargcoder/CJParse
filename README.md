# CJParse   
JSON standard this lib follows is (*RFC 7159*): https://www.rfc-editor.org/rfc/rfc7159

CJParse is a library written in c++ to parse any json file into a CJParse struct containing all the original contents parsed into memory exactly as they appear in the JSON file, following the above mentioned JSON standard.

Its very compact and portable since its only depenancy is the c++ Standard Library.

It has an internal class dedicated to Generating JSON strings (with options more on that below).

Can also be used to map live memory into a JSON file (will add soon)

At this moment (Dec 25st, 2024) it does not have error handling or any type of exception or termination; Including message (both inside c++ and print to terminal) to trace exactly in what line and character the parsing error happened. Will add these features soon. 

FUNCIONALLITY OF class "cjparse_json_parser"
    this class parses the inputed JSON string into memory, objects DO NOT retain the original order since they are contained in a "std::unordered_map" this is by design since objects are name-value pairs and they should be accesed by key (aka name).
    Now arrays DO retain their original order, the exact same one as in their string. These are cointained in a "std::vector".
    All 4 of the other JSON values are stored in their "equivalent" datatypes in c++, these been:
        JSON STRING = std::string
        JSON NUMBER = int or double (depending on what it is)
        JSON BOOL = bool
        JSON NULL = std::any. (may change that but for now (Dec 25th 2024) its staying that way)

FUNCTIONALITY OF class "cjparse_json_generator"
    this class can generate the standard obliging (see standard in beggining of this file) for the inputed "cjparse::cjparse_json_value" not to be confused with "cjparse::json_value" this is designed this way so if you modify or add     any elements to the previously parsed or newly created "cjparse::cjparse_json_value" you can still generate JSON with comfort     and quickly into a "std::string" (contained in "cjparse_json_generate::JSON_string").
        THIS CLASS HAS 2 Generate OPTIONS as of (DEC 25th 2024)
            1. "cjparse_json_generate::pretty = false"
                This option would generate a "compact" JSON standard obliging string
            2. "cjparse_json_generate::pretty = true"
                This option would generate a "pretty" or "more" human redable JSON standard obliging string.
    NOTE: this class will be modified so it can generate option 2 with more customization like wider more whitespace, end of line 
    after certain patterns, generate arrays compactly but the rest pretty and other options.

IMPORTANT DISCLAIMER
  As of (Dec 25th 2024) this library cannot correctly parse decimal places (will fix soon) and exponentials are unknown and might throw an exception.
    
