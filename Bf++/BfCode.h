#pragma once

#include <vector>
#include <string>
#include <unordered_map>

class BfCode {
public:
    int Compress();

    BfCode(std::string filepath, std::string filename);

    static const char basic_cmds[8];

    int Interpret();
    int Bf_to_Cpp();

    void Createlog();

private:

    struct cmd {
        char ch = ' ';
        int counter = 1;
    };

    std::string filepath;
    std::string filename;

    std::vector<cmd> code;

    std::unordered_map<unsigned int,unsigned int> left_bracket;
    std::unordered_map<unsigned int, unsigned int> right_bracket;

    int length = 0;

    bool is_char_bscmd(char ch);
    bool is_cmd_countable(char ch);
    bool is_zeroer(int index);

    void add_openloop(std::vector<int>* unclosed_brackets);
    void add_closeloop(std::vector<int>* unclosed_brackets);
    void add_zeroer(std::vector<int>* unclosed_brackets);
    void add_cmd(char ch);
  
};