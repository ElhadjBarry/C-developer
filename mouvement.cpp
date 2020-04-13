#include "mouvement.h"


int Mouvement::getTourJouer() {
	return tourJoueur;
}
int Mouvement::getX() {
	return x;
}
int Mouvement::getY() {
	return y;
}
void Mouvement::setMouve(int tourJoueur, int x, int y) {
	this->tourJoueur = tourJoueur;
	this->x = x;
	this->y = y;
}
