all:
	g++ game.cpp window.cpp territoireMaritime.cpp menu.cpp joueur.cpp navire.cpp fenetreDeJeu.cpp mouvement.cpp score.cpp -o game -lncurses
clean:
	rm -f hello


