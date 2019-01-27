#include<fstream>
#include<string>
#include"joueur.h"

Joueur::Joueur(bool num, std::string nom, unsigned int score):nom_(nom), numero_(num), score_(score){}

Joueur::Joueur(Joueur* player) {
	numero_ = player->getNum();
	nom_ = player->getNom();
	score_ = player->getScore();
}

void Joueur::sauver(std::ofstream &ofs) {
	ofs << nom_ << std::endl;
	ofs << numero_ << std::endl;
	ofs << score_ << std::endl;
}

void Joueur::charger(std::ifstream &ifs) {
	ifs >> nom_;
	ifs >> numero_;
	ifs >> score_;
}