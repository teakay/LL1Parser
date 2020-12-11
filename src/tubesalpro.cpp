#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <regex>

using namespace std;

string strline;
string currentToken;
ifstream readFile; //read
ofstream writeFile; // write
int startpos = 0;  //first position to get next token
int endpos = 0; // last position to get next token
const std::string WHITESPACE = " \n\r\t\f\v";

void scan();
void parse();
string getToken();
bool isDelimiter(string str);
void program();
void code();
void if_else_stmt();
void assignment_stmt();
void condition();
void cond_var();
void cond_op();
void var();
void var_name();
void value();
void val_assign();
void boolean();
void array_init();
void object_init();
void class_stmt();
void accept(string str);
string ltrim(string str);

int main() {
	scan();
	system("Pause");
	return 0;
}

void scan(){
	readFile.open("test.txt");

	if (readFile.is_open())
	{
		while(getline(readFile,strline)){
			//trim whitespace - not working
			//std::regex_replace(strline, std::regex(" \t\n+"), std::string(""));
		    //cout << "Strline  :" <<strline << endl;
		    parse();
			startpos = 0;
			endpos = 0;
		}
		readFile.close();
	}
	else {
		cout << "Specify a file.txt to read from" << endl;
	}
}

std::string ltrim(const std::string& s)
{
    size_t start = s.find_first_not_of(" ");
    return (start == std::string::npos) ? "" : s.substr(start);
}
//============== Lexical Analyzer =========================
string getToken(){
	int strlen = strline.length();
	string token;
	while(endpos <= strlen){
		string str = strline.substr(endpos,1); //get first character from string input

		if(isDelimiter(str) == false){
			endpos++;
		}
		if(isDelimiter(str) == true && endpos == startpos){
			token = strline.substr(startpos, 1);
//			cout << "token1 : " << token << endl;
			endpos++;
			startpos = endpos;
			return token;

		}else if (isDelimiter(str) == true && endpos != startpos || (endpos == strlen && startpos != endpos)) {
			token = strline.substr(startpos, endpos-startpos);
//			cout << "token2 :" << token << endl;
			startpos = endpos;
			return token;

		}
	}
	return token;
}
//return true if delimiter
bool isDelimiter(string ch){
//	if(ch == "<"  ){
//		string nextch = strline.substr(startpos+1, 1);
//		if(nextch == "?"){
//			endpos++;
//		}
//		return true;
//	}
//	else if(ch == "?"){
//		string nextch = strline.substr(startpos+1, 1);
//		if(nextch == ">"){
//			return true;
//		}
//		return true;
//	}
	if (ch == " " || ch == "+" || ch == "-" || ch == "*" ||
		ch == "/" || ch == "," || ch == ";" || ch == "=" ||
		ch == "(" || ch == ")" || ch == "[" || ch == "]" ||
		ch == "{" || ch == "}" || ch == "\n"){
			return (true);
	}
	return (false);
}


//===================End Lexical Analyzer ======================

// ===================== Parser =================================
void parse(){
	program();
}

//<program> ::= <?php <code> ?>
void program(){
	currentToken = getToken();
//	cout << "token : "<< currentToken << endl;
	if(currentToken == "<?php"){
		cout << "<program> found \t\t" << strline << endl;
	}else if(currentToken == "?>"){
		cout << "end <program>" << endl;
	}else{
		code();
	}
}

//<code> ::= <if_else_stmt> | <loop_stmt> | <class_stmt> | <print_stmt> | <assignment_stmt>
void code(){
	cout << "<code> found \t\t\t" + strline << endl;

	if(currentToken == "if"){
		if_else_stmt();
	}else if(currentToken == "for" || currentToken == "foreach"|| currentToken == "while" || currentToken == "do"){
		cout << "<loop_stmt> found" << endl;
	}else if(currentToken == "abstract" || currentToken == "class"){
		class_stmt();
	}else if(currentToken == "echo" || currentToken == "print"){
		cout << "<print_stmt> found" << endl;
	}else if(currentToken.substr(0,1) == "$"){
	    assignment_stmt();accept(";");
	}
}

//assignment_stmt ::= <var> = <val-assign>
void assignment_stmt(){
    cout << "<assignment> found \t\t" + strline << endl;
	if(currentToken.substr(0,1) == "$"){
        var();
		currentToken = getToken();
	}
	accept("=");
	currentToken = getToken();
	if(currentToken == " "){
		currentToken = getToken();
	}
    if(currentToken.substr(0,1) == "$"){
		var();
	}
	else{
		val_assign();
	}
}

//<if_else_stmt> ::= if(<conditions>){<block_inline_statement>} [elseif(<conditions>){<block_inline_statement>}] [else{<block_inline_statement>}]
void if_else_stmt(){
	cout << "<if_else_stmt> found \t\t" << currentToken << endl;
	accept("(");
	condition();
	accept(")");
	accept("{");

//	currentToken = getToken();
//	code();
//	accept("}");

}

//<condition> ::=  <cond_var><cond_op><cond_var>
void condition(){
	cout << "<condition> found \t\t" + strline << endl;
	cond_var();
	cond_op();
	cond_var();
}

//<cond_var> ::= <var> | <value>
void cond_var(){
	currentToken = getToken();
	if(currentToken == " "){
		currentToken = getToken();
	}
	cout << "<cond_var> found \t\t" + currentToken << endl;
	if(currentToken.substr(0,1) == "$"){
		var();
	}
	else{
		value();
	}
}

void cond_op(){
	currentToken = getToken();
	if(currentToken == " "){
		currentToken = getToken();
	}
	cout << "<cond_op> found \t\t" + currentToken << endl;

}

//var
void var(){
	cout << "<var> found \t\t\t" + currentToken << endl;
}

//value
void value(){
	cout << "<value> found \t\t\t" + currentToken << endl;
}

//<val_assign> ::= <boolean> | <array-init> | <object-init>
void val_assign(){
    cout << "<val_assign> found \t\t\t" + currentToken << endl;
    if (currentToken=="true" || currentToken=="false"){
        boolean();
	}
	else if(currentToken=="array"){
        array_init();
	}
	else if(currentToken=="new"){
        object_init();
	}
}

//boolean ::= true | false
void boolean(){
    cout << "<boolean> found \t\t\t" + currentToken << endl;
}

//array_init::= array(<value>)
void array_init(){
    cout << "<array_init> found \t\t\t" + currentToken << endl;
    accept("(");
    accept(")");
}

//object_init::= new <var_name>()
void object_init(){
    cout << "<object_init> found \t\t\t" + strline << endl;
    currentToken = getToken();
	if(currentToken == " "){
		currentToken = getToken();
	}
    var_name();
    accept("(");
    accept(")");
}

//class_stmt ::= class var_name{} | abstract class var_name{}
void class_stmt(){
    cout << "<class> found \t\t" << currentToken << endl;
    currentToken = getToken();
	if(currentToken == " "){
		currentToken = getToken();
		//run when abstract
		if(currentToken=="class"){
            cout << "<class> found \t\t" << currentToken << endl;
            currentToken = getToken();
            if(currentToken == " "){
                currentToken = getToken();
            }
		}
	}
	var_name();
    accept("{");
    accept("}");
}

//var name
void var_name(){
	cout << "<var_name> found \t\t\t" + currentToken << endl;
}

void accept(string str){
	currentToken = getToken();
	cout << "accept token \t\t\t" + currentToken << endl;
	if(str != currentToken){
		cout << "error";
		return;
	}else{
		return;
	}
}
//================= End Parser ===============================
