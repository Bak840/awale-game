#pragma once
#include<fstream>
#include"joueur.h"

class Trou { // réalisée par Bakoura IBRAHIMA
	bool joueur_; //indique a qui appartient le trou, true pour le joueur 1, false pour le joueur 2
	unsigned int numero_;
	unsigned int nbGraines_; //nombre de graines dans le trou
	unsigned int ligne_;	//coordonnees du trou
	unsigned int colonne_;
public:
	Trou(bool joueur, unsigned int numero, unsigned int ligne, unsigned int colonne, unsigned int nbGraines = 4);
	Trou(Trou* trou);
	Trou(){}

	bool getJoueur() { return joueur_; }
	unsigned int getNumero() { return numero_; }
	unsigned int getLigne() { return ligne_; }
	unsigned int getColonne() { return colonne_; }
	unsigned int getNbGraines() { return nbGraines_; }
	void ajouterGraine() { nbGraines_++; }
	void setNbGraines(unsigned int nbGraines) { nbGraines_ = nbGraines; }

	void sauver(std::ofstream &ofs);
	void charger(std::ifstream &ifs);
};
