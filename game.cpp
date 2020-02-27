#include "window.h"
#include "joueur.h"
#include "territoireMaritime.h"
#include "navire.h"
#include "menu.h"
#include "fenetreDeJeu.h"
#include "mouvement.h"
#include "score.h"
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <string>
#include <sstream>
#include <algorithm>


class Game
{

    Window *background = NULL;
    Window *battlefront = NULL;
    Window *territoireMaritimeEnnemi = NULL;
    Window *selecteurNavire = NULL;
    Window *apercuNavire = NULL;
    Window *windowActuelle = NULL;


    Menu menu;
    Joueur joueur[2], *joueurActuel;
    Navire ship[5]; Navire modelShip[5];

    fenetreDeJeu frameCell[50][50], oldFrameCell[50][50];
    fenetreDeJeu enemyFrameCell[50][50], enemyOldFrameCell[50][50];

    Mouvement move[10000];

    char fullChar;
    bool selectionActive;
    int windowDim, selectionTemps, moveTime;
    int nbrMouveActuel;


    void updateScreen();
    void initialSetup();
    void lireInfoJoueur(Joueur *joueurActuel, std::string defaultName);
    void shipSelection(Joueur *joueurActuel);
    void placePieceOnWindow(Joueur *joueurActuel);
    void jouer(Joueur joueur[2], bool load);
    void chargerModelNavire();
    void sauvegarderScore(std::string nomJoueur, int nouveauScore);
    void montrerScore();
    void clear();

    int verifierFinPartie();
    int ennemiID(int ID);
    int getMax(int a, int b);
    int getMin(int a, int b);
    int validerSelectionPosition(int pos);

    bool verifieNavireRevele(TerritoireMaritime &map , TerritoireMaritime &mask, int dim,int navireID);
    bool validatePieceInBounds(int x, int y, int dimX, int dimY);

    public:
        void startGame(bool montrerAide);
        Game();
        ~Game();
};

void Game::startGame(bool montrerAide)
{
    short int input;
    initialSetup();

    if(montrerAide)  {
        menu.CMDInfo();
        while((input = getch()) != 'c');
    }

    chargerModelNavire();
    menu.CMDMenuPrincipal();
    updateScreen();

    while((input = getch()) != 'q') {
        switch (input) {
            case 32:

                joueur[0].init(windowDim); joueur[1].init(windowDim);
                lireInfoJoueur(&joueur[0], "Joueur 1"); lireInfoJoueur(&joueur[1], "joueur 2");
                for(int i=0; i<2; i++) {
                    joueurActuel = &joueur[i];
                    shipSelection(joueurActuel);
                    joueurActuel->genererMask();
                    updateScreen();
                }
                nbrMouveActuel = 0;

                jouer(joueur, false);
                clear();
                break;

            case 's':
                montrerScore();
                menu.advance();
                menu.CMDMenuPrincipal();
                break;
            case 'l':
                joueur[0].init(windowDim); joueur[1].init(windowDim);
                updateScreen();joueur[0].genererMask();
                updateScreen(); joueur[1].genererMask();
                updateScreen();
                nbrMouveActuel = 0;
                jouer(joueur, true);
                clear();
                break;
        }
    }
}

void Game::clear() {
    selectionActive = false;
    menu.advance();
    battlefront->clear();
    territoireMaritimeEnnemi->clear();
    menu.CMDMenuPrincipal();
}

void Game::jouer(Joueur joueur[2], bool load) {


    int x, y;
    int joueurID = 1;
    short int input;
    int derniereValeur;
    int score;
    bool pret;
    bool bouge;
    bool annuler = false;
    bool dejaSelectionne;
    bool optionJeu;
    std::ostringstream ss;
    std::string nomJoueur;




    if(load)
    {

        std::fstream f("log.txt", std::ios::in);
        int value;
        int pozX, pozY;

        f>>windowDim;

        for(int i=0; i<2; i++) {
            f>>nomJoueur; f>> optionJeu ; f>>score;
            joueur[i].setNomJouer(nomJoueur);
            joueur[i].setControleOrdinateur(optionJeu);
            joueur[i].setScore(score);
        }


        for(int i=0; i<2; i++) {
            for(int x=0; x<windowDim; x++) {

                for(int y=0; y<windowDim; y++){
                    f>>value;
                    joueur[i].getMap().setCell(x, y, value);
                }
            }
        }

        f>>nbrMouveActuel;
        ss << nbrMouveActuel;

        for(int i=0; i<nbrMouveActuel; i++) {
            f >> joueurID >> pozX >> pozY;
            joueur[ennemiID(joueurID)].getMask().setCell(pozX, pozY, -1);
            move[i].setMouve(joueurID, pozX, pozY);
        }
        f.close();
    }
    bool sauvegarderEtQuitter = false;



    while(!verifierFinPartie() && (sauvegarderEtQuitter == false)) {


        if(joueurID == 1) {
            joueurID = 0;
        }
        else {
            joueurID = 1;
        }

        menu.cmdMessage(1, joueur[joueurID].getNomJoueur() +  " a selectionne une cellule de " +
            joueur[ennemiID(joueurID)].getNomJoueur() +" territoire Maritime !", true);
        if(annuler)
            menu.cmdMessage(0, "Coup annule", false);

        x=0, y=0;
        if(!joueur[joueurID].getControleOrdinateur()){
            derniereValeur = joueur[ennemiID(joueurID)].getMask().getCell(x, y);

            if(joueur[ennemiID(joueurID)].getMask().getCell(x, y) == -1)
                dejaSelectionne = true;
            else
                dejaSelectionne = false;
            joueur[ennemiID(joueurID)].getMask().setCell(x, y, 2);
        }

        updateScreen();

        while(1) {


            if(!joueur[joueurID].getControleOrdinateur()) {
                input = getch();
            }
            else  {
                do{
                    x = rand() % windowDim;;
                    y = rand() % windowDim;;
                }while(joueur[ennemiID(joueurID)].getMask().getCell(x, y) == -1);
                joueur[ennemiID(joueurID)].getMask().setCell(x, y, -1);
                usleep(moveTime);
                move[nbrMouveActuel].setMouve(joueurID, x, y);
                joueur[joueurID].setScore(joueur[joueurID].getScore()-1);
                nbrMouveActuel++;
                break;
            }


            pret = false;
            bouge = false;
            annuler = false;


            if(!joueur[joueurID].getControleOrdinateur()){
                switch(input) {
                    case KEY_LEFT:

                        x--;
                        if(validatePieceInBounds(x, y, 1, 0)) {
                            x++;  joueur[ennemiID(joueurID)].getMask().setCell(x, y, derniereValeur);
                            x--;
                            derniereValeur = joueur[ennemiID(joueurID)].getMask().getCell(x, y);
                            bouge = true;
                        }
                        else
                            x++;
                        break;
                    case KEY_RIGHT:
                        x++;
                        if(validatePieceInBounds(x, y, 1, 0)) {
                            x--;  joueur[ennemiID(joueurID)].getMask().setCell(x, y, derniereValeur);
                            x++;
                            derniereValeur = joueur[ennemiID(joueurID)].getMask().getCell(x, y);
                            bouge = true;
                        }
                        else
                            x--;
                        break;

                    case KEY_UP:
                        y--;
                        if(validatePieceInBounds(x, y, 0, 1)) {
                            y++;  joueur[ennemiID(joueurID)].getMask().setCell(x, y, derniereValeur);
                            y--;
                            derniereValeur = joueur[ennemiID(joueurID)].getMask().getCell(x, y);
                            bouge = true;
                        }
                        else
                            y++;
                        break;

                    case KEY_DOWN:
                        y++;
                        if(validatePieceInBounds(x, y, 0, 1))  {
                            y--;  joueur[ennemiID(joueurID)].getMask().setCell(x, y, derniereValeur);
                            y++;
                            derniereValeur = joueur[ennemiID(joueurID)].getMask().getCell(x, y);
                            bouge = true;
                        }
                        else
                            y--;
                        break;
                    case 32:
                        pret = true;
                        break;
                    case 'u':
                        annuler = true;
                        break;
                    case 'q':
                        menu.cmdMessage(1, "T'as choisi de sauvegarder et quitter le jeu!", true);
                        sauvegarderEtQuitter = true;
                        break;
                }
            }

            if(bouge)  {
                if(joueur[ennemiID(joueurID)].getMask().getCell(x, y) == -1)
                    dejaSelectionne = true;
                else
                    dejaSelectionne = false;
                joueur[ennemiID(joueurID)].getMask().setCell(x, y, 2);
                updateScreen();
            }

            if(pret == true) {
                if(!dejaSelectionne) {
                    joueur[ennemiID(joueurID)].getMask().setCell(x, y, -1);
                    move[nbrMouveActuel].setMouve(joueurID, x, y);
                    nbrMouveActuel++;
                    joueur[joueurID].setScore(joueur[joueurID].getScore()-1);

                    if(verifieNavireRevele( joueur[ennemiID(joueurID)].getMap(),
                                           joueur[ennemiID(joueurID)].getMask(),
                                           windowDim, joueur[ennemiID(joueurID)].getMap().getCell(x, y)))
                    {
                        menu.CMDnavireTrouve();
                        while((input = getch()) != 'c');
                    }



                    break;
                }
                else
                {
                    menu.cmdMessage(1, "Cellule déjà selectionne!", true);
                }
            }

            if(annuler == true)
            {

                int numberOfTimes = 1;
                if(joueur[ennemiID(joueurID)].getControleOrdinateur())
                    numberOfTimes = 2;


                joueur[joueurID].setScore(joueur[joueurID].getScore()-1);
                if(nbrMouveActuel > 0)
                {

                    joueur[0].genererMask();
                    joueur[1].genererMask();
                    nbrMouveActuel-=numberOfTimes;
                    if(nbrMouveActuel < 0) nbrMouveActuel = 0;
                    for(int i=0; i<nbrMouveActuel; i++)
                    {
                        joueur[ennemiID(move[i].getTourJouer())].getMask().setCell(move[i].getX(), move[i].getY(), -1);

                    }
                    updateScreen();
                    break;
                }

            }

            if(sauvegarderEtQuitter)
                break;
        }
    }

    updateScreen();
    int result = verifierFinPartie();
    ss.str("");
    if(result == 1)
    {

        ss << joueur[1].getScore();
        menu.cmdMessage(0, "Fin de partie!",true);
        menu.cmdMessage(1, joueur[1].getNomJoueur() + ", Vous avez gagne. Votre score est " + ss.str() +"!", false);
        sauvegarderScore(joueur[1].getNomJoueur(), joueur[1].getScore());

    }
    if(result == 2)
    {
        ss << joueur[0].getScore();
        menu.cmdMessage(0, "Fin de partie!",true);
        menu.cmdMessage(1, joueur[0].getNomJoueur() + ", Vous avez gagne. Votre score est" + ss.str() +"!", false);
        sauvegarderScore(joueur[0].getNomJoueur(), joueur[0].getScore());
    }


    std::fstream f("log.txt", std::ios::out);


    f<<windowDim<<"\n";

    for(int i=0; i<2; i++)
    {

        f << joueur[i].getNomJoueur() <<" ";
        f << joueur[i].getControleOrdinateur() <<" ";
        f << joueur[i].getScore() <<"\n";
    }

    for(int i=0; i<2; i++)
    {
        for(int x=0; x<windowDim; x++)
        {
            for(int y=0; y<windowDim; y++)
            {
                f<<joueur[i].getMap().getCell(x, y)<<" ";
            }
            f<<"\n";
        }
    }

    f<<nbrMouveActuel<<"\n";


    for(int i=0; i<nbrMouveActuel; i++)
    {
        f << move[i].getTourJouer() << " " <<
        move[i].getX() << " " <<move[i].getY() << "\n";
    }

    f.close();
}

void Game::chargerModelNavire() {



    int lignes, cols;
    std::string line;
    std::fstream f("config3.txt", std::ios::in);
    f>>windowDim;
    for(int i=0; i<5; i++) {
        f>>lignes>>cols;
        ship[i].setDim(lignes, cols);
        modelShip[i].setDim(lignes, cols);
        for(int k=0; k<lignes; k++) {
            f>>line;
            for(int j=0; j<cols; j++) {
                ship[i].getModelMatrice().setCell(k, j, line[j]-'0');
                modelShip[i].getModelMatrice().setCell(k, j, line[j]-'0');
            }
        }
    }
    f.close();
}

int Game::ennemiID(int ID) {
    if(ID == 0)
        return 1;
    return 0;
}

int Game::verifierFinPartie() {


    bool fin[2];
    for(int k=0; k<=1; k++) {
        fin[k] = true;
        for(int i=0; i<windowDim; i++)  {
            for(int j=0; j<windowDim; j++)
            {
                if((joueur[k].getMap().getCell(i, j) != -1) && (joueur[k].getMask().getCell(i, j) != -1))
                {
                    fin[k] = false; break;
                }

            }
        }
        if(fin[k])
            return k+1;
    }

    return 0;
}

void Game::initialSetup() {


    fullChar = 32;


    std::fstream f("config3.txt", std::ios::in);
    f>>windowDim;
    f.close();


    background = new Window(45, 120, 0, 0);
    background->setCouleurFenetre(WBLACK);
    background->print(windowDim * 1.5, 0, "NAVIRES", BRED);
    background->print(windowDim * 0.3, 1, "ESPACE MARITIME", BRED);
    background->print(2 * windowDim + 16, 1, "SELECTEUR NAVIRES", BRED);
    background->print(2 * windowDim + 16, 7, "APERCU NAVIRE", BRED);

    for(int i=3; i<=windowDim + 4; i++) {
        background->print(2 * windowDim + 13, i, fullChar, BRED);

    }

    battlefront = new Window(windowDim ,windowDim , 1 , 4);
    battlefront->setCouleurFenetre(WCYAN);
    battlefront->setCouleurBordure(BWHITE);

    territoireMaritimeEnnemi = new Window(windowDim,windowDim , windowDim+11 , 4);
    territoireMaritimeEnnemi->setCouleurFenetre(WCYAN);
    territoireMaritimeEnnemi->setCouleurBordure(BWHITE);

    selecteurNavire = new Window(1,5, 2 * windowDim + 21, 4);
    selecteurNavire->setCouleurFenetre(WCYAN);
    selecteurNavire->setCouleurBordure(BWHITE);

    apercuNavire = new Window(5,5, 2 * windowDim + 21,10);
    apercuNavire->setCouleurFenetre(WCYAN);
    apercuNavire->setCouleurBordure(BWHITE);

    ship[0].setSize(2); ship[0].setColor(BMAGENTA);
    ship[1].setSize(3); ship[1].setColor(BBLUE);
    ship[2].setSize(3); ship[2].setColor(BYELLOW);
    ship[3].setSize(4); ship[3].setColor(BGREEN);
    ship[4].setSize(5); ship[4].setColor(BRED);

    selectionActive = false;

    menu.init(getMax(19,6+windowDim+1));
}

void Game::updateScreen() {



    selecteurNavire->clear();

    for(int i = 0; i<2; i++){
        if(i==0) {
            windowActuelle = battlefront;

        }
        else{
            windowActuelle = territoireMaritimeEnnemi;

        }

        for(int x=0; x<windowDim; x++) {
            for(int y=0; y<windowDim; y++) {
                if(joueur[i].getMap().getCell(x,y) != -1) {

                    if(i==0) {
                        frameCell[x][y].set(fullChar, ship[joueur[i].getMap().getCell(x, y)].getColor());
                    }
                    else {
                        enemyFrameCell[x][y].set(fullChar, ship[joueur[i].getMap().getCell(x, y)].getColor());
                    }

                }
                else {

                    if(i==0) {
                        frameCell[x][y].set(fullChar, WCYAN);
                    }
                    else {
                        enemyFrameCell[x][y].set(fullChar, WCYAN);
                    }
                }
            }
        }

        for(int x=0; x<windowDim; x++) {
            for(int y=0; y<windowDim; y++) {

                if(joueur[i].getTempMap().getCell(x,y) != -1) {

                    if(i==0) {
                        frameCell[x][y].set(fullChar, ship[joueur[i].getTempMap().getCell(x,y)].getColor());
                    }
                    else {
                        enemyFrameCell[x][y].set(fullChar, ship[joueur[i].getTempMap().getCell(x,y)].getColor());
                    }

                }
            }
        }

        for(int x=0; x<windowDim; x++) {
            for(int y=0; y<windowDim; y++) {

                if(joueur[i].getMask().getCell(x,y) == 0) {

                   if(i==0) {
                        frameCell[x][y].set(fullChar, BWHITE);
                    }
                    else {
                        enemyFrameCell[x][y].set(fullChar, BWHITE);
                    }
                }
                if(joueur[i].getMask().getCell(x,y) == 2) {

                    if(i==0) {
                        frameCell[x][y].set('X', BWHITE);
                    }
                    else {
                        enemyFrameCell[x][y].set('X', BWHITE);
                    }

                }
            }
        }
    }

    for(int x=0; x<windowDim; x++){
        for(int y=0; y<windowDim; y++)  {
           if(( frameCell[x][y].getCh() != oldFrameCell[x][y].getCh() ) || (frameCell[x][y].getColor() != oldFrameCell[x][y].getColor()) )  {
               battlefront->print(x, y, frameCell[x][y].getCh(), frameCell[x][y].getColor());
           }

           oldFrameCell[x][y].set(frameCell[x][y].getCh(), frameCell[x][y].getColor());
        }

        for(int y=0; y<windowDim; y++) {
           if(( enemyFrameCell[x][y].getCh() != enemyOldFrameCell[x][y].getCh() ) || (enemyFrameCell[x][y].getColor() != enemyOldFrameCell[x][y].getColor()) ) {
               territoireMaritimeEnnemi->print(x, y, enemyFrameCell[x][y].getCh(), enemyFrameCell[x][y].getColor());
           }

           enemyOldFrameCell[x][y].set(enemyFrameCell[x][y].getCh(), enemyFrameCell[x][y].getColor());
        }
    }


    if(selectionActive) {
        for(int i=0; i<5; i++)  {
            if(joueurActuel->getSelectionState(i) == false) {
                for(int j=0; j<ship[i].getSize(); j++)
                {
                    selecteurNavire->print(i, j, fullChar, ship[i].getColor());
                }
            }
        }
        selecteurNavire->print(joueurActuel->getSelectionPosition(), 0, 'X', ship[joueurActuel->getSelectionPosition()].getColor());


        apercuNavire->clear();
        for(int i=0; i<5; i++){
            for(int j=0; j<5; j++) {
                if(modelShip[joueurActuel->getSelectionPosition()].getModelMatrice().getCell(i,j) == 1)
                {
                    apercuNavire->print(j, i, fullChar, ship[joueurActuel->getSelectionPosition()].getColor());
                }
            }
        }
    }
}

bool Game::verifieNavireRevele(TerritoireMaritime &map , TerritoireMaritime &mask, int dim, int navireID) {


    for(int i=0; i<dim; i++) {
        for(int j=0; j<dim; j++) {
            if(map.getCell(i, j) == navireID && mask.getCell(i, j) == 0)
                return false;
        }
    }
    return true;
}

void Game::shipSelection(Joueur *joueurActuel) {


    bool pret = false;
    short int input;
    selectionActive = true;
    menu.CMDmenuSelectionNavire(joueurActuel->getNomJoueur());
    updateScreen();

    if(!joueurActuel->getControleOrdinateur()) {

        while(pret != true) {
            while((input = getch()))
            {

                if(joueurActuel->selectionPrete()) {
                    pret = true;
                    break;
                }
                switch (input) {
                    case KEY_LEFT:

                        joueurActuel->setSelectionPosition(joueurActuel->getSelectionPosition() -1 );
                        joueurActuel->setSelectionPosition(validerSelectionPosition(joueurActuel->getSelectionPosition()));
                        menu.CMDmenuSelectionNavire(joueurActuel->getNomJoueur());
                        updateScreen();
                        break;
                    case KEY_RIGHT:
                        joueurActuel->setSelectionPosition(joueurActuel->getSelectionPosition() +1 );
                        joueurActuel->setSelectionPosition(validerSelectionPosition(joueurActuel->getSelectionPosition()));
                        menu.CMDmenuSelectionNavire(joueurActuel->getNomJoueur());
                        updateScreen();
                        break;
                    case 32:

                        if(joueurActuel->getSelectionState(joueurActuel->getSelectionPosition()) == false) {
                            joueurActuel->setSelectionState(joueurActuel->getSelectionPosition(), true);
                            placePieceOnWindow(joueurActuel);
                            menu.CMDmenuSelectionNavire(joueurActuel->getNomJoueur());
                            updateScreen();
                        }
                        break;
                    case 'c':
                        break;
                }
            }
        }
    }
    else
    {
        if(!joueurActuel->selectionPrete()) {
            for(int i=0; i<5; i++) {
                joueurActuel->setSelectionPosition(i);
                joueurActuel->setSelectionState(i, true);
                placePieceOnWindow(joueurActuel);
            }
        }
    }
}

int Game::validerSelectionPosition(int pos) {
    if(pos < 0)
        return 4;
    if(pos > 4)
        return 0;
    return pos;
}

void Game::placePieceOnWindow(Joueur *joueurActuel) {
    int x=0, y=0;
    bool pret = false;
    short int input;
    bool bouge = true;
    int rotated;



    if(!joueurActuel->getControleOrdinateur()) {
        while(pret == false){
            while((input = getch()) && !pret) {

                if(bouge){

                    joueurActuel->getTempMap().init(-1);

                    for(int i=0; i<5; i++) {
                        for(int j=0; j<5; j++) {
                            if(ship[joueurActuel->getSelectionPosition()].getModelMatrice().getCell(j,i) == 1) {
                                joueurActuel->getTempMap().setCell(x+i,y+j,joueurActuel->getSelectionPosition());
                            }
                        }
                    }
                    updateScreen();
                }
                bouge = false;
                switch (input) {
                    case KEY_LEFT:
                        x--;

                        if(validatePieceInBounds(x, y, ship[joueurActuel->getSelectionPosition()].getDimY(),
                            ship[joueurActuel->getSelectionPosition()].getDimX())) {
                            bouge = true;
                        }
                        else {
                            menu.cmdMessage(1, "Don't cross the borders, bro :c!", true);
                            x++;
                        }
                        break;
                    case KEY_RIGHT:
                        x++;
                        if(validatePieceInBounds(x, y, ship[joueurActuel->getSelectionPosition()].getDimY(),
                            ship[joueurActuel->getSelectionPosition()].getDimX())) {
                            bouge = true;
                        }
                        else{
                            menu.cmdMessage(1, "Don't cross the borders, bro :c!", true);
                            x--;
                        }
                        break;
                    case KEY_UP:
                        y--;
                        if(validatePieceInBounds(x, y, ship[joueurActuel->getSelectionPosition()].getDimY(),
                            ship[joueurActuel->getSelectionPosition()].getDimX())) {
                            bouge = true;
                        }
                        else{
                            menu.cmdMessage(1, "Don't cross the borders, bro :c!", true);
                            y++;
                        }
                        break;
                    case KEY_DOWN:
                        y++;
                        if(validatePieceInBounds(x, y, ship[joueurActuel->getSelectionPosition()].getDimY(),
                            ship[joueurActuel->getSelectionPosition()].getDimX())) {
                            bouge = true;
                        }
                        else{
                            menu.cmdMessage(1, "Don't cross the borders, bro :c!", true);
                            y--;
                        }
                        break;

                    case 'r':
                        ship[joueurActuel->getSelectionPosition()].rotationDroite();
                        if(validatePieceInBounds(x, y, ship[joueurActuel->getSelectionPosition()].getDimY(),
                            ship[joueurActuel->getSelectionPosition()].getDimX())) {
                            bouge = true;
                        }
                        else {
                            ship[joueurActuel->getSelectionPosition()].rotationGauche();
                            menu.cmdMessage(1, "INVALID ROTATION! Don't cross the borders, bro :c!", true);}
                        break;
                    case 'l':
                        ship[joueurActuel->getSelectionPosition()].rotationGauche();
                        if(validatePieceInBounds(x, y, ship[joueurActuel->getSelectionPosition()].getDimY(),
                            ship[joueurActuel->getSelectionPosition()].getDimX())) {
                            bouge = true;
                        }
                        else {
                            ship[joueurActuel->getSelectionPosition()].rotationDroite();
                            menu.cmdMessage(1, "INVALID ROTATION! Don't cross the borders, bro :c!", true);}
                        break;
                    case 32:
                        pret = joueurActuel->validerCarteTempo(windowDim);
                        if(!pret) {menu.cmdMessage(1, "Do not overlap the ships, amigo :c!", true);}
                        break;
                }
            }
        }
    }

    else{
        while(pret == false)  {

            rotated = rand() % 4;
            for(int i=0; i<rotated; i++) {
                ship[joueurActuel->getSelectionPosition()].rotationGauche();
            }
            x = rand()%windowDim;;
            y = rand()%windowDim;;

            if(validatePieceInBounds(x, y, ship[joueurActuel->getSelectionPosition()].getDimX(),
               ship[joueurActuel->getSelectionPosition()].getDimY() )) {

                joueurActuel->getTempMap().init(-1);
                for(int i=0; i<5; i++){
                    for(int j=0; j<5; j++) {
                        if(ship[joueurActuel->getSelectionPosition()].getModelMatrice().getCell(i,j) == 1) {
                            joueurActuel->getTempMap().setCell(x+i,y+j,joueurActuel->getSelectionPosition());
                        }
                    }
                 }

                pret = joueurActuel->validerCarteTempo(windowDim);
                usleep(selectionTemps);
            }
        }
        updateScreen();
    }
}

bool Game::validatePieceInBounds(int x, int y, int dimX, int dimY) {

    if(x < 0 || x + dimX - 1 > windowDim-1 )
            return false;
    if(y < 0 || y + dimY -1 > windowDim-1)
            return false;
    return true;
}

void Game::sauvegarderScore(std::string nomJoueur, int nouveauScore) {
    /* IDEA
     *on lit les scores déjà enregistrés (qui sont maximum 5),
      * et stockez-les dans le tableau des partitions. Ensuite, nous ajoutons le score donné
      * comme paramètre. Ensuite, nous trions tout le tableau et wrtie pour classer
      * les meilleurs 5
    */

    int scores[6];
    int score;
    int numberOfScores;
    std::string name[6];
    std::fstream f("score.txt", std::ios::in);

    f >> numberOfScores;
    for(int i=0; i<numberOfScores; i++)  {
        f>>scores[i];
        f>>name[i];
    }

    f.close();
    numberOfScores++;

    scores[numberOfScores-1] = nouveauScore;
    name[numberOfScores-1] = nomJoueur;


    f.open("score.txt", std::ios::out);
    f<<getMin(numberOfScores, 5)<<"\n";
    for(int i=0; i<getMin(numberOfScores, 5); i++) {
        f<<scores[i] << " "<<name[i] <<"\n";
    }
    f.close();

}
void Game::montrerScore()
{
    int numberOfScores;
    int score = -1;
    std::string name;
    std::string line;
    std::fstream f("score.txt", std::ios::in);

    f>>numberOfScores;

    for(int i=0; i<numberOfScores; i++) {
         std::ostringstream ss;
         f >> score >> name;
         ss << score;
         line.append("|" + name + " " + ss.str() +"| ");
    }

    if(numberOfScores) {
        menu.cmdMessage(0, "MEILLEURS SCORES", true);
        menu.cmdMessage(1, line, false);
    }
    else {
        menu.cmdMessage(0, "PAS DE SCORES!", true);
    }



    f.close();
}

void Game::lireInfoJoueur(Joueur *joueurActuel, std::string defaultName) {
    std::string name;
    menu.CMDdemanderNom(defaultName);
    menu.cmdMessage(1, "Ton pseudo est : " + name, false);
    short int ch;


    while( 1) {
        ch = getch();
        if( (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') ) {
            name += ch;
            menu.cmdMessage(1, "Ton pseudo est : " + name, false);
        }
        if( ch == KEY_BACKSPACE) {
            if(name.size() > 0) {
                name.erase(name.end() - 1);
                menu.CMDdemanderNom(defaultName);
                menu.cmdMessage(1, "Ton pseudo est : " + name, false);
            }
        }
        if( ch == 32)
            break;
    }
    joueurActuel->setNomJouer(name);

    menu.cmdMessage(1, "Joueur ou Machine, " + name + "? (Appuyez O(OUI)/N(NON)) :", false);
    while( 1) {
        ch = getch();
        if(ch == 'o' || ch == 'O') {
            joueurActuel->setControleOrdinateur(true);
            break;
        }
        if(ch == 'n' || ch == 'N') {
            joueurActuel->setControleOrdinateur(false);
            break;
        }
    }
}


int Game::getMax(int a, int b) {
    return a > b ? a: b;
}
int Game::getMin(int a, int b) {
    return a < b ? a: b;
}


int main(int argc, char *argv[]){

    Game game;
    startProgramX();

    if(argc >= 2)
        game.startGame(true);
    else
        game.startGame(false);
    stopProgramX();
    exit(0);
}
Game::~Game() {
    delete background;
    delete battlefront;
    delete territoireMaritimeEnnemi;
    delete selecteurNavire;
}



Game::Game(){
    windowDim = -1;
    srand(time(NULL));
    for(int i=0; i<50; i++) {
        for(int j=0; j<50; j++) {
            frameCell[i][j].setCh(0);
            oldFrameCell[i][j].setCh(0);
            enemyFrameCell[i][j].setCh(0);
            enemyOldFrameCell[i][j].setCh(0);
            frameCell[i][j].setColor(BRED);
            oldFrameCell[i][j].setColor(BRED);
            enemyFrameCell[i][j].setColor(BRED);
            enemyOldFrameCell[i][j].setColor(BRED);
        }
    }
    //Les valeurs de retard que nous utilisons pour l'ordinateur, pour éviter les mouvements de recherche instantanée.
    selectionTemps = 200000;
    moveTime = 500000;
}
