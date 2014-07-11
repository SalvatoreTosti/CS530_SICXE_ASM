/* Yvette Titus, Anthony Perata, Steve Rutherford, Salvatore Tosti
	Team Kansas
	masc1256
	prog3
	CS530, Spring 2014
	symtab.cc */
	
#include <map>
#include <string>
#include <sstream>
#include "symtab.h"
#include "symtab_exception.h"

using namespace std;	

symtab::symtab(){}
symtab::~symtab(){}
void symtab::add_symbol(string label,string address,unsigned int lineNum){
	map<string,string>::iterator symbol_iter = symbol_table.find(label);
	if(symbol_iter != symbol_table.end()){ //already in table
		string err_msg = "Symbol "+label+" declared a second time at line ";
		error_helper(err_msg,lineNum);}
	symbol_table[label] = address;}

bool symtab::contains_symbol(string s){
	map<string,string>::iterator iter = symbol_table.find(s);
	return iter != symbol_table.end();}

string symtab::get_value(string key){
	map<string,string>::iterator iter = symbol_table.find(key);
	if( iter == symbol_table.end()){return "";}
	return symbol_table[key];}
	
		

void symtab::error_helper(string s,unsigned int lineNum){
	stringstream ss;
	ss << lineNum;
	string lineString = ss.str();
	throw symtab_exception(s+lineString+ ".");}

