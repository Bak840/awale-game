#pragma once
#include<fstream>
#include<string>

class Joueur { // réalisée par Khadimou DIOP
	bool numero_;
	std::string nom_;
	unsigned int score_;
public:
	Joueur(bool num, std::string nom, unsigned int score = 0);
	Joueur(Joueur* joueur);
	Joueur(){}

	bool getNum() { return numero_; }
	std::string getNom() { return nom_; }
	unsigned int getScore() { return score_; }

	void incrementerScore(unsigned int score) { score_ += score; }

	void sauver(std::ofstream &ofs);
	void charger(std::ifstream &ifs);
};
