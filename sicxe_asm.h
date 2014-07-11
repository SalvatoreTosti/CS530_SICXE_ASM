/* Yvette Titus, Anthony Perata, Steve Rutherford, Salvatore Tosti
	Team Kansas
	masc1256
	prog3
	CS530, Spring 2014
	sicxe_asm.h */
#ifndef SICXE_ASM_H
#define SICXE_ASM_H

#include <string>	
#include <map>
#include "file_parser.h"
#include "opcodetab.h"
#include "symtab.h"

class sicxe_asm{
	public:
		sicxe_asm(string);
		~sicxe_asm();
		void pass1(file_parser,string);
	private:
		struct Listing_File_Row{
			int line_number;
			string address;
			string label;
			string opcode;
			string operand;
			string machine_code;};
		vector <Listing_File_Row> lis_container;	
	
		opcodetab opcode_table;
		symtab symbol_table;
		
		
		
		/*PASS 1*/
		void write_listing_file(string);
		bool is_assembler_directive(string);
		void pass1_assembler_directive_handler(bool *strt,bool *end,
			string,string,string,string *addr,unsigned int); 
		//directive helpers are called from assembler_directive_handler()	
		void start_directive_helper(string,string *pntr,unsigned int);
		void byte_directive_helper(string,string *pntr,unsigned int);
		void resb_directive_helper(string,string *pntr,unsigned int);
		void resw_directive_helper(string,string *pntr,unsigned int);
		
		void equ_directive_handler(string,string,string,unsigned int);
		void symtab_handler(string,string,string,unsigned int);
		string increase_address(string,int);
		
		unsigned int string_to_hex(string);
		unsigned int hex_add(unsigned int,unsigned int);
		string hex_to_string(unsigned int);
		string to_upper(string);
		bool is_number(string s);
		bool is_hex_number(string s);
		void operand_safety_check(string,unsigned int);
		void opcode_error_helper(string, unsigned int);
		void symtab_error_helper(string, unsigned int);
		
		/*PASS 2*/
		string BASE_ADDRESS;
		void pass2(string);
		string pass2_assembler_directive_handler(Listing_File_Row);
		string pass2_byte_directive_helper(Listing_File_Row);
		string pass2_word_directive_helper(Listing_File_Row);
		void pass2_base_directive_helper(Listing_File_Row);
		void pass2_nobase_directive_helper(Listing_File_Row);
		string machine_code_generator(Listing_File_Row);
		string nixbpe_generator(Listing_File_Row);
		bool is_valid_nixbpe(string);
		string apply_bitmask(string,string);
		string address_displacement(string,string);
		bool is_valid_pc_disp(int,bool);
		bool is_valid_base_disp(int);
		bool is_valid_address(string);
		string instr4_handler(Listing_File_Row,string);
		string instr3_handler(Listing_File_Row,string,string);
		string instr2_handler(Listing_File_Row);
		//string instr1_handler(Listing_File_Row);
		string recursive_symtab_search(string);
		string get_register_number(string);
		string hex_string_to_bin_string(string);
		string bin_string_to_hex_string(string);
		char get_hex_char(string);
		int string_to_hex_signed(string);
		string number_conversion_helper(string);
		string protected_recursive_search(string,Listing_File_Row);
		string enhanced_recursive_search(string,Listing_File_Row);
		string get_ascii(char);
		bool is_whitespace(string);
		bool is_2reg(string);
		bool is_reg_and_num(string);
		bool is_1reg(string);
		bool is_num(string);
		
		vector<string> obj_file_handler();
		void write_obj_file(string,vector<string>);
		string get_header_record(string,string,string);
		vector<string> get_text_record();
		vector<string> get_modification_record();
		string get_end_record(string);
		
		string get_prog_length();
		string get_first_address();
		string get_prog_name();
		string get_last_address();
		
		string replace_text_record_size(string);
		
};	
const string directives [] = {
			"START","END","BYTE","WORD","RESB","RESW","BASE","NOBASE","EQU"};
	
#endif
