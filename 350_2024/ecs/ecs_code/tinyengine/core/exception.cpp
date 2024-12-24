#include "tinyengine/core/exception.h"

#include <iostream>
#include <sstream>
#include <string>

namespace tinyengine {

TinyEngineException::TinyEngineException(
    const std::string &input_message,
    const char *file_name,
    const char *function_signature,
    int line_number
) {
    std::ostringstream os;
    os << "File: " << file_name << ":" << line_number << std::endl;
    os << "In function: " << std::endl;
    os << "\t" << function_signature << std::endl;
    os << std::endl;
    os << "TinyEngine Error: " << input_message << std::endl;
    message = os.str();
}

auto TinyEngineException::what() const noexcept -> const char * {
    return message.c_str();
}

}    // namespace tinyengine
