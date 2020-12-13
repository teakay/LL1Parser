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
void var_list();
void value();
void val_assign();
void num();
void array_index();

void var_operation();

void expr();
void expr_();
void term();
void term_();
void factor();

void constant();
void const_val();
void case_value();

void boolean();
void array_init();
void inc_dec_stmt();
void value_list();
void val_list();
void object_init();
void interface();
void class_stmt();
void function_stmt();
void access();
void abstract();
void ex_impl();
void interface_name_list();
void interface_list();
void interface_const();
void return_stmt();
void var_this();
void call_function();
void call_method();
void accept(string str);
void print_stmt();
void all_chars();
void loop_stmt();
bool isAlphabet(string str);
bool isPunctuation(string str);
void alpha();
void punctuation();
void alphanum();
void digit();
bool isDigit(string str);
void switch_case_stmt();
void break_stmt();
void continue_stmt();

int main() {
	scan();
	// system("Pause");
	return 0;
}

void scan(){

	readFile.open("test6.txt");
	// readFile.open("../test5.txt");

	if (readFile.is_open())
	{
		while(getline(readFile,strline)){
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
			endpos++;
			startpos = endpos;
			return token;

		}else if (isDelimiter(str) == true && endpos != startpos || (endpos == strlen && startpos != endpos)) {
			token = strline.substr(startpos, endpos-startpos);
			startpos = endpos;
			return token;

		}
	}
	return token;
}

// see nextToken (untuk melihat token selanjutnya tanpa menambahkan endpos dan startpos yang asli
string nextToken(){
	int strlen = strline.length();
	string token;
	int endp = endpos;
	int startp = startpos;
	while(endp <= strlen){
		string str = strline.substr(endp,1); //get first character from string input

		if(isDelimiter(str) == false){
			endp++;
		}
		if(isDelimiter(str) == true && endp == startp){
			token = strline.substr(startp, 1);
//			cout << "token1 : " << token << endl;
			endp++;
			startp = endp;
			return token;

		}else if (isDelimiter(str) == true && endp != startp || (endp == strlen && startp != endp)) {
			token = strline.substr(startp, endp-startp);
//			cout << "token2 :" << token << endl;
			startp = endp;
			return token;

		}
	}
	return token;
}

//return true if delimiter
bool isDelimiter(string ch){
	if(ch == "<"  ){
		string nextch = ch.substr(1, 1);
		if(nextch != "?"){
			return true;
		}
	}
	else if(ch == ">"){
		string nextch = ch.substr(1, 1);
		if(nextch != ""){
			return true;
		}
	}else if (ch == " " || ch == "+" || ch == "-" || ch == "*" ||
		ch == "/" || ch == "," || ch == ";" || ch == "=" ||
		ch == "(" || ch == ")" || ch == "[" || ch == "]" ||
		ch == "{" || ch == "}" || ch == "\n" || ch == ":"){
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
	if(currentToken == "?>"){
		cout << "end <program>" << endl;
	}else{
		code();
	}
}

//<code> ::= <if_else_stmt> | <loop_stmt> | <class_stmt> | <print_stmt> | <assignment_stmt>
void code(){

	if(currentToken.substr(0,1) == "<" ||currentToken.substr(1,1) == "?"){
		cout << "<program> found \t\t" + strline << endl;
	}else{
		cout << "<code> found \t\t\t" + strline << endl;
		if(currentToken == "if" || currentToken =="elseif" || currentToken == " else"){
			if_else_stmt();
		}else if(currentToken == "switch" || currentToken == "case" || currentToken == "default"){
			switch_case_stmt();
		}else if(currentToken == "for" || currentToken == "foreach"|| currentToken == "while" || currentToken == "do"){
			loop_stmt();
		}else if(currentToken == "abstract" || currentToken == "class"){
			class_stmt();
		}else if(currentToken == "private" || currentToken == "public" || currentToken == "protected"){
			access();
			currentToken = getToken();
			currentToken = getToken();
			if (currentToken == "function") {
				function_stmt();
			} else if (currentToken != "function") {
				if(currentToken.substr(0,1) == "$"){
					var();
					if (nextToken()==" "){
						currentToken = getToken();
						assignment_stmt();
					}
					accept(";");
				}
			}
		} else if(currentToken == "echo" || currentToken == "print"){
			print_stmt();
		}else if (currentToken.substr(0,1) == "$" and nextToken()=="-") {
			call_method();
		}
		else if(currentToken.substr(0,1) == "$"){
			var_operation();accept(";");
		}else if(currentToken == "define"){
			constant();accept(";");
		}else if(currentToken == "interface"){
			interface();
		} else if(currentToken == "const"){
			interface_const();
		} else if(currentToken == "return"){
			return_stmt();
		} else if(nextToken() == "("){
			call_function();
		} else if(currentToken == "}") {
	//			accept("}");
		} else if(currentToken == "break"){
			break_stmt();
		} else if(currentToken == "continue"){
			continue_stmt();
		}else {
			if(strline.length() > 2){
				currentToken = getToken();
				code();
			}
		}
	}
}

//<var_operation> ::== <assignment_stmt> | <inc_dec_stmt>
void var_operation(){
    cout << "<var_operation> found \t\t" + strline << endl;
    if(currentToken.substr(0,1) == "$"){
        var();
        currentToken = getToken();
    }
    if(nextToken()=="="){
        assignment_stmt();
    }
    else if(currentToken=="+"||currentToken=="-"){
        inc_dec_stmt();
    }
}

//<inc_dec_stmt>::= ++ | --
void inc_dec_stmt(){
    cout << "<inc-dec-stmt> found \t\t" << currentToken << endl;
    if (currentToken=="+"){
        accept("+");
    }
    else if (currentToken=="-"){
        accept("-");
    }
}

//<constant>::=define("<var_name>",<const_val><case_value>)
void constant(){
    cout << "<constant> found \t\t" + strline << endl;
    accept("(");
	currentToken = getToken();
    var_name();
    accept(",");
    const_val();
    case_value();
    accept(")");
}

//<const_val>::= <value> | [<value_list>]
void const_val(){
    if(nextToken()=="["){
        accept("[");
        currentToken=getToken();
        value_list();
        accept("]");
    } else{
        currentToken=getToken();
        value();
    }
}

//<case_value>::=<boolean>|epsilon
void case_value(){
    if(nextToken()==","){
        accept(",");
        currentToken=getToken();
        if(currentToken=="true" || currentToken=="false")
            boolean();
    }
}

//assignment_stmt ::= = <val-assign>
void assignment_stmt(){
    cout << "<assignment> found \t\t" + currentToken << endl;
	accept("=");
	currentToken = getToken();
	if(currentToken == " "){
		currentToken = getToken();
	}
	val_assign();
/*    if(currentToken.substr(0,1) == "$"){
		var();
	}
	else{
		val_assign();
	}*/
}

//<if_else_stmt> ::= if(<conditions>){<block_inline_statement>} [elseif(<conditions>){<block_inline_statement>}] [else{<block_inline_statement>}]
void if_else_stmt(){
	cout << "<if_else_stmt> found \t\t" << currentToken << endl;
	if(currentToken == "if"){
		accept("(");
		condition();
		accept(")");
		accept("{");
	}else if(currentToken == "elseif"){
		accept("(");
		condition();
		accept(")");
		accept("{");
		cout<<"end elseif" << endl;
	}else if(currentToken == "else"){
		accept("{");
	}
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
	}else{
		value();
	}
}
//<cond_op> ::= < | > | <= | >= | == | === | ! | <> | != | !=== | && | || | and | or
void cond_op(){
	currentToken = getToken();
	if(currentToken == " "){
		currentToken = getToken();
	}
	if( currentToken == "<" || currentToken == ">" || currentToken == "<=" || currentToken == ">=" ||
		currentToken == "==" || currentToken == "===" || currentToken == "!" || currentToken == "<>" ||
		currentToken == "!=" || currentToken == "!===" || currentToken == "&&" || currentToken == "||" ||
		currentToken == "and" || currentToken == "or"){
		cout << "<cond_op> found \t\t" + currentToken << endl;
	}

}

//<var> ::= $<alphanum>
void var(){
	cout << "<var> found \t\t\t" + currentToken << endl;

	if(currentToken.substr(0,1) == "$"){
		alphanum();
		array_index();
//		cout << "<var> found \t\t\t" + currentToken << endl;
//		currentToken = getToken();
	}
}

void array_index(){
    if (nextToken()=="["){
        accept("[");
        cout << "<array_index> found \t\t\t" <<endl;
        currentToken=getToken();
        num();
        accept("]");
    }
}

//value ::= <boolean> | <expr> | "<all_chars>" | '<all_chars>'
void value(){
	cout << "<value> found \t\t\t" + currentToken << endl;
    if (currentToken=="true" || currentToken=="false"){
        boolean();
	}
	else if(currentToken.substr(0,1) == "-" ||
        currentToken.substr(0,1) == "0" ||
        currentToken.substr(0,1) == "1" ||
        currentToken.substr(0,1) == "2" ||
        currentToken.substr(0,1) == "3" ||
        currentToken.substr(0,1) == "4" ||
        currentToken.substr(0,1) == "5" ||
        currentToken.substr(0,1) == "6" ||
        currentToken.substr(0,1) == "7" ||
        currentToken.substr(0,1) == "8" ||
        currentToken.substr(0,1) == "9" ||
        currentToken.substr(0,1) == "$" ||
        currentToken == "(" ){
        expr();
    }
    else if(currentToken.substr(0,1)=="\"" || currentToken.substr(0,1)=="\'"){
        all_chars();
    }
}

//<expr> ::= <term><expr_>
void expr(){
    term();
    expr_();
}

//<expr_> ::= +<term><expr_> | -<term><expr_> | epsilon
void expr_(){
    if(nextToken()=="+"){
        accept("+");
        currentToken=getToken();
        term();
        expr_();
    }
    else if(nextToken()=="-"){
        accept("-");
        currentToken=getToken();
        term();
        expr_();
    }
}

//<term> ::= <factor><term_>
void term(){
    factor();
    term_();
}

//<term_> ::= *<term><expr_> | /<term><expr_> | %<term><expr_>
void term_(){
    if(nextToken()=="*"){
        accept("*");
        currentToken=getToken();
        factor();
        term_();
    }
    else if(nextToken()=="/"){
        accept("/");
        currentToken=getToken();
        factor();
        expr_();
    }
    else if(nextToken()=="%"){
        accept("%");
        currentToken=getToken();
        factor();
        expr_();
    }
}

//<factor> ::= (<expr>) | <num> | -<num> | <var>
void factor(){
    if(currentToken=="("){
        cout << "accept token \t\t\t" + currentToken << endl;
        currentToken=getToken();
        expr();
        accept(")");
    } else if(currentToken.substr(0,1)=="$"){
        var();
    } else if(currentToken.substr(0,1) == "-") {
        cout << "negative found \t\t\t" + currentToken << endl;
        currentToken=getToken();
        num();
    } else if(currentToken.substr(0,1) == "0" ||
        currentToken.substr(0,1) == "1" ||
        currentToken.substr(0,1) == "2" ||
        currentToken.substr(0,1) == "3" ||
        currentToken.substr(0,1) == "4" ||
        currentToken.substr(0,1) == "5" ||
        currentToken.substr(0,1) == "6" ||
        currentToken.substr(0,1) == "7" ||
        currentToken.substr(0,1) == "8" ||
        currentToken.substr(0,1) == "9"){
        num();
    }
}

//<val_assign> ::= <array-init> | <object-init> | <value>
void val_assign(){
    cout << "<val_assign> found \t\t\t" + currentToken << endl;
    if (currentToken=="true" ||
        currentToken=="false" ||
        currentToken.substr(0,1) == "-" ||
        currentToken.substr(0,1) == "0" ||
        currentToken.substr(0,1) == "1" ||
        currentToken.substr(0,1) == "2" ||
        currentToken.substr(0,1) == "3" ||
        currentToken.substr(0,1) == "4" ||
        currentToken.substr(0,1) == "5" ||
        currentToken.substr(0,1) == "6" ||
        currentToken.substr(0,1) == "7" ||
        currentToken.substr(0,1) == "8" ||
        currentToken.substr(0,1) == "9" ||
        currentToken.substr(0,1) == "$" ||
        currentToken=="("||currentToken.substr(0,1)=="\""){
        value();
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
    currentToken=getToken();
    value_list();
    accept(")");
}

//value_list::= <value><val_list>
void value_list(){
    value();
    val_list();
}

//val_list::= ,<value><val_list> | epsilon
void val_list(){
    if(nextToken()==","){
        accept(",");
        currentToken=getToken();
        value();
        val_list();
    }
}

//object_init::= new <var_name>(<value_list>)
void object_init(){
    cout << "<object_init> found \t\t\t" + strline << endl;
    currentToken = getToken();
	if(currentToken == " "){
		currentToken = getToken();
	}
    var_name();
    accept("(");
    if(nextToken() != ")"){
    	value_list();
    }
    accept(")");
}

//class_stmt ::= <abstract> class <var_name> <ex-imp>{}
void class_stmt(){
    if (currentToken=="abstract")
    {
        abstract();
    }
    else
    {
        cout << "<class> found \t\t" << currentToken << endl;
    }
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
	if (nextToken()==" "){
        currentToken = getToken();
        currentToken = getToken();
		//run when extend
		if(currentToken=="extends" || currentToken=="implements"){
            cout << "<ex_impl> found \t\t" << currentToken << endl;
            ex_impl();
		}
	}
    accept("{");
    //accept("}");
}

//abstract ::= abstract | epsilon
void abstract(){
    if (currentToken=="abstract"){
        cout << "<abstract> found \t\t" << currentToken << endl;
    }
    else if(currentToken=="class"){}
}

//ex_impl ::= extends <var_name> | implements <interface_name_list> | epsilon
void ex_impl(){
    if(currentToken=="extends"){
        cout << "<ex_impl> found \t\t" << currentToken << endl;
        currentToken = getToken();
        if(currentToken == " "){
            currentToken = getToken();
        }
        var_name();
    }
    else if(currentToken=="implements"){
        cout << "<ex_impl> found \t\t" << currentToken << endl;
        currentToken = getToken();
        if(currentToken == " "){
            currentToken = getToken();
        }
        interface_name_list();
    }
    else if(currentToken=="{"){}
}

//function → access “function” var_name “(“ var_list ”){“ code ”}”
void function_stmt(){
    currentToken = getToken();
	if(currentToken == " "){
		currentToken = getToken();

		if(currentToken=="function"){
            cout << "<function> found \t\t" << currentToken << endl;
            currentToken = getToken();
            if(currentToken == " "){
                currentToken = getToken();
            }
		}
	}
	var_name();
	accept("(");

	if (nextToken() != ")") {
		var_list();
	}
	accept(")");
	if (nextToken()!=";") {
		accept("{");
	} else {
		accept(";");
	}
}

//access → public | private | protected
void access(){
	cout << "<access> found \t\t\t" << currentToken << endl;
}

// ignore this, ini hanya pengecekkan integer.
bool check_number(string str) {
   	for (int i = 0; i < str.length(); i++)
   	if (isdigit(str[i]) == false)
    	return false;
      	return true;
}
//var_list → var | var var_list
void var_list(){
	if (nextToken() == " "){
		currentToken = getToken();
		var_list();
	} else if(nextToken() == ","){
		accept(",");
		var_list();
	}
	else if(currentToken != ")"){
		currentToken = getToken();
		var();
		if (check_number(currentToken)==true) {
			num();
		}
		if (nextToken()==",") {
			var_list();
		} else if (nextToken() == ")") {

		}

	}
}

//interface_name_list ::= <var_name><interface_list>
void interface_name_list(){
    var_name();
    interface_list();
}

//interface_list ::= ,<var_name><interface_list> | epsilon
void interface_list(){
    if(nextToken()==","){
        accept(",");
        currentToken=getToken();
        var_name();
        interface_list();
    }
}

//interface → “interface” var_name “{“ interface_list “}”
void interface(){
	cout << "<interface> found \t\t" + currentToken << endl;
	currentToken = getToken();
	if (currentToken==" "){
		currentToken = getToken();
		var_name();
	}
	accept("{");
}

//const → “const” var_name “=” value
void interface_const(){
	cout << "<const> found \t\t\t" + currentToken << endl;
	currentToken = getToken();
	if(currentToken == " ") {
		currentToken = getToken();
		var_name();
		accept("=");
		currentToken = getToken();
		value();
		accept(";");
	}
}

//<var_name> ::= <alphanum>
void var_name(){
	cout << "<var_name> found \t\t\t" + currentToken << endl;
	alphanum();
}

// return → “return” value | “return” var_this
void return_stmt(){
	cout << "<return> found \t\t\t" + currentToken << endl;
	currentToken = getToken();
	if (currentToken == " ") {
		currentToken = getToken();

		if (currentToken == "$this") {
			var_this();
		} else {
			var();
			accept(";");
		}
	}
}

// var_this → “$this->” var_name
void var_this(){
	currentToken = getToken();
	cout << "accept token \t\t\t" << "$this->" << endl;
	currentToken = getToken();
	currentToken = getToken();
	currentToken = getToken();
	var();
	accept(";");
}

// call_function → var_name “(“ var_list “)”
void call_function(){
	cout << "<call_function> found \t" + strline << endl;
	var_name();
	accept("(");
	if (nextToken() != ")") {
		var_list();
	}
	accept(")");
	accept(";");
}

// call_method → $var_name”->”var_name”(“ [var_list] ”)”
void call_method(){
	cout << "<call_method> found \t" + strline << endl;
	var_name();
	cout << "accept token \t\t\t" << "->" << endl;
	currentToken = getToken();
	currentToken = getToken();
	currentToken.replace(0,1,"");
	var_name();
	accept("(");
	if (nextToken() != ")") {
		var_list();
	}
	accept(")");
	accept(";");
}

void accept(string str){
	currentToken = getToken();
	cout << "accept token \t\t\t" + str + " == " + currentToken << endl;
	if(str != currentToken){
		cout << "error";
	}
}

//<print_stmt> ::= echo <all_chars> | print <all_chars>
void print_stmt(){
	cout << "<print_stmt> found \t\t\t" + currentToken << endl;
    currentToken = getToken();
	if(currentToken == " "){
		currentToken = getToken();
	}
	all_chars();
	accept(";");
}

void all_chars(){
	cout << "<all_chars> found \t\t\t" + currentToken << endl;

	int i = 0;
	size_t tokenlen = currentToken.length();
	while((unsigned)i < tokenlen){
		string ch = currentToken.substr(i,1);
		if(isAlphabet(ch)){
			alpha();
			cout << "\t\t" + ch << endl;
		}
		else if(isPunctuation(ch)){
			punctuation();
			cout << "\t\t" + ch << endl;
		}else if(ch == "_"){
			cout << "underscore \t\t" + ch << endl;
		}else{
			cout << "Error" << endl;
		}
		i++;
	}
}
bool isAlphabet(string str){
	if( str == "a" || str == "b" || str == "c" || str == "d" || str == "e" || str == "f" || str == "g" || str == "h" ||
		str == "i" || str == "j" || str == "k" || str == "l" || str == "m" || str == "n" || str == "o" || str == "p" ||
		str == "q" || str == "r" || str == "s" || str == "t" || str == "u" || str == "v" || str == "w" || str == "x" ||
		str == "y" || str == "z" ||
		str == "A" || str == "B" || str == "C" || str == "D" || str == "E" || str == "F" || str == "G" || str == "H" ||
		str == "I" || str == "J" || str == "K" || str == "L" || str == "M" || str == "N" || str == "O" || str == "P" ||
		str == "Q" || str == "R" || str == "S" || str == "T" || str == "U" || str == "V" || str == "W" || str == "X" ||
		str == "Y" || str == "Z"
		)
		return true;
	else{
		return false;
	}

}
bool isPunctuation(string str){
	if( str == "." || str == "," || str == "!" || str == "?"  || str == "@" || str == "#" ||
		str == "$" || str == "%" || str == "&" || str == ":" || str == ";" ||  str == "/" ||
		str == "\"" || str == "\'"){
			return true;
	}
		else{
			return false;
		}

}
void alpha(){
	cout << "<alpha> found" ;
}
void punctuation(){
	cout << "<punctuation> found" ;
}

//<loop_stmt> ::= for(<var><assignment_stmt>;<condition>;<var>){<code>} || foreach(<var> as <var>){<code>}
//do{<code>}while(<condition>); || while(<condition>){<code>}
void loop_stmt(){
	cout << "<loop_stmt> found \t\t" << currentToken << endl;
		if(currentToken == "for"){
			accept("(");
			currentToken = getToken();
            if(currentToken.substr(0,1) == "$"){
                var();
                currentToken = getToken();
            }
			assignment_stmt();
			accept(";");
			condition();
			accept(";");
			currentToken = getToken();
			if(currentToken == " ")
				currentToken = getToken();
			var();
			accept("+");
			accept("+");
			accept(")");
			accept("{");
		}else if(currentToken == "foreach"){
			accept("(");
			currentToken = getToken();
			var();
			currentToken = getToken();
			accept("as");
			currentToken = getToken();
			if(currentToken == " ")
				currentToken = getToken();
			var();
			accept(")");
			accept("{");
		}else if(currentToken == "do"){
			accept("{");
		}else if(currentToken == "while"){
			accept("(");
			condition();
			accept(")");
			if(nextToken() == "{")
				accept("{");
			else if(nextToken() == ";")
				accept(";");
		}
}

void alphanum(){
	cout << "<alphanum> found \t\t" << currentToken << endl;

	int i = 0;
	size_t tokenlen = currentToken.length();
	while((unsigned)i < tokenlen){
		string ch = currentToken.substr(i,1);
		if(ch == "$"){
			//do nothing
			i++;
			continue;
		}
		if(isAlphabet(ch)){
			alpha();
			cout << "\t\t" + ch << endl;
		}
		else if(isDigit(ch)){
			digit();
			cout << "\t\t" + ch << endl;
		}else if(ch == "_"){
			cout << "<underscore> \t\t" + ch << endl;
		}else{
			cout << "Error" << endl;
		}
		i++;
	}
}

//num ::= {digit}[.{digit}]
void num(){
	cout << "<num> found \t\t" << currentToken << endl;

	int i = 0;
	bool isDecimal = false;
	size_t tokenlen = currentToken.length();
	while((unsigned)i < tokenlen){
		string ch = currentToken.substr(i,1);
		if(isDigit(ch)){
			digit();
			cout << "\t\t" + ch << endl;
        }
        else if (isDecimal==false && ch=="."){
            isDecimal = true;
            cout << "decimal found \t\t" << ch << endl;
        }
        else{
			cout << "Error" << endl;
		}
		i++;
	}
}

bool isDigit(string ch){
	if(	ch == "0" || ch == "1" || ch == "2" || ch == "3" ||
		ch == "4" || ch == "5" || ch == "6" || ch == "7"||
		ch == "8" || ch == "9"	){
		return true;
	}
}

void digit(){
	cout << "<digit> found" ;
}
//<switch_case_stmt> ::= switch(<var>){ case <all_chars> || <alphanum> : <code> break; default: <code> }
void switch_case_stmt(){
	cout << "<switch_case_stmt> found " << currentToken << endl;
	if(currentToken == "switch"){
		if(nextToken() == " ")
			currentToken = getToken();
		accept("(");
		currentToken = getToken();
		var();
		accept(")");
		if(nextToken() == " ")
			currentToken = getToken();
		accept("{");
	}
	else if(currentToken == "case"){
		if(nextToken() == " ")
			currentToken = getToken();

		if(nextToken().substr(0,1) == "\""){
			all_chars();
		}else{
			alphanum();
		}
		accept(":");
	}else if(currentToken == "default"){
		accept(":");
	}
}
//<break_stmt> ::= break;
void break_stmt(){
	if(currentToken == "break"){
		cout << "<break_stmt> found" << endl;
		accept(";");
	}
}
//<continue_stmt> ::= continue;
void continue_stmt(){
	if(currentToken == "continue"){
		cout << "<continue_stmt> found" << endl;
		accept(";");
	}
}
//================= End Parser ===============================
