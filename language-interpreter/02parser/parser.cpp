/*
 * parser.cpp
 *
 */

#include <string>
#include <iostream>
#include <istream>
#include <fstream>
#include <map>
#include <sstream>
using std::string;

#include "lexer.h"
#include "parser.h"

using namespace std;
string filename;
map<string,string> variables;

class ParserToken {
private:
	Token	tok;
	bool	pushedBack;
	

public:
	ParserToken() : pushedBack(false) {}

	Token getToken(istream *in) {
		if( pushedBack ) {
			pushedBack = false;
			return tok;
		}

		return ::getToken(in);
	}
	void pushbackToken(const Token& t) {
		if( pushedBack ) {
			// error
			return;
		}
		tok = t;
		pushedBack = true;
	}
} ParserToken;

void
error(const string& msg) {
	cout << msg << endl;
	return;
}

ParseTree *Prog(istream* in, string& filename) {
	//cout << "prog" << endl;
	return StmtList(in);
}

ParseTree *StmtList(istream* in) {
	//cout << "stmtlist" << endl;
	ParseTree *stmt = Stmt(in);
	Token tok = ParserToken.getToken(in);
	
	if( stmt == 0 ) {
		return 0;
	}
	
	if (tok == T_SC) {
		return new StatementList( stmt, StmtList(in) );
	}
	
	else if (tok != T_SC) {
		error(filename, tok.GetLinenum(), "Syntax error semicolon required");
	}
	
	return 0;
}

ParseTree *	Stmt(istream* in) {
	//cout << "stmt" << endl;
	
	ParseTree *decl = Decl(in);
	ParseTree *set = Set(in);
	ParseTree *print = Print(in);
	
	if (decl != 0) {
		return decl;
	}
	
	else if (set != 0) {
		return set;
	}
	
	else if (print != 0) {
		return print;
	}
	
	return 0;
}

ParseTree *	Decl(istream* in) {
	//cout << "decl" << endl;
	Token tok = ParserToken.getToken(in);
	
	if (tok == T_INT) {
		Token tok1 = ParserToken.getToken(in);
		if (tok1 == T_ID) {
			ParseTree *id = new Identifier(tok1);
			return new DeclarationStatement(tok1.GetLinenum(), id, 0);
		}
		else {
			ParserToken.pushbackToken(tok1);
			error(filename, tok.GetLinenum(), "Syntax error primary required");
		}
	}
	else {
		ParserToken.pushbackToken(tok);
	}
	
	if (tok == T_STRING) {
		Token tok1 = ParserToken.getToken(in);
		if (tok1 == T_ID) {
			ParseTree *id = new Identifier(tok1);
			return new DeclarationStatement(tok1.GetLinenum(), id, 0);
		}
		else {
			ParserToken.pushbackToken(tok1);
			error(filename, tok.GetLinenum(), "Syntax error primary required");
		}
	}
	
	else {
		ParserToken.pushbackToken(tok);
	}
	return 0;
}

ParseTree *	Set(istream* in) {
	//cout << "set" << endl;
	Token tok = ParserToken.getToken(in);
	if (tok == T_SET) {
		Token tok2 = ParserToken.getToken(in);
		if (tok2 == T_ID) {
			ParseTree *expr = Expr(in);
			if (expr == 0) {
				error(filename, tok2.GetLinenum(), "Syntax error primary required");
			}
			return new SetStatement(tok2.GetLinenum(), expr, 0);
		}
		else {
			error(filename, tok.GetLinenum(), "Syntax error primary required");
		}
	}
	else {
		ParserToken.pushbackToken(tok);
	}
	return 0;
}

ParseTree *	Print(istream* in) {
	//cout << "print" << endl;
	Token tok = ParserToken.getToken(in);
	if (tok == T_PRINT) {
		ParseTree *expr = Expr(in);
		if (expr == 0) {
			error(filename, tok.GetLinenum(), "Syntax error expression required");
		}
		return new PrintStatement(tok.GetLinenum(), expr, 0);
	}
	
	else if (tok == T_PRINTLN) {
		ParseTree *expr = Expr(in);
		if (expr == 0) {
			error(filename, tok.GetLinenum(), "Syntax error expression required");
		}
		return new PrintlnStatement(tok.GetLinenum(), expr, 0);
	}
	
	else {
		ParserToken.pushbackToken(tok);
		return 0;
	}
}

ParseTree *Expr(istream* in) {
	//cout << "expr" << endl;
	ParseTree *t1 = Term(in);
	if( t1 == 0 ) {
		return 0;
	}

	for(;;) {
		Token op = ParserToken.getToken(in);
		if( op != T_PLUS && op != T_MINUS ) {
			ParserToken.pushbackToken(op);
			return t1;
		}

		ParseTree *t2 = Expr(in);
		if( t2 == 0 ) {
			error(filename, op.GetLinenum(), "Syntax error expression required after + or - operator");
			return 0;
		}

		// combine t1 and t2 together
		if( op == T_PLUS )
			t1 = new Addition(op.GetLinenum(), t1, t2);
		else
			t1 = new Subtraction(op.GetLinenum(), t1, t2);
	}

	// should never get here...
	return 0;
}

ParseTree *Term(istream* in) {
	//cout << "term" << endl;
	ParseTree *p1 = Primary(in);
	if( p1 == 0 ) {
		return 0;
	}

	for(;;) {
		Token op = ParserToken.getToken(in);
		if( op != T_PLUS && op != T_MINUS ) {
			ParserToken.pushbackToken(op);
			return p1;
		}

		ParseTree *p2 = Term(in);
		if( p2 == 0 ) {
			error(filename, op.GetLinenum(), "Syntax error expression required after * or / operator");
			return 0;
		}

		// combine p1 and p2 together
		if( op == T_STAR )
			p1 = new Multiplication(op.GetLinenum(), p1, p2);
		else
			p1 = new Division(op.GetLinenum(), p1, p2);
	}

	// should never get here...
	return 0;
}

ParseTree *	Primary(istream* in) {
	//cout << "primary" << endl;
	Token tok = ParserToken.getToken(in);
	
	if (tok == T_ICONST) {
		return new IntegerConstant(tok);
	}
	
	else if (tok == T_SCONST) {
		return new StringConstant(tok);
	}
	
	else if (tok == T_ID) {
		return new Identifier(tok);
	}
	
	if (tok == T_LPAREN) {
		ParseTree *expr = Expr(in);
		if (expr != 0) {
			Token tok2 = ParserToken.getToken(in);
			if (tok2 == T_RPAREN) {
				return expr;
			}
			else {
				error(filename, tok.GetLinenum(), "Syntax error right paren expected");
				ParserToken.pushbackToken(tok2);
				return 0;
			}
		}
		else {
			error(filename, tok.GetLinenum(), "Syntax error primary required");
			ParserToken.getToken(in);
			return 0;
		}
	}
	
	else {
		error(filename, tok.GetLinenum(), "Syntax error primary required");
		return 0;
	}
}

void Traverse(ParseTree *node) {
	if (node->getLeft() != 0) {
		cout << "L";
		Traverse(node->getLeft());
		cout << "u";
	}
	
	if (node->getRight() != 0) {
		cout << "R";
		Traverse(node->getRight());
		cout << "U";
	}
	cout << "N";
}

int lineNumber;

void error(const string& filename, int linenum, const string& message) {
	cout << filename << ":" << linenum+1 << ":" << message << endl;
}

int
main(int argc, char *argv[])
{		
	bool tFlag = false;
	istream *in = &cin;
	ifstream file;
	
	for (int i = 1; i < argc; i++) {
		filename = argv[i];
		
		if (filename == "-t") {
			tFlag = true;
		}
		
		else if( filename[0] == '-' ) {
			cerr << filename << " UNRECOGNIZED FLAG" << endl;
			return 0;
		}
		
		else if (in != &cin) {
			cerr << "TOO MANY FILES" << endl;
			return 0;
		}
		
		else {
			file.open(filename);
			if(!file.is_open()) {
				cerr << filename << " FILE NOT FOUND" << endl;
				return 0;
			}
			in = &file;
		}
	}
	
	ParseTree *tree = Prog( in, filename );
	
	if( tree == 0 ) {
		// there was some kind of parse error
		return 0;
	}
	
	if (tFlag) {
		//cout << "t flag" << endl;
		Traverse(tree);
		cout << endl;
	}
	
	map<string,string> variables;
	
	cout << "Total number of identifiers: " << tree->countIDNodes() << endl;
	cout << "Total number of set: " << tree->countSetNodes() << endl;
	cout << "Total number of +: " << tree->countPlusNodes() << endl;
	cout << "Total number of *: " << tree->countStarNodes() << endl;

	return 0;
}