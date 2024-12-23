# CJParse   
JSON standard this lib follows is (*RFC 7159*): https://www.rfc-editor.org/rfc/rfc7159

CJParse is a library written in c++ to parse any json file into a CJParse struct containing all the original contents parsed into memory exactly as they appear in the JSON file, following the above mentioned JSON standard.

Its very compact and portable since its only depenancy is the c++ Standard Library.

It has a class dedicated to printing the JSON data stored into memory to the terminal.

Can also be used to map live memory into a JSON file (will add soon)

At this moment (Dec 21st, 2024) it does not have error handling or any type of exception or termination.
Will add these features soon.
