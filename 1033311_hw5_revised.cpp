#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
void ConvertToBinary(unsigned int n, vector<int> &bi_vec);
int modular_pow(int base, unsigned int exponent, unsigned int modular);
int main() {
	int base, exponent, divisor;
	while (cin >> base >> exponent >> divisor) {
		//cout<<modular_pow(base, exponent, divisor)<<endl;
		
		vector <int> temp;
		ConvertToBinary(exponent, temp);
		reverse(temp.begin(), temp.end());
		vector<int>moduleResult;
		int tempDividend = base;
		//moduleResult.push_back(tempDividend % divisor);
		for (int i = 0; i < temp.size(); i++) {
			//double module = tempDividend;
			
			moduleResult.push_back(fmod(tempDividend, divisor));
			tempDividend = moduleResult[i] * moduleResult[i];
		}

		vector<int>powermul;
		for (int i = 0; i < temp.size(); i++) {
			if (temp[i] == 1) {
				powermul.push_back(moduleResult[i]);

			}
		}
		int k = 0;
		while (powermul.size() != 1) {
			int mul = (powermul[k] * powermul[k + 1]) % divisor;
			powermul.push_back(mul);
			powermul.erase(powermul.begin() + k);
			powermul.erase(powermul.begin() + k);


		}
		if (powermul.size() == 1) {
			//double module = powermul[0];
			/*
			while (true) {
				if (module >= 0 && module < divisor)
					break;
				module = module - divisor;
			}
			*/
			//module = fmod();
			cout << powermul[0] % divisor << endl;
		}
		
	}
	//system("pause");
	return 0;
}
void ConvertToBinary(unsigned int n, vector<int> &bi_vec) /*Alex says: Good to use unsigned int if u can*/
{
	if (n / 2 != 0) {
		ConvertToBinary(n / 2, bi_vec);
	}
	bi_vec.push_back(double(n % 2));
}

int modular_pow(int base,unsigned int exponent, unsigned int modular)
{
	
	if (modular == 1)
		return 0;
	int result = 1;
	base = base % modular;
	while (exponent > 0) {
		if (exponent % 2 == 1)
			result = (result * base) % modular;
		exponent = exponent >> 1;
		base = (base*base) % modular;
	}
	return result;
	
}
