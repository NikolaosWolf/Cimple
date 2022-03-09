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
#define str_tk         1 // a, ..., z and A, ..., Z
#define num_tk         2 // 0, ..., 9
#define plus_tk        3 // +
#define minus_tk       4 // -
#define star_tk        5 // *
#define slash_tk       6 // /
#define eq_tk          7 // =
#define lt_tk          8 // <
#define gt_tk          9 // >
#define le_tk         10 // <=
#define ge_tk         11 // >=
#define ne_tk         12 // <>
#define assign_tk     13 // :=
#define semicolon_tk  14 // ;
#define comma_tk      15 // ,
#define colon_tk      16 // :
#define lsqbr_tk      17 // [
#define rsqbr_tk      18 // ]
#define lpar_tk       19 // (
#define rpar_tk       20 // )
#define lbr_tk        21 // {
#define rbr_tk        22 // }
#define end_tk        23 // .
#define comment_tk    24 // #

/****************** Reserved Words ******************/
#define program_tk  100
#define declare_tk  101
#define if_tk       102
#define else_tk     103
#define switch_tk   104
#define while_tk    105
#define forcase_tk  106
#define incase_tk   107
#define case_tk     108
#define default_tk  109
#define not_tk      110
#define and_tk      111
#define or_tk       112
#define function_tk 113
#define prcdr_tk    114
#define call_tk     115
#define return_tk   116
#define in_tk       117
#define inout_tk    118
#define input_tk    119
#define print_tk    120

/***************** Exceptions ****************/
class CompilationException : public exception {
private:
	string _message;
public:
	explicit CompilationException(const string& message) : _message(message) {}
	const char* what() const noexcept override {
		return _message.c_str();
	}
};


/****************** Classes ******************/
class LexicalAnalyst {
private:
	FILE* input;

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

	int handle_alphanumeric(char c) {
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

	int handle_numeric(char c) {
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

	int handle_lesser(char c) {
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

	int handle_greater(char c) {
		string buffer = "";

		buffer.push_back(c);

		c = getc(input);

		if (c == '=')
			return ge_tk;

		ungetc(c, input);
		return ge_tk;
	}

	int handle_colon(char c) {
		string buffer = "";

		buffer.push_back(c);

		c = getc(input);
		if (c == '=')
			return assign_tk;

		ungetc(c, input);
		return colon_tk;
	}

	int handle_comment(char c) {
		c = getc(input);

		while (c != '#') {
			if (c == EOF)
				throw CompilationException("Comments were left open.");

			c = getc(input);
		}

		return get_token();
	}

public:
	LexicalAnalyst(FILE* file) {
		input = file;
	}

	int get_token() {
		unsigned char c = 0;
		c = getc(input);

		string buffer = "";

		while (isspace(c))
			c = getc(input);

		if (isalpha(c))
			return handle_alphanumeric(c);

		if (isdigit(c))
			return handle_numeric(c);

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
			return handle_lesser(c);

		if (c == '>')
			return handle_greater(c);

		if (c == ':')
			return handle_colon(c);

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
			return handle_comment(c);

		if (c == EOF)
			return -1;

		cout << "Error! Unknown character!";

		return 0;
	}
};

class GrammaticalAnalyst {
private:
	LexicalAnalyst lex;
	int token;
	bool success;

	void program() {
		token = lex.get_token();
		if (token != program_tk)
			throw CompilationException("Program keyword was expected.");

		return program_id();
	}

	void program_id() {
		token = lex.get_token();
		if (token != str_tk)
			throw CompilationException("Program ID was expected");

		block();
		end();
	}

	void block() {
		token = lex.get_token();
		if (token != lbr_tk)
			throw CompilationException("Every code block must start with a left bracket.");

		token = lex.get_token();

		declarations();
		sub_programs();
		block_statements();
	}

	void declarations() {
		while (token == declare_tk) {
			var_list();

			if (token != semicolon_tk)
				throw CompilationException("Semi-colon expected after each variable declaration.");

			token = lex.get_token();
		}
	}

	void var_list() {
		token = lex.get_token();
		if (token != str_tk)
			throw CompilationException("A variable ID is expected after the declare keyword.");

		token = lex.get_token();
		while (token == comma_tk) {
			token = lex.get_token();
			if (token != str_tk)
				throw CompilationException("A variable ID is expected after a comma.");

			token = lex.get_token();
		}
	}

	void sub_programs() {
		while (token == function_tk || token == prcdr_tk) {
			sub_program_id();

			token = lex.get_token();
			if (token != lpar_tk)
				throw CompilationException("A left parenthesis is expected after a sub-program ID.");

			formal_par_list();

			if (token != rpar_tk)
				throw CompilationException("A right parenthesis is expected after the parameters of a function or procedure.");

			block();
		}
	}

	void sub_program_id() {
		token = lex.get_token();
		if (token != str_tk)
			throw CompilationException("An ID is expected after the function or procedure keywords.");
	}

	void formal_par_list() {
		formal_par_item();

		token = lex.get_token();
		while (token == comma_tk) {
			formal_par_item();

			token = lex.get_token();
		}
	}

	void formal_par_item() {
		token = lex.get_token();
		if (token != in_tk && token != inout_tk)
			throw CompilationException("Keyword in or inout is expected as parameter type.");

		token = lex.get_token();
		if (token != str_tk)
			throw CompilationException("A parameter ID is expected after the in or inout keyword.");
	}

	void block_statements() {
		statement();

		token = lex.get_token();
		while (token == semicolon_tk) {
			token = lex.get_token();
			statement();

			token = lex.get_token();
		}
	}

	void statement() {
		switch (token)
		{
			case str_tk:
				assign_statement();
				break;

			case if_tk:
				if_statement();
				break;

			case while_tk:
				while_statement();
				break;

			case switch_tk:
				switchcase_statement();
				break;

			case forcase_tk:
				forcase_statement();
				break;

			case incase_tk:
				incase_statement();
				break;

			case call_tk:
				call_statement();
				break;

			case return_tk:
				return_statement();
				break;

			case input_tk:
				input_statement();
				break;

			case print_tk:
				print_statement();
				break;

			default:
				break;
		}
	}

	void assign_statement() {
		token = lex.get_token();
		if (token != assign_tk)
			throw CompilationException("Assignment symbol is expected.");

		expression();
	}

	void if_statement() {
		token = lex.get_token();
		if (token != lpar_tk)
			throw CompilationException("A left parenthesis is expected after the if keyword.");

		condition();

		token = lex.get_token();
		if (token != rpar_tk)
			throw CompilationException("A right parenthesis is expected after the condition.");

		statements();
		else_part();
	}

	void else_part() {
		if (token == else_tk) {
			statements();
		}
	}

	void while_statement() {
		token = lex.get_token();
		if (token != lpar_tk)
			throw CompilationException("A left parenthesis is expected after the while keyword.");

		condition();

		token = lex.get_token();
		if (token != rpar_tk)
			throw CompilationException("A right parenthesis is expected after the condition.");

		statements();
	}

	void switchcase_statement() {
		token = lex.get_token();
		while (token == case_tk) {
			token = lex.get_token();
		}

		if (token != default_tk)
			throw CompilationException("Default handling is always required in a switchcase.");

		statements();
	}

	void forcase_statement() {
		token = lex.get_token();
		while (token == forcase_tk) {
			token = lex.get_token();
		}

		if (token != default_tk)
			throw CompilationException("Default handling is always required in a forcase.");

		statements();
	}

	void incase_statement() {
		token = lex.get_token();
		while (token == forcase_tk) {
			token = lex.get_token();
		}

		statements();
	}

	void call_statement() {
		token = lex.get_token();
		if (token != str_tk)
			throw CompilationException("An ID is expected after the call keyword.");

		token = lex.get_token();
		if (token != lpar_tk)
			throw CompilationException("A left parenthesis is expected after the ID.");

		//actual_par_list();

		token = lex.get_token();
		if (token != rpar_tk)
			throw CompilationException("A right parenthesis is expected after the parameter list.");
	}

	void return_statement() {
		token = lex.get_token();
		if (token != lpar_tk)
			throw CompilationException("A left parenthesis is expected after the return keyword.");

		expression();

		token = lex.get_token();
		if (token != rpar_tk)
			throw CompilationException("A right parenthesis is expected after the return expression.");
	}

	void input_statement() {
		token = lex.get_token();
		if (token != lpar_tk)
			throw CompilationException("A left parenthesis is expected after the input keyword.");

		token = lex.get_token();
		if (token != str_tk)
			throw CompilationException("A string is expected as parameter in the input statement.");

		token = lex.get_token();
		if (token != rpar_tk)
			throw CompilationException("A right parenthesis is expected after the input parameter.");

	}

	void print_statement() {
		token = lex.get_token();
		if (token != lpar_tk)
			throw CompilationException("A left parenthesis is expected after the print keyword.");

		expression();

		token = lex.get_token();
		if (token != rpar_tk)
			throw CompilationException("A right parenthesis is expected after the print expression.");

	}

	void actual_par_list() {
		if (token == in_tk || token == inout_tk) {
			actual_par_item();

			token = lex.get_token();
			while (token == comma_tk) {
				token = lex.get_token();
				if (token != in_tk && token != inout_tk)
					throw CompilationException("in or inout keyword is expected before each parameter.");

				actual_par_item();

				token = lex.get_token();
			}
		}
	}

	void actual_par_item() {
		if (token == in_tk) {
			expression();
		}
		else {
			token = lex.get_token();
			if (token != str_tk)
				throw CompilationException("ID is expected after the inout keyword.");
		}
	}

	void condition() {

	}

	void statements() {
		if (token == lbr_tk) {
			token = lex.get_token();

			statement();

			token = lex.get_token();
			while (token == semicolon_tk) {
				token = lex.get_token();
				statement();

				token = lex.get_token();
			}
		}
		else {
			statement();

			token = lex.get_token();
			if (token != semicolon_tk)
				throw CompilationException("A statement must be followed by a semi-colon.");

			token = lex.get_token();
		}
	}

	void expression() {
		token = lex.get_token();
		optional_sign();
		term();

		while (token == plus_tk || token == minus_tk) {
			token = lex.get_token();
			term();

			token = lex.get_token();
		}
	}

	void optional_sign() {
		if (token == plus_tk || token == minus_tk) {
			token = lex.get_token();
		}
	}

	void term() {
		factor();

		token = lex.get_token();
		while (token == star_tk || token == slash_tk) {
			token = lex.get_token();
			factor();

			token = lex.get_token();
		}
	}

	void factor() {
		if (token == num_tk) {
			token = lex.get_token();
		}
		else if (token == lpar_tk) {
			expression();

			if (token != rpar_tk)
				throw CompilationException("A right parenthesis is expected after an expression.");
		}
		else if (token == str_tk) {
			id_tail();
		}
		else
			throw CompilationException("Something went wrong!");
	}

	void id_tail() {
		token = lex.get_token();
		if (token == lpar_tk) {
			token = lex.get_token();
			actual_par_list();

			if(token != rpar_tk)
				throw CompilationException("A right parenthesis is expected after the parameters.");
		}
	}

	void end() {

	}

public:
	GrammaticalAnalyst(FILE* input) : lex(input) { token = 0; success = true; }

	bool run() {
		try
		{
			program();
		}
		catch (CompilationException& ex)
		{
			cout << ex.what() << endl;
			success = false;
		}

		return success;
	}
};

int main(int argc, char* argv[])
{
	cout << "######################################\n";
	cout << "###  Welcome to C-imple compliler  ###\n";
	cout << "######################################\n";

	if (argc > 1) {
		cout << "C-imple can only support 1 file as argument. Please try again!";
		return -1;
	}
	else if (argc == 1) {

	}
	else {

	}

	string fileName = "test.txt";
	FILE* input = fopen(fileName.c_str(), "r");
	if (input == nullptr) {
		cout << "Error opening the file.";
		return -1;
	}

	GrammaticalAnalyst roslyn(input);
	bool success = roslyn.run();

	cout << endl;
	fclose(input);

	if (!success) {
		cout << "######################################\n";
		cout << "########  Compilation Failed  ########\n";
		cout << "######################################\n";

		return -1;
	}

	cout << "######################################\n";
	cout << "#######  Compilation Succeded  #######\n";
	cout << "######################################\n";

	return 0;
}