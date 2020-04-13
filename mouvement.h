#ifndef __MOUVEMENT_H
#define __MOUVEMENT_H

#include "window.h"


class Mouvement {
	int tourJoueur, x, y;
	public:
		int getTourJouer();
		int getX();
		int getY();
		void setMouve(int tourJoueur, int x, int y);
};


#endif
