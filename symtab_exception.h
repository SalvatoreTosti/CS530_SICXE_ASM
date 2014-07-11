/* Yvette Titus, Anthony Perata, Steve Rutherford, Salvatore Tosti
	Team Kansas
	masc1256
	prog3
	CS530, Spring 2014
	symtab_exception.h */

#ifndef SYMTAB_EXCEPTION_H
#define SYMTAB_EXCEPTION_H

using namespace std;

class symtab_exception {

public:
    symtab_exception(string s){message = s;}  
    symtab_exception(){message = "An error has occurred";}  
    string getMessage(){return message;}
    
private:
    string message;
};    

#endif
