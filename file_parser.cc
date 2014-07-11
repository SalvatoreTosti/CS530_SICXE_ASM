/* Yvette Titus, Anthony Perata, Steve Rutherford, Salvatore Tosti
	masc1256
	prog1
	CS530, Spring 2014
	file_parser.cc */
	
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>

#include "file_parser.h"
#include "file_parse_exception.h"

using namespace std;

file_parser::file_parser(string path){file_path = path;}	
file_parser::~file_parser(){}

void file_parser::read_file(){
	string line;
	ifstream infile;
	infile.open(file_path.c_str());
	unsigned int lineNum = 1;
	while(!infile.eof()){
		getline(infile,line);
		string buffer;
		stringstream stream(line);

		struct Row tmpRow;
		string *labelpntr = &tmpRow.label;
		string *opcodepntr = &tmpRow.opcode;
		string *operandpntr = &tmpRow.operand;
		string *commentpntr = &tmpRow.comment;

		line = labelGrabber(lineNum,line,labelpntr);
		line = opcodeGrabber(lineNum,line,opcodepntr);
		line = operandGrabber(lineNum,line,operandpntr);
		line = commentGrabber(lineNum,line,commentpntr);	
		
		fileContainer.push_back(tmpRow);
		lineNum++;}
		
	reverse(fileContainer.end(),fileContainer.begin());
	infile.close();}
	
string file_parser::labelGrabber(unsigned int lineNum,string line,string *label){
	if((line.empty())  || (line.at(0) == '.')){
		pointer_to_empty(label);
		return line;}
	else if(isspace(line.at(0))){ 
		pointer_to_empty(label);
		line = wspace_clear_with_empty_check(line,label);
		return line;}
	else if(!isalpha(line.at(0))){
		error_helper("invalid label at line ",lineNum);
		return line;} //not an acutal return, keeps complier 
	else{
		string labelstr = "";
		char tmp = line.at(0);
		while(!isspace(tmp)){
			labelstr += line.at(0);
			line = line.erase(0,1);
			tmp = line.at(0);}
		*label = labelstr;
		return line;}
		}
	
string file_parser::opcodeGrabber(unsigned int lineNum, string line,string *opcode){
	if((line.empty())){
		pointer_to_empty(opcode);
		return line;}
		
	line = wspace_clear_with_empty_check(line,opcode);
	if(line.size() == 0){return line;}	//protects from trailing white space
	
	if((line.at(0) == '.')){
		pointer_to_empty(opcode);
		return line;}
	else{
		line = grab_single_token(line,opcode);
		return line;}
		/*string opcdstr = "";
		char tmp = line.at(0);
		while(!isspace(tmp)){
			opcdstr += line.at(0);
			line = line.erase(0,1);
			if(line.size()!=0){tmp = line.at(0);}
			else{
				*opcode = opcdstr;
				return line;}
		}
		*opcode = opcdstr;
		return line;}*/
	}
	
string file_parser::operandGrabber(unsigned int lineNum,string line,string *operand){
	if(line.size() == 0){
		pointer_to_empty(operand);
		return line;}
	line = wspace_clear_with_empty_check(line,operand);
	if(line.size() == 0){return line;} //protects from trailing white space

	if((line.at(0) == '.')){
		pointer_to_empty(operand);
		return line;}
		
	else{
		char tmp = line.at(0);
		char quote = '\'';
		string opndstr = "";
		string *opnd_str_pntr = &opndstr;
		while((!isspace(tmp)) && (!line.empty())){
			tmp = line.at(0);
			if(tmp == quote){
				opndstr+=tmp;
				line = line.erase(0,1);
				line = parseQuotes(lineNum,line, opnd_str_pntr);}
			if(!isspace(tmp)){			//This may look redundant, but it is not
				opndstr += tmp; 		//because the tmp is advanced after loop
				line = line.erase(0,1);}	//check, this is cruical, otherwise
			}								//there is trailing whitespace
	*operand = opndstr;
	return line;}
	}
	

string file_parser::parseQuotes(unsigned int lineNum, string line, string *opndptr){
	if(line.size() == 0){error_helper("invalid operand at line ",lineNum);}
	char tmp = line.at(0);
	char quote = '\'';
	while(tmp != quote){
		*opndptr += tmp;
		line = line.erase(0,1);
		if((line.size()==0) && (tmp != quote)){error_helper("invalid operand at line ",lineNum);}
		tmp = line.at(0);}
	return line;}

string file_parser::commentGrabber(unsigned int lineNum,string line,string *comment){
	if(line.size() == 0){
		pointer_to_empty(comment);
		return line;}
		
	line = wspace_clear_with_empty_check(line,comment);
	if(line.size()==0){return line;}
	/*char tmp = line.at(0);
	while(isspace(tmp)){
		line.erase(0,1);
		if(line.size()!=0){tmp = line.at(0);}
		else{
			pointer_to_empty(comment);
			return line;}
		}*/
		
	if(line.at(0) != '.'){
		error_helper("too many tokens at line ",lineNum);
		return line;}	//not acutal return, keeps compiler from complaining
	else{
		string commentstr = "";
		string::iterator it;
		for(it=line.begin();it<line.end();it++){commentstr+= *it;}
		*comment = commentstr;
		return line;}
	}
	
void file_parser::pointer_to_empty(string *pntr){
	string empty = "";
	*pntr = empty;}

void file_parser::error_helper(string s,unsigned int lineNum){
	stringstream ss;
	ss << lineNum;
	string lineString = ss.str();
	throw file_parse_exception(s+lineString+ ".");}

string file_parser::clear_leading_whitespace(string line){
	char tmp = line.at(0);
	while(isspace(tmp)){
		line.erase(0,1);
		tmp = line.at(0);}
	return line;}
string file_parser::wspace_clear_with_empty_check(string line,string *pntr){
	char tmp = line.at(0);
	while(isspace(tmp)){
		line.erase(0,1);
		if(line.size()!=0){tmp = line.at(0);}
		else{
			pointer_to_empty(pntr);
			return line;}
		}
	return line;}
	
string file_parser::grab_single_token(string line,string *pntr){
	string token = "";
	char tmp = line.at(0);
	while(!isspace(tmp)){
		token += line.at(0);
		line = line.erase(0,1);
		if(line.size()!=0){tmp = line.at(0);}
		else{
			*pntr = token;
			return line;}
		}
	*pntr = token;
	return line;}
	

	
string file_parser::get_token(unsigned int x, unsigned int y){
	Row tmpRow = fileContainer.at(x);
	if(y==0){return tmpRow.label;}
	else if(y==1){return tmpRow.opcode;}
	else if(y==2){return tmpRow.operand;}
	else if(y==3){return tmpRow.comment;}
	else{
		//cout << "Invalid position given to get_token." << endl;
		return "";}}
	
void file_parser::print_file(){
	if ( !(size() > 0) ) //if size isn't greater than 0, a file hasn't been read
		throw file_parse_exception("A file must be successfully read before it can be printed.");
		
	//Print Formatting constants
	int COLUMNWIDTH = 80;
	int LABELSPACE = 12;
	int OPCODESPACE = 12;
	int OPERANDSPACE = 12;

	//Print column headers
	cout << file_path << endl;
	cout << left << setw(LABELSPACE) << "Label "
		 << left << setw(OPCODESPACE) << "Opcode "
		 << left << setw(OPERANDSPACE) << "Operand " 
		 << "Comments " << endl;
	
	//Print line
	for (int i = 0; i < COLUMNWIDTH; i++)
		cout << '-';
	cout << endl;

	//Print assembly line by line, separated by token type
	for (int i = 0; i < (size() - 1); i++)
		cout << left << setw(LABELSPACE) << fileContainer[i].label 
			<< left << setw(OPCODESPACE) << fileContainer[i].opcode 
			<< left << setw(OPERANDSPACE) << fileContainer[i].operand 
			<< "  " << fileContainer[i].comment << endl;
	}
		
int file_parser::size(){return fileContainer.size();}
