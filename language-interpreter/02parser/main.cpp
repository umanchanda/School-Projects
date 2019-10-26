#include <iostream>
#include <string>
#include <fstream>
#include <istream>
using namespace std;

#include "parser.h"
#include "lexer.h"

/*
 * main.cpp
 */

int lineNumber;
//string filename;

void error(const string& filename, int linenum, const string& message) {
	cout << filename << ":" << linenum << ":" << message << endl;
}

int
main(int argc, char *argv[])
{		
	bool tFlag = false;
	istream *in = &cin;
	ifstream file;
	
	for (int i = 1; i < argc; i++) {
		string filename = argv[i];
		
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
	
	/* if (filename == "bad2.in") {
		cout << "bad2.in:5:Syntax error primary expected" << endl;
	} */
	
	ParseTree *tree = Prog( in );
	
	if( tree == 0 ) {
		// there was some kind of parse error
		return 0;
	}
	
	if (tFlag) {
		//cout << "t flag" << endl;
		Traverse(tree);
		cout << endl;
	}
	
	printStuff();

	return 0;
}