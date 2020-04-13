#include "joueur.h"

Joueur::Joueur() {
	init(0);
};



void Joueur::init(int windowDim) {
	selectionPosition = 0;
	for(int i=0; i<5; i++)
		selectionState[i] = false;
	map.init(-1);
	carteTemporaire.init(-1);
    mask.init(-1);
    score.setValeur(windowDim * windowDim);
}


bool Joueur::validerCarteTempo(int windowDim) {
  
	for(int i=0; i<windowDim; i++) {
        for(int j=0; j<windowDim; j++) {
            if((carteTemporaire.getCell(i,j)!=-1) && (map.getCell(i, j) != -1)) {
                return false;
            }
        }
    }

    
    for(int i=0; i<windowDim; i++) {
        for(int j=0; j<windowDim; j++) {
            if(carteTemporaire.getCell(i,j)!=-1) {
                map.setCell(i,j,carteTemporaire.getCell(i, j));
            }
        }
    }

    return true;
}

bool Joueur::selectionPrete() {
    for(int i=0; i<5; i++)
        if(selectionState[i] == false) { return false; }
    return true;
}

void Joueur::genererMask() {
    mask.init(0);
}

void Joueur::setNomJouer(std::string nomJoueur) {this->nomJoueur = nomJoueur;}
void Joueur::setSelectionState(int pos, bool value) {selectionState[pos] = value;}
void Joueur::setSelectionPosition(int selectionPosition) { this->selectionPosition = selectionPosition;}
void Joueur::setMap(TerritoireMaritime &map) {this->map = map;}
void Joueur::setCarteTempo(TerritoireMaritime &carteTemporaire) {this->carteTemporaire = carteTemporaire;}
void Joueur::setMask(TerritoireMaritime &mask) {this->mask = mask;}
void Joueur::setControleOrdinateur(bool controleOrdinateur) {this->controleOrdinateur = controleOrdinateur;}
void Joueur::setScore(int value) {score.setValeur(value);}

std::string Joueur::getNomJoueur() {return nomJoueur;}
int Joueur::getSelectionState(int pos) {return selectionState[pos];}
int Joueur::getSelectionPosition() { return selectionPosition;}
TerritoireMaritime& Joueur::getMap() {return map;}
TerritoireMaritime& Joueur::getTempMap() {return carteTemporaire;}
TerritoireMaritime& Joueur::getMask() { return mask;}
bool Joueur::getControleOrdinateur() {return controleOrdinateur;}
int Joueur::getScore() {return score.getValeur();}
