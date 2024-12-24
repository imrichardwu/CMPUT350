#ifndef BASE_GAME_H_
#define BASE_GAME_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

// Base state
class BaseState {
public:
    BaseState() = default;
    BaseState(const BaseState &) = default;
    BaseState& operator=(const BaseState &) = default;
    virtual ~BaseState(){};
    virtual void apply_action(int action) = 0;
    virtual std::vector<int> child_actions() const = 0;
    virtual double get_heuristic() const = 0;
    virtual uint64_t get_hash() const = 0;
    virtual bool is_solution() const = 0;
    virtual void assign(BaseState *other) = 0;
    virtual std::string to_str() const = 0;

    // Clone (deepcopy) of of state in a new unique_ptr (uses trick)
    auto clone() const {
        return std::unique_ptr<BaseState>(clone_impl());
    }

    friend std::ostream &operator<<(std::ostream &os, const BaseState &state);

    // Equality through base pointer (uses trick)
    friend bool operator==(const BaseState &lhs, const BaseState &rhs);

protected:
    virtual BaseState *clone_impl() const = 0;
    virtual bool is_equal(const BaseState &other) const = 0;
};


// Friend functions
inline std::ostream &operator<<(std::ostream &os, const BaseState &state) {
    os << state.to_str();
    return os;
}

inline bool operator==(const BaseState &lhs, const BaseState &rhs) {
    return typeid(lhs) == typeid(rhs) && lhs.is_equal(rhs);
}


#endif    // SLIDING_PUZZLE_H_