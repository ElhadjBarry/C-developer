#ifndef __MENU_H
#define __MENU_H

#include "window.h"



class Menu {
	Window *cmd;
	public:
		void cmdMessage(int rangee, std::string msg, bool clear);
		void CMDMenuPrincipal();
		void CMDmenuSelectionNavire(std::string nomJoueur);
		void CMDdemanderNom(std::string joueurID);
		void CMDnavireTrouve();
		void CMDInfo();
		void clear();
		void init(int pos);
		void advance(); 
		Menu();
		~Menu();
};
#endif
