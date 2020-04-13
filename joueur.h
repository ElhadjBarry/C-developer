#ifndef __JOUEUR_H
#define __JOUEUR_H

#include "window.h"
#include "territoireMaritime.h"
#include "score.h"

class Joueur
{
    std::string nomJoueur;
    
    bool selectionState[5];
  

    int selectionPosition;

    

    TerritoireMaritime map;
    

    TerritoireMaritime carteTemporaire;
    

    TerritoireMaritime mask;

  

    bool controleOrdinateur;

  
    Score score;

    public:
        Joueur();
        void init(int windowDim);
        bool validerCarteTempo(int windowDim);
        bool selectionPrete();
        void genererMask();

        void setNomJouer(std::string nomJoueur);
        void setSelectionState(int pos, bool value);
        void setSelectionPosition(int selectionPosition);
        void setMap(TerritoireMaritime &map);
        void setCarteTempo(TerritoireMaritime &carteTemporaire);
        void setMask(TerritoireMaritime &mask);
        void setControleOrdinateur(bool controleOrdinateur);
        void setScore(int value);

        std::string getNomJoueur();
        int getSelectionState(int pos);
        int getSelectionPosition();
        TerritoireMaritime& getMap();
        TerritoireMaritime& getTempMap();
        TerritoireMaritime& getMask();
        bool getControleOrdinateur();
        int getScore();
};
#endif
