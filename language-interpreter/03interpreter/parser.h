/*
 * parser.h
 *
 *  Created on: Oct 23, 2017
 *      Author: gerardryan
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
using std::istream;

#include <string>
#include <map>
using std::string;
using std::stoi;
using std::map;

#include <ostream>
using std::ostream;
using namespace std;

#include "lexer.h"

extern void error(int linenum, const string& message);
extern void semanticError(int linenum, const string& message);

enum TypeForNode { INT_TYPE, STRING_TYPE, ERROR_TYPE };

class Value {
	TypeForNode	type;
	int			ival;
	string		sval;

public:
	Value(TypeForNode t = ERROR_TYPE) : type(t), ival(0) {}
	Value(int i) : ival(i) { type = INT_TYPE; }
	Value(string s) : sval(s) { type = STRING_TYPE; }
	
	TypeForNode GetType() const { return type; }

	int GetIntValue() const {
		if( type == INT_TYPE ) return ival;
		throw "This variable is not an int";
	}
	void SetIntValue(int v) {
		if( type == INT_TYPE ) ival = v;
		throw "This variable is not an int";
	}

	string GetStringValue() const {
		if( type == STRING_TYPE ) return sval;
		throw "This variable is not a string";
	}
	void SetStringValue(string v) {
		if( type == STRING_TYPE ) sval = v;
		throw "This variable is not a string";
	}
};

extern map<string,Value> SymbolTable;

class ParseTree {
private:
	int			linenumber;
	ParseTree	*left;
	ParseTree	*right;

public:
	ParseTree(int n, ParseTree *l = 0, ParseTree *r = 0) : linenumber(n), left(l), right(r) {}
	virtual ~ParseTree() {}

	ParseTree* getLeft() const { return left; }
	ParseTree* getRight() const { return right; }
	int getLineNumber() const { return linenumber; }

	virtual TypeForNode GetType() const { return ERROR_TYPE; }
	virtual int GetIntValue() const { throw "no integer value"; }
	virtual string GetStringValue() const { throw "no string value"; }

	int traceAndCount(int (ParseTree::*f)() const) {
		int cnt = 0;
		if( left ) {
			cnt += left->traceAndCount(f);
		}
		if( right ) {
			cnt += right->traceAndCount(f);
		}
		cnt += (this->*f)();
		return cnt;
	}
	
	virtual int countSet() const { return 0; }
	virtual int countPlus() const { return 0; }
	virtual int countStar() const { return 0; }

	virtual int findSemanticErrors() const { return 0; }
	virtual Value eval() const { return Value(); }
};

class StatementList : public ParseTree {
public:
	StatementList(ParseTree *first, ParseTree *rest) : ParseTree(0, first, rest) {}
	
	TypeForNode GetType() const {
		TypeForNode type = getLeft()->GetType();
		return type;
	}
	
	Value eval() const {
		if (getLeft()) {
            getLeft()->eval();
        }
        if (getRight()) {
            getRight()->eval();
        }
        return 0;
	}
};

class DeclStatement : public ParseTree {
	TypeForNode	type;
	string 		id;

public:
	DeclStatement(int line, TypeForNode ty, string id) : ParseTree(line), type(ty), id(id) {}

	int findSemanticErrors() const {
		if( SymbolTable.find(id) != SymbolTable.end() ) {
			semanticError(this->getLineNumber(), "variable " + id + " was already declared");
			return 1;
		}
		SymbolTable[id] = Value(type);
		return 0;
	}
};

class SetStatement : public ParseTree {
	string id;

public:
	SetStatement(int line, string id, ParseTree *ex) : ParseTree(line, ex), id(id) {}
	int countSet() const { return 1; }

	int findSemanticErrors() const {
		if( SymbolTable.find(id) == SymbolTable.end() ) {
			semanticError(this->getLineNumber(), "variable " + id + " is used before being declared");
			return 1;
		}
		return 0;
	}
	
	TypeForNode GetType() const {
		TypeForNode type = getLeft()->GetType();
		if (type == ERROR_TYPE) {
			semanticError(this->getLineNumber(), "type error");
		}
		else if (SymbolTable.find(id) != SymbolTable.end() && (SymbolTable[id].GetType() != type)) {
			semanticError(this->getLineNumber(), "type error");
		}
		return type;
	}
	
	Value eval() const {
		Value value = getLeft()->eval();
		if (value.GetType() == STRING_TYPE && SymbolTable[id].GetType() == STRING_TYPE) {
			SymbolTable[id] = value;
		}
		else if (value.GetType() == INT_TYPE && SymbolTable[id].GetType() == INT_TYPE) {
			SymbolTable[id] = value;
		}
		return value;
	}
};

class PrintStatement : public ParseTree {
	bool addNL;

public:
	PrintStatement(int line, bool isPrintln, ParseTree *ex) : ParseTree(line, ex), addNL(isPrintln) {}

	void doprinting() const {
		if( addNL ) std::cout << std::endl;
	}
		
	Value eval() const {
		if (getLeft()->eval().GetType() == INT_TYPE) {
			cout << getLeft()->eval().GetIntValue();
			doprinting();
		}
		if (getLeft()->eval().GetType() == STRING_TYPE) {
			cout << getLeft()->eval().GetStringValue();
			doprinting();
		}
		return Value();
	}
	
};

class Addition : public ParseTree {
public:
	Addition(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}

	// will need to fill in type and value;
	// remember type is a function of
	int countPlus() const { return 1; }
	
	TypeForNode GetType() const {
		TypeForNode l = getLeft()->GetType();
		TypeForNode r = getRight()->GetType();
		if (l == r && l != ERROR_TYPE && r != ERROR_TYPE) {
			return l;
		}
		else {
			return ERROR_TYPE;
		}
	}
	
	Value eval() const {
		Value result;
		if (getLeft()->eval().GetType() == INT_TYPE && getRight()->eval().GetType() == INT_TYPE) {
			result = Value(getLeft()->eval().GetIntValue() + getRight()->eval().GetIntValue());
		}
		else if (getLeft()->eval().GetType() == STRING_TYPE && getRight()->eval().GetType() == STRING_TYPE) {
			result =  Value(getLeft()->eval().GetStringValue() + getRight()->eval().GetStringValue());
		}
		return result;
	}
};

class Subtraction : public ParseTree {
public:
	Subtraction(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}

	// will need to fill in type and value;
	// remember type is a function of
	
	int findSemanticErrors() const {
		if(getLeft()->eval().GetType() == STRING_TYPE && getRight()->eval().GetType() == INT_TYPE) {
			semanticError(getLeft()->getLineNumber(), "type error");
			return 1;
		}
		return 0;
	}
	
	TypeForNode GetType() const {
		TypeForNode l = getLeft()->GetType();
		TypeForNode r = getRight()->GetType();
		if (l == INT_TYPE && r == INT_TYPE) {
			return INT_TYPE;
		}
		else {
			return ERROR_TYPE;
		}
	}
	
	Value eval() const {
		Value result;
		if (getLeft()->eval().GetType() == INT_TYPE && getRight()->eval().GetType() == INT_TYPE) {
			result =  Value(getLeft()->eval().GetIntValue() - getRight()->eval().GetIntValue());
		}
		return result;
	}
	
	
};

class Multiplication : public ParseTree {
public:
	Multiplication(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}

	// will need to fill in type and value;
	// remember type is a function of
	int countStar() const { return 1; }
	
	int findSemanticErrors() const {
		if(getLeft()->eval().GetType() == STRING_TYPE && getRight()->eval().GetType() == STRING_TYPE) {
			semanticError(getLeft()->getLineNumber(), "type error");
			return 1;
		}
		return 0;
	}
	
	TypeForNode GetType() const {
		TypeForNode l = getLeft()->GetType();
		TypeForNode r = getRight()->GetType();
		
		if (l == INT_TYPE && r == INT_TYPE) {
			return INT_TYPE;
		}
		else if (l == INT_TYPE && r == STRING_TYPE) {
			return STRING_TYPE;
		}
		else if (l == STRING_TYPE && r == INT_TYPE) {
			return STRING_TYPE;
		}
		else {
			return ERROR_TYPE;
		}
	}
	
	Value eval() const {
		Value answer;
		if (getLeft()->eval().GetType() == INT_TYPE && getRight()->eval().GetType() == INT_TYPE) {
			answer = Value(getLeft()->eval().GetIntValue() * getRight()->eval().GetIntValue());
		}
		else if (getLeft()->eval().GetType() == INT_TYPE && getRight()->eval().GetType() == STRING_TYPE) {
			string result = "";
			for (int i = 0; i < getLeft()->eval().GetIntValue(); i++) {
				result += getRight()->eval().GetStringValue();
			}
			answer = Value(result);
		}
		else if (getLeft()->eval().GetType() == STRING_TYPE && getRight()->eval().GetType() == INT_TYPE) {
			string result = "";
			for (int i = 0; i < getRight()->eval().GetIntValue(); i++) {
				result += getLeft()->eval().GetStringValue();
			}
			answer = Value(result);
		}
		return answer;
	}
};

class Division : public ParseTree {
public:
	Division(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}

	// will need to fill in type and value;
	// remember type is a function of
	
	int findSemanticErrors() const {
		if (getLeft()->eval().GetType() == INT_TYPE && getRight()->eval().GetType() == STRING_TYPE) {
			semanticError(getLeft()->getLineNumber(), "type error");
			return 1;
		}
		if (getLeft()->eval().GetType() == STRING_TYPE && getRight()->eval().GetType() == INT_TYPE) {
			semanticError(getLeft()->getLineNumber(), "type error");
			return 1;
		}
		return 0;
	}
	
	TypeForNode GetType() const {
		TypeForNode l = getLeft()->GetType();
		TypeForNode r = getRight()->GetType();
		
		if (l == r && l != ERROR_TYPE && r != ERROR_TYPE) {
			return l;
		}
		else {
			return ERROR_TYPE;
		}
	}
	
	Value eval() const {
		Value result;
		if (getLeft()->eval().GetType() == INT_TYPE && getRight()->eval().GetType() == INT_TYPE) {
			long a = (long)getLeft()->eval().GetIntValue();
			long b = (long)getRight()->eval().GetIntValue();
			int c = (int)(a / b);
			//result = Value(getLeft()->eval().GetIntValue() / getRight()->eval().GetIntValue());
			result = Value(c);
		}
		else if (getLeft()->eval().GetType() == STRING_TYPE && getRight()->eval().GetType() == STRING_TYPE) {
			string s1 = getLeft()->eval().GetStringValue();
			string s2 = getRight()->eval().GetStringValue();
			size_t find = s1.find(s2);
			if (find != string::npos) {
				string answer = s1.replace(find, s2.length(), "");
				result = Value(answer);
			}
			else {
				result = Value(s1);
			}
		}
		else {
			semanticError(getLeft()->getLineNumber(), "type error");
		}
		return result;
	}
};

class IntegerConstant : public ParseTree {
	int	value;

public:
	IntegerConstant(const Token& tok) : ParseTree(tok.GetLinenum()) {
		value = stoi( tok.GetLexeme() );
	}

	TypeForNode GetType() const { return INT_TYPE; }
	int GetIntValue() const { return value; }
	
	Value eval() const {
		Value result;
		if (GetType() == INT_TYPE) {
			result = Value(value);
		}
		else if (getLeft()->eval().GetType() == STRING_TYPE || getLeft()->eval().GetType() == ERROR_TYPE) {
			semanticError(getLeft()->getLineNumber(), "type error");
		}
		return result;
	}
};

class StringConstant : public ParseTree {
	string	value;

public:
	StringConstant(const Token& tok) : ParseTree(tok.GetLinenum()) {
		value = tok.GetLexeme();
	}

	TypeForNode GetType() const { return STRING_TYPE; }
	string GetStringValue() const { return value; }
	
	Value eval() const {
		Value result;
		if (GetType() == STRING_TYPE) {
			result =  Value(value);
		}
		else if (getLeft()->eval().GetType() == INT_TYPE || getLeft()->eval().GetType() == ERROR_TYPE) {
			semanticError(getLeft()->getLineNumber(), "type error");
		}
		return result;
	}
};

class Identifier : public ParseTree {
	string	value;

public:
	Identifier(const Token& tok) : ParseTree(tok.GetLinenum()) {
		value = tok.GetLexeme();
	}

	TypeForNode GetType() const { return STRING_TYPE; }
	int GetIntValue() const { return 0; }
	string GetStringValue() const { return "foo"; }

	int findSemanticErrors() const {
		TypeForNode type = getLeft()->GetType();
		if( SymbolTable.find(value) == SymbolTable.end() ) {
			semanticError(this->getLineNumber(), "variable " + value + " is used before being declared");
			return 1;
		}
		else if (type == ERROR_TYPE) {
			semanticError(this->getLineNumber(), "type error");
			return 1;
		}
		else if (SymbolTable.find(value) != SymbolTable.end() && (SymbolTable[value].GetType() != type)) {
			semanticError(this->getLineNumber(), "type error");
			return 1;
		}
		return 0;
	}
	
	Value eval() const {
		Value result;
		if (SymbolTable[value].GetType() == INT_TYPE) {
			int i = SymbolTable[value].GetIntValue();
			result = Value(i);
		}
		else if (SymbolTable[value].GetType() == STRING_TYPE) {
			string s = SymbolTable[value].GetStringValue();
			result = Value(s);
		}
		
		return result;
	}
};

extern ParseTree *	Prog(istream* in);
extern ParseTree *	StmtList(istream* in);
extern ParseTree *	Stmt(istream* in);
extern ParseTree *	Decl(istream* in);
extern ParseTree *	Set(istream* in);
extern ParseTree *	Print(istream* in);
extern ParseTree *	Expr(istream* in);
extern ParseTree *	Term(istream* in);
extern ParseTree *	Primary(istream* in);


#endif /* PARSER_H_ */