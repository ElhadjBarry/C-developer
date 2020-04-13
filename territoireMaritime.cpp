#include "territoireMaritime.h"


TerritoireMaritime::TerritoireMaritime() {
	init(-1);
}


void TerritoireMaritime::init(int value) {
	for(int i=0; i<50; i++)
        for(int j=0; j<50; j++){
            cell[i][j] = value;
        }
}
void TerritoireMaritime::setCell(int x, int y, int value) {cell[x][y] = value;}
int TerritoireMaritime::getCell(int x, int y) {return cell[x][y];}

