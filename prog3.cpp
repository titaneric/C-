#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <utility>
#include <algorithm>
#include <sstream>
#include <vector>
#include <climits>
#include <pthread.h>
#include <iterator>
#include <iomanip>
#include <math.h>
#define THREAD_NUM 3
using namespace std;
struct Argu{
	vector<pair<int, int> > process;
	int avgTime;
	int quantum;
	map<int, int> start;
	map<int, int> end;
	vector<vector<int> > schedule;

};
Argu thread_data_array[THREAD_NUM];

void *nonpreem_SJF(void *argu);
void *preem_SJF(void *argu);
void *roundRobin(void *argu);
int countDigits(int num){
	int digit = 0;
	while(num>10){
		num /= 10;
		digit++;
	}
	digit++;
	return digit;
}
string repeat( const std::string &word, int times ) {
   std::string result ;
   result.reserve(times*word.length()); // avoid repeated reallocation
   for ( int a = 0 ; a < times ; a++ ) 
      result += word ;
   return result ;
}	
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
int main(int argc, char * argv[]) {
	ifstream ff;
	ff.open(argv[1]);
	if (!ff)
		cerr << "File doesn't exist!" << endl;
	int quantum;
	//int num = 1;
	string line;
	vector<pair<int, int> >process;
	process.push_back(pair<int, int>(0, 0));
	/*deal with the document*/
	while (getline(ff, line)) {
		if (line[0] == 'Q'){
			string temp = line.substr(2, line.size());
			quantum = atoi(temp.c_str());
		}
		if(count(line.begin(), line.end(), ',') == 2 && count(line.begin(), line.end(), '#') != 1){
			//stringstream ss(line);
			//string token;
			vector<string> str_vec = split(line, ",");
			//string name = str_vec[0];
			pair<int, int> temp(atoi(str_vec[1].c_str()), atoi(str_vec[2].c_str()));
			process.push_back(temp);


		}
		
	}
	
	pthread_t threads[THREAD_NUM];
	pthread_attr_t attr;
	void *status;
	pthread_attr_init(&attr);
    	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	thread_data_array[0].process = process;
	thread_data_array[1].process = process;
	thread_data_array[2].process = process;
	thread_data_array[2].quantum = quantum;
	pthread_create(&threads[0], NULL, nonpreem_SJF, &thread_data_array[0]);
	pthread_create(&threads[1], NULL, preem_SJF, &thread_data_array[1]);
	pthread_create(&threads[2], NULL, roundRobin, &thread_data_array[2]);
	pthread_attr_destroy(&attr);

	pthread_join(threads[0], &status);
	pthread_join(threads[1], &status);
	pthread_join(threads[2], &status);
	
	
	//pthread_exit(NULL);
	/*
	cout<<thread_data_array[0].avgTime<<endl;
	cout<<thread_data_array[1].avgTime<<endl;
	cout<<thread_data_array[2].avgTime<<endl;
	*/
	int i=0;
	cout<<"SJF (non-preemptive)"<<endl;
	cout<<endl;
	cout<<"Starting time | Ending time | processID"<<endl;
	for(int i=0;i < thread_data_array[0].schedule.size();i++){
		cout<<setw(6)<<thread_data_array[0].schedule[i][0]<<setw(16)<<thread_data_array[0].schedule[i][1]<<setw(12)<<thread_data_array[0].schedule[i][2]<<endl;
	}
	
	cout<<"Chart:"<<endl;
	int len=0;
	while(i != thread_data_array[0].schedule.size()){
		
		if(i==0){
			cout<<setw(ceil((thread_data_array[0].schedule[i][1] - thread_data_array[0].schedule[i][0]) * 3 / 2)
			+ 1)<<"P"<<thread_data_array[0].schedule[i][2];
			len = ceil((thread_data_array[0].schedule[i][1] - thread_data_array[0].schedule[i][0]) * 3 / 2);
			//cout<<len<<" ";
		}
		else{
			//cout<<len<<" ";
			cout<<setw(ceil((thread_data_array[0].schedule[i][1] - thread_data_array[0].schedule[i][0]) * 3 / 2)
			+ 1 + len)<<"P"<<thread_data_array[0].schedule[i][2];
			len = ceil((thread_data_array[0].schedule[i][1] - thread_data_array[0].schedule[i][0]) * 3 / 2);
			//cout<<len<<" ";
		}
		i++;
	}
	i=0;
	cout<<endl;
	cout<<"|";
	while(i != thread_data_array[0].schedule.size()){
		string tempstr = repeat("---", thread_data_array[0].schedule[i][1] - thread_data_array[0].schedule[i][0]);
		cout<<tempstr<<"|";
		i++;
	}
	i=0;
	cout<<endl;
	while(i != thread_data_array[0].schedule.size()){
		/*
		string space = repeat("   ", thread_data_array[0].schedule[i][1] - thread_data_array[0].schedule[i][0]);
		int count = 1;
		while(count < countDigits(thread_data_array[0].schedule[i][1])){
			space.erase(space.begin());
			count++;
		}*/
		cout<<thread_data_array[0].schedule[i][0]
		<<setw((thread_data_array[0].schedule[i][1] - thread_data_array[0].schedule[i][0]) * 3 + 1);
		if(i == thread_data_array[0].schedule.size()-1)
		cout<<thread_data_array[0].schedule[i][1];
		i++;
	}
	cout<<endl;
	cout<<"Average waiting time: "<<thread_data_array[0].avgTime<<" ms"<<endl;
	cout<<"----------------------------------------"<<endl;
	cout<<"SJF (preemptive)"<<endl;	
	cout<<endl;
	cout<<"Starting time | Ending time | processID"<<endl;
	
	for(int i=0;i < thread_data_array[1].schedule.size();i++){
		cout<<setw(6)<<thread_data_array[1].schedule[i][0]<<setw(16)<<thread_data_array[1].schedule[i][1]<<setw(12)<<thread_data_array[1].schedule[i][2]<<endl;
	}
	i=0;
	cout<<"Chart:"<<endl;
	len=0;
	while(i != thread_data_array[1].schedule.size()){
		
		if(i==0){
			cout<<setw(ceil((thread_data_array[1].schedule[i][1] - thread_data_array[1].schedule[i][0]) * 3 / 2)
			+ 1)<<"P"<<thread_data_array[1].schedule[i][2];
			len = ceil((thread_data_array[1].schedule[i][1] - thread_data_array[1].schedule[i][0]) * 3 / 2);
			//cout<<len<<" ";
		}
		else{
			//cout<<len<<" ";
			cout<<setw(ceil((thread_data_array[1].schedule[i][1] - thread_data_array[1].schedule[i][0]) * 3 / 2)
			+ 1 + len)<<"P"<<thread_data_array[1].schedule[i][2];
			len = ceil((thread_data_array[1].schedule[i][1] - thread_data_array[1].schedule[i][0]) * 3 / 2);
			//cout<<len<<" ";
		}
		i++;
	}
	i=0;
	cout<<endl;
	cout<<"|";
	while(i != thread_data_array[1].schedule.size()){
		string tempstr = repeat("---", thread_data_array[1].schedule[i][1] - thread_data_array[1].schedule[i][0]);
		cout<<tempstr<<"|";
		i++;
	}
	i = 0;
	cout<<endl;
	
	while(i != thread_data_array[1].schedule.size()){
		/*
		string space = repeat("   ", thread_data_array[1].schedule[i][1] - thread_data_array[1].schedule[i][0]);
		int count = 1;
		while(count < countDigits(thread_data_array[1].schedule[i][1])){
			space.erase(space.begin());
			count++;
		}*/
		cout<<thread_data_array[1].schedule[i][0]
		<<setw((thread_data_array[1].schedule[i][1] - thread_data_array[1].schedule[i][0]) * 3 + 1);//<<thread_data_array[1].schedule[i][1];
		if(i == thread_data_array[1].schedule.size()-1)
		cout<<thread_data_array[1].schedule[i][1];
		i++;
	}
	
	cout<<endl;
	cout<<"Average waiting time: "<<thread_data_array[1].avgTime<<" ms"<<endl;
	cout<<"----------------------------------------"<<endl;
	cout<<"Round Robin"<<endl;
	cout<<endl;
	cout<<"Starting time | Ending time | processID"<<endl;
	for(int i=0;i < thread_data_array[2].schedule.size();i++){
		cout<<setw(6)<<thread_data_array[2].schedule[i][0]<<setw(16)<<thread_data_array[2].schedule[i][1]<<setw(12)<<thread_data_array[2].schedule[i][2]<<endl;
	}
	i=0;
	cout<<"Chart:"<<endl;
	len=0;
	while(i != thread_data_array[2].schedule.size()){
		
		if(i==0){
			cout<<setw(ceil((thread_data_array[2].schedule[i][1] - thread_data_array[2].schedule[i][0]) * 3 / 2)
			+ 1)<<"P"<<thread_data_array[2].schedule[i][2];
			len = ceil((thread_data_array[2].schedule[i][1] - thread_data_array[2].schedule[i][0]) * 3 / 2);
			//cout<<len<<" ";
		}
		else{
			//cout<<len<<" ";
			cout<<setw(ceil((thread_data_array[2].schedule[i][1] - thread_data_array[2].schedule[i][0]) * 3 / 2)
			+ 1 + len)<<"P"<<thread_data_array[2].schedule[i][2];
			len = ceil((thread_data_array[2].schedule[i][1] - thread_data_array[2].schedule[i][0]) * 3 / 2);
			//cout<<len<<" ";
		}
		i++;
	}
	i=0;
	cout<<endl;
	cout<<"|";
	while(i != thread_data_array[2].schedule.size()){
		string tempstr = repeat("---", thread_data_array[2].schedule[i][1] - thread_data_array[2].schedule[i][0]);
		cout<<tempstr<<"|";
		i++;
	}
	cout<<endl;
	
	i=0;
	while(i != thread_data_array[2].schedule.size()){
		/*
		string space = repeat("   ", thread_data_array[2].schedule[i][1] - thread_data_array[2].schedule[i][0]);
		int count = 1;
		while(count < countDigits(thread_data_array[2].schedule[i][1])){
			space.erase(space.begin());
			count++;
		}*/
		cout<<thread_data_array[2].schedule[i][0]
		<<setw((thread_data_array[2].schedule[i][1] - thread_data_array[2].schedule[i][0]) * 3 + 1);
		if(i == thread_data_array[2].schedule.size()-1)
		cout<<thread_data_array[2].schedule[i][1];
		i++;
	}
	
	cout<<endl;
	cout<<"Average waiting time: "<<thread_data_array[2].avgTime<<" ms"<<endl;
	cout<<endl;
	/*
	nonpreem_SJF(process, time);
	preem_SJF(process, time); 
	roundRobin(process, time, quantum);
	*/
	pthread_exit(NULL);
	//system("pause");
	return 0;
}

void *nonpreem_SJF(void *argu)
{
	Argu *my_argu = (Argu*)argu;
	vector<pair<int, int> >process =my_argu->process;
	float avgTime = my_argu->avgTime;
	map<int, int>start_time = my_argu->start;
	map<int, int>end_time = my_argu->end;
	int total_time = 0;
	map<int, int>waiting_time;
	vector<vector<int> >schedule;
	//waiting_time.resize(process.size());
	for(int i=0;i<waiting_time.size();i++)
		waiting_time[i] = 0;
	map<int, int>ready_queue;
	waiting_time.erase(0);
	map<int ,int>burst_time;
	//burst_time.resize(process.size());
	
	for (int i = 0; i < process.size(); i++) {
		burst_time[i] = process[i].second;//pair<int, int>(i, process[i].second);
		total_time += process[i].second;
	}
	burst_time.erase(0);
	//sort(burst_time.begin(), burst_time.end(), cmp);
	map<int, int>arrival_time;
	map<int, bool>arrive;
	for (int i = 0; i < process.size(); i++) {
		arrive[i] = false;//pair<int, int>(i, process[i].first);
	}
	arrive.erase(0);
	//arrival_time.resize(process.size());
	for (int i = 0; i < process.size(); i++) {
		arrival_time[i] = process[i].first;//pair<int, int>(i, process[i].first);
	}
	arrival_time.erase(0);
	
	int clk = 0;
	//bool findProcess = false;
	while (clk < total_time) {
		
		map<int, int>::iterator it;
		/*to denote which process had arrived*/
		for (it = arrival_time.begin(); it != arrival_time.end(); it++) {
			if (clk >= it->second && arrive[it->first] == false) {
				arrive[it->first] = true;
				ready_queue[it->first] = burst_time[it->first];
			}
		}
		for (it = ready_queue.begin(); it != ready_queue.end(); it++) {
			if (it->second == 0) {
				ready_queue.erase(it->first);
				break;
			}
		}
	
		
		/*	If no process is arrived, clk++	*/
		/*
		if(findProcess == false){
			map<int, bool>::iterator findIt;
			for(findIt = arrive.begin();findIt!=arrive.end();findIt++){
				if(findIt->second == true)
					findProcess = true;

			}
		}
		*/
		/*find the first process which should be execute*/
		if(ready_queue.size()>0){
			map<int, int>::iterator min = ready_queue.begin();
			
			for (it = ready_queue.begin(); it != ready_queue.end(); it++) {
				
				if (ready_queue[min->first] > ready_queue[it->first])
					min = it;
				
			}
		
			int tmp = clk;
			start_time[min->first] = clk;
			waiting_time[min->first] = (clk - arrival_time[min->first]);
			clk += min->second;
			ready_queue[min->first] = 0;
			vector<int> vec;
			vec.push_back(tmp);
			vec.push_back(clk);
			vec.push_back(min->first);
			//arrival_time.erase(min);
			schedule.push_back(vec);
		}
		else
			clk++;
		
		
	}
	int total = 0;
	map<int, int>::iterator it;
	for (it = waiting_time.begin();it!=waiting_time.end();it++) {
		total += it->second;
	}
	avgTime = float(total) / float(waiting_time.size());
	my_argu->avgTime = avgTime;
	my_argu->start = start_time;
	my_argu->end = end_time;
	my_argu->schedule = schedule;
	
	//cout<<"non "<<avgTime<<endl;

}

void *preem_SJF(void *argu)
{
	Argu *my_argu = (Argu*)argu;
	vector<pair<int, int> >process =my_argu->process;
	float avgTime = my_argu->avgTime;
	map<int, int>start_time = my_argu->start;
	map<int, int>end_time = my_argu->end;
	vector<vector<int> >schedule;
	int total_time = 0;
	map<int, int>waiting_time;
	//map<int, int>start_time;
	map<int, int>lastStart_time;
	map<int, int>frontEnd_time;
	//map<int, int>end_time;
	map<int, int>count;
	map<int, int>ready_queue;
	for (int i = 0; i<waiting_time.size(); i++)
		count[i] = 0;

	count.erase(0);
	//waiting_time.resize(process.size());
	for (int i = 0; i<waiting_time.size(); i++)
		waiting_time[i] = 0;

	waiting_time.erase(0);
	map<int, int>burst_time;
	//burst_time.resize(process.size());

	for (int i = 0; i < process.size(); i++) {
		burst_time[i] = process[i].second;//pair<int, int>(i, process[i].second);
		total_time += process[i].second;
	}
	burst_time.erase(0);
	//sort(burst_time.begin(), burst_time.end(), cmp);
	map<int, int>arrival_time;
	map<int, bool>arrive;
	for (int i = 0; i < process.size(); i++) {
		arrive[i] = false;//pair<int, int>(i, process[i].first);
	}
	arrive.erase(0);
	//arrival_time.resize(process.size());
	for (int i = 0; i < process.size(); i++) {
		arrival_time[i] = process[i].first;//pair<int, int>(i, process[i].first);
	}
	arrival_time.erase(0);
	bool findProcess = false;
	int clk = 0; //determine whether jump out the while loop
	//int current_time = 0;//determine the schedule
	int offset  = 0;
	while (clk < total_time) {
		
		map<int, int>::iterator it;
		/*to denote which process had arrived*/
		for (it = arrival_time.begin(); it != arrival_time.end(); it++) {
			if (clk >= it->second && arrive[it->first] == false) {
				arrive[it->first] = true;
				ready_queue[it->first] = burst_time[it->first];
				offset++;
			}
		}
		for (it = ready_queue.begin(); it != ready_queue.end(); it++) {
			if (it->second == 0) {
				ready_queue.erase(it->first);
				break;
			}
		}
		/*
		if(findProcess == false){
			map<int, bool>::iterator findIt;
			for(findIt = arrive.begin();findIt!=arrive.end();findIt++){
				if(findIt->second == true)
					findProcess = true;

			}
		}
		*/
		if(ready_queue.size() > 0){
		/*find the first process which should be execute*/
			map<int, int>::iterator min = ready_queue.begin();
			
			for (it = ready_queue.begin(); it != ready_queue.end(); it++) {
				
				if (ready_queue[min->first] > ready_queue[it->first])
					min = it;
				
			}
			//map<int, int>::iterator backup_min = min;
			lastStart_time[min->first] = clk;
			count[min->first]++;
			if (count[min->first] == 1)
				start_time[min->first] = clk;

			
			/*Case that all processes had arrived*/
			if (offset == arrival_time.size()) {
				//start_time[min->first] = clk;
				vector<int>vec;
				int tmp = clk;
				clk += ready_queue[min->first];
				if (count[min->first] == 1)
					frontEnd_time[min->first] = clk;
				end_time[min->first] = clk;
				ready_queue[min->first] = 0;
				vec.push_back(tmp);
				vec.push_back(clk);
				vec.push_back(min->first);
				schedule.push_back(vec);
				

			}
		/*Case that several processes had't arrived*/
			else {
				//start_time[min->first] = clk;

					map<int, int>::iterator next_arrival = arrival_time.begin();
					while (arrive[next_arrival->first] == true || next_arrival->first == min->first) {
						//next_arrival = next(next_arrival, 1);
						next_arrival++;
					}
					/*find the smallest arrival time in the un-arrived processes*/
					for (it = arrival_time.begin(); it != arrival_time.end(); it++) {
						if (arrive[it->first] == false) {
							if (it->second< next_arrival->second)
								next_arrival = it;
						}
					}
					int increment = 0;
					/*if find the next arrival's burst time is bigger than min's, adjust the clk*/
					if (burst_time[next_arrival->first] > min->second) {
						int tmp = clk;
						vector<int>vec;
						clk += ready_queue[min->first];
						increment = ready_queue[min->first];
						vec.push_back(tmp);
						vec.push_back(clk);
						vec.push_back(min->first);
						schedule.push_back(vec);
					}
					else {
						int tmp = clk;
						vector<int>vec;
						increment = arrival_time[next_arrival->first] - clk;
						clk = arrival_time[next_arrival->first];
						vec.push_back(tmp);
						vec.push_back(clk);
						vec.push_back(min->first);
						schedule.push_back(vec);
						
					}
					if (count[min->first] == 1)
						frontEnd_time[min->first] = clk;
					end_time[min->first] = clk;
					ready_queue[min->first] -= increment;
			}
		}
		else
			clk++;
	}
	int total = 0;
	map<int, int>::iterator it;
	for (it = start_time.begin(); it != start_time.end(); it++) {
		if(count[it->first] == 1)
			waiting_time[it->first] = start_time[it->first] - arrival_time[it->first];
		else
			waiting_time[it->first] = (lastStart_time[it->first] - frontEnd_time[it->first]);
		
		total += waiting_time[it->first];
	}
	avgTime = float(total) / float(waiting_time.size());
	my_argu->avgTime = avgTime;
	my_argu->start = start_time;
	my_argu->end = end_time;
	my_argu->schedule = schedule;
	//cout<<"preem "<<avgTime<<endl;
}

void *roundRobin(void *argu)
{
	Argu *my_argu = (Argu*)argu;
	vector<pair<int, int> >process =my_argu->process;
	float avgTime = my_argu->avgTime;
	map<int, int>start_time = my_argu->start;
	map<int, int>end_time = my_argu->end;
	int timeQuantum = my_argu->quantum;	
	int total_time = 0;
	map<int, int>waiting_time;
	//map<int, int>start_time;
	map<int, int>lastStart_time;
	vector<vector<int> >schedule;
	//map<int, int>frontEnd_time;
	//map<int, int>end_time;
	map<int, int>cnt;
	for (int i = 0; i < process.size(); i++) {
		cnt[i] = 0;
	}
	cnt.erase(0);
	//waiting_time.resize(process.size());
	for (int i = 0; i<process.size(); i++)
		waiting_time[i] = 0;

	waiting_time.erase(0);
	map<int, int>burst_time;
	//burst_time.resize(process.size());
	//vector<int>readyQueue;
	//readyQueue.resize(process.size());
	
	for (int i = 0; i < process.size(); i++) {
		burst_time[i] = process[i].second;//pair<int, int>(i, process[i].second);
		total_time += process[i].second;
	}
	burst_time.erase(0);
	//sort(burst_time.begin(), burst_time.end(), cmp);
	map<int, int>arrival_time;
	map<int, int>ready_queue;
	map<int, bool>arrive;
	for (int i = 0; i < process.size(); i++) {
		arrive[i] = false;//pair<int, int>(i, process[i].first);
	}
	arrive.erase(0);
	//arrival_time.resize(process.size());
	for (int i = 0; i < process.size(); i++) {
		arrival_time[i] = process[i].first;//pair<int, int>(i, process[i].first);
	}
	arrival_time.erase(0);

	int clk = 0; //determine whether jump out the while loop
				 
	map<int, int>::iterator next_process = arrival_time.begin();
	map<int, int>::iterator it;
	it = next_process;
	it++;
	map<int, int>::iterator previous_process = it;//next(arrival_time.begin(), arrival_time.size() - 1);
	//bool findProcess = false;
	
	int round = 0;
	while (clk < total_time) {

		
		/*to denote which process had arrived*/
		for (it = arrival_time.begin(); it != arrival_time.end(); it++) {
			if (clk >= it->second && arrive[it->first] == false) {
				arrive[it->first] = true;
				ready_queue[it->first] = burst_time[it->first];
			}
		}
		
		for (it = ready_queue.begin(); it != ready_queue.end(); it++) {
			if (it->second == 0) {
				ready_queue.erase(it->first);
				break;
			}
		}
		

		/*find the first process which should be execute*/
		
		/*if(findProcess == false){
			map<int, bool>::iterator findIt;
			for(findIt = arrive.begin();findIt!=arrive.end();findIt++){
				if(findIt->second == true)
					findProcess = true;

			}
		}*/
		
		if(ready_queue.size() > 0){
			while (ready_queue[next_process->first] == 0 || arrive[next_process->first] == false) {
				//next_process = next(next_process, 1);
				next_process++;
				if (next_process == arrival_time.end())
					next_process = arrival_time.begin();
			}

			/*for (it = arrival_time.begin(); it != arrival_time.end(); it++) {
				if (arrive[it->first] == true && cnt[it->first] == round) {
					
					next_process = it;
					break;
				}
			}*/
			
			
			if (*previous_process != *next_process) {
				lastStart_time[next_process->first] = clk;
				cnt[next_process->first]++;
			}
			
			
			if (cnt[next_process->first] == 1)
				start_time[next_process->first] = clk;
			//cout << clk;
			int tmp = clk;
			vector<int>vec;
			clk = ready_queue[next_process->first] - timeQuantum > 0 ? clk + timeQuantum : clk+ready_queue[next_process->first];
			vec.push_back(tmp);
			vec.push_back(clk);
			vec.push_back(next_process->first);
			schedule.push_back(vec);
			//cout <<" "<<next_process->first<<":"<<cnt[next_process->first]<<endl;
			ready_queue[next_process->first] = ready_queue[next_process->first] - timeQuantum > 0 ? ready_queue[next_process->first] - timeQuantum : 0;
			previous_process = next_process;
			//next_process = next(next_process, 1);
			next_process++;
			if (next_process == arrival_time.end())
				next_process = arrival_time.begin();

		}
		else
			clk++;
		
	}
	int total = 0;
	//map<int, int>::iterator it;
	for (it = start_time.begin(); it != start_time.end(); it++) {
		if(cnt[it->first] > 1)
			waiting_time[it->first] = lastStart_time[it->first] - (cnt[it->first] - 1) * timeQuantum;
		else
			waiting_time[it->first] = lastStart_time[it->first] - arrival_time[it->first];
		
		total += waiting_time[it->first];
	}
	avgTime = float(total) / float(waiting_time.size());
	my_argu->avgTime = avgTime;
	my_argu->start = start_time;
	my_argu->end = end_time;
	my_argu->schedule = schedule;
	//cout<<"RR "<<avgTime<<endl;
}
