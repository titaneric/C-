#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
vector<vector<string> >grammer;
string search_parents(string word)
{
	string child = "";
	for (int k = 0; k < grammer.size(); k++) {
		int j = 1;
		while (j != grammer[k].size()) {
			//child = "";
			if (grammer[k][j] == word) { // find the non-terminal
				child += grammer[k][0];
				//vec.push_back(child);
			}
			j++;
		}
	}
	return child;
}
int judgeIndex(string str) {
	if (str == "S")
		return 0;
	if (str == "A")
		return 1;
	if (str == "B")
		return 2;
	if (str == "C")
		return 3;
}

int main() {
	
	grammer.resize(4);
	grammer[0].push_back("S");
	grammer[0].push_back("AB");
	grammer[0].push_back("BC");
	grammer[1].push_back("A");
	grammer[1].push_back("BA");
	grammer[1].push_back("a");
	grammer[2].push_back("B");
	grammer[2].push_back("CC");
	grammer[2].push_back("b");
	grammer[3].push_back("C");
	grammer[3].push_back("AB");
	grammer[3].push_back("a");
	string str;
	while (cin >> str) {
	//cin >> str;
		vector< vector< vector<int> > > cyk;
		cyk.resize(str.size() + 1);
		for (int i = 0; i < cyk.size(); i++) {
			cyk[i].resize(cyk.size());
		}
		for (int i = 0; i < cyk.size(); i++) {
			for (int j = 0; j < cyk[i].size(); j++) {
				cyk[i][j].resize(4);
			}
		}
		for (int i = 0; i < cyk.size(); i++) {
			for (int j = 0; j < cyk[i].size(); j++) {
				for (int k = 0; k < cyk[i][j].size(); k++) {
					cyk[i][j][k] = 0;
				}
			}
		}
		for (int i = 0; i < str.size(); i++) {
			string token = "";
			token += str[i];
			string child = search_parents(token);
			if (child.size() > 1) {
				string temp = "";
				temp += child[0];
				cyk[1][i + 1][judgeIndex(temp)] = 1;
				temp = "";
				temp += child[1];
				cyk[1][i + 1][judgeIndex(temp)] = 1;
			}
			else
				cyk[1][i + 1][judgeIndex(child)] = 1;

		}
		for (int i = 2; i < cyk.size(); i++) {
			for (int j = 1; j < cyk.size(); j++) {
				if (j > str.size() - i + 1)
					break;
				for (int k = 1; k < i; k++) {
					for (int m = 0; m < grammer.size(); m++) {
						for (int n = 1; n < grammer[m].size(); n++)
							if (grammer[m][n].size() > 1) {
								string B = "";
								B += grammer[m][n][0];
								string C = "";
								C += grammer[m][n][1];
								string word = B + C;
								string child = search_parents(word);
								int b = judgeIndex(B), c = judgeIndex(C);
								if (child.size() == 1) {

									int a = judgeIndex(child);
									if (cyk[k][j][b] == 1 && cyk[i - k][j + k][c] == 1) {
										cyk[i][j][a] = 1;
									}
								}
								else {
									string temp = "";
									temp += child[0];
									int a = judgeIndex(temp);
									if (cyk[k][j][b] == 1 && cyk[i - k][j + k][c] == 1) {
										cyk[i][j][a] = 1;
									}
									temp = "";
									temp += child[1];
									a = judgeIndex(temp);
									if (cyk[k][j][b] == 1 && cyk[i - k][j + k][c] == 1) {
										cyk[i][j][a] = 1;
									}
								}
							}
					}

				}

			}
		}
		string start = "S";
		for (int x = 0; x < start.size(); x++) {
			if (cyk[str.size()][1][x] == 1)
				cout << "yes" << endl;
			else
				cout << "no" << endl;
		}

	}

	return 0;
}