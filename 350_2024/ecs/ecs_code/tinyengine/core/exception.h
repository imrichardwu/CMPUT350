#ifndef TINYENGINE_CORE_EXCEPTION_H_
#define TINYENGINE_CORE_EXCEPTION_H_

#include <exception>
#include <string>

namespace tinyengine {

class TinyEngineException : public std::exception {
public:
    TinyEngineException(
        const std::string &message,
        const char *file_name,
        const char *function_signature,
        int line_number
    );
    [[nodiscard]] auto what() const noexcept -> const char * override;

private:
    std::string message;
};

// NOLINTNEXTLINE(*-macro-usage,*-pro-bounds-array-to-pointer-decay)
#define TE_EXCEPTION(msg) throw TinyEngineException(msg, __FILE__, __func__, __LINE__)

}    // namespace tinyengine

#endif    // TINYENGINE_CORE_EXCEPTION_H_
