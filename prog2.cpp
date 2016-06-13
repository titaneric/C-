#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <set>
#include <locale>
#include <map>
#include <stdlib.h>  
#include <iterator>
#include <sys/syscall.h>
#include <pthread.h>
#include <time.h>
using namespace std;
struct Argu{
		string doc1;
		string doc2;
		float Jacaad;
};
map<string, set<string> > mapSet;
float sumJacaad;
void *calJacaad(void *argument);
int main(int argc, char*argv[]){
	
	//string file = string(argv[1]);
	fstream fp;
	string line;
	fp.open(argv[1], ios::in);
	
	if (!fp)
		cerr << "File doesn't exist!" << endl;

	//map<string, set<string> >totalSet;

	locale loc;
	string str = "";
	int range = 1;
	//int count = 0;
	string doc_id = "";
	/*Deal with the context of each document*/
	while (getline(fp, line, '\n')){
		//cout<<line<<endl;
		line = line.substr(0, line.size()-1);
		if (line.size()<6)
			doc_id = line;
		else{
				set<string>str_set;
				for (int i = 0; i < line.size(); i += range){
					/*If the current char is alpha, then find the next char if possible*/
					if (isalpha(line[i], loc)){

						str += line[i];
						for (int j = i + 1; j < line.size(); j++){
							if (isalpha(line[j], loc)){
								str += line[j];
							}
							else
								break;
						}
					}
					else if (line[i] == '\''){
						str += line[i];
						if (line[i + 1] == 'd' || line[i + 1] == 's'){
							str += line[i + 1];
						}
						
					}
				

				
					
					/*If find the str, then insert into str_set and change the range*/
					if (str.size() != 0){
						if (str == "\'s" || str == "\'d") //skip the abbr.
						{
							range = str.size();
							str = "";
						}
						else{
							str_set.insert(str);
							range = str.size();
							str = "";
						}
					}
					else 
						range = 1;

				}
				//cout<<doc_id.size()<<" "<<doc_id<<endl;
				mapSet[doc_id] = str_set;//put the str_set into map
			}
		
	}
	
	fp.close();	
	/*End of the doc handling*/
	// = pthread_self();
	//cout<<mapSet.size()<<endl;
	//map<pair<string, string>, float> Jacaad_map;
	map<string, set<string> >::iterator iter;
	map<string, set<string> >::iterator it;
	float maxJ = 0.0;
	string maxDoc="";
	pthread_t tid;
	//pid_t tid = (pid_t) syscall (SYS_gettid);
	for (iter = mapSet.begin(); iter != mapSet.end(); iter++){
		cout<<"[Main thread]: generate TID = "<<pthread_self()<<", DocID = "<<iter->first<<"\n";
		sumJacaad = 0;
		int count = 0;
		clock_t start, end;
		double cpu_time_used;
		start = clock();
		for (it = mapSet.begin(); it != mapSet.end(); it++){
			if (iter->first != it->first){
				count++;
				
				Argu argu;
				float Jacaad = 0.0;
				argu.doc1 = iter->first;
				argu.doc2 = it->first;
				argu.Jacaad = Jacaad;
				
				//cout<<iter->first<","<<it->first<<":";
				pthread_create(&tid, NULL, calJacaad, &argu);
				//pthread_exit(0);
				pthread_join(tid, NULL);
				
				
				//pair<string, string> temppair(iter->first, it->first);
				//Jacaad_map[temppair] = Jacaad;

			}
		}
		if(sumJacaad/float(count) > maxJ){
			maxJ = sumJacaad/float(count);
			maxDoc = iter->first;
		}
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		cout<<"[TID = "<<pthread_self()<<"] AvgJ = "<<sumJacaad/float(count)<<endl;
		cout<<"[TID = "<<pthread_self()<<"] CPU time = "<<cpu_time_used*1000<<"ms"<<endl;
		cout<<endl;

	}
	/*
	for(int i=0;i<mapSet.size();i++){
		 pthread_join(tid[i], NULL);

	}
	*/
	cout<<"[Main thread]: HighestJ = "<<maxJ<<", "<<"DocID = "<<maxDoc<<endl;
	pthread_exit(NULL);
	
	/*
	map<pair<string, string>, float>::iterator map_iter;
	for(map_iter = Jacaad_map.begin();map_iter != Jacaad_map.end();map_iter++){
		cout<<(map_iter->first).first<<","<<(map_iter->first).second<<":"<<map_iter->second<<endl;

	}
	*/
	//cout<<"1234";
	//system("pause");
	return 0;
}
void *calJacaad(void *argument){
	//pthread_t tid;// = pthread_self();
	string doc1, doc2;
	float Jacaad;
	Argu *my_argu = (Argu*)argument;
	doc1 = my_argu->doc1;
	doc2 = my_argu->doc2;
	Jacaad = my_argu->Jacaad;
	//tid = pthread_self();
	//pid_t tid = (pid_t) syscall (SYS_gettid)
	cout<<"[TID = "<<pthread_self()<<"] ";
	cout<<"J("<<doc1<<", "<<doc2<<") = ";
	//cout<<doc1<<" "<<doc2<<"\n";
	set<string>setUnion, setInter;
	//cout<<doc2<<endl;
	set_union(mapSet[doc1].begin(), mapSet[doc1].end(),
		mapSet[doc2].begin(), mapSet[doc2].end(), inserter(setUnion, setUnion.begin()));

	set_intersection(mapSet[doc1].begin(), mapSet[doc1].end(),
		mapSet[doc2].begin(), mapSet[doc2].end(), inserter(setInter, setInter.begin()));
	Jacaad = float(setInter.size()) / float(setUnion.size());
	sumJacaad += Jacaad;
	//cout<<"J("<<doc1<<", "<<doc2<<") = "<<Jacaad<<endl;
	
	//printf("J(%s, %s) = ",doc1.c_str(), doc2.c_str());
	printf("%f\n", Jacaad);
	pthread_exit(0);
	
	
	
}
