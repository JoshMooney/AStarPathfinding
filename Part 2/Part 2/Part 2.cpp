// Part 2.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 

#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <sstream>

#include "Graph.h"
#include "Button.h"

using namespace std;

typedef GraphArc<pair<string, int>, int > Arcs;
typedef GraphNode<pair<string, int>, int> Node;

sf::Font font;

string numToString(int w)
{
	stringstream s;
	if (w >= INT_MAX - 100000)
		s << "MAX";
	else
		s << w;
	return s.str();
}
string numToString(int w, string n)
{
	stringstream s;
	s << n;
	if (w >= INT_MAX - 100000)
		s << "MAX";
	else
		s << w;
	return s.str();
}

int getStart(Graph<pair<string, int>, int> &g)
{
	for (int i = 0; i < g.GetNodeCount() - 1; i++)
	{
		if (g.nodeArray()[i]->GetShape().getFillColor() == sf::Color::Green)
			return i;
	}
	return NULL;
}
int getDestination(Graph<pair<string, int>, int> &g)
{
	for (int i = 0; i < g.GetNodeCount() - 1; i++)
	{
		if (g.nodeArray()[i]->GetShape().getFillColor() == sf::Color::Red)
			return i;
	}
	return NULL;
}

void setUpGraph(Graph<pair<string, int>, int> &g)
{
	pair<string, int> c = pair<string, int>("", INT_MAX);
	int i = 0;

	ifstream myfile;

	//Load the Nodes
	//myfile.open("TestNodes.txt");
	myfile.open("AStarNodes.txt");
	float x, y;
	while (myfile >> c.first >> x >> y)	
	{
		g.addNode(c, i++, sf::Vector2f(x,y));
	}
	myfile.close();

	
	//Loads the Arcs
	myfile.open("AStarArcs2.txt");
	int from, to, weight,x1,y1,x2,y2;
	while (myfile >> from >> to >> weight)
	{
		g.addArc(from, to, weight);
	}
	myfile.close();
	
}
void DrawGraph(Graph<pair<string, int>, int> &g, sf::RenderWindow &w, sf::Font f)
{
	sf::Text t;

	for (int i = 0; i < g.GetNodeCount(); i++)
	{
		if (g.nodeArray()[i]->marked() == false ||
			g.nodeArray()[i]->isDest() ||
			g.nodeArray()[i]->isStart() ||
			g.nodeArray()[i]->isPath())
			w.draw(g.nodeArray()[i]->GetShape());
		else
		{
			g.nodeArray()[i]->Visited();
			w.draw(g.nodeArray()[i]->GetShape());
		}
		//Draw Node Name
		t = g.nodeArray()[i]->GetName();
		t.setFont(f);
		w.draw(t);

		t.setCharacterSize(12);
		//t.setStyle(sf::Text::Bold);

		//Draw Node G Value
		t.setString(numToString(g.nodeArray()[i]->GetG(), "G : "));
		t.setPosition(t.getPosition().x+16, t.getPosition().y-16);
		t.setColor(sf::Color::Magenta);
		w.draw(t);

		//Draw Node H Value
		t.setString(numToString(g.nodeArray()[i]->GetH(), "H : "));
		t.setPosition(t.getPosition().x, t.getPosition().y + 32);
		t.setColor(sf::Color::Cyan);
		w.draw(t);
	}
}
void DrawArcs(Graph<pair<string, int>, int> &g, sf::RenderWindow &w, sf::Font f)
{
	sf::Text text;
	text.setFont(f);

	for (int i = 0; i < g.GetNodeCount(); ++i)
	{
		Node* const v1 = g.nodeArray()[i];
		Node* v2;
		list<Arcs>::const_iterator iter = v1->arcList().begin();
		list<Arcs>::const_iterator endIter = v1->arcList().end();

		for (; iter != endIter; ++iter)
		{
			v2 = (*iter).node();
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(v1->GetPosition().x + 16, v1->GetPosition().y + 16), sf::Color::White),
				sf::Vertex(sf::Vector2f(v2->GetPosition().x + 16, v2->GetPosition().y + 16), sf::Color::White)
			};

			w.draw(line, 2, sf::Lines);

			text.setPosition(sf::Vector2f((v2->GetPosition().x + v1->GetPosition().x) / 2.0f, (v2->GetPosition().y + v1->GetPosition().y) / 2.0f));

			text.setCharacterSize(18);
			text.setString(numToString((*iter).weight()));
			text.setColor(sf::Color::Blue);
			w.draw(text);
		}
	}
}
void DrawPath(Graph<pair<string, int>, int> &g, sf::RenderWindow &w, vector<Node *> &path)
{
	vector<Node *>::iterator iter = path.begin();
	vector<Node *>::iterator endIter = path.end();

	for (; iter != endIter; ++iter)
	{
		Node* tempNode = (*iter);
		if ((*iter)->isDest() == false && (*iter)->isStart() == false)
			(*iter)->ColourPath();

		if (tempNode->getPreviousNode() == NULL)
			continue;

		sf::Vector2f centered = sf::Vector2f(16, 16);
		sf::Vertex line[] =
		{
			sf::Vertex(tempNode->GetPosition() + centered, sf::Color::Yellow),
			sf::Vertex(tempNode->getPreviousNode()->GetPosition() + centered, sf::Color::Yellow)
		};

		w.draw(line, 2, sf::Lines);
	}
}

//Button Inputs
void StartButtonPressed(Graph<pair<string, int>, int> &g, vector<Node *> &path)
{
	bool start = false;
	bool dest = false;
	sf::Vector2i points;

	g.clearMarksAndHeuristics();
	for (int i = 0; i < g.GetNodeCount(); i++)
	{
		g.nodeArray()[i]->resetNodeForMultipath();
	}
	path.clear();

	for (int i = 0; i < g.GetNodeCount(); i ++)
	{
		if (start == false)
		{
			start = g.nodeArray()[i]->isStart();
			points.x = i;
		}
		if (dest == false)
		{
			dest = g.nodeArray()[i]->isDest();
			points.y = i;
		}
		if (start && dest)
		{
			g.AStarPathfinding(g.nodeArray()[points.x], g.nodeArray()[points.y], path);
		}
	}
	if (start == false && dest == false)
		cout << "Please Choose Both a Starting node and Destination node" << endl;
}
void UCSButtonPressed(Graph<pair<string, int>, int> &g, vector<Node *> &path)
{
	bool start = false;
	bool dest = false;
	sf::Vector2i points;

	g.clearMarksAndHeuristics();
	for (int i = 0; i < g.GetNodeCount(); i++)
	{
		g.nodeArray()[i]->resetNodeForMultipath();
	}
	path.clear();

	for (int i = 0; i < g.GetNodeCount(); i++)
	{
		if (start == false)
		{
			start = g.nodeArray()[i]->isStart();
			points.x = i;
		}
		if (dest == false)
		{
			dest = g.nodeArray()[i]->isDest();
			points.y = i;
		}
		if (start && dest)
		{
			g.UCS(g.nodeArray()[points.x], g.nodeArray()[points.y], path);
		}
	}
	if (start == false && dest == false)
		cout << "Please Choose Both a Starting node and Destination node" << endl;
}
void ResetButtonPressed(Graph<pair<string, int>, int> &g, vector<Node *> &path)
{
	g.clearMarksAndHeuristics();
	for (int i = 0; i < g.GetNodeCount(); i++)
	{
		g.nodeArray()[i]->resetNodeForMultipath();
	}
	path.clear();
}
void RandomButtonPressed(Graph<pair<string, int>, int> &g, vector<Node *> &path)
{
	ResetButtonPressed(g, path);
	int pStart, pDest;
	pStart = rand() % g.GetNodeCount();
	pDest = rand() % g.GetNodeCount();
	while (pStart == pDest)
		pDest = rand() % g.GetNodeCount();

	g.nodeArray()[pStart]->setStart();
	g.nodeArray()[pDest]->setDestination();
}
void ClickedNode(sf::Vector2i pos, sf::Mouse::Button b, Graph<pair<string, int>, int> &g)
{
	bool nodeClicked = false;
	for (int i = 0; i < g.GetNodeCount(); i++)
	{
		int z;
		nodeClicked = g.nodeArray()[i]->Contains(pos);
		if (nodeClicked && b == sf::Mouse::Button::Left)
		{
			z = getStart(g);
			g.nodeArray()[z]->resetNode();
			g.nodeArray()[i]->setStart();
		}
		else if (nodeClicked && b == sf::Mouse::Button::Right)
		{
			z = getDestination(g);
			g.nodeArray()[z]->resetNode();
			g.nodeArray()[i]->setDestination();
		}
		if (nodeClicked)
			break;
	}
}
void CalculateHeuristicOnSelected(Graph<pair<string, int>, int> &g)
{
	int s = getStart(g);
	int d = getDestination(g);

	if (s != NULL && d != NULL)
		g.UCSFindHeuristics(g.nodeArray()[s], g.nodeArray()[d]);
	g.clearMarks();
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "A Star");		//Creates the Rendering Window
	vector<Node *> path;
	Button BTN_AStar(sf::Vector2f(620, 60), "Start A*");
	Button BTN_UCS(sf::Vector2f(620, 120), "Start UCS");
	Button BTN_reset(sf::Vector2f(620, 180), "Reset");
	Button BTN_random(sf::Vector2f(620, 240), "Random");

	const sf::Mouse mouse;

	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\GARA.ttf");
	//font.loadFromFile("Starjedi.ttf");
	int nodesInGraph = 30;
	Graph<pair<string, int>, int> graph(nodesInGraph);
	setUpGraph(graph);

	while (window.isOpen())		//The game loop starts
	{
		sf::Event Event;		//Process events
		//window.clear(sf::Color(100, 149, 237));		//Cornflower Blue

		window.setFramerateLimit(60);		//Sets the Frame Limit

		if ((mouse.isButtonPressed(sf::Mouse::Button::Left) || mouse.isButtonPressed(sf::Mouse::Button::Right)) &&
			mouse.getPosition(window).x < 800 && mouse.getPosition(window).x > 0 &&
			mouse.getPosition(window).y < 600 && mouse.getPosition(window).y > 0)
		{
			sf::Mouse::Button b;
			if (mouse.isButtonPressed(sf::Mouse::Button::Left))
				b = sf::Mouse::Button::Left;
			else
				b = sf::Mouse::Button::Right;
			sf::Vector2i pos = mouse.getPosition(window);
			bool pressed = BTN_AStar.CheckClicked(pos);
			if (pressed)
				StartButtonPressed(graph, path);
			pressed = BTN_UCS.CheckClicked(pos);
			if (pressed)
				UCSButtonPressed(graph, path);
			pressed = BTN_reset.CheckClicked(pos);
			if (pressed)
				ResetButtonPressed(graph, path);
			pressed = BTN_random.CheckClicked(pos);
			if (pressed)
				RandomButtonPressed(graph, path);
			else if (path.empty())
			{
				ClickedNode(pos, b, graph);
			}
		}

		window.clear();

		DrawArcs(graph, window, font);
		DrawGraph(graph, window, font);
		BTN_AStar.Draw(window);
		BTN_UCS.Draw(window);
		BTN_reset.Draw(window);
		BTN_random.Draw(window);

		if (!path.empty())
			DrawPath(graph, window, path);
		CalculateHeuristicOnSelected(graph);

		window.display();		//Display rendered frame on screen
	}
}

