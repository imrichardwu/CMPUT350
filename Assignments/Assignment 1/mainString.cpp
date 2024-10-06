// tests String implementation

#include <iostream>

#include "String.h"

using namespace std;

String::String(const char *p) {
    shared = new SharedData;
    shared->n = strlen(p);
    shared->data = new char[shared->n + 1];
    strcpy(shared->data, p);
    shared->count = 1;
}

String::~String(){
	if(--shared->count == 0){
		delete[] shared->data;
		delete shared;
	}
}

String::String(const String &x){
	shared = x.shared;
	shared->count++;
}

String &String:: operator=(const String &x){
	if(shared != x.shared){
    if(--shared->count == 0){
      delete[] shared->data;
      delete shared;
    }
    shared = x.shared;
    shared->count++;
	}
	return *this;
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

int main() {
    String s("aaa"), t("bbbb");    // construct
    String u = t;                  // copy construct
    String v("cccc");              // construct
    v = s;                         // assign

    cout << "s: " << s.cstr() << " " << s.size() << " " << s.ref_count() << endl
         << "t: " << t.cstr() << " " << t.size() << " " << t.ref_count() << endl
         << "u: " << u.cstr() << " " << u.size() << " " << u.ref_count() << endl
         << "v: " << v.cstr() << " " << v.size() << " " << v.ref_count() << endl;

    return 0;
}

/*
  output:

s: aaa 3 2
t: bbbb 4 2
u: bbbb 4 2
v: aaa 3 2

*/
