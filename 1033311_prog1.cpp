//1033311 ∂¿±·≈t 2016/03/21 program_1
//print the dealing time of iterative & recursive Fibonacci number  
#include <iostream>
#include <time.h>
#include <stdio.h>
using namespace std;
unsigned long long recursiveFibo(unsigned int index);
unsigned long long iterativeFibo(unsigned int index);
int main(){


	int i, step = 5;
	unsigned int n;
	unsigned long long fibo;
	double duration;
	printf("\t\tIterative Fibonacci Number\n\n");
	printf("     n                 f(n)   repetitions\ttime\n");
	printf("-----------------------------------------------------\n");
	for (n = 0; n <= 90; n += step)
	{
		long repetitions = 0;
		clock_t start = clock();//statr
		do
		{
			repetitions++;//count the repetition
			fibo = iterativeFibo(n); 
		} while (clock() - start < 1000);//when the clock time is smaller than 1 sec, do the loop

		duration = ((double)(clock() - start)) / CLOCKS_PER_SEC;
		duration /= repetitions;
		printf("%6d %20llu %12d\t%f\n", n, fibo, repetitions, duration);

	}

	duration = 0;
	step = 5;
	fibo = 0;
	printf("\n");

	printf("\t\tRecursive Fibonacci Number\n\n");
	printf("     n                 f(n)   repetitions\ttime\n");
	printf("-----------------------------------------------------\n");
	for (n = 0; n <= 50; n += step)
	{
		long repetitions = 0;
		clock_t start = clock();//clock start
		do
		{
			repetitions++;//count the repetition 
			fibo = recursiveFibo(n);
		} while (clock() - start < 1000);//when the clock time is smaller than 1 sec, do the loop

		duration = ((double)(clock() - start)) / CLOCKS_PER_SEC;
		duration /= repetitions;
		printf("%6d %20llu %12d\t%f\n", n, fibo, repetitions, duration);
		if (n == 40) step = 1;
	}
	
	system("pause");
	return 0;
}


unsigned long long recursiveFibo(unsigned int index){
	if (index == 0)
		return 0;
	if (index == 1)
		return 1;

	return recursiveFibo(index - 1) + recursiveFibo(index - 2);
}
unsigned long long iterativeFibo(unsigned int index){
	if (index == 0) return 0;
	unsigned long long previous = 0;
	unsigned long long current = 1;
	for (unsigned int i = 1; i < index; ++i) {
		unsigned long long next = current + previous;
		previous = current;
		current = next;
	}
	return current;
}