/*
 *  Assembler program that translates programs written in the symbolic Hack assembly
 *  language into binary code that can execute on the Hack hardware platform built in
 *  the previous projects
 *
 *  Author: Lauren Sherburne
 *  Date: 03/13/2022
 *  CSCI 410 - Elements of Computing Systems
 *
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <map>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

void getCommands(string line, vector <string> &listCommands, map<string, int> &symbols, int &pc);
void removeWhiteSpace(string &instruction);
bool getInt(string s, int &val);
string findDest(string instr, map<string, string> dstTable);
string findComp(string instr, map<string, string> cmpTable);
string findJmp(string instr, map<string, string> jmpTable);

// global constants
string GLOBAL_NONE = "None";


int main(int argc, char** argv) {
    // check the number of arguments
    if (argc != 2) {
        // wrong number of arguments
        cout << "Incorrect number of arguments." << endl;
        return -1;
    }

    // initialize jump table
    map<string, string> jumpTable;
    jumpTable["None"] = "000";
    jumpTable["JGT"] = "001";
    jumpTable["JEQ"] = "010";
    jumpTable["JGE"] = "011";
    jumpTable["JLT"] = "100";
    jumpTable["JNE"] = "101";
    jumpTable["JLE"] = "110";
    jumpTable["JMP"] = "111";

    // initialize comp table
    map<string, string> compTable;
    compTable["0"] = "0101010";
    compTable["1"] = "0111111";
    compTable["-1"] = "0111010";
    compTable["D"] = "0001100";
    compTable["A"] =  "0110000";
    compTable["!D"] = "0001101";
    compTable["!A"] = "0110001";
    compTable["-D"] = "0001111";
    compTable["-A"] = "0110011";
    compTable["D+1"] = "0011111";
    compTable["A+1"] = "0110111";
    compTable["D-1"] =  "0001110";
    compTable["A-1"] =  "0110010";
    compTable["D+A"] = "0000010";
    compTable["D-A"] = "0010011";
    compTable["A-D"] = "0000111";
    compTable["D&A"] = "0000000";
    compTable["D|A"] = "0010101";
    compTable["M"] =   "1110000";
    compTable["!M"] =  "1110001";
    compTable["-M"] =  "1110011";
    compTable["M+1"] = "1110111";
    compTable["M-1"] = "1110010";
    compTable["D+M"] = "1000010";
    compTable["D-M"] = "1010011";
    compTable["M-D"] = "1000111";
    compTable["D&M"] = "1000000";
    compTable["D|M"] = "1010101";

    // initialize dest table
    map<string, string> destTable;
    destTable["None"] = "000";
    destTable["M"] = "001";
    destTable["D"] = "010";
    destTable["MD"] = "011";
    destTable["A"] = "100";
    destTable["AM"] = "101";
    destTable["AD"] = "110";
    destTable["AMD"] = "111";

    // initialize symbol table
    map<string, int> symbolTable;
    symbolTable["R0"] = 0;
    symbolTable["R1"] = 1;
    symbolTable["R2"] = 2;
    symbolTable["R3"] = 3;
    symbolTable["R4"] = 4;
    symbolTable["R5"] = 5;
    symbolTable["R6"] = 6;
    symbolTable["R7"] = 7;
    symbolTable["R8"] = 8;
    symbolTable["R9"] = 9;
    symbolTable["R10"] = 10;
    symbolTable["R11"] = 11;
    symbolTable["R12"] = 12;
    symbolTable["R13"] = 13;
    symbolTable["R14"] = 14;
    symbolTable["R15"] = 15;
    symbolTable["SCREEN"] = 16384;
    symbolTable["KBD"] = 24576;
    symbolTable["SP"] = 0;
    symbolTable["LCL"] = 1;
    symbolTable["ARG"] = 2;
    symbolTable["THIS"] = 3;
    symbolTable["THAT"] = 4;

    vector <string> commands;   // list of commands
    int programCounter = 0; // program counter
    int nextSymbolNum = 16;


    // open file
    string filename = argv[1];
    ifstream input;
    input.open(filename);

    // check for error
    if (input.fail()) {
        cerr << "Failed to open file to read(" << filename << "). " << endl;
        return -1;
    }

    // read from file
    string line;
    while (getline(input, line))
    {
        getCommands(line, commands, symbolTable, programCounter);
    }
    

    // close file
    input.close();

    // print the commands
    // cout << "Printing commands..." << endl << endl;
    // for (string s : commands) {
    //     cout << s << endl;
    // }

    // set up output file - fileName.hack
    string outFilename = filename.substr(0, filename.size() - 3) + "hack";
    ofstream output(outFilename);

    // check for error
    if (output.fail()) {
        cerr << "Failed to open file to read(" << filename << "). " << endl;
        return -1;
    }
    
    // declare variables
    int opcode, aInstVal;
    int cInstUnused = 11;

    // second pass - iterate over commands in array and translate to binary
    for(string s : commands) {
        if (s.at(0) == '@') {   // A-instruction
            output << 0;    // opcode first
            s = s.erase(0, 1);

            // find the 15 bit value
            int value = -1;
            bool hasMemValue = getInt(s, value);

            if (!hasMemValue) {
                // check symbol table
                map<string, int>::iterator it;
                for (it = symbolTable.begin(); it != symbolTable.end(); it++) {
                    if (it->first == s) {
                        value = symbolTable[s];
                        break;
                    }
                }
                
                // if not in the symbol table then add to symbol table
                if (value == -1) {
                    symbolTable[s] = nextSymbolNum;
                    value = nextSymbolNum;
                    nextSymbolNum++;
                }
            }

            // add bits to output file
            bitset<15> uh(value);
            output << uh;

        } else {    // C-Instruction
            output << 1;    // opcode
            output << cInstUnused;

            // dest = comp;jump
            string comp, dest, jump;
            output << findComp(s, compTable);   // find computation
            output << findDest(s, destTable);   // find destination (if exists)
            output << findJmp(s, jumpTable);    // find jump (if exists)
        }

        output << endl;
    }

    // close file stream
    output.close();

    // return 0 to indicate the program completed successfully
    return 0;
}

void getCommands(string line, vector <string> &listCommands, map<string, int> &symbols, int &pc) {
    // skip empty lines
    if (line.empty()) {
        return;
    }

    // first check for labels: (address)
    if (line.size() != 0 && line.at(0) == '(') {
        string label = line.substr(1, line.size() - 2);
        // add to symbol table
        symbols[label] = pc;
        return;
    }


    string command = "";
    bool isCommand = false;
    // iterate over the line
    for (int i = 0; i < line.size(); i++) {
        // look for comments '//'
        if ((i != line.size() - 1) && (line.at(i) == '/') && (line.at(i + 1) == '/')) {
            if (isCommand) {
                removeWhiteSpace(command);
                listCommands.push_back(command);
                pc++;
            }
            return; // this is a comment so skip everything else
        }

        // look for A and C instructions
        isCommand = true;
        command += line.at(i);        
    }

    // remove white space and add command to list
    removeWhiteSpace(command);
    listCommands.push_back(command);
    pc++;
    return; 
}

// removes the white space from a string/instruction
void removeWhiteSpace(string &instruction) {
    instruction.erase(remove(instruction.begin(), instruction.end(), ' '), instruction.end());
    instruction.erase(remove(instruction.begin(), instruction.end(), '\t'), instruction.end());
    return;
}

// take integers out of a string
bool getInt(string s, int &val) {
    // use stringstream to look for int
    stringstream str(s);
    int value;
    string temp;

    while(!str.eof()) {
        temp = "";
        str >> temp;
        if (stringstream(temp) >> value) {
            val = value;
            return true;
        }
    }
    return false;
}

// find the binary string corresponding to the destination specified in the instruction, if one exists
string findDest(string instr, map<string, string> dstTable) {
    // check if there is a destination specified in the command
    bool containsDest = false;
    for (char c : instr) {
        if (c == '=') {
            containsDest = true;
        }
    }

    if (!containsDest) {
        return dstTable[GLOBAL_NONE];
    }

    string dest = "";
    int iter = 0;
    while(instr.at(iter) != '=') {
        dest += instr.at(iter);
        iter++;
    }

    string result;
    map<string, string>::iterator it;
    for(it = dstTable.begin(); it != dstTable.end(); it++) {
        if (it->first == dest) {
            result = dstTable[dest];
        }
    }
    return result;
}

// find the binary string corresponding to the computation in the instruction
string findComp(string instr, map<string, string> cmpTable) {
    int start = 0, end = instr.size();
    for (int i = 0; i < instr.size(); i++) {
        // does string contain '='?
        if (instr.at(i) == '=') {
            start = i + 1;
        }

        // does string contain ';'?
        if (instr.at(i) == ';') {
            end = i;
        }
    }

    string cmp = instr.substr(start, end - start);
    string result;
    map<string, string>::iterator it;
    for(it = cmpTable.begin(); it != cmpTable.end(); it++) {
        if (it->first == cmp) {
            result = cmpTable[cmp];
        }
    }

    if (result.empty()) {
        result = cmpTable[GLOBAL_NONE];
    }

    return result;
}

// find the binary string corresponding to the jump command in the instruction, if one exists
string findJmp(string instr, map<string, string> jmpTable) {
    string temp = instr.substr(instr.size() - 3);
    string jump;
    map<string, string>::iterator it;
    for (it = jmpTable.begin(); it != jmpTable.end(); it++) {
        if (it->first == temp) {
            jump = jmpTable[temp];
        }
    }

    if (jump.empty()) {
        jump = jmpTable[GLOBAL_NONE];
    }
    return jump;
}