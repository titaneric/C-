#include "SelectExtremes.h"
#include "Homework2.h"
#include <vector>


Homework2::~Homework2() {}

int * Homework2::doSelect(int *ans) {
	/*
	int** winary = new int*[13];//13 is the log _2  5001
	for (int i = 0; i < getDataLength(); ++i)
	winary[i] = new int[getDataLength()];
	*/
	vector< vector<int> >winVec;
	vector< vector<int> >loseVec;
	//int winary[14][5001];
	vector<int>l_vec;
	
	vector<int>w_vec;
	for (int i = 0; i < getDataLength(); i += 2){
		if (i != getDataLength() - 1){
			if (compare(i, i + 1) >= 0) {//put the winner into the first row
				w_vec.push_back(i);
				l_vec.push_back(i + 1);
			}
			else{
				w_vec.push_back(i + 1);
				l_vec.push_back(i);
			}
		}
	}
	l_vec.push_back(getDataLength() - 1);

	int size = w_vec.size();
	//cout << "0 " << size<<endl;
	winVec.push_back(w_vec);
	loseVec.push_back(l_vec);
	w_vec.clear();
	l_vec.clear();
	//cout << size << endl;
	int k = 1;
	while (size != 1){
		
		for (int i = 0; i < size; i += 2){

			
			if (i != size - 1){
				if (compare(winVec[k - 1][i], winVec[k - 1][i + 1]) >= 0)//put the winner into the first row
					w_vec.push_back(winVec[k - 1][i]);
					
				
				else
					w_vec.push_back(winVec[k - 1][i + 1]);
					
				
			}

		}
		if (size % 2 == 1){
			w_vec.push_back(winVec[k - 1].back());
			

		}

		winVec.push_back(w_vec);
		
		//cout << k << " " << winary[k].size() << endl;
		size = winVec[k].size();
		w_vec.clear();
		
		k++;

	}
	
	vector<int>competitor;	//store the maximum competitor index
	for (int i = 0; i < 12; i++){
		int index = findIndex(winVec[12][0], winVec[i]);
		
		if (winVec[i].size() % 2 == 0){
			if (index % 2 == 0)
				competitor.push_back(winVec[i][index + 1]);
			else
				competitor.push_back(winVec[i][index - 1]);
		}
		else{
			if(index % 2 == 0 && index!=(winVec[i].size()-1))
				competitor.push_back(winVec[i][index + 1]);
			else if (index % 2 == 1)
				competitor.push_back(winVec[i][index - 1]);

		}
		
	}
	if (winVec[12][0] % 2 == 0)
		competitor.push_back(winVec[12][0] + 1);
	else
		competitor.push_back(winVec[12][0] - 1);
	int max = 0;
	for (int i = 1; i < competitor.size(); i++){
		if (compare(competitor[i], competitor[max]) >= 0)
			max = i;
	}
	

	ans[0] = winVec[12][0];
	ans[2] = competitor[max];

	size = loseVec[0].size();
	k = 1;
	while (size != 1){

		for (int i = 0; i < size; i += 2){


			if (i != size - 1){
				if (compare(loseVec[k - 1][i], loseVec[k - 1][i + 1]) >= 0)//put the winner into the first row
					l_vec.push_back(loseVec[k - 1][i+1]);


				else
					l_vec.push_back(loseVec[k - 1][i]);


			}

		}
		if (size % 2 == 1){
			l_vec.push_back(loseVec[k - 1].back());


		}

		loseVec.push_back(l_vec);

		//cout << k << " " << winary[k].size() << endl;
		size = loseVec[k].size();
		l_vec.clear();

		k++;

	}
	competitor.clear();

	
	for (int i = 0; i < 12; i++){
		int index = findIndex(loseVec[12][0], loseVec[i]);
		
		if (loseVec[i].size() % 2 == 0){
			if (index % 2 == 0)
				competitor.push_back(loseVec[i][index + 1]);
			else
				competitor.push_back(loseVec[i][index - 1]);
		}
		else{
			if (index % 2 == 0 && index != (loseVec[i].size() - 1))
				competitor.push_back(loseVec[i][index + 1]);
			else if (index % 2 == 1)
				competitor.push_back(loseVec[i][index - 1]);

		}
	}
	if (loseVec[12][0] % 2 == 0)
		competitor.push_back(loseVec[12][0] + 1);
	else
		competitor.push_back(loseVec[12][0] - 1);
	int min = 0;
	for (int i = 1; i < competitor.size(); i++){
		if (compare(competitor[i], competitor[min]) <= 0)
			min = i;
	}

		
	
	
	ans[1] = loseVec[12][0];
	ans[3] = competitor[min];
		/*
		int size = 1;
		int adder = 1;
		//int *myArray = new int[2*getDataLength()];


		//find the maximum in the array, and put it to index 0
		//int cur = 0;	//the last competitor of maximum index, and it must be even number
		//int count = 0;
		while (adder < (getDataLength() / 2)){
		for (int i = 0; i < getDataLength(); i += (2 * size)){
		if (i + adder < getDataLength()){
		if (compare(i, i + adder) < 0){//&& i + adder > 0){	//if i < i+adder then swap them
		swap(i, i + adder);	//(winner, loser)
		//*(myArray + count) = i + adder;	//store the competitor info

		}

		}
		}
		size *= 2;
		adder *= 2;
		}
		//cout << count<<endl;

		if (getDataLength() % 2 == 1){
		if (compare(0, getDataLength() - 1) < 0){
		swap(0, getDataLength() - 1);
		}


		ans[0] = 0;
		*/
		return ans;
}

int Homework2::findIndex(int value, vector<int>vec){
	int index=0;
	for (int i = 0; i < vec.size(); i++){
		if (vec[i] == value)
			index = i;

	}
	return index;

}


