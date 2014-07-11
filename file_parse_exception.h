/* Yvette Titus, Anthony Perata, Steve Rutherford, Salvatore Tosti
	masc1256
	prog1
	CS530, Spring 2014
	Created by Alan Riggins
	file_parse_exception */

#ifndef FILE_PARSE_EXCEPTION_H
#define FILE_PARSE_EXCEPTION_H

using namespace std;

class file_parse_exception {

public:
    file_parse_exception(string s) {
        message = s;        
        }
        
    file_parse_exception() {
        message = "An error has occurred";
        }
        
    string getMessage() {
        return message;
    }
    
private:
    string message;
};    

#endif
