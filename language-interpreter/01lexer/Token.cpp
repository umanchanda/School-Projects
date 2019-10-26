#include "lexer.h"
#include <iostream>
#include <istream>
#include <map>
#include <string>
#include <regex>

using namespace std;
int lineNumber = 0;
extern Token getToken(istream* br);
extern ostream& operator<<(ostream& out, const Token& tok);

map<TokenType, string> tokens = {
	{T_ERROR, "T_ERROR"},
	{T_DONE, "T_DONE"},
	{T_ICONST, "T_ICONST"},
	{T_INT, "T_INT"}, 
	{T_STRING, "T_STRING"}, 
	{T_SET, "T_SET"}, 
	{T_PRINT, "T_PRINT"}, 
	{T_PRINTLN, "T_PRINTLN"}, 
	{T_SCONST, "T_SCONST"}, 
	{T_PLUS, "T_PLUS"}, 
	{T_MINUS, "T_MINUS"}, 
	{T_STAR, "T_STAR"}, 
	{T_SLASH, "T_SLASH"}, 
	{T_LPAREN, "T_LPAREN"}, 
	{T_RPAREN, "T_RPAREN"}, 
	{T_SC, "T_SC"}
};

Token getToken(istream* br) {
	char ch;
	string word = "";
	
	while (ch = br->get() != EOF) {
		
		/* while (!(br->peek() == (int)' ') {
			word += ch;
		} */
		
		switch (ch) {
			case ' ':
				ch++;
			case '\n':
				lineNumber++;
			case ';':
				return T_SC;
				break;
			case '(':
				return T_LPAREN;
				break;
			case ')':
				return T_RPAREN;
				break;
			case '+':
				return T_PLUS;
				break;
			case '-':
				if ((br->peek()) == (char)' ') {
					return T_MINUS;
				}
				break;
			case '*':
				return T_STAR;
				break;
			case '/':
				if (br->peek() == (char)'/') {
					return T_SLASH;
				}
				break;
			default:
				return T_ERROR;
		}
		
		if (isdigit(ch)) {
			
		}
		
		
		/* if (word == "set") {
			return T_SET;
		}
	
		if (word == "println") {
			return T_PRINTLN;
		}
		
		if (word == "print") {
			return T_PRINT;
		}
		
		if (word == "int") {
			return T_INT;
		} */
	}
	return T_DONE;
}

ostream& operator<<(ostream& out, const Token& tok) {
	out << tokens[tok.GetTokenType()] << "(" << tok.GetLexeme() << ")";
	return out;
}