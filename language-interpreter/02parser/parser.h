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
using std::string;
using std::stoi;

#include <map>
using std::map;

#include "lexer.h"

extern void error(const string& filename, int linenum, const string& message);


enum TypeForNode { INT_TYPE, STRING_TYPE, ERROR_TYPE };

class ParseTree {
	int			linenumber;
	ParseTree	*left;
	ParseTree	*right;
	Token t;

public:
	ParseTree(int n, ParseTree *l = 0, ParseTree *r = 0) : linenumber(n), left(l), right(r) {}
	virtual ~ParseTree() {}

	ParseTree* getLeft() const { return left; }
	ParseTree* getRight() const { return right; }
	int getLineNumber() const { return linenumber; }

	virtual TypeForNode GetType() const { return ERROR_TYPE; }
	virtual int GetIntValue() const { throw "no integer value"; }
	virtual string GetStringValue() const { throw "no string value"; }
	
	virtual bool isPlus() { return false; }
	virtual bool isStar() { return false; }
	virtual bool isSet() { return false; }
	virtual bool isID() { return false; }
	virtual int checkVariables(map<string,string>& variables) { return 0; }
	
	int countPlusNodes() {
		int count = 0;
		if (left) {
			count += left->countPlusNodes();
		}
		if (right) {
			count += right->countPlusNodes();
		}
        if(this->isPlus()) {
            count++;
		}
		return count;
	}
	
	int countStarNodes() {
		int count = 0;
		if (left) {
			count += left->countStarNodes();
		}
		if (right) {
			count += right->countStarNodes();
		}
        if(this->isStar()) {
            count++;
		}
		return count;
	}
	
	int countSetNodes() {
		int count = 0;
		if (left) {
			count += left->countSetNodes();
		}
		if (right) {
			count += right->countSetNodes();
		}
        if(this->isSet()) {
            count++;
		}
		return count;
	}
	
	int countIDNodes() {
		int count = 0;
		if (left) {
			count += left->countIDNodes();
		}
		if (right) {
			count += right->countIDNodes();
		}
        if(this->isID()) {
            count++;
		}
		return count;
	}
	
	int countVariables(map<string, string>& variables) {
		int count = 0;
		if (left) {
			count += left->countVariables(variables);
		}
		if (right) {
			count += right->countVariables(variables);
		}
		return count + this->checkVariables(variables);
	}
};

class StatementList : public ParseTree {
public:
	StatementList(ParseTree *first, ParseTree *rest) : ParseTree(0, first, rest) {}

};

class PrintStatement : public ParseTree {
public:
	PrintStatement(int line, ParseTree *left, ParseTree *right) : ParseTree(line, left, right) {}
	
};

class PrintlnStatement : public ParseTree {
public:
	PrintlnStatement(int line, ParseTree *left, ParseTree *right) : ParseTree(line, left, right) {}
	
};

class SetStatement : public ParseTree {
public:
	SetStatement(int line, ParseTree *left, ParseTree *right) : ParseTree(line, left, right) {}

	bool isSet() { return true; }
};

class DeclarationStatement : public ParseTree {
public:
	DeclarationStatement(int line, ParseTree *left, ParseTree *right) : ParseTree(line, left, right) {}
	
};

class Addition : public ParseTree {
public:
	Addition(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}
	
	bool isPlus() { return true; }
};

class Subtraction : public ParseTree {
public:
	Subtraction(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}

};

class Multiplication : public ParseTree {
public:
	Multiplication(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}
	
	bool isStar() { return true; }
};

class Division : public ParseTree {
public:
	Division(int line, ParseTree *op1, ParseTree *op2) : ParseTree(line, op1, op2) {}
	
};

class IntegerConstant : public ParseTree {
	int	value;
	
public:
	IntegerConstant(const Token& tok) : ParseTree(tok.GetLinenum()) {
		value = stoi( tok.GetLexeme() );
	}

	TypeForNode getType() const { return INT_TYPE; }
	int GetIntValue() const { return value; }
};

class StringConstant : public ParseTree {
	string value;

public:
	StringConstant(const Token& tok) : ParseTree(tok.GetLinenum()) {
		value = tok.GetLexeme();
	}
	
	TypeForNode GetType() const { return STRING_TYPE; }
	string GetStringValue() const { return value; }
};

class Identifier : public ParseTree {
public:
	Identifier(const Token& tok) : ParseTree(tok.GetLinenum()) {}
	
	bool isID() { return true; }
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
extern void Traverse(ParseTree *node);
extern void statistics(ParseTree *node);
extern void printTree(ParseTree *node, int space);


#endif /* PARSER_H_ */