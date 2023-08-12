/*
 *  Assembler program that translates programs written in the symbolic Hack assembly
 *  language into binary code that can execute on the Hack hardware platform built in
 *  the previous projects
 *
 *  Author: Lauren Sherburne
 *  Date: 04/12/2022
 *  CSCI 410 - Elements of Computing Systems
 * 
 * 
 * Note: The code that uses C++'s filesystem library to find .vm file was written
 * by CPW.
 * 
 */


#include <iostream>
#include <filesystem>
#include <fstream>
using namespace std;
namespace fs = std::filesystem;

void pop(string line, ofstream &output);
void push(string line, ofstream &output);
void add(string line, ofstream &output);
void sub(string line, ofstream &output);
void neg(string line, ofstream &output);
void eq(string line, ofstream &output, string filename, int count);
void gt(string line, ofstream &output, string filename, int count);
void lt(string line, ofstream &output, string filename,  int count);
void andFun(string line, ofstream &output);
void orFun(string line, ofstream &output);
void notFun(string line, ofstream &output);
int extractConstant(string line);
string extractSecondWord(string line);

// global variable for generating unique labels
string EQ_GENERIC_LABEL = "ComparisonEQ";
string LT_GENERIC_LABEL = "ComparisonLT";
string GT_GENERIC_LABEL = "ComparisonGT";
int TEMP_VALUE = 5;
int POINTER_VALUE = 3;
string LOCAL_ADDR = "LCL";
string THIS_ADDR = "THIS";
string THAT_ADDR = "THAT";
string ARG_ADDR = "ARG";


int main(int argc, char* argv[]) {
    // counter to help generate unique labels
    int counter = 0;

    if (argc != 2) {
        cout << "usage: " << argv[0] << " <filename or directory>" << endl;
        return -1;
    }

    fs::path p = argv[1];

    if (is_directory(p)) {
        cout << "contents of " << p.filename() << ":" << endl;
        for (auto const &entry: fs::directory_iterator(argv[1])) {
            if (is_regular_file(entry)) {
                if (entry.path().extension() == ".vm") {
                    fs::path vm_file = entry.path();
                    cout << "\t" << vm_file << " - a VM file, should produce " << vm_file.replace_extension(".asm") << endl;
                    // ifstream input(vm_file);
                    // ofstream output(vm_file.replace_extension(".asm");
                } else {
                    cout << "\t" << entry.path() << " - not a VM file, ignore" << endl;
                }
            }
        }
    } else if (fs::is_regular_file(argv[1])) {
        cout << p << " has stem: " << p.stem() << " and extension: " << p.extension() << endl;
        if (p.extension() == ".vm") {
            cout << "This is a VM file; we should output a new file named " << p.replace_extension(".asm") << endl;
            std::string filename = argv[1];                          

            // create output file stream
            ofstream out(p.replace_extension(".asm"));
            if (out.fail()) {
                cerr << "Failed to open output file: " << p.replace_extension(".asm") << endl;
                return -1;
            }

 
            ifstream input(argv[1]);
            while(!input.eof()) {
                string s;
                getline(input, s);


                if (s.substr(0, 4) == "push") {
                    push(s, out);
                }

                if (s.substr(0, 3) == "pop") {
                    pop(s, out);
                }

                if (s.substr(0, 3) == "add") {
                    add(s, out);
                }

                if (s.substr(0, 3) == "sub") {
                    sub(s, out);
                }

                if (s.substr(0, 3) == "neg") {
                    neg(s, out);
                }

                if (s.substr(0, 2) == "eq") {
                    eq(s, out, filename, counter);
                    counter++;
                }

                if (s.substr(0, 2) == "gt") {
                    gt(s, out, filename, counter);
                    counter++;
                }

                if (s.substr(0, 2) == "lt") {
                    lt(s, out, filename, counter);
                    counter++;
                }

                if (s.substr(0, 3) == "and") {
                    andFun(s, out);
                }

                if (s.substr(0, 2) == "or") {
                    orFun(s, out);
                }

                if (s.substr(0, 3) == "not") {
                    notFun(s, out);
                }

                out << endl;
            }
        }
    } else {
        cout << "\"" << argv[1] << "\" is not a file or directory, or does not exist." << endl;
        return -1;
    }
 

    // return 0 to indicate the program completed successfully
    return 0;
}


void pop(string line, ofstream &output) {
    output << "// " << line << endl;

    // first check for keywords: constant, local, this, that, argument, temp
    string keyword = extractSecondWord(line);

    if (keyword == "constant ") {
        // decrement stack pointer and get first value
        output << "@SP" << endl << "M=M-1" << endl << "A=M" << endl;
    } else if (keyword == "temp ") {
        // get the ram register value
        int ram_val = TEMP_VALUE;

        int offset = extractConstant(line);

        output << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl;

        output << "@13" << endl << "M=D" << endl;

        output << "@" << offset << endl << "D=A" << endl;

        output << "@" << ram_val << endl << "D=A+D" << endl;

        output << "@14" << endl << "M=D" << endl <<  "@13" << endl << "D=M" << endl;

        output << "@14" << endl << "A=M" << endl << "M=D" << endl;


    } else if (keyword == "pointer ") {
        output << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl;

        // find the pointer
        string pointer = "THIS";
        int val = extractConstant(line);
        if (val == 1) {
            pointer = "THAT";            
        }

        output << "@" << pointer << endl << "M=D" << endl;
    
    } else if (keyword == "this " || keyword == "that " || keyword == "argument " || keyword == "local ") {
        string start_addr = LOCAL_ADDR;
        if (keyword == "this ") {
            start_addr = THIS_ADDR;
        } else if (keyword == "that ") {
            start_addr = THAT_ADDR;
        } else if (keyword == "argument ") {
            start_addr = ARG_ADDR;
        }

        int val = extractConstant(line);

        output << "@SP" << endl << "M=M-1" << endl << "A=M" << endl;

        output << "D=M" << endl;

        output << "@13" << endl << "M=D" << endl;

        output << "@" << val << endl << "D=A" << endl;

        output << "@" << start_addr << endl << "A=M" << endl << "D=A+D" << endl;

        output << "@14" << endl << "M=D" << endl << "@13" << endl << "D=M" << endl;
        
        output << "@14" << endl << "A=M" << endl << "M=D" << endl;;

    } else if (keyword == "static ") {
        // variable Foo.4 represents static 4
        // push from D code here

        int val = extractConstant(line);

        output << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl;

        output << "@13" << endl << "M=D" << endl;

        output << "@" << val << endl << "D=A" << endl;

        output << "@16" << endl << "D=A+D" << endl;

        output << "@14" << endl << "M=D" << endl << "@13" << endl << "D=M" << endl;

        output << "@14" << endl << "A=M" << endl << "M=D" << endl;

    } 
}


void push(string line, ofstream &output) {
    output << "// " << line << endl;

    // first check for keywords: constant, local, this, that, argument, temp
    string keyword = extractSecondWord(line);

    if (keyword == "constant ") {
        // load constant
        int num = extractConstant(line);
        output << "@" << num << endl << "D=A" << endl;

        // place constant on stack
        output << "@SP" << endl << "A=M" << endl << "M=D" << endl;  

        // increment stack pointer
        output << "@SP" << endl << "M=M+1" << endl;
    } else if (keyword == "temp " || keyword == "pointer ") {
        // get the ram register value
        int ram_val = TEMP_VALUE;
        if (keyword == "pointer ") {
            ram_val = POINTER_VALUE;
        }
        // add offset to ram value
        int offset = extractConstant(line);
        ram_val += offset;

        // copy value from ram[num] into D register
        output << "@R" << ram_val << endl << "D=M" << endl;

        // copy value from D register into next available stack
        output << "@SP" << endl << "A=M" << endl << "M=D" << endl;

        // increment stack pointer
        output << "@SP" << endl << "M=M+1" << endl;
    } else if (keyword == "this " || keyword == "that " || keyword == "argument " || keyword == "local ") {
        string start_addr = LOCAL_ADDR;
        if (keyword == "this ") {
            start_addr = THIS_ADDR;
        } else if (keyword == "that ") {
            start_addr = THAT_ADDR;
        } else if (keyword == "argument ") {
            start_addr = ARG_ADDR;
        }

        // offset value
        int offset = extractConstant(line);
        output << "@" << offset << endl << "D=A" << endl;

        // compute address using offset and starting address
        output << "@" << start_addr << endl << "D=D+M" << endl;

        // store computed address in R13
        output << "@R13" << endl << "M=D" << endl;

        // copy value from RAM at computed address into D
        output << "A=D" << endl << "D=M" << endl;

        // push from D
        output << "@SP" << endl << "A=M" << endl << "M=D" << endl;  

        // increment stack pointer
        output << "@SP" << endl << "M=M+1" << endl;
    } else if (keyword == "static ") {
        // push from D code here

        int val = extractConstant(line);
        output << "@" << val << endl << "D=A" << endl;

        output << "@16" << endl << "A=A+D" << endl << "D=M" << endl;

        output << "@SP" << endl << "A=M" << endl << "M=D" << endl;

        output << "@SP" << endl << "M=M+1" << endl;
    }   

}


void add(string line, ofstream &output) {
    output << "// " << line << endl;

    // decrement stack pointer and get first value
    output << "@SP" << endl << "M=M-1" << endl << "A=M" << endl;;

    // place value in D register
    output << "D=M" << endl;

    // get next value from stack and add it to first (D register)
    output << "A=A-1" << endl << "D=D+M" << endl;

    // value should end up at the top of the stack
    output << "M=D" << endl;
}

void sub(string line, ofstream &output) {
    output << "// " << line << endl;

    // decrement stack pointer and get first value
    output << "@SP" << endl << "M=M-1" << endl << "A=M" << endl;

    // place value in D register
    output << "D=M" << endl;

    // get next value from stack and subtract it from first (D register)
    output << "A=A-1" << endl << "D=M-D" << endl;

    // value should end up at the top of the stack
    output << "M=D" << endl;
}

void neg(string line, ofstream &output) {
    output << "// " << line << endl;

    // decrement stack pointer and get first value
    output << "@SP" << endl << "A=M-1" << endl;

    // negate value
    output << "M=-M" << endl;
}


void eq(string line, ofstream &output, string filename, int count) {
     output << "// " << line << endl;

    // Generate unique label and end label: (filename + generic label + counter)
    string unique_label = filename + LT_GENERIC_LABEL + to_string(count);
    string end_label = unique_label + "_END";

    // Pop to D
    output << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl;

    // Copy D to R13
    output << "@R13" << endl << "M=D" << endl;

    // Pop to D, again
    output << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl;

    // Subtract R13 from D
    output << "@R13" << endl << "D=M-D" << endl;

    // Jump if equal to zero to unique label
    output << "@" << unique_label << endl << "D;JEQ" << endl;

    // Set D to false
    // output << "@65535" << endl << "D=A" << endl;
    output << "D=0" << endl;

    // Jump to unique end label
    output << "@" << end_label << endl << "0;JMP" << endl;

    // Output unique label
    output << "(" << unique_label << ")" << endl;

    // Set D to true
    // output << "@0" << endl << "D=A" << endl;
    output << "D=-1" << endl;

    // Output unique end label
    output << "(" << end_label << ")" << endl;

    // Push from D
    output << "@SP" << endl << "A=M" << endl << "M=D" << endl;

    // increment stack pointer
    output << "@SP" << endl << "M=M+1" << endl;

}


void gt(string line, ofstream &output, string filename, int count) {
    output << "// " << line << endl;

    // Generate unique label and end label: (filename + generic label + counter)
    string unique_label = filename + LT_GENERIC_LABEL + to_string(count);
    string end_label = unique_label + "_END";

    // Pop to D
    output << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl;

    // Copy D to R13
    output << "@R13" << endl << "M=D" << endl;

    // Pop to D, again
    output << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl;

    // Subtract R13 from D
    output << "@R13" << endl << "D=D-M" << endl;

    // Jump if less than zero to unique label
    output << "@" << unique_label << endl << "D;JGT" << endl;

    // Set D to false
    // output << "@65535" << endl << "D=A" << endl;
    output << "D=0" << endl;

    // Jump to unique end label
    output << "@" << end_label << endl << "0;JMP" << endl;

    // Output unique label
    output << "(" << unique_label << ")" << endl;

    // Set D to true
    // output << "@0" << endl << "D=A" << endl;
    output << "D=-1" << endl;

    // Output unique end label
    output << "(" << end_label << ")" << endl;

    // Push from D
    output << "@SP" << endl << "A=M" << endl << "M=D" << endl;

    // increment stack pointer
    output << "@SP" << endl << "M=M+1" << endl;
}


void lt(string line, ofstream &output, string filename, int count) {
    output << "// " << line << endl;

    // Generate unique label and end label: (filename + generic label + counter)
    string unique_label = filename + LT_GENERIC_LABEL + to_string(count);
    string end_label = unique_label + "_END";

    // Pop to D
    output << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl;

    // Copy D to R13
    output << "@R13" << endl << "M=D" << endl;

    // Pop to D, again
    output << "@SP" << endl << "M=M-1" << endl << "A=M" << endl << "D=M" << endl;

    // Subtract R13 from D
    output << "@R13" << endl << "D=D-M" << endl;

    // Jump if less than zero to unique label
    output << "@" << unique_label << endl << "D;JLT" << endl;

    // Set D to false
    // output << "@65535" << endl << "D=A" << endl;
    output << "D=0" << endl;

    // Jump to unique end label
    output << "@" << end_label << endl << "0;JMP" << endl;

    // Output unique label
    output << "(" << unique_label << ")" << endl;

    // Set D to true
    // output << "@0" << endl << "D=A" << endl;
    output << "D=-1" << endl;

    // Output unique end label
    output << "(" << end_label << ")" << endl;

    // Push from D
    output << "@SP" << endl << "A=M" << endl << "M=D" << endl;

    // increment stack pointer
    output << "@SP" << endl << "M=M+1" << endl;

}

void andFun(string line, ofstream &output) {
    output << "// " << line << endl;

    // decrement stack pointer and get first value
    output << "@SP" << endl << "M=M-1" << endl << "A=M" << endl;;

    // place value in D register
    output << "D=M" << endl;

    // get next value from stack and add it to first (D register)
    output << "A=A-1" << endl << "D=D&M" << endl;

    // value should end up at the top of the stack
    output << "M=D" << endl;
}

void orFun(string line, ofstream &output) {
    output << "// " << line << endl;

    // decrement stack pointer and get first value
    output << "@SP" << endl << "M=M-1" << endl << "A=M" << endl;;

    // place value in D register
    output << "D=M" << endl;

    // get next value from stack and add it to first (D register)
    output << "A=A-1" << endl << "D=D|M" << endl;

    // value should end up at the top of the stack
    output << "M=D" << endl;
}


void notFun(string line, ofstream &output) {
    output << "// " << line << endl;

    // decrement stack pointer and get first value
    output << "@SP" << endl << "A=M-1" << endl;

    // not value
    output << "M=!M" << endl;
}

string extractSecondWord(string line) {
    size_t first = line.find(' ');
    size_t second = line.find(' ', first + 1);
 
    string second_word = line.substr(first + 1, second - first);
    return second_word;
}


int extractConstant(string line) {
    struct not_digit {
        bool operator()(const char c) {
            return c != ' ' && !std::isdigit(c);
        }
    };
    
    not_digit checkDigit;
    string::iterator end = remove_if(line.begin(), line.end(), checkDigit);
    string result(line.begin(), end);
    stringstream stream(result);
    int constant;
    stream >> constant;
    return constant;
}