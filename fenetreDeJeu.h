#ifndef __FENETREDEJEU_H
#define __FENETREDEJEU_H

#include "window.h"

class fenetreDeJeu
{
	char ch;
	Color color;

	public:
		int getCh();
		Color getColor();
		void setColor(Color color);
		void setCh(char ch);
		void set(char ch, Color color);
};

#endif
