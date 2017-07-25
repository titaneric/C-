#include "MedianSelection.h"
#include "Homework1.h"
#include "stdlib.h"


int Homework1::doSelectMedian(void) {
  /*
  for (int i = 0; i < getDataLength(); ++i)
    for (int j = i + 1; j < getDataLength(); ++j)
      if (compare(i, j) > 0)
        swap(i, j);

  return (getDataLength() - 1) / 2;
  */
	
	
	int *test = new int[getDataLength()];

	for (int i = 0; i < getDataLength(); i++)
		*(test + i) = i;
	
	/*for (int i = 0; i < getDataLength(); i++)
		cout << *(arr + i)<<endl;*/
	
	int median = selection(test, getDataLength(), floor(float((getDataLength() + 1)) / float(2)));
	//int median = select(test, 0, getDataLength() - 1, floor(float(getDataLength() + 1)/float(2)) );
	
	delete []test;
	return median;
	


}

int Homework1::selection(int *list, int len, int index){
	int mid;
	if (len == 1)
		mid = list[0];
	else{
		
		int range = int(ceil(float(len) / float(5)));
		//cout << range << endl;
		int *medians = new int[range];
		

		for (int i = 0; i < range; i++){
			/*
			int *temp = list + 5 * i;
			int left = i;
			int right = i + 4;
			if (right >(len - 1))
				right = len - 1;

			medians[count++] = smartSort(temp, list[left], list[right]);
			//cout << i<<" "<<medians[i]<<endl;
			*/
			if (5 * i + 4 < len) {
				int *w = list + 5 * i;
				
				for (int j0 = 0; j0<3; j0++) {
					int jmin = j0;
					for (int j = j0 + 1; j<5; j++) {
						if (compare(w[j], w[jmin]) < 0) jmin = j;
					}
					swap(w[j0], w[jmin]);
				}
				medians[i] = w[2];

				
			}
			else {
				medians[i] = list[5 * i];
			}

		}

		int median = selection(medians, range, floor(float(range + 1) / float(2)));
		delete[]medians;
		medians = NULL;
		
		//int median = rand() % len;
		for (int i = 0; i < len; i++){
			if (list[i] == median){
				list[i] = list[0];
				list[0] = median;
			}
		}
		int *left = new int[len];
		int *right = new int[len];
		int lsize = 0;
		int rsize = 0;

		for (int i = 1; i<len; i++){
			if (compare(list[0], list[i]) > 0)
				left[lsize++] = list[i];
			else
				right[rsize++] = list[i];
		}
		if (lsize + 1 == index) {
			mid = list[0];
		}
		else if (lsize + 1 > index) {
			mid = selection(left, lsize, index);
		}
		else {
			mid = selection(right, rsize, index - lsize - 1);
		}
		delete[]left;
		left = NULL;
		delete[]right;
		right = NULL;
	}
	return mid;
}

int Homework1::select(int *list, int left, int right, int index){
	if (left == right)
		return left;
	int pivotindex = 0;
	while (true){
		pivotindex = pivot(list, left, right);
		pivotindex = partition(list, left, right, pivotindex);
		if (index == pivotindex)
			break;
		else if (index < pivotindex)
			right = pivotindex - 1;
		else
			left = pivotindex + 1;
	}
	return pivotindex;
	

}
int Homework1::partition(int *list, int left, int right, int pivotindex){
	int pivotValue = list[pivotindex];
	swap(list[pivotindex], list[right]);
	int storeindex = left;
	for (int i = left; i < right; i++){
		if (compare(list[i], pivotValue) < 0){
			swap(list[i], list[storeindex]);
			storeindex++;
		}
	}
	swap(list[right], list[storeindex]);
	return storeindex;

}
int Homework1::pivot(int *list, int left, int right){
	if ((right - left) < 5){
		return quickSort(list, left, right);
	}
	for (int i = left; i < right; i += 5){
		int subright = i + 4;
		if (subright > right)
			subright = right;
		int median5 = quickSort(list, i, subright);
		
		swap(list[median5], list[left + int(floor(float(i - left) / float(5)))]);

	}
	
	
	return select(list, left, left + int(ceil(float(right - left) / float(5))) - 1, left + (right - left) / 10);
}

int Homework1::quickSort(int *list, int left, int right){
	if (left < right)
	{
		// divide (partition)
		
		swap(right, (left + right) / 2);  

		int i = left, j = left;
		for (; j < right; ++j)
		if (compare(j, (left+right) / 2) != 1)
		{
			if (i < j) swap(i, j);   
			i = i + 1;
		}
		if (i < right) swap(i, right);    

		// then conquer
		quickSort(list, left, i - 1);
		quickSort(list, i + 1, right);

		// no need to combine sub-solutions
	}
	return list[2];
}

int Homework1::ceil(float num){
	int inum = (int)num;
	if (num == (float)inum) {
		return inum;
	}
	return inum + 1;
}
int Homework1::floor(float num){
	return int(num);
}

Homework1::~Homework1() {}



