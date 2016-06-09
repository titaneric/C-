#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <limits>
#include <algorithm>
#include <iterator>
using namespace std;
void Dijkstra(map<int, vector<pair<int, int>>> graph, map <int, int>& short_path, int start);
int main(){
	int VertexNum = 0;
	//cout << "Please input the graph vertex number!" << endl;
	cin >> VertexNum;
	int edgeNum = 0;
	cin >> edgeNum;
	int count = 0;
	pair<int, int>path;
	map <pair<int, int>, int> list;
	//graph.resize(num);
	while (count<edgeNum){
		int start = 0;
		int end = 0;
		int weight = 0;
		cin >> start;
		cin >> end;
		cin >> weight;
		/*
		while ((start < 1 && start > num) || (end < 1 && end > num)){
			cout << "Please input the test data correctly!" << endl;
			cin >> start >> end;
		}
		*/

		path.first = start;
		path.second = end;
		list[path] = weight;
		count++;


	}
	map<int, vector<pair<int, int>>>graph;
	
	//relation.resize(VertexNum);
	map <pair<int, int>, int>::iterator iter;
	for (iter = list.begin(); iter != list.end(); iter++){
		vector<pair<int, int>>relation;
		for (map <pair<int, int>, int>::iterator it = list.begin(); it != list.end(); it++){
			
			if ((iter->first).first == (it->first).first){
				pair<int, int>temp;
				temp.first = (it->first).second;
				temp.second = it->second;
				relation.push_back(temp);
			}
		}
		graph[(iter->first).first] = relation;
	}
	map <int, int>short_path;
	/*set each element in short_path to infinity*/
	for (int i = 0;i<VertexNum;i++){
		short_path[i] = numeric_limits<int>::max();
	}
	int start = 0;
	Dijkstra(graph, short_path, start);

	map<int, int>::iterator max_iter;
	int max = short_path.begin()->second;
	for (max_iter = short_path.begin(); max_iter != short_path.end(); max_iter++){
		if (max_iter->second > max)
			max = max_iter->second;
	}
	//map<int, int>::iterator max_iter = max_element(short_path.begin(), short_path.end());
	cout << max<<endl;

	//system("pause");




	return 0;
}
void Dijkstra(map<int, vector<pair<int, int>>> graph, map <int, int>& short_path, int start){
	set<int>vertex;
	for (map<int, vector<pair<int, int>>>::iterator i = graph.begin(); i != graph.end(); i++)
		vertex.insert(i->first);
	short_path[0] = 0;
	set<int>Z;	//store the vertex whom has shortest path from start
	while (Z != vertex){
		set<int>vzdiff;
		set_difference(vertex.begin(), vertex.end(), Z.begin(), Z.end(), inserter(vzdiff, vzdiff.begin()));
		set<int>::iterator diff_it;
		int u = *vzdiff.begin();
		int min = short_path[*vzdiff.begin()];
		for (diff_it = vzdiff.begin(); diff_it != vzdiff.end(); diff_it++){
			if (short_path[*diff_it] < min){
				min = short_path[*diff_it];
				u = *diff_it;
			}
		}
		//map<int, int>::iterator iter = min_element(short_path.begin(), short_path.end());
		//int u = iter->first;
		Z.insert(u);
		vector<pair<int, int>>::iterator it;
		for (it = graph[u].begin(); it != graph[u].end(); it++){
			if (short_path[it->first] > (short_path[u] + it->second))
				short_path[it->first] = (short_path[u] + it->second);
		}

	}


}