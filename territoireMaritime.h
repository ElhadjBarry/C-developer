#ifndef __TERRITOIREMARITIME_H
#define __TERRITOIREMARITIME_H

#include "window.h"




class TerritoireMaritime {

	int cell[50][50];
	public:

		TerritoireMaritime();
    	void init(int value);
    	void setCell(int x, int y, int value);
    	int getCell(int x, int y);
};

#endif
