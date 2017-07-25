#include <stdlib.h>
#include <iostream>
#include <math.h>

using namespace std;

int compare(const void *a, const void *b){

	if (sin(*(double*)a) <  sin(*(double*)b)) return -1;
	if (sin(*(double*)a) == sin(*(double*)b)) return 0;
	if (sin(*(double*)a) >  sin(*(double*)b)) return 1;


}
int main(){
	double inputArr[5] = {};

	cout << "Input Array: ";
	for (int i = 0; i < 5; i++){
		cin >> inputArr[i];
		while (inputArr[i]>100||inputArr[i]<0)
		{
			cout << "Input error!";
			cin >> inputArr[i];
		}


	}

	qsort(inputArr, 5, sizeof(double), compare);


	cout << "Output:" << endl;
	for (int i = 0; i < 5; i++)
		cout << inputArr[i] << " ";
	cout << endl;
	system("pause");

	return 0;
}