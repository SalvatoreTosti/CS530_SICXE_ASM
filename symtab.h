/* Yvette Titus, Anthony Perata, Steve Rutherford, Salvatore Tosti
	Team Kansas
	masc1256
	prog3
	CS530, Spring 2014
	symtab.h */
#ifndef SYMTAB_H
#define SYMTAB_H

#include <map>
#include <string>

using namespace std;

class symtab {
public:
	symtab();
	~symtab();
	
	void add_symbol(string,string,unsigned int);
	bool contains_symbol(string);
	string get_value(string);
private:
	map<string,string> symbol_table;
	
	void error_helper(string, unsigned int);
	
};
#endif
