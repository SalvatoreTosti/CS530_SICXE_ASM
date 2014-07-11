/* Yvette Titus, Anthony Perata, Steve Rutherford, Salvatore Tosti
	masc1256
	prog1
	CS530, Spring 2014
	file_parser.h */

#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class file_parser {
    public:
        // ctor, filename is the parameter.  A driver program will read
        // the filename from the command line, and pass the filename to
        // the file_parser constructor.  Filenames must not be hard-coded.
        file_parser(string); 
        
        // dtor
        ~file_parser();
        
        // reads the source file, storing the information is some
        // auxiliary data structure you define in the private section. 
        // Throws a file_parse_exception if an error occurs.
        // if the source code file fails to conform to the above
        // specification, this is an error condition.     
        void read_file();    
        
        // returns the token found at (row, column).  Rows and columns
        // are zero based.  Returns the empty string "" if there is no 
        // token at that location. column refers to the four fields
        // identified above.
        string get_token(unsigned int, unsigned int);
        
        // prints the source code file to stdout.  Should not repeat 
        // the exact formatting of the original, but uses tabs to align
        // similar tokens in a column. The fields should match the 
        // order of token fields given above (label/opcode/operands/comments)
        void print_file();
        
        // returns the number of lines in the source code file
        int size();
        
    private:
		// your variables and private methods go here	
		string file_path;
		struct Row {
			string label;
			string opcode;
			string operand;
			string comment;
		};
		vector <Row> fileContainer;
		/*vector <string> commentHelper(vector <string>, string *comment);
		void remainderHelper(vector<string>, unsigned int, bool, string *lbl,
		string *opcd, string *ornd);
		vector<string> checkForQuotes(vector<string>, bool, string *opnd, bool *err);
		*/
		
		//new methods
		string labelGrabber(unsigned int,string,string *lbl);
		string opcodeGrabber(unsigned int,string,string *opcd);
		string operandGrabber(unsigned int,string,string *opnd);
		string commentGrabber(unsigned int,string,string *cmnt);
		string parseQuotes(unsigned int,string,string *strpnt);
		void pointer_to_empty(string *pntr);
		void error_helper(string,unsigned int);
		string clear_leading_whitespace(string);
		string wspace_clear_with_empty_check(string,string *pntr);
		string grab_single_token(string,string *pntr);
};

#endif   
