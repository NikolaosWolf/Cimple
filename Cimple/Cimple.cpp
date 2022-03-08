/*
    Name: Nikolaos Tselikas - Kourtidis
    A.M: 1780
*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/******************* Declarations *******************/

/********************* Alphabet *********************/
#define str_tk         1; // a, ..., z and A, ..., Z
#define num_tk         2; // 0, ..., 9
#define plus_tk        3; // +
#define minus_tk       4; // -
#define star_tk        5; // *
#define slash_tk       6; // /
#define eq_tk          7; // =
#define lt_tk          8; // <
#define gt_tk          9; // >
#define le_tk         10; // <=
#define ge_tk         11; // >=
#define ne_tk         12; // <>
#define assign_tk     13; // :=
#define semicolon_tk  14; // ;
#define comma_tk      15; // ,
#define colon_tk      16; // :
#define lsqbr_tk      17; // [
#define rsqbr_tk      18; // ]
#define lpar_tk       19; // (
#define rpar_tk       20; // )
#define lbr_tk        21; // {
#define rbr_tk        22; // }
#define end_tk        23; // .
#define comment_tk    24; // #

/****************** Reserved Words ******************/
#define program_tk  100;
#define declare_tk  101;
#define if_tk       102;
#define else_tk     103;
#define switch_tk   104;
#define while_tk    105;
#define forcase_tk  106;
#define incase_tk   107;
#define case_tk     108;
#define default_tk  109;
#define not_tk      110;
#define and_tk      111;
#define or_tk       112;
#define function_tk 113;
#define prcdr_tk    114;
#define call_tk     115;
#define return_tk   116;
#define in_tk       117;
#define inout_tk    118;
#define input_tk    119;
#define print_tk    120;

/****************** Classes ******************/
class LexicalAnalyst {
    int check_reserved(string s) {
        if (s.compare("program") == 0)
            return program_tk;

        if (s.compare("declare") == 0)
            return declare_tk;

        if (s.compare("if") == 0)
            return if_tk;

        if (s.compare("else") == 0)
            return else_tk;

        if (s.compare("switchcase") == 0)
            return switch_tk;

        if (s.compare("while") == 0)
            return while_tk;

        if (s.compare("forcase") == 0)
            return forcase_tk;

        if (s.compare("incase") == 0)
            return incase_tk;

        if (s.compare("case") == 0)
            return case_tk;

        if (s.compare("default") == 0)
            return default_tk;

        if (s.compare("not") == 0)
            return not_tk;

        if (s.compare("and") == 0)
            return and_tk;

        if (s.compare("or") == 0)
            return or_tk;

        if (s.compare("function") == 0)
            return function_tk;

        if (s.compare("procedure") == 0)
            return prcdr_tk;

        if (s.compare("call") == 0)
            return call_tk;

        if (s.compare("return") == 0)
            return return_tk;

        if (s.compare("in") == 0)
            return in_tk;

        if (s.compare("inout") == 0)
            return inout_tk;

        if (s.compare("input") == 0)
            return input_tk;

        if (s.compare("print") == 0)
            return print_tk;

        return str_tk;
    }

    int handle_alphanumeric(char c, FILE* input) {
        string buffer = "";

        buffer.push_back(c);

        c = getc(input);
        while (isalnum(c)) {
            buffer.push_back(c);
            c = getc(input);
        }

        ungetc(c, input);
        return check_reserved(buffer);
    }

    int handle_numeric(char c, FILE* input) {
        string buffer = "";

        buffer.push_back(c);

        c = getc(input);
        while (isdigit(c)) {
            buffer.push_back(c);
            c = getc(input);
        }

        ungetc(c, input);
        return num_tk;
    }

    int handle_lesser(char c, FILE* input) {
        string buffer = "";

        buffer.push_back(c);
        c = getc(input);

        if (c == '=')
            return le_tk;

        if (c == '>')
            return ne_tk;

        ungetc(c, input);
        return lt_tk;
    }

    int handle_greater(char c, FILE* input) {
        string buffer = "";

        buffer.push_back(c);

        c = getc(input);

        if (c == '=')
            return ge_tk;

        ungetc(c, input);
        return ge_tk;
    }

    int handle_colon(char c, FILE* input) {
        string buffer = "";

        buffer.push_back(c);

        c = getc(input);
        if (c == '=')
            return assign_tk;

        ungetc(c, input);
        return colon_tk;
    }

    int handle_comment(char c, FILE* input) {
        c = getc(input);

        while (c != '#') {
            if (c == EOF)
                return -1;
            
            c = getc(input);
        }

        return comment_tk;
    }

public:
    int get_token(FILE* input) {
        unsigned char c = 0;
        c = getc(input);

        string buffer = "";

        while (isspace(c))
            c = getc(input);

        if (isalpha(c))
            return handle_alphanumeric(c, input);

        if (isdigit(c))
            return handle_numeric(c, input);

        if (c == '+')
            return plus_tk;

        if (c == '-')
            return minus_tk;

        if (c == '*')
            return star_tk;

        if (c == '/')
            return slash_tk;

        if (c == '=')
            return eq_tk;

        if (c == '<')
            return handle_lesser(c, input);

        if (c == '>')
            return handle_greater(c, input);

        if (c == ':')
            return handle_colon(c, input);

        if (c == ';')
            return semicolon_tk;

        if (c == ',')
            return comma_tk;

        if (c == ':')
            return colon_tk;

        if (c == '[')
            return lsqbr_tk;

        if (c == ']')
            return rsqbr_tk;

        if (c == '(')
            return lpar_tk;

        if (c == ')')
            return rpar_tk;

        if (c == '{')
            return lbr_tk;

        if (c == '}')
            return rbr_tk;

        if (c == '.')
            return end_tk;

        if (c == '#')
            return handle_comment(c, input);

        if (c == EOF)
            return -1;

       cout << "Error! Unknown character!";

        return 0;
    }
};

void main(int argc, char *argv[])
{
    cout << "######################################\n";
    cout << "###  Welcome to C-imple compliler  ###\n";
    cout << "######################################\n";

    if (argc > 1) {
        cout << "C-imple can only support 1 file as argument. Please try again!";
        return;
    }
    else if (argc == 1) {

    }
    else {

    }


    string fileName = "test.txt";
    FILE* cimpleFile = fopen(fileName.c_str(), "r");
    if (cimpleFile == nullptr) {
        cout << "Error opening the file.";
        return;
    }

    LexicalAnalyst lex;

    int token;
    while (!feof(cimpleFile)) {
        token = lex.get_token(cimpleFile);
        cout << token << endl;;
    }

    cout << endl;
    fclose(cimpleFile);

    cout << "######################################\n";
    cout << "#######  Compilation Succeded  #######\n";
    cout << "######################################\n";

    return;
}