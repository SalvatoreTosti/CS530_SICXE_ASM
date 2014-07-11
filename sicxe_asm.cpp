/* Yvette Titus, Anthony Perata, Steve Rutherford, Salvatore Tosti
	Team Kansas
	masc1256
	prog3
	CS530, Spring 2014
	sicxe_asm.cpp */

#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <iomanip>
#include <fstream> 

#include "file_parser.h"
#include "file_parse_exception.h"
#include "opcodetab.h"
#include "opcode_error_exception.h"
#include "symtab.h"
#include "symtab_exception.h"
#include "sicxe_asm.h"


using namespace std;

sicxe_asm::sicxe_asm(string filename){
	try{
		file_parser parser(filename);
		pass1(parser,filename);
		BASE_ADDRESS = "";
		pass2(filename);}
	catch(file_parse_exception excpt){
		 cout << "**Sorry, error " << excpt.getMessage() << endl;}
	catch(opcode_error_exception excpt){
		 cout << "**Sorry, error " << excpt.getMessage() << endl;}
	catch(symtab_exception excpt){
		 cout << "**Sorry, error " << excpt.getMessage() << endl;}
	}
sicxe_asm::~sicxe_asm(){}

/*PASS ONE*/

void sicxe_asm::pass1(file_parser parser,string filename){
	parser.read_file();
	int lineNum = 0;
	string current_address ="00000";
	unsigned int instruction_size = 0;
	bool start_encountered = false;
	bool end_encountered = false;
	bool *start_pntr = &start_encountered;
	bool *end_pntr = &end_encountered;
	while(lineNum < parser.size()){
		unsigned int pretty_lineNum = lineNum+1;
		struct Listing_File_Row lis_row;
		lis_row.line_number = lineNum+1;
		lis_row.address = current_address;
		
		string label = parser.get_token(lineNum,0);
		string opcode = parser.get_token(lineNum,1);
		string operand = parser.get_token(lineNum,2);
		lis_row.label = label;
		lis_row.opcode = opcode;
		lis_row.operand = operand;
		
		opcode = to_upper(opcode);
		string* strpntr = &current_address;
		
		//if out of bounds
		if(!start_encountered || end_encountered){
			bool strt = opcode == "START";
			bool end = opcode == "END";
			bool dir = strt || end;
			bool empty = opcode.empty();
			//and if is not empty or 'start','end' assembler directives
			if(!dir && !empty){
				opcode_error_helper("operand out of bounds at line ",pretty_lineNum);}
			}			
		
		if(is_assembler_directive(opcode)){
			pass1_assembler_directive_handler(
			start_pntr,end_pntr,label,opcode,operand,strpntr,pretty_lineNum);
			instruction_size = 0;}
		if(is_assembler_directive(opcode)){instruction_size = 0;}
		else if(opcode.empty()){instruction_size = 0;}
		else{instruction_size = opcode_table.get_instruction_size(opcode);}	
		
		if(!label.empty()){symtab_handler(lis_row.address,label,opcode,pretty_lineNum);}
		current_address = increase_address(current_address,instruction_size);
		current_address = to_upper(current_address);
		lis_container.push_back(lis_row);		
		lineNum++;}
	}//write_listing_file(filename);}

void sicxe_asm::write_listing_file(string file_path){	
	//File creation/opening
	ofstream listFile;
	string tempPath;
	stringstream inputStream;
	
	//Path file input into string stream, take everything before the file extension,
	// and add new extension
	inputStream << file_path;
	getline(inputStream, tempPath, '.');
	string listPath = tempPath + ".lis";
	
	listFile.open( listPath.c_str() , ios::out );
	if (!listFile)
		throw symtab_exception("Listing File could not be created.");
		//does this have to be a symtab_exception? 
		// No, but didn't seem like any existing exception fit and didn't think we could create a new one
		//feel free to change   -SR
	
	//Print Formatting constants
	int LINE_SPACE = 5;
	int ADDRESS_SPACE = 12;
	int LABEL_SPACE = 12;
	int OPCODE_SPACE = 12;
	int OPERAND_SPACE = 12; //Changed to 12, from 24 -ST
	int MACHINE_SPACE = 12;


	//print header
	listFile << "        **" << file_path << "**" << endl;
	listFile << "Line#   "
	     << left << setw(ADDRESS_SPACE) << "Address"
	     << left << setw(LABEL_SPACE) << "Label"
		 << left << setw(OPCODE_SPACE) << "Opcode"
		 << left << setw(OPERAND_SPACE) << "Operand"
		  << left << setw(MACHINE_SPACE) << " Machine Code" << endl;
	
	//Print underlines
	listFile << left << setw(LINE_SPACE) << "=====   "
	     << left << setw(ADDRESS_SPACE) << "======="
	     << left << setw(LABEL_SPACE) << "====="
		 << left << setw(OPCODE_SPACE) << "======"
		 << left << setw(OPERAND_SPACE) << "======="
		 << left << setw(OPERAND_SPACE) << " ============" << endl;
	
	
	
	//Print assembly line by line, separated by token type
	//An extra space is added to the end of the operand to prevent long literals from
	//running into the machine code
	for (unsigned int i = 0; i < lis_container.size(); i++) //-ST changed to unsigned
	listFile << right << setw(LINE_SPACE) << lis_container[i].line_number << "   "
	     << left << setw(ADDRESS_SPACE) << lis_container[i].address
	     << left << setw(LABEL_SPACE) << lis_container[i].label
		 << left << setw(OPCODE_SPACE) << lis_container[i].opcode
		 << left << setw(OPERAND_SPACE) << lis_container[i].operand 
		 //NOTE currently no .machineCode in the struct -SR
		 << left << setw(MACHINE_SPACE) << lis_container[i].machine_code << " " <<endl;
		 
	listFile.close();}
	
bool sicxe_asm::is_assembler_directive(string opcd){
	opcd = to_upper(opcd);
	int size = sizeof(directives) / sizeof(directives[0]);
	for(int i=0;i<size;i++){if(directives[i] == opcd){return true;}}
	return false;}

void sicxe_asm::pass1_assembler_directive_handler(bool *start_pntr, bool *end_pntr,	
	string lbl,string opcd,string opnd,string *curr_addr,unsigned int lineNum){
	opcd = to_upper(opcd);
	if(opcd == "START"){
		start_directive_helper(opnd,curr_addr,lineNum);
		bool tmp = true;
		*start_pntr = tmp;}
	else if(opcd == "END"){
		bool tmp = true;
		*end_pntr = tmp;}
	else if(opcd == "BYTE"){byte_directive_helper(opnd,curr_addr,lineNum);}	
	else if(opcd == "WORD"){
		unsigned int tmp = string_to_hex(*curr_addr);
		tmp = hex_add(tmp,3);
		string new_addr = hex_to_string(tmp);
		*curr_addr = new_addr;}
	else if(opcd == "RESB"){resb_directive_helper(opnd,curr_addr,lineNum);}
	else if(opcd == "RESW"){resw_directive_helper(opnd,curr_addr,lineNum);}
	else if(opcd == "BASE"){}
	else if(opcd == "NOBASE"){}
	else if(opcd == "EQU"){equ_directive_handler(lbl,opcd,opnd,lineNum);}
	else{}
	return;}	

void sicxe_asm::start_directive_helper(
	string opnd,string *curr_addr,unsigned int lineNum){
	if(opnd.at(0) == '$'){
		operand_safety_check(opnd,lineNum);
		opnd.erase(0,1);
		*curr_addr = opnd;
		return;}
	else if(isdigit(opnd.at(0))){
		operand_safety_check(opnd,lineNum);
		unsigned int addr = atoi(opnd.c_str()); 
		opnd = hex_to_string(addr);
		*curr_addr = opnd;
		return;}
	else{opcode_error_helper("Invalid operand passed at line ",lineNum);}
	}

void sicxe_asm::byte_directive_helper(
	string opnd, string *curr_addr,unsigned int lineNum){
	string lead_chars = opnd.substr(0,2);
	lead_chars = to_upper(lead_chars);
	unsigned int disp = 0;
	if(lead_chars == "X'"){
		string inner = opnd.substr(2,opnd.size()-3);
		if(inner.size() % 2 == 1){
			opcode_error_helper("Invalid operand at line ",lineNum);}
		if(!is_hex_number(inner)){
			opcode_error_helper("Invalid operand at line ",lineNum);}
		disp = inner.size()/2;}
	else if(lead_chars == "C'"){
		string inner = opnd.substr(2,opnd.size()-3);
		disp = inner.size();}
	else{opcode_error_helper("Invalid operand at line ",lineNum);}
	unsigned int new_addr = string_to_hex(*curr_addr);
	new_addr = new_addr + disp;
	string str_addr = hex_to_string(new_addr);
	*curr_addr = str_addr;
	return;}
	
void sicxe_asm::resb_directive_helper(
string opnd,string *curr_addr,unsigned int lineNum){
	if(!is_number(opnd)){ 
			opcode_error_helper("Invalid operand passed at line ",lineNum);}
		string disp;
		if(opnd.at(0) == '$'){
			opnd.erase(0,1);
			disp = opnd;}
		else{
			unsigned int addr = atoi(opnd.c_str()); 
			disp = hex_to_string(addr);}
		unsigned int int_disp = string_to_hex(disp);
		unsigned int int_addr = string_to_hex(*curr_addr);
		unsigned int new_addr = hex_add(int_disp,int_addr);
		if(new_addr > 0xFFFFF){
			opcode_error_helper("Operation exceeds valid address range at ",lineNum);}
		string str_addr = hex_to_string(new_addr);
		*curr_addr = str_addr;
		return;}		

void sicxe_asm::resw_directive_helper(
	string opnd,string *curr_addr,unsigned int lineNum){
	if(!is_number(opnd)){ 
			opcode_error_helper("Invalid operand passed at line ",lineNum);}
		string disp;
		if(opnd.at(0) == '$'){
			opnd.erase(0,1);
			disp = opnd;}
		else{
			unsigned int addr = atoi(opnd.c_str()); 
			disp = hex_to_string(addr);}
		unsigned int int_disp = string_to_hex(disp);
		int_disp = int_disp*3;
		unsigned int int_addr = string_to_hex(*curr_addr);
		unsigned int new_addr = hex_add(int_disp,int_addr);
		if(new_addr > 0xFFFFF){
			opcode_error_helper("Operation exceeds valid address range at ",lineNum);}
		string str_addr = hex_to_string(new_addr);
		*curr_addr = str_addr;
		return;}
	
void sicxe_asm::operand_safety_check(string s,unsigned int lineNum){
	if(!is_number(s)){
		opcode_error_helper("Invalid operand passed at line ",lineNum);}
	if(s.at(0) == '$'){s.erase(0,1);}
	unsigned int max = 0xFFFFF;
	unsigned int opnd = string_to_hex(s);
	if(opnd > max){
		opcode_error_helper("Invalid operand passed at line ",lineNum);}
	}	
	
void sicxe_asm::equ_directive_handler(
	string lbl, string opcd, string opnd,unsigned int lineNum){
	if(symbol_table.contains_symbol(opnd)){
		symbol_table.add_symbol(lbl,symbol_table.get_value(opnd),lineNum);}
	
	/*NEW ADDED HERE*/
	else if(is_number(opnd)){
		opnd = number_conversion_helper(opnd);
		/*if(opnd.at(0)=='$'){opnd=opnd.substr(1,opnd.size()-1);}
		else{
			int tmp = atoi(opnd.c_str());
			opnd = hex_to_string(tmp);}*/
		symbol_table.add_symbol(lbl,opnd,lineNum);}
		
	else {symbol_table.add_symbol(lbl,opnd,lineNum);}
	}

void sicxe_asm::symtab_handler(
	string address, string label, string opcode, unsigned int lineNum){
	opcode = to_upper(opcode);
	if(opcode == "EQU"){return;}
	else{symbol_table.add_symbol(label,address,lineNum);}
	}  

string sicxe_asm::increase_address(string addr,int size){
	unsigned int hex_tmp_addr = string_to_hex(addr);
	unsigned int new_hex_addr = hex_add(hex_tmp_addr,size);
	string final_addr = hex_to_string(new_hex_addr);
	return final_addr;}

unsigned int sicxe_asm::string_to_hex(string s){
	unsigned int x; 
	stringstream ss;
	ss << std::hex << s;
	ss >> x;
	return x;}
int sicxe_asm::string_to_hex_signed(string s){
	int x;
	stringstream ss;
	ss << std::hex << s;
	ss >> x;
	return x;}
	
unsigned int sicxe_asm::hex_add(unsigned int a,unsigned int b){return a+b;}

string sicxe_asm::hex_to_string(unsigned int x){
	string tmp;
	stringstream ss;
	ss << hex << x;
	tmp = ss.str();
	while(tmp.size() < 5){tmp = "0"+tmp;} //0 fill
	return tmp;}

bool sicxe_asm::is_number(string s){
	if(s.at(0) == '$'){
		s.erase(0,1);
		if(s.at(0)=='-'){s.erase(0,1);}
		for(unsigned int i=0;i<s.length();i++){
			if(!isxdigit(s.at(i))){return false;}}}
	else{
		if(s.at(0)=='-'){s.erase(0,1);}
		for(unsigned int i=0; i<s.size();i++){
			if(!isdigit(s.at(i))){return false;}}}
	return true;}

bool sicxe_asm::is_hex_number(string s){
	for(unsigned int i=0;i<s.length();i++){
		if(!isxdigit(s.at(i))){return false;}}
	return true;}
	
string sicxe_asm::to_upper(string s){
	transform(s.begin(),s.end(),s.begin(),::toupper);
	return s;}
	
void sicxe_asm::opcode_error_helper(string s, unsigned int lineNum){
	stringstream ss;
	ss << lineNum;
	string lineString = ss.str();
	throw opcode_error_exception(s+lineString+ ".");}

void sicxe_asm::symtab_error_helper(string s, unsigned int lineNum){
	stringstream ss;
	ss << lineNum;
	string lineString = ss.str();
	throw symtab_exception(s+lineString+ ".");}
	
	
/*PASS TWO*/

void sicxe_asm::pass2(string filename){
	for(unsigned int i=0;i<lis_container.size();i++){
		struct Listing_File_Row row = lis_container.at(i);
		string opcode = row.opcode;
		opcode = to_upper(opcode);
		string machine_code = "";
		if(opcode.empty()){machine_code = "";}
		else if(is_assembler_directive(opcode)){
			machine_code = pass2_assembler_directive_handler(row);}
		else{machine_code = machine_code_generator(row);}
		lis_container.at(i).machine_code = machine_code;}
cout <<"broke the loop!" << endl;
	write_listing_file(filename);
cout << "wrote the lis file!" << endl;
	vector<string> records = obj_file_handler();
cout << "got to writing obj" << endl;
	write_obj_file(filename,records);}

string sicxe_asm::pass2_assembler_directive_handler(Listing_File_Row row){
	string tmp = "";
	string opcd = row.opcode;
	opcd = to_upper(opcd);
	if(opcd == "START"){}
	else if(opcd == "END"){}
	else if(opcd == "BYTE"){tmp = pass2_byte_directive_helper(row);}
	else if(opcd == "WORD"){tmp = pass2_word_directive_helper(row);}
	else if(opcd == "RESB"){}
	else if(opcd == "RESW"){}
	else if(opcd == "BASE"){pass2_base_directive_helper(row);}
	else if(opcd == "NOBASE"){pass2_nobase_directive_helper(row);}
	else if(opcd == "EQU"){}
	else{}
	return tmp;}
	
string sicxe_asm::pass2_byte_directive_helper(Listing_File_Row row){
	string opnd = row.operand;
	string print_opnd = row.operand;
	string first = opnd.substr(0,1);
	first = to_upper(first);
	string machcd = "";
	
	if(first == "C"){
		opnd = opnd.substr(2,opnd.size()-2);	//trim front
		opnd = opnd.substr(0,opnd.size()-1);	//trim back
		string tmp;
		for(unsigned int i=0;i<opnd.size();i++){
			tmp = get_ascii(opnd.at(i));	//returns "" for bad values
			if(tmp == ""){	
				opcode_error_helper(
				"invalid argument, "+print_opnd+", passed on line ",
				row.line_number);}
			machcd = machcd+tmp;}}

	if(first == "X"){
		opnd = opnd.substr(2,opnd.size()-2); //trim front
		opnd = opnd.substr(0,opnd.size()-1); //trim back
		if(!is_hex_number(opnd)){
			opcode_error_helper(
			"invalid argument, "+print_opnd+", passed on line ",
			row.line_number);}
		machcd = opnd;}
	return machcd;}
	
string sicxe_asm::pass2_word_directive_helper(Listing_File_Row row){
	string opnd = row.operand;
	if(!is_number(opnd)){
		opcode_error_helper(
		"invalid argument, "+opnd+", passed on line ",row.line_number);}
	opnd = number_conversion_helper(opnd);
	while(opnd.size()>6){opnd.erase(0,1);}
	while(opnd.size()<6){opnd = "0"+opnd;}
	return opnd;}
	
void sicxe_asm::pass2_base_directive_helper(Listing_File_Row row){
	string tmp = enhanced_recursive_search(row.operand,row);
	while(tmp.size()<5){tmp ="0"+tmp;}
	if(is_valid_address(tmp)){BASE_ADDRESS = tmp;}
	else{
		opcode_error_helper(
		"invalid argument, "+tmp+", passed on line ",row.line_number);}
	}
	
void sicxe_asm::pass2_nobase_directive_helper(Listing_File_Row row){
	BASE_ADDRESS="";}
	
string sicxe_asm::machine_code_generator(Listing_File_Row row){
	string machine_code = "";
	machine_code = opcode_table.get_machine_code(row.opcode);
	int inst_size = opcode_table.get_instruction_size(row.opcode);
	
	string nixbpe = "";
	if(inst_size > 2){nixbpe = nixbpe_generator(row);}
	
	//size 2 and 1 instructions do not have nixbpe
	if(inst_size > 2){
		if(!is_valid_nixbpe(nixbpe)){
			opcode_error_helper(
			"invalid opcode / argument on line ",row.line_number);} //throw error
		machine_code = apply_bitmask(machine_code,nixbpe);}
	string addr_or_disp = "";
	if(inst_size == 4){addr_or_disp = instr4_handler(row,nixbpe);}
	else if(inst_size == 3){
		addr_or_disp = instr3_handler(row,nixbpe,BASE_ADDRESS);}
	else if(inst_size == 2){addr_or_disp = instr2_handler(row);}
	else{
		if(!row.operand.empty()){
			opcode_error_helper(
			"invalid argument on line ",row.line_number);}
		addr_or_disp = "";} //(inst_size == 1)
	
	string final_machine_code = machine_code+addr_or_disp;
	final_machine_code = to_upper(final_machine_code);
	return final_machine_code;}
	
string sicxe_asm::nixbpe_generator(Listing_File_Row row){
	string opcd = row.opcode;
	string opnd = row.operand;
	string print_opnd = row.operand;
	string N = "0";
	string I = "0";
	string X = "0";
	string B = "0";
	string P = "0";
	string E = "0";

	//Special cases
	if(to_upper(opcd) == "RSUB"){return "110000";}
	if(is_whitespace(opnd)){
		opcode_error_helper(
			"invalid opcode / argument on line ",row.line_number);} 
		
	//N / I handling
	if(opnd.at(0) == '@'){
		N = "1";
		opnd = opnd.substr(1,opnd.size()-1);}
	else if(opnd.at(0) == '#'){
		I = "1";
		opnd = opnd.substr(1,opnd.size()-1);}
	else{
		N = "1";
		I = "1";}
	
	//X handling
	if(opnd.size() >= 2){
		string end = opnd.substr(opnd.size()-2,2);
		end = to_upper(end);
		if(end == ",X"){
			X = "1";
			opnd = opnd.substr(0,opnd.size()-2);}}
	
	//E handling	
	if(opcd.at(0) == '+'){E = "1";}
	
	bool constant = false;
	if(is_number(opnd)){
		opnd = number_conversion_helper(opnd);
		constant = true;}
	
	string val = protected_recursive_search(opnd,row);		
	
	if(!constant && E == "0"){
		string disp = address_displacement(row.address,val);
		unsigned int addr1 = string_to_hex(row.address);
		unsigned int addr2 = string_to_hex(val);
		int sum;
		bool negative_displacement = false;
		if(addr1<addr2){sum = addr2 - addr1;}
		else{
			sum = addr1 - addr2;
			negative_displacement = true;}
		if(is_valid_pc_disp(sum,negative_displacement)){P = "1";}
		else if(is_valid_base_disp(sum) && !negative_displacement){B = "1";}
		else{
			symtab_error_helper(
			"operand out of range, "+print_opnd+ ", on line ",row.line_number);}
		}
	return N+I+X+B+P+E;}

bool sicxe_asm::is_valid_pc_disp(int disp,bool neg){
	if(neg){return disp < 2048;}
	return disp < 2047;}
	
bool sicxe_asm::is_valid_base_disp(int disp){
	if(disp >= 0 && disp <= 4095){return true;}
	return false;}
	
bool sicxe_asm::is_valid_address(string addr){
	if(addr.size() > 5){return false;}
	return true;}

bool sicxe_asm::is_whitespace(string s){
	for(unsigned int i=0;i<s.size();i++){
		char c = s.at(i);
		if(!isspace(c)){return false;}}
	return true;}
	
bool sicxe_asm::is_valid_nixbpe(string s){
	/*these two checks not needed if the nixbpe generator is working properly
	//verifies nixbpe is 6 digits and only 1/0's
	if ( s.length() != 6)
		//throw SPECIFIC exception("Invalid instruction on line " + row.line_number );
		
	for (int i = 0; i < s.length(); i++)
		if ( s.at(i) != '0' && s.at(i) != '1' )
			//throw SPECIFIC exception("Invalid instruction on line " + row.line_number);		
	*/
	bool temp = true;
	
	//n or i must be 1
	if ( s.at(0) == '0' &&  s.at(1) == '0' )
		temp = false;
	//no x with addressing mode
	/*else if ( s.at(2) == '1' && ( s.at(0) == '1' || s.at(1) == '1' ) ) 
		temp = false; */ //changed back, this causes an error when it shouldn't
						//-ST
	//no format 4 with pc or base relative
	else if ( s.at(5) =='1' && ( s.at(3)=='1'|| s.at(4)=='1' ) ) 
		temp = false;
	//no base and pc relative
	else if ( s.at(3)=='1' && s.at(4)=='1' )
		temp = false;
	return temp;}

string sicxe_asm::apply_bitmask(string machcd, string nixbpe){
	// machcd is just the 2 letters which represent command
	string bin = hex_string_to_bin_string(machcd);
	bin = bin.substr(0,bin.size()-2);
	bin = bin+nixbpe;
	string final_hex = bin_string_to_hex_string(bin);
	return final_hex;}
	
string sicxe_asm::address_displacement(string source, string destination){
	//destination-source+3
	unsigned int src = string_to_hex(source);
	unsigned int dest = string_to_hex(destination);
	int disp;	//was unsigned int
	cout << "src: " << src << endl;
	cout << "dest: " << dest << endl;
	if(src > dest){disp = dest - (src+3);}
	else{disp = dest- (src+3);}
	//if(src < dest){disp = dest-(src+3);}
	//else{disp = src-(dest+3);}
	string tmp = hex_to_string(disp);
cout << " disp is: " <<disp << endl;
	return tmp;}

string sicxe_asm::instr4_handler(Listing_File_Row row,string nixbpe){
	bool regular_reference = (nixbpe.at(0) == '1' && nixbpe.at(1) == '1');
	bool indirect = nixbpe.at(0) == '1';
	bool immediate = nixbpe.at(1) == '1';
	bool index_mode = nixbpe.at(2) == '1';
	
	string opnd = row.operand;
	string print_opnd = row.operand;
	
	//special cases
	if(to_upper(row.opcode) == "RSUB"){
		if(opnd.empty()){return "00000";}
		else{opcode_error_helper(
				"invalid opcode/operand combination "
				"on line ",row.line_number);}}
	
	if(index_mode){opnd = opnd.substr(0,opnd.size()-2);} //clip ',x'
	
	if(regular_reference){
		if(is_number(opnd)){
			string hex = number_conversion_helper(opnd);
			int tmp = string_to_hex(hex);
			while(hex.size()<5){hex = "0"+hex;} //may be redundant
			if(tmp >= 0 && tmp <= 1048575){return hex;}
			else{
				opcode_error_helper(
				"operand out of range on line ",row.line_number);}}}
		
	else if(indirect){opnd = opnd.substr(1,opnd.size()-1);}
	else if(immediate){
		opnd = opnd.substr(1,opnd.size()-1);
		if(is_number(opnd)){
			opnd = number_conversion_helper(opnd);
			while(opnd.size()<5){opnd = "0"+opnd;} //may be redundant
			int tmp = string_to_hex(opnd);
			if(tmp >= -524288 && tmp <= 524287){return opnd;}
			else{
				opcode_error_helper(
				"operand out of range on line ",row.line_number);}}}
			
	string val = protected_recursive_search(opnd,row);
	if(!is_valid_address(val)){
		symtab_error_helper(
		"invalid value, "+val+" pulled from symbol table, "+print_opnd+ 
		", on line ",row.line_number);}
	return val;}
	
string sicxe_asm::instr3_handler(Listing_File_Row row,string nixbpe,string base){
	bool regular_reference = (nixbpe.at(0) == '1' && nixbpe.at(1) == '1');
	bool indirect = nixbpe.at(0) == '1';
	bool immediate = nixbpe.at(1) == '1';
	bool index_mode = nixbpe.at(2) == '1';
	bool base_mode = nixbpe.at(3) == '1';
	bool pc_mode = nixbpe.at(4) == '1';
	
	string opnd = row.operand;
	string print_opnd = row.operand;
	
	//special cases
	//if(to_upper(row.opcode) == "RSUB"){return "000";}
	if(to_upper(row.opcode) == "RSUB"){
		if(opnd.empty()){return "000";}
		else{opcode_error_helper(
				"invalid opcode/operand combination "
				"on line ",row.line_number);}}
	
	if(index_mode){opnd = opnd.substr(0,opnd.size()-2);}
	
	if(regular_reference){
		if(is_number(opnd)){
			opnd = number_conversion_helper(opnd);
			int tmp = string_to_hex(opnd);
			if(tmp >= 0 && tmp <= 4095){return opnd;}
			else{
				opcode_error_helper(
				"operand out of range on line ",row.line_number);}}}
				
	else if(indirect){
		opnd = opnd.substr(1,opnd.size()-1);
		if(is_number(opnd)){
			opnd = number_conversion_helper(opnd);
			int tmp = string_to_hex(opnd);
			if(tmp >= -2048 && tmp <= 2047){
				while(opnd.size()>3){opnd=opnd.substr(1,opnd.size()-1);}
				return opnd;}
			else{
				opcode_error_helper(
				"operand out of range on line ",row.line_number);}}}
	else if(immediate){
		opnd = opnd.substr(1,opnd.size()-1);
		if(is_number(opnd)){
			opnd = number_conversion_helper(opnd);
			int tmp = string_to_hex(opnd);
			if(tmp >= -2048 && tmp <= 2047){
				while(opnd.size()>3){opnd=opnd.substr(1,opnd.size()-1);}
				return opnd;}
			else{
				opcode_error_helper(
				"operand out of range on line ",row.line_number);}}}
		
	string val = protected_recursive_search(opnd,row);
	int hex_source;
	int hex_destination = string_to_hex(val);
	int hex_disp;
	string disp;	
					
	if(base_mode){
		disp = address_displacement(base,val);
		hex_disp = string_to_hex(disp);
		hex_disp +=3;
		disp = hex_to_string(hex_disp);
		while(disp.size()>3){disp = disp.substr(1,disp.size()-1);}
		if(!is_valid_base_disp(hex_disp)){
			opcode_error_helper(
			"displacement out of base mode range on line ",
			row.line_number);}}
			
	if(pc_mode){
		disp = address_displacement(row.address,val);
		while(disp.size()>3){disp = disp.substr(1,disp.size());}
		hex_source = string_to_hex(row.address);
		hex_disp = (hex_destination+3) - hex_source;
		bool neg = hex_disp < 0;
		if(neg){
			int x = string_to_hex(row.address);
			string tmp = hex_to_string(x);
			disp = address_displacement(tmp,val);
			while(disp.size()>3){disp = disp.substr(1,disp.size());}}
		if(!is_valid_pc_disp(hex_disp,neg)){
			opcode_error_helper(
			"displacement out of PC relative mode range on line ",
			row.line_number);}}
						
	return disp;}
	
string sicxe_asm::instr2_handler(Listing_File_Row row){
	string opcd = row.opcode;
	string opnd = row.operand;
	string print_opnd = row.operand;
	string first_opnd = "";
	string second_opnd = "";
	string machine_code_holder= "";
	
	if(is_2reg(opcd)){
		if(opnd.size()<3){
			opcode_error_helper(
			"invalid argument, "+opnd+" on line ",row.line_number);}
		first_opnd = opnd.substr(0,1);
		second_opnd = opnd.substr(opnd.size()-1,opnd.size());
		string tmp = get_register_number(first_opnd); //returns empty if not valid
		if(tmp.empty()){
			opcode_error_helper(
			"invalid argument, "+first_opnd+" on line ",row.line_number);}
		machine_code_holder = machine_code_holder+tmp;
		tmp = get_register_number(second_opnd);
		if(tmp.empty()){
			opcode_error_helper(
			"invalid argument, "+second_opnd+" on line ",row.line_number);}
		machine_code_holder = machine_code_holder+tmp;
		return machine_code_holder;}
		
	else if(is_reg_and_num(opcd)){
		opcd = to_upper(opcd);
		bool is_shift = (opcd == "SHIFTR" || opcd == "SHIFTL");
		
		if(opnd.size()<3){
			opcode_error_helper(
			"invalid argument, "+opnd+" on line ",row.line_number);}
		first_opnd = opnd.substr(0,1);
		string tmp = get_register_number(first_opnd);
		if(tmp.empty()){
			opcode_error_helper(
			"invalid argument, "+first_opnd+" on line ",row.line_number);}	
		opnd = opnd.substr(2,opnd.size()-2);	//clip first and comma
		if(!is_number(opnd)){
			opcode_error_helper(
			"invalid argument, "+print_opnd+" on line ",row.line_number);}
		opnd = number_conversion_helper(opnd);
		if(is_shift){
			unsigned int shift_opnd = string_to_hex(opnd);
			shift_opnd--;
			opnd = hex_to_string(shift_opnd);
			}
		while(opnd.size() > 1){opnd = opnd.substr(1,opnd.size()-1);}
		tmp = tmp+opnd;
		return tmp;}
	
	else if(is_1reg(opcd)){
		string tmp = get_register_number(opnd); 
		if(tmp.empty()){
			opcode_error_helper(
			"invalid argument, "+opnd+" on line ",row.line_number);}
		return tmp+"0";}
		
	else if(is_num(opcd)){
		opnd = number_conversion_helper(opnd);
		while(opnd.size()>2){opnd = opnd.substr(1,opnd.size()-1);}
		return opnd;}
		
	else{
		opcode_error_helper(
			"invalid argument, "+opnd+" on line ",row.line_number);
		return "";} //unreachable, but keeps compiler happy
	}
	
/*	
	//handler for commands with numeric only arg
	if(is_number(opnd)){
		//return  number_conversion_helper(opnd);
		opnd = number_conversion_helper(opnd);
		while(opnd.size()>2){opnd = opnd.substr(1,opnd.size()-1);}
		return opnd;}

	//handler for commands with only 1 non-numeric arg. (register)
	if(opnd.size() == 1){
		string tmp = get_register_number(opnd); 
		if(tmp.empty()){
			opcode_error_helper(
			"invalid argument, "+opnd+" on line ",row.line_number);}
		return tmp+"0";}
	
	first_opnd = opnd.substr(0,1);
	second_opnd = opnd.substr(opnd.size()-1,opnd.size());
	
	//1st argument must be a register
	string tmp = get_register_number(first_opnd); //returns empty if not valid
	if(tmp.empty()){
		opcode_error_helper(
		"invalid argument, "+first_opnd+" on line ",row.line_number);}
	machine_code_holder = machine_code_holder+tmp;
		
	if(is_number(second_opnd)){
		second_opnd = number_conversion_helper(second_opnd);
		machine_code_holder = machine_code_holder + second_opnd;}	
	else{
		string tmp = get_register_number(second_opnd); //returns empty if not valid
		if(tmp.empty()){
			opcode_error_helper(
			"invalid argument, "+first_opnd+" on line ",row.line_number);}
		machine_code_holder = machine_code_holder + tmp;}
	return machine_code_holder;}
*/
	
bool sicxe_asm::is_2reg(string s){	//Opcd  r1,r2
	s = to_upper(s);
	if( s=="ADDR" || s=="COMPR" || s=="DIVR" || s=="MULR" || s=="RMO"){
		return true;}
	return false;}
	
bool sicxe_asm::is_reg_and_num(string s){ //Opcd  r1,n
	s = to_upper(s);
	if(s=="SHIFTL" || s=="SHIFTR"){return true;}
	return false;}
	
bool sicxe_asm::is_1reg(string s){//Opcd r1
	s = to_upper(s);
	if(s=="CLEAR" || s=="TIXR"){return true;}
	return false;}
	
bool sicxe_asm::is_num(string s){ //Opcd n
	s = to_upper(s);
	if(s=="SVC"){return true;}
	return false;}


string sicxe_asm::number_conversion_helper(string s){
	if(s.at(0) == '$'){s = s.substr(1,s.size()-1);}
	else{
		int tmp = atoi(s.c_str());
		s = hex_to_string(tmp);}
	return s;}

string sicxe_asm::get_register_number(string reg){
	reg = to_upper(reg);
	if(reg == "A"){return "0";}
	if(reg == "X"){return "1";}	
	if(reg == "L"){return "2";}
	if(reg == "B"){return "3";}
	if(reg == "S"){return "4";}
	if(reg == "T"){return "5";}
	if(reg == "F"){return "6";}
	if(reg == "PC"){return "8";}
	if(reg == "SW"){return "9";}
	return "";}
	
string sicxe_asm::recursive_symtab_search(string s){
	if(s.empty()){return s;}
	if(is_hex_number(s)){return s;}
	string val = symbol_table.get_value(s);
	return recursive_symtab_search(val);}

string sicxe_asm::protected_recursive_search(string s,Listing_File_Row row){
	string print_opnd = row.operand;
	s = recursive_symtab_search(s);
	if(s.empty()){
		symtab_error_helper(
		"undeclared symbol, "+print_opnd+ ", on line ",row.line_number);}
	return s;}
	
string sicxe_asm::enhanced_recursive_search(string s,Listing_File_Row row){
	if(is_number(s)){return number_conversion_helper(s);}
	return protected_recursive_search(s,row);}

string sicxe_asm::hex_string_to_bin_string(string hex){
	hex = to_upper(hex);
	string binstr = "";
	for(unsigned int i=0;i<hex.size();i++){
		char c = hex.at(i);
    	if(c =='0'){ binstr = binstr+ "0000";}
    	else if(c== '1'){ binstr = binstr+ "0001";}
    	else if(c== '2'){ binstr = binstr+ "0010";}
    	else if(c== '3'){ binstr = binstr+ "0011";}
    	else if(c== '4'){ binstr = binstr+ "0100";}
    	else if(c== '5'){ binstr = binstr+ "0101";}
    	else if(c== '6'){ binstr = binstr+ "0110";}
    	else if(c== '7'){ binstr = binstr+ "0111";}
    	else if(c== '8'){ binstr = binstr+ "1000";}
    	else if(c== '9'){ binstr = binstr+ "1001";}
    	else if(c== 'A'){ binstr = binstr+ "1010";}
    	else if(c== 'B'){ binstr = binstr+ "1011";}
    	else if(c== 'C'){ binstr = binstr+ "1100";}
    	else if(c== 'D'){ binstr = binstr+ "1101";}
    	else if(c== 'E'){ binstr = binstr+ "1110";}
    	else if(c== 'F'){ binstr = binstr+ "1111";}
		else{
			cout << "error, non-hex char passed to hex_string_to_bin. "
			<< "Char was: " << c << endl; 
     	}}
	return binstr;}
	
string sicxe_asm::bin_string_to_hex_string(string bin){
	string hexstr = "";
	unsigned int i = 0;
	string tmpbin = "";
	while(i <= bin.size()){
		if(i%4 == 0 && i!=0){
			hexstr = hexstr+get_hex_char(tmpbin);
			if(i!=bin.size()){tmpbin = bin.at(i);}
			}
		else{tmpbin = tmpbin+bin.at(i);}
		i++;}
	return hexstr;}

char sicxe_asm::get_hex_char(string bin){
	if(bin == "0000"){return '0';}
	else if(bin == "0001"){return '1';}
	else if(bin == "0010"){return '2';}
	else if(bin == "0011"){return '3';}
	else if(bin == "0100"){return '4';}
	else if(bin == "0101"){return '5';}
	else if(bin == "0110"){return '6';}
	else if(bin == "0111"){return '7';}
	else if(bin == "1000"){return '8';}
	else if(bin == "1001"){return '9';}
	else if(bin == "1010"){return 'A';}
	else if(bin == "1011"){return 'B';}
	else if(bin == "1100"){return 'C';}
	else if(bin == "1101"){return 'D';}
	else if(bin == "1110"){return 'E';}
	else if(bin == "1111"){return 'F';}
	else{
		cout << "error, non-hex binary combinaton passed to get_hex_char "
		<< "binary was: " << bin << endl;
		return '\0';}
	}
	
string sicxe_asm::get_ascii(char c){
	unsigned int tmp;
	tmp = (int) c;
	string code = hex_to_string(tmp);
	while(code.size()>2){code = code.substr(1,code.size()-1);}
	code = to_upper(code);
	return code;}
	
/***************/
/*Literal Pools*/
/***************/	
	
	
	
	
	
/****************/		
/*Record Writing*/
/****************/

vector<string> sicxe_asm::obj_file_handler(){
	vector<string> records;	
	vector<string> tmp;
	string length = get_prog_length();
	string first_addr = get_first_address();
	string prog_name = get_prog_name();
	string last_addr = get_last_address();
	
	string header = get_header_record(prog_name,first_addr,length);
	records.push_back(header);
	
	tmp = get_text_record();
	reverse(tmp.begin(),tmp.end());
	while(!tmp.empty()){
		records.push_back(tmp.back());
		tmp.pop_back();}
	tmp = get_modification_record();
	reverse(tmp.begin(),tmp.end());
	while(!tmp.empty()){
		records.push_back(tmp.back());
		tmp.pop_back();}
	string end = get_end_record(first_addr);
	records.push_back(end);
	
	reverse(records.end(),records.begin());
	return records;
	/*test functionality*/
	/*reverse(records.begin(),records.end());
	while(!records.empty()){
		cout << records.back() << endl;
		records.pop_back();}*/
	}

void sicxe_asm::write_obj_file(string file_path,vector<string> records){
	//File creation/opening
	ofstream objFile;
	string tempPath;
	stringstream inputStream;
	
	//Path file input into string stream, take everything before the file extension,
	// and add new extension
	inputStream << file_path;
	getline(inputStream, tempPath, '.');
	string objPath = tempPath + ".obj";
	
	objFile.open(objPath.c_str() , ios::out );
	if (!objFile)
		throw symtab_exception("Listing File could not be created.");
		
	objFile << "        **" << file_path << "**" << endl;

	for (unsigned int i = 0; i < records.size(); i++){ //-ST changed to unsigned
		objFile << left << records.at(i) << endl;}
	
	objFile.close();}
	
string sicxe_asm::get_prog_length(){
	string start = get_first_address();
	//cout << "Start is: " << start << endl;
	string end = get_last_address();
	//cout << "End is: " << end << endl;
	unsigned int a = string_to_hex(start);
	unsigned int b = string_to_hex(end);
	unsigned int disp = b-a;
	string dist = hex_to_string(disp);
	dist = to_upper(dist);
	return dist;}
	
string sicxe_asm::get_first_address(){
	unsigned int i=0;
	Listing_File_Row tmp;
	while(i<lis_container.size()){
		tmp = lis_container.at(i);
		string opcd = to_upper(tmp.opcode);
		if(opcd == "START"){return tmp.operand;}
		else{i++;}
		}
	return "";}
	
string sicxe_asm::get_prog_name(){
	unsigned int i=0;
	Listing_File_Row tmp;
	while(i<lis_container.size()){
		tmp = lis_container.at(i);
		string opcd = to_upper(tmp.opcode);
		if(opcd == "START"){return tmp.label;}
		else{i++;}
		}
	return "";}
	
string sicxe_asm::get_last_address(){
	unsigned int i=0;
	Listing_File_Row tmp;
	while(i<lis_container.size()){
		tmp = lis_container.at(i);
		string opcd = to_upper(tmp.opcode);
		if(opcd == "END"){return tmp.address;}
		else{i++;}
		}
	return "";}
	
string sicxe_asm::get_header_record(string name, string first, string length){
	string record = "H";
	while(name.size()>6){name = name.substr(1,name.size());}
	while(name.size()<6){name = name+" ";}
	record = record+name;
	while(first.size()>6){first = first.substr(1,first.size());}
	while(first.size()<6){first = "0"+first;}
	record = record+first;	//first address, starting address
	while(length.size()>6){length = length.substr(1,first.size());}
	while(length.size()<6){length = "0"+length;}
	record = record+length;
	return record;}
	
vector<string> sicxe_asm::get_text_record(){
	string current_record = "";
	unsigned int i=0;
	Listing_File_Row tmpRow;
	vector<string> text_records;
	while(i<lis_container.size()){
		tmpRow = lis_container.at(i);
		if(current_record.empty()){
			current_record = "T";
			current_record = current_record + "0"+tmpRow.address;
			current_record = current_record+"XX";} //length of record

		string test_record = current_record + tmpRow.machine_code;
		if(test_record.size()>68){
			current_record = replace_text_record_size(current_record);
			/*unsigned int rcrd_size = current_record.size()-9; //9 is record info
			string sz = hex_to_string(rcrd_size);
			while(sz.size()>2){sz=sz.substr(1,sz.size()-1);}
			sz = to_upper(sz);
			current_record.replace(7,2,sz);*/	
			text_records.push_back(current_record);
			current_record="";
			continue;}
		else{current_record = test_record;}	

		string opcd = tmpRow.opcode;
		opcd = to_upper(opcd);
		if(opcd =="RESW" || opcd =="RESB"){
			if(current_record.size()>8){
				current_record = replace_text_record_size(current_record);
				text_records.push_back(current_record);
				current_record = "";}
			else{current_record = "";}
			i++;
			continue;}
		
		if(opcd == "END"){
			current_record = replace_text_record_size(current_record);
			text_records.push_back(current_record);
			break;}
		i++;}
	return text_records;}
				
string sicxe_asm::replace_text_record_size(string record){
	unsigned int rcrd_size = record.size()-9; //9 is record info
	string sz = hex_to_string(rcrd_size);
	while(sz.size()>2){sz=sz.substr(1,sz.size()-1);}
	sz = to_upper(sz);
	record.replace(7,2,sz);
	return record;}
	
vector<string> sicxe_asm::get_modification_record(){
	string current_record = "";
	unsigned int i=0;
	Listing_File_Row tmpRow;
	string opcd;
	string opnd;
	vector<string> mod_records;
	//string addr;
	while(i<lis_container.size()){

		tmpRow = lis_container.at(i);
		opcd = tmpRow.opcode;
		opnd = tmpRow.operand;
		bool is_symbol = symbol_table.contains_symbol(opnd);
		if(opcd.empty() || opnd.empty()){
			i++;
			continue;}
		if(opcd.at(0) != '+'){
			i++;
			continue;}
		if(opnd.at(0) == '#'){
			i++;
			continue;}
		/*if(opcd =="WORD"){
			current_record="M";
			addr = tmpRow.address;
			while(addr.size()<6){addr="0"+addr;}
			current_record=current_record+"06";
			mod_records.push_back(current_record);}	*/
			
		if(is_symbol){
			current_record = "M";
			string addr = tmpRow.address;
			unsigned int tmp = string_to_hex(addr);
			tmp++;
			addr = hex_to_string(tmp);
			addr = to_upper(addr);
			while(addr.size()<5){addr="0"+addr;}
			current_record = current_record+addr;
			current_record = current_record+"06";
			mod_records.push_back(current_record);
			} //size 6 
		else{
			current_record = "M";
			string addr = tmpRow.address;
			unsigned int tmp = string_to_hex(addr);
			tmp++;
			addr = hex_to_string(tmp);
			addr = to_upper(addr);
			while(addr.size()<5){addr="0"+addr;}
			current_record = current_record+addr;
			current_record = current_record+"05";
			mod_records.push_back(current_record);}
		i++;}
		return mod_records;}
	
string sicxe_asm::get_end_record(string first){
	string record = "E";
	while(first.size()>6){first = first.substr(1,first.size());}
	while(first.size()<6){first = "0"+first;}
	record = record+first;	//first address, starting address
	return record;}
	

/*MAIN*/

int main(int argc, char *argv[]) {
    if(argc != 2) {
        cout << "Error, you must supply the name of the file " <<
        "to process at the command line." << endl;
        exit(1);}    
    new sicxe_asm(argv[1]);
	return 0;}
		
