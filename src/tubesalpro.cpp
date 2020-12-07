#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

string oneword;
ifstream readFile; //read
ofstream writeFile; // write

void simple_exp();
void stmt_sequence();

void match(string s)
{
	if (oneword == s)
	{
		readFile >> oneword;
	}
	else
	{
		cout << "Error!" << endl;
		writeFile << "Error!" << endl;
		exit(true);
	}
}

void mulop()
{
	if (oneword == "*")
		match("*");
	else if (oneword == "/")
		match("/");
	else
	{
		cout << "Error!" << endl;
		writeFile << "Error!" << endl;
		exit(true);
	}
	cout << "mulop found" << endl;
	writeFile << "mulop found" << endl;
}

void addop()
{
	if (oneword == "+")
		match("+");
	else if (oneword == "-")
		match("-");
	else
	{
		cout << "Error!" << endl;
		exit(true);
	}
	cout << "addop found" << endl;
	writeFile << "addop found" << endl;
}

void comparison_op()
{
	if (oneword == "<")
		match("<");
	else if (oneword == "=")
		match("=");
	else
	{
		cout << "Error!" << endl;
		writeFile << "Error!" << endl;
		exit(true);
	}
	cout << "comparison_op found" << endl;
	writeFile << "comparison_op found" << endl;
}

void exp() // simple-exp {comparison-op simple-exp}
{
	simple_exp();
	while (oneword == "<" || oneword == "=")
	{
		comparison_op();
		simple_exp();
	}
	cout << "exp found" << endl;
	writeFile << "exp found" << endl;
}

void factor() // factor->(exp)|number|identifier
{
	char c = oneword[0];
	switch (c){
	case 'n':
		match("number");
		cout << "factor found" << endl;
		writeFile << "factor found" << endl;
		break;
	case 'i':
		match("identifier");
		cout << "factor found" << endl;
		writeFile << "factor found" << endl;
		break;
	default:
		match("(");
		cout << "( found" << endl;
		writeFile << "( found" << endl;
		exp();
		match(")");
		cout << ") found" << endl;
		writeFile << ") found" << endl;
		cout << "factor found" << endl;
		writeFile << "factor found" << endl;
		break;
	}
}

void term() // factor {mulop factor}
{
	factor();
	while (oneword == "*" || oneword == "/")
	{
		mulop();
		factor();
	}
	cout << "term found" << endl;
	writeFile << "term found" << endl;
}

void simple_exp() // term {addop term}
{
	term();
	while (oneword == "+" || oneword == "-")
	{
		addop();
		term();
	}
	cout << "simple-exp found" << endl;
	writeFile << "simple-exp found" << endl;
}

void write_stmt() // write exp
{
	match("write");
	exp();
	cout << "--write-stmt found--" << endl;
	writeFile << "--write-stmt found--" << endl;
}

void read_stmt() // read identifier
{
	match("read");
	match("identifier");
	cout << "--read-stmt found--" << endl;
	writeFile << "--read-stmt found--" << endl;
}

void assign_stmt() // identifier := exp
{
	match("identifier");
	match(":=");
	exp();
	cout << "--assign-stmt found--" << endl;
	writeFile << "--assign-stmt found--" << endl;
}

void repeat_stmt() // repeat stmt-sequence until exp
{
	match("repeat");
	stmt_sequence();
	match("until");
	exp();
	cout << "--repeat-stmt found--" << endl;
	writeFile << "--repeat-stmt found--" << endl;
}

void if_stmt() // if(exp)then stmt-sequence [else stmt-sequence] end
{
	match("if");
	exp();
	match("then");
	stmt_sequence();
	if (oneword == "else")
	{
		match("else");
		stmt_sequence();
	}
	match("end");
	cout << "--if-stmt found--" << endl;
	writeFile << "--if-stmt found--" << endl;
}

void statement() // if-stmt|repeat-stmt|assign-stmt|read-stmt|write-stmt
{
	char c = oneword[0];
	switch (c)
	{
	case 'i':
		if (oneword[1]=='f') // if
			if_stmt();
		else if (oneword[1] == 'd') // assign (identifier)
			assign_stmt();
		break;
	case 'r':
		if (oneword[2] == 'p') //repeat
			repeat_stmt();
		else if (oneword[2] == 'a')//read
			read_stmt();
		else
		{
			cout << "Error!" << endl;
			writeFile << "Error!" << endl;
			exit(true);
		}
		break;
	case'w': //write
		write_stmt();
		break;
	default:
		break;
	}
}

void stmt_sequence() // statement{;statement}
{
	statement();
	while (oneword == ";")
	{
		match(";");
		statement();
	}
	cout << "stmt-sequence found" << endl;
	writeFile << "stmt-sequence found" << endl;
}

void program()
{
	stmt_sequence();
}

void parse(){
	//readFile.open("C:\\Users\\Ronia\\Desktop\\Compilers\\Parser\\Scanner\\Scanner\\scanner_output2.txt");
	readFile.open("test.txt");
	writeFile.open("parser_output.txt"); //write

	if (readFile.is_open() && writeFile.is_open())
	{
		while (readFile >> oneword)
		{
			program();
			cout << "Program DONE" << endl;
			writeFile << "Program DONE" << endl;
		}
		readFile.close();
		writeFile.close();
	}
	else {
		cout << "Specify a file.txt to read from" << endl;
	}
}

//return true if delimiter
bool isDelimiter(string ch){
	if (ch == " " || ch == "+" || ch == "-" || ch == "*" ||
			ch == "/" || ch == "," || ch == ";" || ch == ">" ||
			ch == "<" || ch == "=" || ch == "(" || ch == ")" ||
			ch == "[" || ch == "]" || ch == "{" || ch == "}")

			return (true);
		return (false);
}

//returns 'true' if the character is an OPERATOR.
bool isOperator(string ch)
{
	if (ch == "+" || ch == "-" || ch == "*" ||
		ch == "/" || ch == ">" || ch == "<" ||
		ch == "=")
		return (true);
	return (false);
}
//return true if keyword
bool isKeyword(string str)
{
	if (!str.compare("if") || !str.compare( "else") ||
		!str.compare("while") || !str.compare("do") ||
		!str.compare("break") ||
		!str.compare("continue") || !str.compare("int")
		|| !str.compare("double") || !str.compare("float")
		|| !str.compare("return") || !str.compare("char")
		|| !str.compare("case") || !str.compare("char")
		|| !str.compare("sizeof") || !str.compare("long")
		|| !str.compare("short") || !str.compare("typedef")
		|| !str.compare("switch") || !str.compare("unsigned")
		|| !str.compare("void") || !str.compare("static")
		|| !str.compare("struct") || !str.compare("goto"))
		return (true);
	return (false);
}
//parse input string
void parse_token(string input){

	int strlen = input.length();

	int idx = 0;
	int pos = 0; //starting position to get next token
	while(idx <= strlen){
		string str = input.substr(idx,1); 	//string example = "int a = b + 1c; float d = 0.32; ";

		if(isDelimiter(str) == false){
			idx++;
		}
		if(isDelimiter(str) == true && pos == idx){
			if(isOperator(str) == true){
				cout << str << endl;
			}
			idx++;
			pos = idx;
		}else if (isDelimiter(str) == true && pos != idx || (idx == strlen && pos != idx)) {
			string token = input.substr(pos, idx-pos);
			cout << "token : " + token << endl;
			pos = idx;
		}
	}
}


int main() {
	//parse();
	string example = "int a = b + 1c; float d = 0.32; ";

	parse_token(example);
	system("Pause");
	return 0;
}
