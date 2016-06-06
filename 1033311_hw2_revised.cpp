#include <iostream>
#include <vector>
//#include <map>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <set>
#include <iterator>
#include <utility>
/*
struct path{
	int start = 0;
	int end = 0;
};
*/
using namespace std;
void DFS(vector<vector<int>> list, int vertex, vector< bool >& mark, vector<pair<int ,int>>& edge);

int main(){
	int num = 0;
	srand(time(NULL));
	cout << "Please input the graph vertex number!" << endl;
	cin >> num;
	pair<int, int>path;
	vector <pair<int, int>> list;
	//graph.resize(num);
	while (true){
		int start = 0;
		int end = 0;
		cin >> start;
		if (start == -1)
			break;
		cin >> end;
		while ((start < 1 && start > num) || (end < 1 && end > num)){
			cout << "Please input the test data correctly!" << endl;
			cin >> start >> end;
		}
		
		path.first = start;
		path.second = end;
		list.push_back(path);
		

	}
	//map < int, vector<int> >graph;
	vector<vector<int>>graph;
	graph.resize(num+1);
	for (int i = 0; i < num+1; i++){
		for (int k = 0; k < list.size(); k++){
			if (list[k].first == i+1){
				graph[i+1].push_back(list[k].second);
				graph[list[k].second].push_back(i+1);
			}
		}
	}

	//cout << list.size() << endl;
	/*
	map<int, vector<int>>::iterator iter;
	vector<int>::iterator vec_iter;
	for (iter = graph.begin();iter!=graph.end();iter++){
		cout << iter->first<<":";
		for (vec_iter = graph[iter->first].begin(); vec_iter != graph[iter->first].end(); vec_iter++)
			cout << *vec_iter<<" ";
		cout << endl;
		
	}
	*/
	//cout << graph[1][0];
	vector< bool >mark;
	vector< pair<int ,int> >edge;
	//mark.resize(num);
	//cout << mark.size() << endl;
	for (int i = 0; i < (graph.size()-1); i++)
		mark.push_back(false);

	int v = rand() % mark.size() + 1;
	//cout << "v is " << v << endl;
	DFS(graph, v, mark, edge);
	/*
	for (int i = 0; i < edge.size(); i++){
		cout << edge[i].first << " " << edge[i].second << endl;
	}
	*/

	/*	if the graph is connected, then do thing below	*/
	if (count(mark.begin(), mark.end(), true) == mark.size()){
		/*	if path on list is reversed to the DFS edge, swap them	*/
		for (int j = 0; j < list.size(); j++){
			for (int k = 0; k < edge.size(); k++){
				if (list[j].first == edge[k].second && list[j].second == edge[k].first){

					path.first = edge[k].first;
					path.second = edge[k].second;
					list[j] = path;
				}
			}
		}
		/*	Find the path on list but not on DFS edge	*/
		vector<pair<int, int>>total = edge;
		for (int i = 0; i < list.size(); i++)
			total.push_back(list[i]);
		vector<pair<int, int>>result;
		for (int i = 0; i < list.size(); i++){
			if (count(total.begin(), total.end(), list[i]) == 1)
				result.push_back(list[i]);
		}
		/*	Find all ancestors of every node on DFS tree	*/
		vector<vector<int>>relation;
		relation.resize(num + 1);
		for (int i = (edge.size() - 1); i >-1; i--){
			vector<int>ancestor;
			ancestor.push_back(edge[i].first);
			int temp_ancestor = edge[i].first;
			for (int j = (i - 1); j > -1; j--){
				if (temp_ancestor == edge[j].second){
					ancestor.push_back(edge[j].first);
					temp_ancestor = edge[j].first;

				}
			}
			relation[edge[i].second] = ancestor;
		}
		/*	adjust the result form like (child, ancestor)*/
		for (int g = 0; g < result.size(); g++){
			if (find(relation[result[g].second].begin(), relation[result[g].second].end(),
				result[g].first) != relation[result[g].second].end())
				swap(result[g].first, result[g].second);
		}
		/*	Put the child to ancestor path of result into mix	*/
		vector<pair<int, int>>mix;
		for (int i = 0; i < result.size(); i++){
			vector<int>::iterator index = find(relation[result[i].first].begin(), relation[result[i].first].end(),
				result[i].second);
			int start = result[i].first;
			for (vector<int>::iterator iter = relation[result[i].first].begin(); iter < index + 1; iter++){
				path.first = start;
				path.second = *iter;
				mix.push_back(path);
				start = *iter;
			}
		}
		/*	Count the common edge in mix	*/
		bool common_edge = false;
		for (int i = 0; i < mix.size(); i++){
			if (count(mix.begin(), mix.end(), mix[i])>1)
				common_edge = true;

		}
		if (result.size() >= 2){
			cout << "y" << endl;
			if (common_edge == true)
				cout << "There are at least three cycles." << endl;
		}
		else
			cout << "n"<<endl;
	}
	else
		cout << "n" << endl;

	
	/*
	if (count(mark.begin(), mark.end(), true) == mark.size())
		cout << "This map is a connectivity!" << endl;
	else
		cout << "n" << endl;
	*/
	//system("pause");
	return 0;
	
}
void DFS(vector<vector<int>> list,int vertex, vector< bool >& mark, vector<pair<int, int>>& edge){
	
	//cout << vertex<<endl;
	mark[vertex-1] = true;
	
	for (int u = 0; u < list[vertex].size(); u++) //vertex' out neighbor
	{
		if (mark[list[vertex][u] - 1] == false){
			//cout << vertex <<" "<<list[vertex][u]<< endl;
			pair<int, int>temp;
			temp.first = vertex;
			temp.second = list[vertex][u];
			edge.push_back(temp);
			DFS(list, list[vertex][u], mark, edge);
		}
		
	}


}