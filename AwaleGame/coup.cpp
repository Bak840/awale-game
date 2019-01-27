#include<fstream>
#include"coup.h"
#include"CImg.h"
#include"trou.h"
#include"joueur.h"

Coup::Coup(unsigned int num, Joueur* joueur, Trou* trouJoue):numero_(num), joueur_(joueur), trouJoue_(trouJoue){}

void Coup::sauver(std::ofstream &ofs) {
	ofs << numero_ << std::endl;
	joueur_->sauver(ofs);
	trouJoue_->sauver(ofs);
}

void Coup::charger(std::ifstream &ifs) {
	ifs >> numero_;
	
	Joueur* joueur = new Joueur;
	joueur->charger(ifs);
	joueur_ = joueur;

	Trou* trou = new Trou;
	trou->charger(ifs);
	trouJoue_ = trou;
}