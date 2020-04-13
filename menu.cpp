#include "menu.h"



void Menu::CMDMenuPrincipal() {
    cmdMessage(0, "Menu Principal", true);
    cmdMessage(1, "Espace =Commencer une nouvelle partie! Appuyez sur L pour charger une partie!", false);
    cmdMessage(2, "S =Pour voir les meilleurs scores!     Appuyez sur Q pour quitter!", false);
}

void Menu::CMDmenuSelectionNavire(std::string nomJoueur) {
    cmdMessage(0, "Menu selection de navire", true);
    cmdMessage(1, nomJoueur + ", utilisez les flèches pour vous déplacer et ESPACE pour sélectionner un navire dans le sélecteur.", false);
    cmdMessage(2, "Ensuite, placez-le sur la carte, en utilisant les flèches et ESPACE. Tourner avec R/ L. Annuler le coup avec U!", false);

}
void Menu::CMDInfo() {
    cmdMessage(0, "Aide: Placez les navires, puis trouvez les navires de l'adversaire.", true);
    cmdMessage(1, "Utiliser les indications de la ligne de commande à travers le jeu!", false);
    cmdMessage(2, "Version 1.0 |Abed Ouail, Amine Soufyani, Barry Elhadj, Jed Boufeid. C =Continuer.", false);
}
void Menu::CMDnavireTrouve() {
    cmdMessage(1, "Vous avez trouve un navire ! Appuyez sur C pour continuer", true);
}

void Menu::cmdMessage(int rangee, std::string msg, bool clear) {
	int space = 1;
    if(clear) cmd->clear();
    if(!rangee) space = 40 - msg.length() / 2;
    cmd->print(space, rangee , msg);
}
void Menu::init(int pos) {
    cmd = new Window(3 ,78 , 0 , pos);
    cmd->setCouleurFenetre(WRED);
    cmd->setCouleurBordure(WRED);
}

void Menu::CMDdemanderNom(std::string joueurID) {
    cmdMessage(0, "veillez saisir un pseudo" + joueurID + "! Après appuyez sur Espace", true);
}
void Menu::advance() {
    char input;
    cmdMessage(2, "Press c to continue!", false);
    while(input = getch() != 'c'){};
}
void Menu::clear() {
    cmd->clear();
}

Menu::Menu() {

}
Menu::~Menu() {
	delete cmd;
}
