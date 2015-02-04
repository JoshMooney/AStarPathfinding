#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include "stdafx.h"
#include <list>

// Forward references
template <typename NodeType, typename ArcType> class GraphArc;

// -------------------------------------------------------
// Name:        GraphNode
// Description: This is the node class. The node class 
//              contains data, and has a linked list of 
//              arcs.
// -------------------------------------------------------
template<class NodeType, class ArcType>
class GraphNode {
private:    
    typedef GraphArc<NodeType, ArcType> Arc;
    typedef GraphNode<NodeType, ArcType> Node;

    NodeType m_data;
	Node* m_previousNode;

	//int m_actualF;
	int m_f;	//Total distance calculated
	int m_g;	//The cost to get to this point on the shortest path (NB Must include parents of G)
	int m_h;	//The estimated distance remaining til target is reached.
	
	int m_radius = 16;
	sf::Vector2f m_position;
	sf::CircleShape m_circle;
	sf::Text m_text;
	sf::Text m_gText;
	sf::Vector2f m_textPosition;
	const sf::Color grey = sf::Color(160, 160, 160);

// -------------------------------------------------------
// Description: list of arcs that the node has.
// -------------------------------------------------------
    list<Arc> m_arcList;

// -------------------------------------------------------
// Description: This remembers if the node is marked.
// -------------------------------------------------------
    bool m_marked;

public:
	void initalise()
	{
		m_text.setString(getData().first);
		m_text.setCharacterSize(20);
		//m_text.setOrigin(sf::Vector2f(m_position.x + 16, m_position.y + 16));
		m_textPosition = sf::Vector2f(m_position.x+8, m_position.y+8);
		m_text.setPosition(m_textPosition);
		m_text.setColor(sf::Color::Blue);

		m_circle.setPosition(m_position);
		m_circle.setRadius(m_radius);
		m_circle.setFillColor(sf::Color::White);
	}
	sf::Text GetName()	{ return m_text; }
	void SetRadius(int rad)	{ m_radius = rad; }
	int GetRadius()	{ return m_radius; }
	void SetPosition(sf::Vector2f pos)	{ m_position = pos; }
	sf::Vector2f GetPosition()	{ return m_position; }
	sf::CircleShape GetShape()	{ return m_circle; }

	//Colour Nodes
	void setStart();
	void setDestination();
	void resetNode();
	bool resetNode(sf::Color col);
	void Visited();
	void ColourPath();

	bool Contains(sf::Vector2i pos);

	int GetG()	{ return m_data.second; }
	void SetG(NodeType set)	{ m_g = set; }
	int GetH()	{	return m_h;	}
	void SetH(int set)	{ m_h = set; }

	int GetF()	{ return m_data.second; }
	void SetF(int set)	{ m_f = set; }

	GraphNode()
	{
		m_previousNode = NULL;
	}

    // Accessor functions
    list<Arc> const & arcList() const 
	{
        return m_arcList;              
    }

    bool marked() const 
	{
        return m_marked;
    }

	Node * getPreviousNode()		//Returns the previousNode
	{
		return m_previousNode;
	}
	void setPreviousNode(Node *node)			//Sets the Previous node to a pointer to a node
	{
		m_previousNode = node;
	}
    // Manipulator functions
    void setData(NodeType data) 
	{
        m_data = data;
    }

	NodeType & getData()
	{
		return m_data;
	}
    
    void setMarked(bool mark) 
	{
        m_marked = mark;
    }
           
    Arc* getArc( Node* pNode );    
	void addArc(Node* pNode, ArcType pWeight);
	void addArc(Node* pNode, ArcType pWeight, sf::Vector2i v1, sf::Vector2i v2);
    void removeArc( Node* pNode );
	bool isStart();
	bool isDest();
	void DrawArcs(sf::RenderWindow &w);
};

template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::DrawArcs(sf::RenderWindow &w)
{
	list<Arc>::iterator iter = m_arcList.begin();
	list<Arc>::iterator endIter = m_arcList.end();

	for (; iter != endIter; iter++)
	{
		sf::RectangleShape r = (*iter).getRectangle();
		w.draw((*iter).getRectangle());
	}
}

// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    This finds the arc in the current node that
//                  points to the node in the parameter.
//  Arguments:      The node that the arc connects to.
//  Return Value:   A pointer to the arc, or 0 if an arc doesn't
//                  exist from this to the specified input node.
// ----------------------------------------------------------------
template<typename NodeType, typename ArcType>
GraphArc<NodeType, ArcType>* GraphNode<NodeType, ArcType>::getArc( Node* pNode ) {

     list<Arc>::iterator iter = m_arcList.begin();
     list<Arc>::iterator endIter = m_arcList.end();
     Arc* pArc = 0;
     
     // find the arc that matches the node
     for( ; iter != endIter && pArc == 0; ++iter ) {         
          if ( (*iter).node() == pNode) {
               pArc = &( (*iter) );
          }
     }

     // returns null if not found
     return pArc;
}

// ----------------------------------------------------------------
//  Name:           addArc
//  Description:    This adds an arc from the current node pointing
//                  to the first parameter, with the second parameter 
//                  as the weight.
//  Arguments:      First argument is the node to connect the arc to.
//                  Second argument is the weight of the arc.
//  Return Value:   None.
// ----------------------------------------------------------------
template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::addArc(Node* pNode, ArcType weight, sf::Vector2i v1, sf::Vector2i v2) {
   // Create a new arc.
   Arc a;
   a.setNode(pNode);
   a.setWeight(weight);   
   a.CreateArcRectangle(v1, v2);
   // Add it to the arc list.
   m_arcList.push_back( a );
}

template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::addArc(Node* pNode, ArcType weight) {
	// Create a new arc.
	Arc a;
	a.setNode(pNode);
	a.setWeight(weight);
	a.m_text.setString(std::to_string(weight));
	// Add it to the arc list.
	m_arcList.push_back(a);
}

// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This finds an arc from this node to input node 
//                  and removes it.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::removeArc( Node* pNode ) {
     list<Arc>::iterator iter = m_arcList.begin();
     list<Arc>::iterator endIter = m_arcList.end();

     int size = m_arcList.size();
     // find the arc that matches the node
     for( ; iter != endIter && m_arcList.size() == size;  
                                                    ++iter ) {
          if ( (*iter).node() == pNode) {
             m_arcList.remove( (*iter) );
          }                           
     }
}

//Colour Nodes
template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::setStart()
{
	m_circle.setFillColor(sf::Color::Green);
}

template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::setDestination()
{
	m_circle.setFillColor(sf::Color::Red);
}

template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::Visited()
{
	m_circle.setFillColor(grey);
}

template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::ColourPath()
{
	m_circle.setFillColor(sf::Color::Yellow);
}

template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::resetNode()
{
	m_circle.setFillColor(sf::Color::White);
}

template<typename NodeType, typename ArcType>
bool GraphNode<NodeType, ArcType>::resetNode(sf::Color col)
{
	if (m_circle.getFillColor() == col)
	{
		m_circle.setFillColor(sf::Color::White);
		return true;
	}
	return false;
}

template<typename NodeType, typename ArcType>
bool GraphNode<NodeType, ArcType>::Contains(sf::Vector2i pos)
{
	if (pos.x >= m_circle.getPosition().x &&
		pos.x <= m_circle.getPosition().x + (m_circle.getRadius() * 2) &&
		pos.y >= m_circle.getPosition().y &&
		pos.y <= m_circle.getPosition().y + (m_circle.getRadius() * 2))
		return true;
	else return false;
}

template<typename NodeType, typename ArcType>
bool GraphNode<NodeType, ArcType>::isStart()
{
	if (m_circle.getFillColor() == sf::Color::Green)
		return true;
	return false;
}

template<typename NodeType, typename ArcType>
bool GraphNode<NodeType, ArcType>::isDest()
{
	if (m_circle.getFillColor() == sf::Color::Red)
		return true;
	return false;
}

#include "GraphArc.h"

#endif
