#include "navire.h"

Navire::Navire() {
	init();
}
void Navire::init() {
	size = 0;
	color = WBLUE;
}
void Navire::setSize(int size) {this->size = size;}
void Navire::setColor(Color color) {this->color = color;}
void Navire::setModelMatrice(TerritoireMaritime &modelMatrice) {this->modelMatrice = modelMatrice;}
void Navire::setDim(int dimX, int dimY){ this->dimX = dimX; this->dimY = dimY;}

int Navire::getDimX() {return dimX;}
int Navire::getDimY() {return dimY;}
TerritoireMaritime& Navire::getModelMatrice() {return modelMatrice;}
int Navire::getSize() {return size;}
Color Navire::getColor() {return color;}

void Navire::rotationGauche() {

	// + 90 degrés
// Transposer puis inverser les lignes
	transposerMatrice();
	inverserRangee();
}
void Navire::rotationDroite() {
	//-90 degrés
// inverse chaque rangée
//transposer
	inverserRangee();
	transposerMatrice();
}

//Inverse le modèle modèleMatrice
void Navire::inverserRangee() {
	int temp;
	for(int i=0; i<dimX/2; i++)	{
		for(int j=0; j<dimY; j++) {
			temp = modelMatrice.getCell(dimX-1-i, j);
			modelMatrice.setCell(dimX-1-i, j, modelMatrice.getCell(i, j));
			modelMatrice.setCell(i, j, temp);
		}
	}
}
//fait la transposition du modèleMatrice
void Navire::transposerMatrice() {
	TerritoireMaritime tempMap;
	for(int i=0; i<dimX; i++) {
		for(int j=0; j<dimY; j++){
			tempMap.setCell(j, i, modelMatrice.getCell(i, j));
		}
	}

	
	int temp = dimY;
	dimY = dimX;
	dimX = temp;

	modelMatrice.init(0);
	for(int i=0; i<dimX; i++) {
		for(int j=0; j<dimY; j++) {
			modelMatrice.setCell(i, j, tempMap.getCell(i, j));
		}
	}
}
