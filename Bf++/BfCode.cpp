#include "BfCode.h"
#include <fstream>
#include <iostream>

const char BfCode::basic_cmds[8] = { '+', '-', '<', '>', '[', ']', ',', '.' };

BfCode::BfCode(std::string filepath, std::string filename)
{
	this->filepath = filepath;
	this->filename = filename;
	Compress();
}

bool BfCode::is_char_bscmd(char ch) {
	for (int i = 0; i < 8; i++)
		if (ch == this->basic_cmds[i]) return true;
	return false;
}

bool BfCode::is_cmd_countable(char ch) {
	for (int i = 0; i < 4; i++)
		if (ch == this->basic_cmds[i]) return true;
	return false;
}

bool BfCode::is_zeroer(int index) {
	if (this->length < 2) return false;
	cmd var = this->code[index-1];
	if (var.counter > 1) return false;
	if (var.ch != '-' && var.ch != '+') return false;
	if (this->code[index - 2].ch != '[') return false;
	return true;
}

void BfCode::add_cmd(char ch) {
	cmd new_cmd;
	new_cmd.ch = ch;

	this->code.push_back(new_cmd);

	length++;
}

void BfCode::add_openloop(std::vector<int>* unclosed_brackets) {
	unclosed_brackets->push_back(this->length);
	add_cmd('[');
}

void BfCode::add_closeloop(std::vector<int>* unclosed_brackets) {
	if (unclosed_brackets->size() < 1) return;
	this->left_bracket[unclosed_brackets->back()] = this->length;
	this->right_bracket[this->length] = unclosed_brackets->back();
	unclosed_brackets->pop_back();
	add_cmd(']');
}

void BfCode::add_zeroer(std::vector<int>* unclosed_brackets) {
	unclosed_brackets->pop_back();
	this->code.pop_back();
	this->code.pop_back();
	this->length -= 2;
	add_cmd('$');
}

int BfCode::Compress() {
	std::ifstream File(this->filepath + this->filename);

	if (!File.good()) return -1;

	std::vector<int> unclosed_brackets;

	char ch;

	while (File.get(ch)) {
		if (!is_char_bscmd(ch)) continue;
		if (this->length > 0 && is_cmd_countable(ch) && this->code.back().ch == ch) this->code.back().counter++;
		else if (ch == '[') add_openloop(&unclosed_brackets);
		else if (ch == ']' && is_zeroer(this->length)) add_zeroer(&unclosed_brackets);
		else if (ch == ']') add_closeloop(&unclosed_brackets);
		else add_cmd(ch);
	}

	if (this->length == 0) return -1;
	if (unclosed_brackets.size() > 0) {
		for (int var : unclosed_brackets)
			this->code[var].ch = ' ';
	}

	File.close();
	return 0;
}

int BfCode::Interpret() {

	uint8_t array[30000] = { 0 };
	uint8_t *ptr = array;

	for (int i = 0; i < this->length; i++) {
		cmd op = this->code[i];
		switch (op.ch) {
		case '+': 
			*ptr += op.counter;break;

		case '-': 
			*ptr -= op.counter;break;

		case '<': 
			ptr -= op.counter; 
			if (ptr < array) return -1; 
			break;

		case '>': 
			ptr += op.counter; 
			if (ptr >= (array+30000)) return -1; 
			break;

		case '[':
			if (*ptr == 0) i = this->left_bracket[i];break;

		case ']': 
			if (*ptr != 0) i = this->right_bracket[i];break;

		case '.': 
			std::cout << static_cast<char>(*ptr);break;

		case ',': 
			std::cin >> *ptr;break;

		case '$':
			*ptr = 0;break;
		}
	}
	return 0;
}

int BfCode::Bf_to_Cpp() {
	std::ofstream Code(this->filepath + "bfcode.cpp");

	Code << "#include <iostream>\n\n";
	Code << "int main(){\n";
	Code << "uint8_t array[30000] = { 0 };\n";
	Code << "int i = 0;\n";

	for (int i = 0; i < this->length; i++) {
		cmd op = this->code[i];
		switch (op.ch) {
		case '+': Code << "array[i] += " << op.counter << ";"; break;
		case '-': Code << "array[i] -= " << op.counter << ";"; break;
		case '<': Code << "i -= " << op.counter << ";"; break;
		case '>': Code << "i += " << op.counter << ";"; break;
		case '[': Code << "while(array[i] != 0){"; break;
		case ']': Code << "}"; break;
		case '.': Code << "std::cout << static_cast<char>(array[i]);"; break;
		case ',': Code << "std::cin >> array[i];"; break;
		case '$': Code << "array[i] = 0;"; break;
		}
	}
	Code << "}";
	return 0;
}

void BfCode::Createlog() { 
	std::ofstream Log(this->filepath + "bfcode.log");
	Log << "Size: " << this->length << "\n\n";
	for (int i = 0; i < this->code.size(); i++)
	{
		cmd var = this->code[i];
		if (var.ch == '[') Log << "\n";
		if (is_cmd_countable(var.ch)) Log << var.ch << var.counter << " ";
		else Log << var.ch << " ";
		if (var.ch == ']') Log << "\n";
	}
}
