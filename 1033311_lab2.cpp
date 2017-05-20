#include <iostream>
#include <limits>
#include <cmath>
using namespace std;
const auto gA= numeric_limits<int>::max();
const auto gB = numeric_limits<unsigned int>::max();
const auto gC = numeric_limits<unsigned long long>::max();

auto digitSum(decltype(gA)gA) -> int{
	auto sum = 0;
	auto temp = gA;
	while (temp > 0) {
		auto t = temp % 10;
		sum += pow(long double(temp % 10), long double(temp % 10));
		temp /= 10;
	}
	return sum;
}
auto digitSum(decltype(gB)gB) ->int{
	auto sum = 0;
	auto temp = gB;
	while (temp > 0) {
		sum += pow(long double(temp % 10), long double(temp % 10));
		temp /= 10;
	}
	return sum;
}
auto digitSum(decltype(gC)gC) ->int{
	auto sum = 0;
	auto temp = gC;
	while (temp > 0) {
		sum += pow(long double(temp % 10), long double(temp % 10));
		temp /= 10;
	}
	return sum;
}

auto isPrime(decltype(gB) num) ->bool{
	if (num % 2 == 0)
		return false;
	else {
		auto i = 3;
		while (i*i <= num) {
			if (num%i == 0)
				return false;
			i ++;

		}
		return true;
	}


}

int main() {
	cout <<"The maximum of type int is "<< gA << endl;
	cout << "The maximum of type unsigned int is " << gB << endl;
	cout << "The maximum of type unsigned long long is " << gC << endl;
	cout << digitSum(gA)<<endl;
	cout << digitSum(gB)<<endl;
	cout << digitSum(gC)<<endl;
	if (isPrime(digitSum(gA)))
		cout << "digitSum(" << gA << ") is " << "prime\n";
	if (isPrime(digitSum(gB)))
		cout << "digitSum(" << gB << ") is " << "prime\n";
	if (isPrime(digitSum(gC)))
		cout << "digitSum(" << gC << ") is " << "prime\n";
	
	





	return 0;
}
