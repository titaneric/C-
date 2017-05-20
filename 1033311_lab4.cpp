#include<iostream>
#include<fstream>
#include<memory>
#include<time.h>
#include<string>
//#include <stdlib.h>
#include<vector>
#include<assert.h>
using namespace std;
void decompress(int &size, int &length, string source, string target);
void compress(int &size, int &length, string source, string target);
int main() {
	
	
	string command, source, target;
	/*source = "Pic0.bmp";
	target = "test.dat";
	command = "C";*/
	while (cin >> command >> source >> target) {
		clock_t start = clock();



		int size = 0;
		int length = 0;
		if (command == "C")
			compress(size, length, source, target);
		else if (command == "D")
			decompress(size, length, source, target);

		clock_t finish = clock();
		double duration = (double)(finish - start) / CLOCKS_PER_SEC;
		cout << "The input file is " << length << " bytes.\n";
		cout << "The output file is " << size << " bytes.\n";
		cout << "It takes " << duration << " seconds." << endl;

	}
	//system("pause");
	return 0;

}

void decompress(int &size, int &length, string source,  string target)
{
	const char *cstr = source.c_str();
	ifstream f;
	f.open(cstr, ios::binary);
	if (f.is_open()) {
		streampos begin = f.tellg();
		f.seekg(0, ios::end);
		streampos end = f.tellg();
		length = end - begin;
		unique_ptr<char[]>buffer(new char[length]);
		f.seekg(0, ios::beg);
		f.read(buffer.get(), length);
		f.close();
		/*vector<int>vec;
		vec.resize(length);
		for (int i = 0; i < length; i++) {
			vec[i] = buffer[i];
		}*/
		int i = 0;
		//int size = 0;
		while (i < length) {
			int x = int(buffer[i]);
			if (x >= 0 && x <= 127) {
				size = size + x + 1;
				i = i + x + 2;
			}
			else if (x >= -127 && x <= -1) {
				size = size + (-x) + 1;
				i += 2;
			}

		}
	
		unique_ptr<char[]>transform(new char[size]);
		i = 0;
		int k = 0;
		while (i < length) {
			int x = int(buffer[i]);
			if (x >= 0 && x <= 127) {
				for (int j = 0; j < x + 1; j++) {
					//char tmp = char(buffer[i + j + 1]);
					transform[k] = char(buffer[i + j + 1]);
					k++;
				}
				i = i + x + 2;
			}
			else if (x >= -127 && x <= -1) {
				for (int j = 0; j < (-x) + 1; j++) {
					//char tmp = char(buffer[i + 1]);
					transform[k] = char(buffer[i + 1]);
					k++;
				}
				i += 2;
			}

		}
		
		const char *cstr = target.c_str();
		fstream file(cstr, ios::out);

		file.write(transform.get(), size);
		file.close();
	}
}

void compress(int &size, int &length, string source, string target)
{
	const char *cstr = source.c_str();
	ifstream f;
	f.open(cstr, ios::binary);
	if (f.is_open()) {
		streampos begin = f.tellg();
		f.seekg(0, ios::end);
		streampos end = f.tellg();
		length = end - begin;
		unique_ptr<char[]>buffer(new char[length]);
		f.seekg(0, ios::beg);
		f.read(buffer.get(), length);
		f.close();
		//calculate the size		
		unique_ptr<int[]>temp(new int[length]);
		int i = 0;
		//int size = 0;
		int m = 0;
		while (i < length) {
			int now = int(buffer[i]);
			int count = 0;
			int k = 1;
			int rear = int(buffer[i + k]);
			if (now == rear) {
				count++;
				while (now == rear) {
					count++;
					if (count > 127)
						break;
					k++;
					rear = int(buffer[i + k]);
				}
				temp[m] = 0 - count;
				assert(temp[m] < -1);
				size += 2;
				m++;
				i += count;
			}
			else {
				while (now != rear) {
					count++;
					if (count > 127)
						break;
					k++;
					now = rear;
					rear = int(buffer[i + k]);
				}
				temp[m] = count;
				m++;
				size = size + count + 1;
				i += count;
			}
		}
		
		unique_ptr<char[]>transform(new char[size]);
		i = 0;
		int k = 0;
		m = 0;
		/*vector<int>vec;
		vec.resize(size);*/
		while (i < length) {
			int stamp = 0;
			if(temp[m] >= 1) {
				
				
				stamp = temp[m] - 1;
				assert(stamp >= 0 && stamp <= 127);
				transform[k] = char(stamp);
				//vec[k] = stamp;
				k++;
				for (int j = 0; j < temp[m]; j++) {
					transform[k] = char(buffer[i + j]);
					//vec[k] = buffer[i + j];
					k++;
				}
				i += temp[m];
			}
			else if(temp[m] <= -1){
				
				stamp = temp[m] + 1;
				assert(stamp>=-127 && stamp<=-1);
				transform[k] = char(stamp);
				//vec[k] = stamp;
				k++;
				transform[k] = buffer[i];
				//vec[k] = buffer[i];
				k++;
				
				i = i - temp[m];
				
			}
			m++;



		}
		/*Test the accuracy*/
		/*string test = "Pic0.dat";
		const char *cstr = test.c_str();
		ifstream fb;
		fb.open(cstr, ios::binary);
		begin = fb.tellg();
		fb.seekg(0, ios::end);
		end = fb.tellg();
		length = end - begin;
		unique_ptr<char[]>testptr(new char[length]);
		fb.seekg(0, ios::beg);
		fb.read(testptr.get(), length);
		fb.close();
		int count = 0;
		for (int j = 0; j < length; j++) {
			if (testptr[j] == transform[j])
				count++;
		}
		if (count == length)
			cout << "Success\n";*/
		const char *cstr = target.c_str();
		fstream file(cstr, ios::out);

		file.write(transform.get(), size);
		file.close();
	}
}
