#include "fenetreDeJeu.h"

int fenetreDeJeu::getCh()
{
	return ch;
}
Color fenetreDeJeu::getColor()
{
	return color;
}
void fenetreDeJeu::setColor(Color color)
{
	this->color = color;
}
void fenetreDeJeu::setCh(char ch){
	this->ch = ch;
}
void fenetreDeJeu::set(char ch, Color color)
{
	setColor(color);
	setCh(ch);
}

