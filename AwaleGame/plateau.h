#pragma once
#include<fstream>
#include<list>
#include"CImg.h"
#include"joueur.h"
#include"trou.h"
#include"coup.h"

class Plateau {	// réalisée par Bakoura IBRAHIMA
	cimg_library::CImgDisplay* fenetre_; //fenetre Cimg
	cimg_library::CImg<unsigned char>* plateau_; //image du plateau d'Awale
	std::list<Coup> listeCoups_;
	Joueur joueurs_[2]; //tableau contenant les informations des 2 joueurs
	Trou trous_[12]; //tableau contenant les informations des 12 trous
	cimg_library::CImg<unsigned char> graines_[4];	//tableau de graines

public:
	Plateau(cimg_library::CImgDisplay* fenetre, cimg_library::CImg<unsigned char>* plateau, std::string nom1, std::string nom2);
	Plateau() {};

	int recupererTrou(int x, int y);
	int distribuer(int trouId, bool joueur);
	void ramasser(int trouId, bool joueur);
	void rafraichir(bool joueur);
	bool partieTerminee();
	std::string resultatPartie();

	void sauver(std::ofstream &ofs, bool joueur, int trouJoue, int numCoup);
	Coup charger(std::ifstream &ifs, cimg_library::CImgDisplay* fenetre, cimg_library::CImg<unsigned char>* plateau);
};
