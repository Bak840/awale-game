#include"plateau.h"
#include<fstream>
#include<iostream>
#include<list>
#include"CImg.h"
#include"joueur.h"
#include"trou.h"
#include"coup.h"
#include <stdlib.h>
#include <windows.h>
#include<string>

Plateau::Plateau(cimg_library::CImgDisplay* fenetre, cimg_library::CImg<unsigned char>* plateau, std::string nom1, std::string nom2) { 
	plateau_ = plateau;	//on initialise les attributs de l'objet
	fenetre_ = fenetre;

	joueurs_[0] = Joueur(true, nom1);
	joueurs_[1] = Joueur(false, nom2);

	trous_[0] = Trou(true, 1, 144, 15);	//pour les trous on a choisi de les ranger dans l'ordre anti-horaire pour faciliter
	trous_[1] = Trou(false, 2, 266, 15); //la distribution de graines
	trous_[2] = Trou(false, 3, 266, 137);
	trous_[3] = Trou(false, 4, 266, 260);
	trous_[4] = Trou(false, 5, 266, 382);
	trous_[5] = Trou(false, 6, 266, 504);
	trous_[6] = Trou(false, 7, 266, 626);
	trous_[7] = Trou(true, 8, 144, 626);
	trous_[8] = Trou(true, 9, 144, 504);
	trous_[9] = Trou(true, 10, 144, 382);
	trous_[10] = Trou(true, 11, 144, 260);
	trous_[11] = Trou(true, 12, 144, 137);

	graines_[0] = cimg_library::CImg<unsigned char>("graine_1.png");
	graines_[1] = cimg_library::CImg<unsigned char>("graine_2.png");
	graines_[2] = cimg_library::CImg<unsigned char>("graine_3.png");
	graines_[3] = cimg_library::CImg<unsigned char>("graine_4.png");

	unsigned char back[] = { 64, 40, 16 };	//fond marron pour les zones de texte
	unsigned char fore[] = { 255, 255, 255 };	//texte en blanc

	std::string scoreJ1 = joueurs_[0].getNom() + " : " + std::to_string(0);	//on affiche les scores des joueurs en haut de la fenetre
	plateau_->draw_text(25, 20, scoreJ1.c_str(), fore, back, 1, 50);
	std::string scoreJ2 = joueurs_[1].getNom() + " : " + std::to_string(0);
	plateau_->draw_text(475, 20, scoreJ2.c_str(), fore, back, 1, 50);

	std::string info = "C'est au tour de " + joueurs_[0].getNom() + " de jouer"; //on affiche qui doit jouer en bas
	plateau_->draw_text(20, 400, info.c_str(), fore, back, 1, 50);

	for (int i = 0; i < 12; i++) { //on parcours chaque trou
		for (int j = 0; j < 4; j++) { //a l'initialisation du plateau, on sait qu'il y a 4 graines par trou
			int graineAleatoire = rand() % 4; //on choisit une graine parmi les 4
			switch (j) { //selon le numero de la graine dans le trou on la place a une position relative a celle du trou
			case 0:
				plateau_->draw_image(trous_[i].getColonne(), trous_[i].getLigne(), graines_[graineAleatoire], graines_[graineAleatoire].get_channel(3), 1, 255);
				break;
			case 1:
				plateau_->draw_image(trous_[i].getColonne() + 27, trous_[i].getLigne(), graines_[graineAleatoire], graines_[graineAleatoire].get_channel(3), 1, 255);
				break;
			case 2:
				plateau_->draw_image(trous_[i].getColonne() + 54, trous_[i].getLigne(), graines_[graineAleatoire], graines_[graineAleatoire].get_channel(3), 1, 255);
				break;
			case 3:
				plateau_->draw_image(trous_[i].getColonne(), trous_[i].getLigne() + 27, graines_[graineAleatoire], graines_[graineAleatoire].get_channel(3), 1, 255);
				break;
			default:
				break;
			}
		}
	}
}

int Plateau::recupererTrou(int x, int y) { //selon l'intervalle des coordonnees du clic, on peut trouver le trou dans lequel on a clique
	if (y > 120 && y < 240) {
		if (x > 0 && x < 120) {
			return 0;
		}
		else if(x > 120 && x < 240) {
			return 11;
		}
		else if (x > 240 && x < 360) {
			return 10;
		}
		else if (x > 360 && x < 480) {
			return 9;
		}
		else if (x > 480 && x < 600) {
			return 8;
		}
		else if (x > 600 && x < 720) {
			return 7;
		}
		else {
			return -1;
		}
	}
	else if (y > 240 && y < 360) {
		if (x > 0 && x < 120) {
			return 1;
		}
		else if (x > 120 && x < 240) {
			return 2;
		}
		else if (x > 240 && x < 360) {
			return 3;
		}
		else if (x > 360 && x < 480) {
			return 4;
		}
		else if (x > 480 && x < 600) {
			return 5;
		}
		else if (x > 600 && x < 720) {
			return 6;
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}
}

int Plateau::distribuer(int trouId, bool joueur) {
	if (joueur == trous_[trouId].getJoueur()) { //un joueur ne peut distribuer qu'a partir de chez lui
		int nbGrainesADistrib = trous_[trouId].getNbGraines(); //on recupere le nombre de graines dans le trou initial
		if (nbGrainesADistrib != 0) { //si ce trou n'est pas vide
			trous_[trouId].setNbGraines(0); //on vide le trou
			plateau_->clear(); //on reinitialise l'affichage
			int trouActuel = 0;
			for (int i = 1; i <= nbGrainesADistrib; i++) { //on parcours les trous suivants
				trouActuel = trouId + i;
				if (trouActuel > 11) {
					trouActuel -= 12;
				}
				trous_[trouActuel].ajouterGraine(); //on y ajoute une graine dans chaque
				rafraichir(joueur); //on met a jour l'affichage
			}
			return trouActuel; //on retourne le numero du trou dans lequel on a termine de distribuer
		}
		else {
			return -1;	//si on en peut pas distribuer on retourne un code d'erreur
		}
	}
	else {
		return -1;
	}
}

void Plateau::ramasser(int trouId, bool joueur) { //on commence dans le trou dans lequel on a fini de distribuer
	if (joueur != trous_[trouId].getJoueur()) { //on ne peut pas ramasser de graines chez soi
		if (!joueur && (trouId==0 || trouId>=7)) { //si on se trouve dans la partie superieure du plateau
			int trouActuel = trouId;
			if (trouActuel == 0) {
				trouActuel = 12;
			}
			for (int i = 0; i <= trouActuel - 7; i++) { //a partir du trou dans lequel on a finit de distribuer on remonte les trous
				int trouActuel2 = trouActuel - i;
				if (trouActuel2 == 12) {
					trouActuel2 = 0;
				}
				if (trous_[trouActuel2].getNbGraines() == 2 || trous_[trouActuel2].getNbGraines() == 3) { //on verifie les trous
					joueurs_[1].incrementerScore(trous_[trouActuel2].getNbGraines()); //et on recupere les graines si c'est possible
					trous_[trouActuel2].setNbGraines(0);
					rafraichir(joueur);
				}
				else {
					break;
				}
			}

		}
		else if(joueur && (trouId > 0 || trouId < 7)) {
			for (int i = 0; i <= trouId - 1; i++) {
				int trouActuel = trouId - i;
				if (trous_[trouActuel].getNbGraines() == 2 || trous_[trouActuel].getNbGraines() == 3) {
					joueurs_[0].incrementerScore(trous_[trouActuel].getNbGraines());
					trous_[trouActuel].setNbGraines(0);
				}
				else {
					break;
				}
			}
		}
	}
}

void Plateau::rafraichir(bool joueur) {

	plateau_->load("plateau_awale.ppm");
	plateau_->display(*fenetre_);	//on re-affiche le plateau

	unsigned char back[] = { 64, 40, 16 };	////on re-affiche les informations
	unsigned char fore[] = { 255, 255, 255 };
	std::string scoreJ1 = joueurs_[0].getNom() + " : " + std::to_string(joueurs_[0].getScore());
	plateau_->draw_text(25, 20, scoreJ1.c_str(), fore, back, 1, 50);
	std::string scoreJ2 = joueurs_[1].getNom() + " : " + std::to_string(joueurs_[1].getScore());
	plateau_->draw_text(475, 20, scoreJ2.c_str(), fore, back, 1, 50);
	std::string info;
	if (joueur) {
		info = "C'est au tour de " + joueurs_[1].getNom() + " de jouer";;
	}
	else {
		info = "C'est au tour de " + joueurs_[0].getNom() + " de jouer";
	}
	plateau_->draw_text(20, 400, info.c_str(), fore, back, 1, 50);

	for (int i = 0; i < 12; i++) {	//et on parcours a nouveau les trous pour dessiner les graines
		if (trous_[i].getNbGraines() <= 9) {
			for (int j = 0; j < trous_[i].getNbGraines(); j++) {
				int graineAleatoire = rand() % 4;
				switch (j) {
				case 0:
					plateau_->draw_image(trous_[i].getColonne(), trous_[i].getLigne(), graines_[graineAleatoire], graines_[graineAleatoire].get_channel(3), 1, 255);
					break;
				case 1:
					plateau_->draw_image(trous_[i].getColonne() + 27, trous_[i].getLigne(), graines_[graineAleatoire], graines_[graineAleatoire].get_channel(3), 1, 255);
					break;
				case 2:
					plateau_->draw_image(trous_[i].getColonne() + 54, trous_[i].getLigne(), graines_[graineAleatoire], graines_[graineAleatoire].get_channel(3), 1, 255);
					break;
				case 3:
					plateau_->draw_image(trous_[i].getColonne(), trous_[i].getLigne() + 27, graines_[graineAleatoire], graines_[graineAleatoire].get_channel(3), 1, 255);
					break;
				case 4:
					plateau_->draw_image(trous_[i].getColonne() + 27, trous_[i].getLigne() + 27, graines_[graineAleatoire], graines_[graineAleatoire].get_channel(3), 1, 255);
					break;
				case 5:
					plateau_->draw_image(trous_[i].getColonne() + 54, trous_[i].getLigne() + 27, graines_[graineAleatoire], graines_[graineAleatoire].get_channel(3), 1, 255);
					break;
				case 6:
					plateau_->draw_image(trous_[i].getColonne(), trous_[i].getLigne() + 54, graines_[graineAleatoire], graines_[graineAleatoire].get_channel(3), 1, 255);
					break;
				case 7:
					plateau_->draw_image(trous_[i].getColonne() + 27, trous_[i].getLigne() + 54, graines_[graineAleatoire], graines_[graineAleatoire].get_channel(3), 1, 255);
					break;
				case 8:
					plateau_->draw_image(trous_[i].getColonne() + 54, trous_[i].getLigne() + 54, graines_[graineAleatoire], graines_[graineAleatoire].get_channel(3), 1, 255);
					break;
				default:
					break;
				}
			}
		}
		else {
			std::string nb = std::to_string(trous_[i].getNbGraines());
			plateau_->draw_text(trous_[i].getColonne(), trous_[i].getLigne() + 20, nb.c_str(), fore, back, 1, 50);
		}
	}
	Sleep(20);
	plateau_->display(*fenetre_);
}

bool Plateau::partieTerminee() {
	if (joueurs_[0].getScore() >= 25 || joueurs_[1].getScore() >= 25) {
		return true;
	}
	else if(joueurs_[0].getScore() == 24 || joueurs_[1].getScore() == 24){
		return true;
	}
	else {
		return false;
	}
}

std::string Plateau::resultatPartie() {
	std::string resultat = "Partie terminee : \n Score final :          " + joueurs_[0].getNom() + ":" + std::to_string(joueurs_[0].getScore())
		+ "     " + joueurs_[1].getNom() + ":" + std::to_string(joueurs_[1].getScore()) + "\n";
	if (joueurs_[0].getScore() > joueurs_[1].getScore()) {
		resultat += " Vainqueur : " + joueurs_[0].getNom();
	}
	else if (joueurs_[0].getScore() < joueurs_[1].getScore()) {
		resultat += " Vainqueur : " + joueurs_[1].getNom();
	}
	else {
		resultat += "Match nul";
	}

	return resultat;
}

//a chaque coup, on va sauvegarder le trou joué, celui qui a joué, les informations des joueurs et les configurations des trous
void Plateau::sauver(std::ofstream &ofs, bool joueur, int trouJoue, int numCoup) {
	Joueur* joueurDuTour;
	if (joueur) {
		joueurDuTour = &joueurs_[0];
	}
	else {
		joueurDuTour = &joueurs_[1];
	}
	Coup* coup = new Coup(numCoup, joueurDuTour, &trous_[trouJoue]);
	listeCoups_.push_back(*coup);
	coup->sauver(ofs);
	
	joueurs_[0].sauver(ofs);
	joueurs_[1].sauver(ofs);

	trous_[0].sauver(ofs);
	trous_[1].sauver(ofs);
	trous_[2].sauver(ofs);
	trous_[3].sauver(ofs);
	trous_[4].sauver(ofs);
	trous_[5].sauver(ofs);
	trous_[6].sauver(ofs);
	trous_[7].sauver(ofs);
	trous_[8].sauver(ofs);
	trous_[9].sauver(ofs);
	trous_[10].sauver(ofs);
	trous_[11].sauver(ofs);
}

//pour charger une sauvegarde on suit le meme schema mais nous avons eu un probleme d'acces en lecture au fichier
Coup Plateau::charger(std::ifstream &ifs, cimg_library::CImgDisplay* fenetre, cimg_library::CImg<unsigned char>* plateau) {
	fenetre_ = fenetre;
	plateau_ = plateau;

	trous_[0] = Trou(true, 1, 144, 15);	//pour les trous on a choisi de les ranger dans l'ordre anti-horaire pour faciliter
	trous_[1] = Trou(false, 2, 266, 15); //la distribution de graines
	trous_[2] = Trou(false, 3, 266, 137);
	trous_[3] = Trou(false, 4, 266, 260);
	trous_[4] = Trou(false, 5, 266, 382);
	trous_[5] = Trou(false, 6, 266, 504);
	trous_[6] = Trou(false, 7, 266, 626);
	trous_[7] = Trou(true, 8, 144, 626);
	trous_[8] = Trou(true, 9, 144, 504);
	trous_[9] = Trou(true, 10, 144, 382);
	trous_[10] = Trou(true, 11, 144, 260);
	trous_[11] = Trou(true, 12, 144, 137);

	graines_[0] = cimg_library::CImg<unsigned char>("graine_1.png");
	graines_[1] = cimg_library::CImg<unsigned char>("graine_2.png");
	graines_[2] = cimg_library::CImg<unsigned char>("graine_3.png");
	graines_[3] = cimg_library::CImg<unsigned char>("graine_4.png");
	
	while(!ifs.eof()){
		
		Coup* coup = new Coup();
		coup->charger(ifs);
		if (coup->getTrou()->getNbGraines() <= 48) {
			listeCoups_.push_back(*coup);
		}

		joueurs_[0].charger(ifs);
		joueurs_[1].charger(ifs);

		trous_[0].charger(ifs);
		trous_[1].charger(ifs);
		trous_[2].charger(ifs);
		trous_[3].charger(ifs);
		trous_[4].charger(ifs);
		trous_[5].charger(ifs);
		trous_[6].charger(ifs);
		trous_[7].charger(ifs);
		trous_[8].charger(ifs);
		trous_[9].charger(ifs);
		trous_[10].charger(ifs);
		trous_[11].charger(ifs);
	}

	return listeCoups_.back();
}