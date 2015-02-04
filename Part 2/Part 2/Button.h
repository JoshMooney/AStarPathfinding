//Include Guard
#ifndef BUTTON_H
#define BUTTON_H

#include "stdafx.h"

class Button
{
private:
	sf::Vector2f m_position;
	float m_width = 120;
	float m_height = 40;
	sf::RectangleShape m_rectangle;
	bool m_active;
	sf::Font font;
	sf::Text m_text;
	sf::Vector2f m_textPosition;

public:
	Button(sf::Vector2f pos, std::string text);
	~Button();
	void LoadAssets();
	bool CheckClicked(sf::Vector2i pos);
	void Clicked();
	void Unclick();
	void Draw(sf::RenderWindow &w);
//======================================================
	sf::Vector2f GetPosition()	{ return m_position; }
	void SetPosition(sf::Vector2f pos)	{ m_position = pos; }
	sf::RectangleShape GetRectangle()	{ return m_rectangle; }
	bool GetActive()	{ return m_active; }
	void SetActive(bool act)	{ m_active = act; }
	sf::Text GetText()	{ return m_text; }
	void SetText(sf::Text text)	{ m_text = text; }
	sf::Font GetFont()	{ return font; }
	sf::Vector2f GetTextPosition()	{ return m_textPosition; }
	void SetTextPosition(sf::Vector2f textpos)	{ m_textPosition = textpos; }
};

Button::Button(sf::Vector2f pos, std::string text)
{
	m_position = pos;
	m_rectangle.setFillColor(sf::Color(138, 132, 132, 255));
	m_rectangle.setOutlineThickness(2);
	m_rectangle.setOutlineColor(sf::Color::White);
	m_rectangle.setPosition(m_position);
	m_rectangle.setSize(sf::Vector2f(m_width, m_height));

	LoadAssets();
	m_text.setString(text);
	m_text.setCharacterSize(20);
	m_textPosition = sf::Vector2f(m_position.x + 40, m_position.y + 10);
	m_text.setPosition(m_textPosition);
	m_text.setColor(sf::Color::Black);
}
Button::~Button()
{

}

void Button::Clicked()
{

}
void Button::Unclick()
{

}

void Button::LoadAssets()
{
	font.loadFromFile("C:\\Windows\\Fonts\\GARA.ttf");
	m_text.setFont(font);
}
bool Button::CheckClicked(sf::Vector2i pos)
{
	if (pos.x >= m_rectangle.getPosition().x &&
		pos.x <= m_rectangle.getPosition().x + m_rectangle.getSize().x &&
		pos.y >= m_rectangle.getPosition().y &&
		pos.y <= m_rectangle.getPosition().y + m_rectangle.getSize().y)
		return true;
	else return false;
}
void Button::Draw(sf::RenderWindow &w)
{
	w.draw(m_rectangle);
	w.draw(m_text);
}

#endif