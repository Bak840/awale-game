#pragma once
#include"CImg.h"
#include"trou.h"
#include"joueur.h"
#include<fstream>

class Coup { // réalisée par Khadimou DIOP
	unsigned int numero_;
	Trou* trouJoue_;
	Joueur* joueur_;
public:
	Coup(unsigned int num, Joueur* joueur, Trou* troueJoue);
	Coup(){}

	void setTrou(Trou* trouJoue) { trouJoue_ = trouJoue; }
	Trou* getTrou() { return trouJoue_; }
	unsigned int getNum() { return numero_; }
	Joueur* getJoueur() { return joueur_; }

	void sauver(std::ofstream &ofs);
	void charger(std::ifstream &ifs);
};
