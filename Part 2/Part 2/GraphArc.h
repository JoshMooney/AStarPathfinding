#ifndef GRAPHARC_H
#define GRAPHARC_H

#include "GraphNode.h"

// -------------------------------------------------------
// Name:        GraphArc
// Description: This is the arc class. The arc class
//              points to a graph node, and contains a 
//              weight.
// -------------------------------------------------------
template<class NodeType, class ArcType>
class GraphArc {
private:

// -------------------------------------------------------
// Description: pointer to the node that the arc points to
// -------------------------------------------------------
    GraphNode<NodeType, ArcType>* m_pNode;

// -------------------------------------------------------
// Description: Weight of the arc
// -------------------------------------------------------
    ArcType m_weight;
	sf::Vector2f pFirst, pSecond;
	sf::RectangleShape m_rectangle;
	float r_Thickness = 4;

public:    
	void CreateArcRectangle(sf::Vector2i v1, sf::Vector2i v2);
	sf::RectangleShape getRectangle()
	{
		return m_rectangle;
	}
    // Accessor functions
    GraphNode<NodeType, ArcType>* node() const {
        return m_pNode;
    }
                              
    ArcType weight() const {
        return m_weight;
    }
    
    // Manipulator functions
    void setNode(GraphNode<NodeType, ArcType>* pNode) {
       m_pNode = pNode;
    }
    
    void setWeight(ArcType weight) {
       m_weight = weight;
    }
	sf::Text m_text;
};

template<class NodeType, class ArcType>
void GraphArc<NodeType, ArcType>::CreateArcRectangle(sf::Vector2i v1, sf::Vector2i v2)
{
	m_rectangle.setFillColor(sf::Color::White);

	if (v1.x == v2.x && v1.y != v2.y)		//Horizontal
	{
		m_rectangle.setPosition(sf::Vector2f(v1.x, v1.y));
		m_rectangle.setSize(sf::Vector2f(r_Thickness, v2.y));
		//m_rectangle.setPosition(sf::Vector2f(v1.x + 16, v1.y + 32));
		//m_rectangle.setSize(sf::Vector2f(r_Thickness, v2.y - 32));
	}
	else if (v1.x != v2.x && v1.y == v2.y)		//Vertical
	{
		m_rectangle.setPosition(sf::Vector2f(v1.x, v1.y));
		m_rectangle.setSize(sf::Vector2f(v2.x, r_Thickness));
		/*m_rectangle.setPosition(sf::Vector2f(v1.x + 32, v1.y + 16));
		m_rectangle.setSize(sf::Vector2f(v2.x - 32, r_Thickness));*/
	}
	else//Diagonal
	{
		if (v1.x < v2.x && v1.y < v2.y)	// [ \ ]
		{
			m_rectangle.setPosition(sf::Vector2f(v1.x, v1.y));
			m_rectangle.setSize(sf::Vector2f(r_Thickness, v2.y));
			/*m_rectangle.setPosition(sf::Vector2f(v1.x + 16, v1.y + 16));
			m_rectangle.setSize(sf::Vector2f(r_Thickness, v2.y + 16));*/
			m_rectangle.setRotation(-45.0f);
		}
		else if (v1.x > v2.x && v1.y < v2.y) // [ / ]
		{
			m_rectangle.setPosition(sf::Vector2f(v1.x, v1.y));
			m_rectangle.setSize(sf::Vector2f(r_Thickness, v2.y));
			/*m_rectangle.setPosition(sf::Vector2f(v1.x + 16, v1.y + 16));
			m_rectangle.setSize(sf::Vector2f(r_Thickness, v2.y + 16));*/
			m_rectangle.setRotation(45.0f);
		}
	}
}

#endif
