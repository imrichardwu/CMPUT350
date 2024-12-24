#pragma once

// for size_t (can represent size of main memory)
#include <cstdlib>

class String {
public:
    // constructor: initializes String with copy of 0-terminated C-string
    String(const char *p);

    // destructor (when can shared data be released?)
    ~String();

    // copy constructor (how does this change reference counts?)
    String(const String &x);

    // assignment operator (how does this change reference counts?)
    String &operator=(const String &x);

    // return number of non-0 characters in string
    size_t size() const;

    // return reference count
    size_t ref_count() const;

    // returns pointer to C-string
    const char *cstr() const;

private:
    // data containing character strings
    // shared by Strings when copying/assigning
    struct SharedData {
        char *data;      // 0-terminated char array
        size_t n;        // number of non-0 characters in string
        size_t count;    // reference count, how many Strings share this object?
    };

    SharedData *shared;

    // release shared object
    void release();
};
