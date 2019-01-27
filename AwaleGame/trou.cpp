#include<fstream>
#include"trou.h"
#include"joueur.h"

Trou::Trou(bool joueur, unsigned int numero, unsigned int ligne, unsigned int colonne, unsigned int nbGraines): joueur_(joueur), numero_(numero), nbGraines_(nbGraines), ligne_(ligne), colonne_(colonne){}

Trou::Trou(Trou* hole) {
	numero_ = hole->getNumero();
	joueur_ = hole->getJoueur();
	nbGraines_ = hole->getNbGraines();
}

void Trou::sauver(std::ofstream &ofs) {
	ofs << numero_ << std::endl;
	ofs << joueur_ << std::endl;
	ofs << nbGraines_ << std::endl;
}

void Trou::charger(std::ifstream &ifs) {
	ifs >> numero_;
	ifs >> joueur_;
	ifs >> nbGraines_;
}