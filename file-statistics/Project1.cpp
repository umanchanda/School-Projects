/*
Uday Manchanda
CS 280-005
Project 1
*/

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
using namespace std;

int main(int argc, char* argv[])
{
	ifstream infile;
	
	bool cFlag = false;
	bool bFlag = false;
	bool mFlag = false;
	
	if (argc <= 1) {
		cerr << "NO FILES" << endl;
		return -1;
	}
	
	for (int i = 1; i < argc; i++) {
		infile.open(argv[i]);
		string filename = argv[i];
		
		// determine if a argument is a flag
		if (filename.front() == '-') {
			if (filename.find("m") == 1) {
				mFlag = true;
				continue;
			}
			else if (filename.find("c") == 1) {
				cFlag = true;
				continue;
			}
			else if (filename.find("b") == 1) {
				bFlag = true;
				continue;
			}
			else {
				cerr << filename << " " << "UNRECOGNIZED FLAG" << endl;
			}
			continue;
		}
		
		// check if the file is valid
		else if (!infile.is_open()) {
			cerr << filename << " " << "FILE NOT FOUND" << endl;
			continue;
		}
		
		string longestLine = "";
		string longestWord = "";
		string currentLine;
		string currentWord;
		map<int, int> lines;
		map<string, int> words;
		
		// adds all linecounts and their associated frequencies to the map called lines
		while(getline(infile, currentLine)) {
			int currlinelength = currentLine.length();
			if (lines.find(currlinelength) == lines.end()) {
				lines[currlinelength] = 1;
			}
			else {
				lines[currlinelength]++;
			}
		}
		infile.close();
		
		// add all words and their associated frequencies to the map called words
		infile.open(argv[i]);
		while (infile >> currentWord) {
			if (words.find(currentWord) == words.end()) {
				words[currentWord] = 1;
			}
			else {
				words[currentWord]++;
			}
		}
		infile.close();
		
		if (cFlag && bFlag && mFlag) {
			
		}
		
		else if (cFlag && mFlag) {
			cout << filename << ":" << endl;
			map<int,int> longestLines;
			map<string,int> longestWords;
			
			int longestlinelength = 0;
			
			for (auto& j : lines) {
				if (j.first == longestlinelength) {
					longestLines[j.first] = j.second;
				}
				else if (j.first > longestlinelength) {
					longestLines.clear();
					longestlinelength = j.first;
					longestLines[j.first] = j.second;
				}
			}
			
			for (auto& i : words) {
				if (i.first.length() == longestWord.length()) {
					longestWords[i.first] = i.second;
				}
				else if (i.first.length() > longestWord.length()) {
					longestWords.clear();
					longestWord = i.first;
					longestWords[i.first] = i.second;
				}
			}
			
			int maxfrequency = 1;
			map<int,int> longestLinesFrequency;
			map<string,int> longestWordsFrequency;
			
			for (auto& a: longestLines) {
				if (a.second == maxfrequency) {
					longestLinesFrequency[a.first] = a.second;
				}
				else if (a.second > maxfrequency) {
					longestLinesFrequency.clear();
					maxfrequency = a.second;
					longestLinesFrequency[a.first] = a.second;
				}
			}
			
			maxfrequency = 1;
			for (auto& b: longestWords) {
				if (b.second == maxfrequency) {
					longestWordsFrequency[b.first] = b.second;
				}
				else if (b.second > maxfrequency) {
					longestWordsFrequency.clear();
					maxfrequency = b.second;
					longestWordsFrequency[b.first] = b.second;
				}
			}
			
			ostringstream os;
			for (auto& c : longestLinesFrequency) {
				os << c.first << "(" << c.second << ")" << ", ";
			}
			string linetext = os.str();
			string lines = linetext.substr(0, linetext.size()-2);
			
 			ostringstream oss;
			for (auto& d : longestWordsFrequency) {
				oss << d.first << "(" << d.second << ")" << ", ";
			}
			string wordtext = oss.str();
			string words = wordtext.substr(0, wordtext.size()-2);
			
			if (!longestWordsFrequency.empty()) {
				cout << words << endl;
			}
			cout << lines << endl;
			continue;
		}
		
		else if (cFlag) {
			cout << filename << ":" << endl;
			map<int,int> longestLines;
			map<string,int> longestWords;
			
			int longestlinelength = 0;
			
			for (auto& j : lines) {
				if (j.first == longestlinelength) {
					longestLines[j.first] = j.second;
				}
				else if (j.first > longestlinelength) {
					longestLines.clear();
					longestlinelength = j.first;
					longestLines[j.first] = j.second;
				}
			}
			
			for (auto& i : words) {
				if (i.first.length() == longestWord.length()) {
					longestWords[i.first] = i.second;
				}
				else if (i.first.length() > longestWord.length()) {
					longestWords.clear();
					longestWord = i.first;
					longestWords[i.first] = i.second;
				}
			}
			
			ostringstream os;
			for (auto& a : longestLines) {
				os << a.first << "(" << a.second << ")" << ", ";
			}				
			string linetext = os.str();
			string lines = linetext.substr(0, linetext.size()-2);
			
				
			ostringstream oss;
			for (auto& b : longestWords) {
				oss << b.first << "(" << b.second << ")" << ", ";
			}
			string wordtext = oss.str();
			string words = wordtext.substr(0, wordtext.size()-2);
			
			if (!longestWords.empty()) {
				cout << words << endl;
			}
			cout << lines << endl;
			continue;
		}
		
		else if (mFlag) {
			cout << filename << ":" << endl;
			map<int,int> longestLines;
			map<string,int> longestWords;
			
			int longestlinelength = 0;
			
			for (auto& j : lines) {
				if (j.first == longestlinelength) {
					longestLines[j.first] = j.second;
				}
				else if (j.first > longestlinelength) {
					longestLines.clear();
					longestlinelength = j.first;
					longestLines[j.first] = j.second;
				}
			}
			
			for (auto& i : words) {
				if (i.first.length() == longestWord.length()) {
					longestWords[i.first] = i.second;
				}
				else if (i.first.length() > longestWord.length()) {
					longestWords.clear();
					longestWord = i.first;
					longestWords[i.first] = i.second;
				}
			}
			
			int maxfrequency = 1;
			map<int,int> longestLinesFrequency;
			map<string,int> longestWordsFrequency;
			
			for (auto& a: longestLines) {
				if (a.second == maxfrequency) {
					longestLinesFrequency[a.first] = a.second;
				}
				else if (a.second > maxfrequency) {
					longestLinesFrequency.clear();
					maxfrequency = a.second;
					longestLinesFrequency[a.first] = a.second;
				}
			}
			
			maxfrequency = 1;
			for (auto& b: longestWords) {
				if (b.second == maxfrequency) {
					longestWordsFrequency[b.first] = b.second;
				}
				else if (b.second > maxfrequency) {
					longestWordsFrequency.clear();
					maxfrequency = b.second;
					longestWordsFrequency[b.first] = b.second;
				}
			}
			
			ostringstream os;
			for (auto& c : longestLinesFrequency) {
				os << c.first << ", ";
			}
			string linetext = os.str();
			string lines = linetext.substr(0, linetext.size()-2);
			
 			ostringstream oss;
			for (auto& d : longestWordsFrequency) {
				oss << d.first << ", ";
			}
			string wordtext = oss.str();
			string words = wordtext.substr(0, wordtext.size()-2);
			
			if (!longestWordsFrequency.empty()) {
				cout << words << endl;
			}
			
			cout << lines << endl;
			continue;
		}
		
		else if (bFlag) {
			int currlinelength;
			int numwordcount;
			//vector<int> blines;
			map<int,int> blines;
			
			infile.open(argv[i]);
			while (getline(infile, currentLine)) {
				currlinelength = 0;
                numwordcount = 0;
				istringstream stream(currentLine);
				while (stream >> currentWord) {
					currlinelength += currentWord.length();
					numwordcount++;
				}
				currlinelength += (numwordcount-1);
				//blines.push_back(currlinelength);
				if (blines.find(currlinelength) == blines.end()) {
					blines[currlinelength] = 1;
				}
				else {
					blines[currlinelength]++;
				}
			}
			infile.close();
			
			for (auto& i : blines) {
				if (blines[i] == maxlinelength) {
					maxlinelength = blines[i];
				}
				//cout << i.first << ":" << i.second << ", ";
			}
			cout << endl;
			
			cout << filename << ":" << endl;
			vector<string> longestWords;
			map<int,int> longestLines;
			int longestlinelength = 0;
			
			for (auto& j : blines) {
				if (j.first == longestlinelength) {
					longestLines[j.first] = j.second;
				}
				else if (j.first > longestlinelength) {
					longestLines.clear();
					longestlinelength = j.first;
					longestLines[j.first] = j.second;
				}
			}
				
			for (auto& i : words) {
				if (i.first.length() == longestWord.length()) {
					longestWords.push_back(i.first);
				}
				else if (i.first.length() > longestWord.length()) {
					longestWords.erase(longestWords.begin(), longestWords.end());
					longestWord = i.first;
					longestWords.push_back(i.first);
				}
			}
				
			for (auto& x : longestLines) {
				cout << x.first << ":" << x.second << ", ";
			}
			cout << endl;

			ostringstream oss;
			for (unsigned b = 0; b < longestWords.size(); b++) {
				oss << longestWords[b] << ", ";
			}
			string wordtext = oss.str();
			string words = wordtext.substr(0, wordtext.size()-2);
			
			ostringstream os;
			for (auto& a : longestLines) {
				os << a.first << ", ";
			}
			string linetext = os.str();
			string lines = linetext.substr(0, linetext.size()-2);
			
			if (!longestWords.empty()) {
				cout << words << endl;
			}
			cout << lines << endl;
			continue;
		}
		
		else {
			cout << filename << ":" << endl;
			vector<string> longestWords;
			vector<int> longestLines;
			int longestlinelength = 0;
			
			for (auto& j : lines) {
				if (j.first == longestlinelength) {
					longestLines.push_back(j.first);
				}
				else if (j.first > longestlinelength) {
					longestLines.erase(longestLines.begin(), longestLines.end());
					longestlinelength = j.first;
					longestLines.push_back(j.first);
				}		
			}
				
			for (auto& i : words) {
				if (i.first.length() == longestWord.length()) {
					longestWords.push_back(i.first);
				}
				else if (i.first.length() > longestWord.length()) {
					longestWords.erase(longestWords.begin(), longestWords.end());
					longestWord = i.first;
					longestWords.push_back(i.first);
				}
			}
				
			ostringstream os;
			for (unsigned a = 0; a < longestLines.size(); a++) {
				os << longestLines[a] << ", ";
			}				
			string linetext = os.str();
			string lines = linetext.substr(0, linetext.size()-2);
				
			ostringstream oss;
			for (unsigned b = 0; b < longestWords.size(); b++) {
				oss << longestWords[b] << ", ";
			}
			string wordtext = oss.str();
			string words = wordtext.substr(0, wordtext.size()-2);
			
			if (!longestWords.empty()) {
				cout << words << endl;
			}
			cout << lines << endl;
			continue;
		}
	}
	return 0;
}