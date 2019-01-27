#define cimg_use_png 1
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<time.h>
#include<png.h>
#include"CImg.h"
#include"menu.h"
#include"plateau.h"
#include"joueur.h"
#include"trou.h"
#include"coup.h"
#include <stdlib.h>
#include <time.h>

using namespace std;
using namespace cimg_library;

int main() {
	srand(time(NULL)); //permet de generer un nombre aleatoire pour le choix d'une graine à afficher parmi les 4
	unsigned int choix;

	do {
		afficherMenu();
		cin >> choix;

		if (choix == 1) {
			ofstream ofs("partie.txt"); //on ouvre un flux d'ecriture

			string nom1, nom2;
			cout << "Entrez les noms des 2 joueurs sans espace, le premier nom entre correspondra au joueur qui jouera en premier." << endl;
			cin >> nom1 >> nom2;

			CImg<unsigned char> plateau("plateau_awale.ppm"); //on declare l'image du plateau
			CImgDisplay fenetre(plateau, "Partie d'Awale"); //on declare la fenetre de la partie
			
			Plateau partie(&fenetre, &plateau, nom1, nom2); //on declare un objet Plateau qui va gerer la partie
			unsigned int numCoup = 1; //compteur de nombre de coups joués
			bool main = true; //indique qui a la "main" pendant le coup

			//tant que la partie n'est pas terminée, qu'on a pas fermé la fenetre ou qu'on a pas appuyé sur Echap
			while (!partie.partieTerminee() && !fenetre.is_closed() && !fenetre.is_keyESC()) {
				plateau.display(fenetre);	//on affiche la fenetre
				if (fenetre.button() & 1) { //si on effectue un clique droit
					int y = fenetre.mouse_y(); //on recupere les coordonnees du clic de souris
					int x = fenetre.mouse_x();
					int trouId = partie.recupererTrou(x, y);	//on recupere le numero du Trou dans lequel on a cliqué
					if (trouId != -1) { //si on a cliqué dans un trou
						bool proprioTrou; //on recupere un bool qui represente le proprietaire du trou
						if (trouId >= 1 && trouId <= 6) {
							proprioTrou = false;
						}
						else {
							proprioTrou = true;
						}

						//si le joueur a bien clique dans un trou qui lui appartient
						if ((main && proprioTrou) || (!main && !proprioTrou)) {
							int trouFinal = partie.distribuer(trouId, main); //on distribue a partir du trou dans lequel on a cliqué
							if (trouFinal != -1) {	//si la distribution a reussie
								partie.ramasser(trouFinal, main); //on regarde si on peut ramasser des graines
								partie.sauver(ofs, main, trouId, numCoup); //on sauvegarde le coup

								main = !main; //on passe la main
								numCoup++; //on incremente le nombre de coups
							}
						}						
					}
				}
			}
			cout << partie.resultatPartie();	//a la fin de la partie on affiche les resultats
			ofs.close(); //on ferme le flux d'ecriture
		}
		else if (choix == 2) {
			Plateau partie; //ici on va delcarer un nouveau plateau qu'on va initialiser a partir des donnees de la partie precedente
			CImg<unsigned char> plateau("plateau_awale.ppm"); //en theorie ca pourrait fonctionner mais on a eu un probleme de violation d'acces en lecture
															  //que nous n'avons pas pu regler
			CImgDisplay fenetre(plateau, "Partie d'Awale");
			ifstream ifs("partie.txt");
			if (!ifs.is_open()) {
				cout << "Erreur d'ouverture" << endl;
			}
			else {
				Coup dernierCoup = partie.charger(ifs, &fenetre, &plateau);
				if (!ifs.good()) {
					cout << "Erreur de lecture" << endl;
					perror("partie.txt");
				}
				ifs.close();
				
				ofstream ofs("partie.txt");
				bool main = !dernierCoup.getJoueur()->getNum();
				partie.rafraichir(main);
				int numCoup = dernierCoup.getNum() + 1;

				//apres avoir charge toutes les donnees de la partie precedente on continue normalement la partie
				while (!partie.partieTerminee() && !fenetre.is_closed() && !fenetre.is_keyESC()) {
					plateau.display(fenetre);
					if (fenetre.button() & 1) {
						int y = fenetre.mouse_y();
						int x = fenetre.mouse_x();
						int trouId = partie.recupererTrou(x, y);
						if (trouId != -1) {
							bool proprioTrou;
							if (trouId >= 1 && trouId <= 6) {
								proprioTrou = false;
							}
							else {
								proprioTrou = true;
							}

							if ((main && proprioTrou) || (!main && !proprioTrou)) {
								int trouFinal = partie.distribuer(trouId, main);
								if (trouFinal != -1) {
									partie.ramasser(trouFinal, main);
									partie.sauver(ofs, main, trouId, numCoup);

									main = !main;
									numCoup++;
								}
							}
						}
					}
				}
				cout << partie.resultatPartie();
				ofs.close();
			}
		}
		else if (choix == 3) {
			Plateau partie; //ici on va delcarer un nouveau plateau qu'on va initialiser a partir des donnees de la partie precedente
			CImg<unsigned char> plateau("plateau_awale.ppm"); //en theorie ca pourrait fonctionner mais on a eu un probleme de violation d'acces en lecture
															  //que nous n'avons pas pu regler
			CImgDisplay fenetre(plateau, "Partie d'Awale");
			ifstream ifs("partie.txt");
			if (!ifs.is_open()) {
				cout << "Erreur d'ouverture" << endl;
			}
			else {
				Coup dernierCoup = partie.charger(ifs, &fenetre, &plateau);
				if (!ifs.good()) {
					cout << "Erreur de lecture" << endl;
					perror("partie.txt");
				}
				ifs.close();

				//mais on va jouer automatiquement le dernier coup
				plateau.display(fenetre);
				bool main = dernierCoup.getJoueur()->getNum();
				int trouId = dernierCoup.getTrou()->getNumero();
				partie.rafraichir(main);
				int trouFinal = partie.distribuer(trouId, main);
				partie.rafraichir(main);
				Sleep(1000);
				partie.ramasser(trouFinal, main);
				partie.rafraichir(main);

				while (!fenetre.is_closed()) {
					plateau.display(fenetre);
				}
			}
		}
	} while (choix != 4);

	system("pause");
	return 0;
}