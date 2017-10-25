#include "Convertions.h"

const char *Convertions::convert_string_down_c(string word){
	return word.c_str();
}
char *Convertions::convert_const_char_down(const char *letters){
    static char *cstr = new char[strlen(letters) + 1];
    return strcpy(cstr, letters);
}
char *Convertions::convert_string_down(string word){
    static char *cstr = new char[word.length() + 1];
    return strcpy(cstr, word.c_str());
}
