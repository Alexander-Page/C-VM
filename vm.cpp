// TO DO: 
// implement branch instructions, or, and
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#define sp (registers[SP])
#define ip (registers[IP])
// instruction set
typedef enum {
	PSH,		// pushes an integer onto the stack
	ADD,		// adds the top two values currently on the stack andsaves the result
	POP,		// pops the top value currently on the stack and prints it
	SET,		// sets the register following to the int following the register
	ADDI,		// ex: ADDI REG VAL; adds an integer value to a register
	MULT,		// Multiply the top two values on the stack and pushes the result to the stack
	SUB,		// Subtract the second to last value from the top value on the stack and pushes the result to the stack
	DIV,		// Divide the top value on the stack by the second the to the top value on the stack and pushes the result to the stack
	LOG,		// Prints value in Register A
	LTS,		// load top of stack. loads top value of stack into a register
	HLT			// when this instruction is read the program is terminated 
} InstructionSet;

typedef enum {
	A, B, C, D, E, F, L, IP, SP,
	NUM_OF_REGISTERS
} Registers;

// instruction pointer
//int ip = 0;
//int sp = -1;
int stack[245];
bool running = true;
int registers[NUM_OF_REGISTERS];



std::vector<std::string> read_program(std::string filename) {
	std::string token;
	std::vector<std::string> program;

	std::ifstream infile;
	infile.open(filename, std::fstream::in);

	while (std::getline(infile, token, ' ')) {
		program.push_back(token);
	}

	return program;
}

template<typename T>
void print_vector(std::vector<T>& vec)
{
	std::cout << "(";
	for (int i = 0; i < vec.size(); i++)
	{
		std::cout << vec[i];
		if (i != vec.size() - 1)
		{
			std::cout << ", ";
		}
	}
	std::cout << ")" << std::endl;
}

// test if a string is all numbers
bool isnum(std::string str) {
	int size = str.size();

	for (int i = 0; i < size; i++) {

		if (!isdigit(str[i])) {
			return false;
		}
	}
	return true;
}


// convert the vector that was made from the program file 
// into and integer array to be run
int *get_instr(std::vector<std::string> input) {
	const int size = input.size();
	int *arr = new int[size];

	for (int i = 0; i < input.size(); i++) {
		
		if (input[i] == "PSH") {
			arr[i] = PSH;
		}
		else if (input[i] == "ADD") {
			arr[i] = ADD;
		}
		else if (input[i] == "POP") {
			arr[i] = POP;
		}
		else if (input[i] == "SET") {
			arr[i] = SET;
		}
		else if (input[i] == "HLT") {
			arr[i] = HLT;
		}
		else if (input[i] == "A") {
			arr[i] = A;
		}
		else if (input[i] == "B") {
			arr[i] = B;
		}
		else if (input[i] == "C") {
			arr[i] = C;
		}
		else if (input[i] == "D") {
			arr[i] = D;
		}
		else if (input[i] == "E") {
			arr[i] = F;
		}
		else if (input[i] == "IP") {
			arr[i] = IP;
		}
		else if (input[i] == "SP") {
			arr[i] = SP;
		}
		else if (input[i] == "ADDI") {
			arr[i] = ADDI;
		}
		else if (input[i] == "MULT") {
			arr[i] = MULT;
		}
		else if (input[i] == "SUB") {
			arr[i] = SUB;
		}
		else if (input[i] == "DIV") {
			arr[i] = DIV;
		}
		else if (input[i] == "LOG") {
			arr[i] = LOG;
		}
		else if (input[i] == "LTS") {
			arr[i] = LTS;
		}
		else if (isnum(input[i])) {
			int x = std::stoi(input[i]);
			arr[i] = x;
		}
		else {
			// if this is reached then an invalid instruction was read
			std::cout << "invalid instruction: " << input[i] << std::endl;
			exit(0);
		}
	}
	return arr;
}

// fetch the instruction currently pointed to by the instruction pointer
int fetch(int instr[]) {
	return instr[ip];
}

void eval(int instr, int program[]) {
	
	switch (instr) {
	case HLT: {
		running = false;
		break;
	}
	case PSH: {
		sp++;
		stack[sp] = program[++ip];
		break;
	}
	case POP: {
		int val_popped = stack[sp--];

		std::cout << "popped " << val_popped << std::endl;
		break;
	}
	case ADD: {
		int a = stack[sp--];
		int b = stack[sp--];
		int result = b + a;
		sp++;
		stack[sp] = result;
		break;
	}
	case SET: {
		// reg is the register to be set. registers[reg] is the value of that register
		// set to the next value of the program
		// Example: SET B 8. sets register B to value 8
		int reg = program[ip + 1];
		registers[reg] = program[ip + 2];
		ip = ip + 2;
		break;
	}
	case ADDI: {
		// Add tp the register following ADDI the value following the rigister
		// Example: ADDI C 5. add 5 to register c
		int reg = program[++ip];
		registers[reg] += program[++ip];
		break;
	}
	case MULT: {
		int a = stack[sp--];
		int b = stack[sp--];
		int result = b * a;
		sp++;
		stack[sp] = result;
		break;
	}
	case SUB: {
		int a = stack[sp--];
		int b = stack[sp--];
		int result = a - b;
		sp++;
		stack[sp] = result;
		break;
	}
	case DIV: {
		int a = stack[sp--];
		int b = stack[sp--];
		int result = a / b;
		sp++;
		stack[sp] = result;
		break;
	}
	case LOG: {
		if (registers[A] == NULL) {
			std::cout << "Register A has NULL value" << std::endl;
		}
		else {
			std::cout << "Register A: " << registers[A] << std::endl;
		}
		ip = ip + 1;
	}
	case LTS: {
		int reg1 = program[++ip];
		registers[reg1] = stack[sp];
		sp = sp - 1;
	}
	}
	
}

int main(int argc, char** argv) {
	
	std::vector<std::string> program;
	std::string filename = "TestProgram.txt";

	program = read_program(filename);
	print_vector(program);
	const int size = program.size();
	// create new array with size of the program vector to store the instructions
	int *instr = new int[size];

	instr = get_instr(program);
	
	// running the program
	while (running){
	eval(fetch(instr), instr);
	std::cout << fetch(instr) << std::endl;
	ip++;
	}

	for (int i = 0; i < size; i++) {
		std::cout << "instruction " << i << ": " << instr[i] << std::endl;
	}
	std::cout << "reg A = " << registers[A] << std::endl;
	std::cout << "reg B = " << registers[B] << std::endl;
	return 0;
}