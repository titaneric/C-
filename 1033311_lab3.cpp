// Copyright (c) @author: TitanEric
#include<iostream>
#include<string>
#include<vector>
using namespace std;
void createStringArray(string text, string delimeter, vector<string> &strVec);
int main() {
	string text;
	string deli;
	while (getline(cin, text) && getline(cin, deli)) {

		vector<string> splitResult;
		createStringArray(text, deli,splitResult);
		cout << splitResult.size() << endl;
		for (int i = 0; i < splitResult.size(); i++) {
			cout << splitResult[i] << endl;
		}
	}
	//system("pause");
	return 0;
}
void createStringArray(string text, string delimeter, vector<string> &strVec) {
	//vector<string> strVec;
	while (text.find(delimeter) != string::npos) {
		size_t found = text.find(delimeter);
		string split = text.substr(0, found);
		strVec.push_back(split);
		text = text.substr(delimeter.size() + split.size());
	}
	
	/*if (text.size() != 0)*/
	strVec.push_back(text);
	
	//return strVec;
}