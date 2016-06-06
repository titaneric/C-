//1033311 prog3 #build a min heap
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iterator>
using namespace std;
struct greater1 {
	bool operator()(const int& a, const int& b) const {
		return a>b;
	}
};
vector<string> split(const string &source, const string &delim)
{
	vector<string> ans;
	size_t begin_pos = 0, end_pos = source.find(delim); // locate the first delimiter in string
	while (end_pos != string::npos) {
		ans.push_back(source.substr(begin_pos, end_pos - begin_pos)); // extract the sub-string before current delimiter
		begin_pos = end_pos + delim.size();
		end_pos = source.find(delim, begin_pos);  // locate the next delimiter in string
	}
	ans.push_back(source.substr(begin_pos, end_pos - begin_pos));  // extract the last sub-string
	return ans;
}
int main() {
	vector<string>vec;
	string str;
	vector<int>deleteVec;
	vector<int>heapVec;
	while (getline(cin, str)) {
		if (str[0] == 'I') {
			str = str.substr(2, str.size());
			vec = split(str, ",");
		}
		else if (str[0] == 'D') {
			str = str.substr(2, str.size());
			deleteVec.push_back(atoi(str.c_str()));
		}

	}
	heapVec.push_back(-1);
	for (int i = 0; i < vec.size(); i++) {
		heapVec.push_back(atoi(vec[i].c_str()));
		if (i == 0)
			make_heap(next(heapVec.begin(), 1), heapVec.end(), greater1());
		else
			push_heap(next(heapVec.begin(), 1), heapVec.end(), greater1());
	}

	
	int i = 0;
	/*
	cout << "Before the heaping:" << endl;
	for (int i = 0; i < heapVec.size(); i++) {
		cout << heapVec[i] << " ";
	}
	cout << endl;
	*/
	//make_heap(heapVec.begin(), heapVec.end(), greater1());
	cout << "After the heaping:" << endl;
	for (int i = 1; i < heapVec.size(); i++) {
		cout << heapVec[i] << " ";
	}
	cout << endl;
	while (deleteVec.size()) {
		if (*deleteVec.begin() > heapVec.size()) {
			cout << "Deleting the index wrongly, program will skip this deleting";
			deleteVec.erase(deleteVec.begin());
		}
		//*find(heapVec.begin(), heapVec.end(), *deleteVec.begin()) == 1000000;
		swap(heapVec[*deleteVec.begin()], heapVec[heapVec.size() - 1]);
		//heapVec[*deleteVec.begin()] = heapVec[heapVec.size() - 1];
		heapVec.erase(heapVec.begin() + (heapVec.size() - 1));
		make_heap(next(heapVec.begin(), 1), heapVec.end(), greater1());
		
		cout << "After deleting the " << *deleteVec.begin() << " th element:" << endl;
		for (int i = 1; i < heapVec.size(); i++) {
			cout << heapVec[i] << " ";
		}
		cout << endl;
		deleteVec.erase(deleteVec.begin());
	}

	




	return 0;
}