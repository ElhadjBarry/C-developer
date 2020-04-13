#ifndef __NAVIRE_H
#define __NAVIRE_H

#include "window.h"
#include "territoireMaritime.h"

class Navire
{
	int size;
	Color color;
	TerritoireMaritime modelMatrice;
	

	int dimX, dimY;

	public:
		Navire();
		void init();

		void setSize(int size);
		void setColor(Color color);
		void setDim(int dimX, int dimY);
		void setModelMatrice(TerritoireMaritime &modelMatrice);

		TerritoireMaritime& getModelMatrice();
		Color getColor();
		int getSize();
		int getDimX();
		int getDimY();

		
		void inverserRangee();
		void transposerMatrice();
		void rotationGauche();
		void rotationDroite();
};
#endif
