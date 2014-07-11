/* Yvette Titus, Anthony Perata, Steve Rutherford, Salvatore Tosti
 Team Kansas
 masc1256
 prog2
 CS530, Spring 2014
 opcodetab.cc*/

#include <string>
#include <map>
#include <utility>
#include <iostream>

#include "opcodetab.h"
#include "opcode_error_exception.h"

using namespace std;

opcodetab::opcodetab() {
	//Populate the map with opcodes
	//[Mnemonic] = (Hex Value, Format/Size)
	for (int i = 0; i < NUM_OF_OPCODES; i++)
		opcodes[codes[i]] = Info(values[i], sizes[i]);
}

string opcodetab::get_machine_code(string inOpcode) {
	//change each element of the string to upper case
	for(unsigned int i=0;i<inOpcode.length();i++)
	  {
	     inOpcode[i] = toupper(inOpcode[i]);
	  }

    if(inOpcode[0] == '+') // check if format  4
     	inOpcode.erase (0,1); //erase the character at first position

    opcodes_iter = opcodes.find(inOpcode);
	if(opcodes_iter == opcodes.end()){
		throw opcode_error_exception("opcode " + inOpcode + " not found");}

    return (opcodes_iter->second.machine_code);

}

int opcodetab::get_instruction_size(string inOpcode) {

	/* Converts string to uppercase, checks to see if first character is a '+' for
	 *  possible extended format then checks to see if it is a valid opcode if it is
	 *  return a 4 else check to see if it is a valid 1 2 or 3 format*/

	for (unsigned int i = 0; i < inOpcode.length(); i++) {
		inOpcode[i] = toupper(inOpcode[i]);
	}

	if (inOpcode[0] == '+') {
		inOpcode.erase(0, 1);
		opcodes_iter = opcodes.find(inOpcode);
		if (opcodes_iter == opcodes.end()){
			throw opcode_error_exception("opcode " + inOpcode + " not found");}
		else if ((opcodes_iter->second.length) != 3){
			throw opcode_error_exception("opcode " + inOpcode + " is not a valid format 4 opcode");}
		else
			return 4;
	} else {
		opcodes_iter = opcodes.find(inOpcode);
		if (opcodes_iter == opcodes.end()){
			throw opcode_error_exception("opcode " + inOpcode + " not found");}
		return (opcodes_iter->second.length);
	}
}
