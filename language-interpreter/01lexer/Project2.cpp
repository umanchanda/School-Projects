#include <iostream>
#include <fstream>
#include "lexer.h"
#include <vector>

using namespace std;
extern Token getToken(istream* br);
extern ostream& operator<<(ostream& out, const Token& tok);

int main(int argc, char* argv[])
{
	ifstream infile;
	Token tok;
	bool vFlag = false;
	bool sFlag = false;
	bool iFlag = false;
	
	bool checkingForArgs = true;
	bool foundOneFile = false;
	
	int tokenCount = 0;
	int identifierCount = 0;
	int stringCount = 0;
	
	
	for (int i = 1; i < argc; i++) {
		string filename = argv[i];
		infile.open(filename);
		
		if (checkingForArgs && filename[0] == '-' ) {
			if (filename == "-v" ) {
				vFlag = true;
			}
			else if (filename == "-s") {
				sFlag = true;
			}
			else if (filename == "-i" ) {
				iFlag = true;
			}
			else {
				cout << filename << " INVALID FLAG" << endl;
				return -1;
			}
			continue;
		}
		
		else {
			checkingForArgs = false;
			if (!checkingForArgs && foundOneFile) {
				if (argc > 5) {
					cerr << "TOO MANY FILES" << endl;
					return -1;
				}
			}
		}
		
		if (!infile.is_open()) {
			cerr << filename << " " << "FILE NOT FOUND" << endl;
			break;
		}
		
		foundOneFile = true;
		
		while((tok = getToken(&infile)) != T_DONE && tok != T_ERROR) {
			if (tok != T_DONE && tok != T_ERROR) {
				tokenCount++;
			}
			
			if (tok == T_ID) {
				identifierCount++;
			}
			
			if (tok == T_SCONST) {
				stringCount++;
			}
		}
			
			/* if (tok == T_ERROR) {
				cout << "Lexical error" << " " << getToken(&infile) << endl;
			} */
		
			cout << "Token count: " << tokenCount << endl;
			cout << "Identifier count: " << identifierCount << endl;
			cout << "String count: " << stringCount << endl;
		
		if (vFlag) {
			
		}
		
		if (sFlag) {
			
		}
		
		if (iFlag) {
			
		}
		
		else {
			
		}
	}
}
