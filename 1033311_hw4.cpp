#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;
void DFS(vector<vector<int>> list, int vertex, vector< bool >& mark);//, vector<pair<int, int>>& edge);
int main(){
	int num = 0;
	cin >> num;
	pair<int, int>path;
	vector <pair<int, int>> list;
	
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

	vector<vector<int>>graph;
	vector<vector<int>>trans_graph;
	graph.resize(num + 1);
	trans_graph.resize(num + 1);
	for (int i = 0; i < num + 1; i++){
		for (int k = 0; k < list.size(); k++){
			if (list[k].first == i + 1){
				graph[i + 1].push_back(list[k].second);
				trans_graph[list[k].second].push_back(i + 1);
			}
		}
	}

	vector< bool >mark;
	vector< bool >transmark;
	for (int i = 0; i < (graph.size() - 1); i++){
		mark.push_back(false);
		transmark.push_back(false);

	}
	DFS(graph, 1, mark);
	DFS(trans_graph, 1, transmark);
	vector<int>strog_component;
	for (int i = 0; i < graph.size() - 1; i++){
		if (mark[i] == transmark[i] && mark[i] == true)
			strog_component.push_back(i + 1);
	}
	for (int i = 0; i < strog_component.size(); i++){
		cout << strog_component[i] << " ";
	}
	//system("pause");
	return 0;
}
void DFS(vector<vector<int>> list, int vertex, vector< bool >& mark){//, vector<pair<int, int>>& edge){

	//cout << vertex<<endl;
	mark[vertex - 1] = true;

	for (int u = 0; u < list[vertex].size(); u++) //vertex' out neighbor
	{
		if (mark[list[vertex][u] - 1] == false){
			//cout << vertex <<" "<<list[vertex][u]<< endl;
			pair<int, int>temp;
			temp.first = vertex;
			temp.second = list[vertex][u];
			//edge.push_back(temp);
			DFS(list, list[vertex][u], mark);//, edge);
		}

	}


}