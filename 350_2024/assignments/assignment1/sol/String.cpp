#include "String.h"

#include <cassert>
#include <cstring>

// okay in .cpp files, but not in .h files
using namespace std;

String::String(const char *p) {
    assert(p);

    shared = new SharedData;
    shared->n = strlen(p);
    shared->count = 1;
    shared->data = new char[shared->n + 1];    // 0-terminated!
    strcpy(shared->data, p);                   // dst, src
}

String::~String() {
    release();
}

String::String(const String &rhs) {
    shared = rhs.shared;
    ++shared->count;
}

String &String::operator=(const String &rhs) {
    if (this == &rhs) {
        return *this;
    }

    release();
    shared = rhs.shared;
    ++shared->count;
    return *this;
}

void String::release() {
    if (!--shared->count) {
        delete[] shared->data;    // [] CRUCIAL !
        delete shared;
    }
}

size_t String::size() const {
    return shared->n;
}

size_t String::ref_count() const {
    return shared->count;
}

const char *String::cstr() const {
    return shared->data;
}
