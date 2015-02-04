#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <sstream>

#include "Graph.h"

using namespace std;


typedef GraphArc<string, int> Arc;
typedef GraphNode<pair<string, int>, int> Node;

void Nothing(Node * pNode)	{}

void visit(Node * pNode)
{
	cout << "Visiting: " << pNode->getData().first << endl;
}
void pathfinding(Node * pNode)
{
	cout << "Path: " << pNode->getData().first << endl;
}

void AllRoutes(vector<Node *> pNode)
{
	for (int i = 0; i < pNode.size() - 1; i++)
	{
		cout << "Journey: " << pNode.at(i)->getData().first << " (" << pNode.at(i)->getData().second << ")"<< endl;
	}
	cout << endl;
}
void OutputMap(vector<pair<string, int>> *p)
{
	cout << endl;

	//each node and it's distance added to a stringstream, add up the total distance
	int i = 0;
	int j = 0; 
	int m = p->size();

	std::stringstream s;
	int total = 0;

	
	for (; i < m; i++)
	{
		total += p->at(i).second;
		s << p->at(i).first << "(" << p->at(i).second << ")";
		if (i < m - 1)
			s << "->";
	}


	//Path begin to path end, total distance
	cout << "[" << p->begin()->first << " - " << (--p->end())->first << "]" << ", Total Weight : "<<"[" << total << "]" << endl;
	//Actual path
	cout << s.str() << endl;
}

void MovePathToMap(vector<vector<pair<string, int>>> &map, vector<Node *> path)
{
	//create a path of pairs
	vector<pair<string, int>> pairpath;

	vector<Node *>::iterator pathEnd = path.end();
	vector<Node *>::iterator pathBegin = path.begin();
	pathEnd--;

	int lastCost = 0;

	//Loop through path and add each node to the pair path
	for (; pathEnd != pathBegin; pathEnd--) {
		pairpath.push_back(pair<string, int>((*pathEnd)->getData().first, (*pathEnd)->getData().second - lastCost));

		lastCost = (*pathEnd)->getData().second;
	}
	//last node
	pairpath.push_back(pair<string, int>((*pathEnd)->getData().first, (*pathEnd)->getData().second - lastCost));

	map.push_back(pairpath);
}

int main(int argc, char *argv[])
{
	Graph<pair<string, int>, int> graph(30);

	pair<string, int> c = pair<string, int>("", INT_MAX);
	int i = 0;
	ifstream myfile;
	myfile.open("AStarNodes.txt");
	//myfile.open("DorNodes.txt");

	while (myfile >> c.first)
	{
		graph.addNode(c, i++);
	}

	myfile.close();
	myfile.open("AStarArcs.txt");
	//myfile.open("DorArcs.txt");

	int from, to, weight;
	while (myfile >> from >> to >> weight)
	{
		graph.addArc(from, to, weight);
	}
	myfile.close();


	vector<Node *> path;
	vector<vector<pair<string, int>>> map;

	for (int i = 0; i < 6; i++)
	{
		for (int j = i + 1; j <= 5; j++)
		{
			path.clear();
			graph.UCS(graph.nodeArray()[i], graph.nodeArray()[j], path);
			MovePathToMap(map, path);
		}
	}

	//Output The Map
	vector<vector<pair<string, int>>>::iterator pmI = map.begin();
	vector<vector<pair<string, int>>>::iterator pmE = map.end();

	cout << "\- UCS - Precompile Pathfinding -/" << endl;
	for (; pmI != pmE; pmI++)
	{
		OutputMap(&*pmI);
	}
	cout << "\--------------------------------/" << endl << endl;

	path.clear();
	graph.AStarPathfinding(graph.nodeArray()[0], graph.nodeArray()[17], path);

	cout << "\------- A * - Pathfinding -------/" << endl;
	cout << "Number of Nodes in Path :"<< path.size() << endl;
	for (int i = 0; i < path.size(); i ++)
	{
		cout << path[i]->getData().first << endl;
	}
	cout << "\--------------------------------/" << endl << endl;
	

	system("PAUSE");
}