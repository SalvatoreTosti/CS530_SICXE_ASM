/* Yvette Titus, Anthony Perata, Steve Rutherford, Salvatore Tosti
 Team Kansas
 masc1256
 prog2
 CS530, Spring 2014
 opcode_error_exception.h*/

#ifndef OPCODE_ERROR_EXCEPTION_H
#define OPCODE_ERROR_EXCEPTION_H

using namespace std;

class opcode_error_exception{

public:
    opcode_error_exception(string s) {message = s;}
    opcode_error_exception() {message = "An error has occurred";}
    string getMessage() {return message;}

private:
    string message;
};

#endif
