// simulation.h - v1
// Arthur CHANSEL - 324265
// Benoît VIGNON - 327737

#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <fstream>
#include <sstream>

#include "base.h"
#include "gisement.h"
#include "geomod.h"

class Simulation {
public:
	Simulation() = default;
	void clear_sim();
	void init_sim (std::string f);
	bool test_comm(int a);
	void lecture(std::string nom_fichier);
	void lire_bool(std::string txt, bool&b);
	void decodageNBG(std::istringstream &data);
	void decodageNBB(std::istringstream &data);
	void newGIS(std::istringstream &data);
	void newBASE(std::istringstream &data);
	void newPRO(std::istringstream &data);
	void newFOR(std::istringstream &data);
	void newTRA(std::istringstream &data);
	void newCOM(std::istringstream &data);
	void write_save(std::string f);
	std::string write_bool(bool a);
	void write_gis(std::ofstream &fichier);
	void write_base(std::ofstream &fichier);
	void write_pro(std::ofstream &fichier, unsigned int i, int nbP);
	void write_for(std::ofstream &fichier, unsigned int i, int nbF);
	void write_tra(std::ofstream &fichier, unsigned int i, int nbT);
	void write_comm(std::ofstream &fichier, unsigned int i, int nbC);
	void decodage(std::string line);
	bool getPb() const {return pb_;}
	
	// Update simulation
	void update_sim();
	void reset_base();
	void update_base(Base* base);
	void update_links(int i);
	void update_voisin(Base* b1, Base* b2);
	void update_detection(int i);
	void delete_base(int i);
	void delete_gis();
	
	void draw_gis();
	void draw_base(bool link, bool range);
	std::vector<Base*> const getEb() {return Eb_;}
	std::vector<Gisement*> const getEg() {return Eg_;}
	
private:
	std::string fi_;
	std::vector<Base*> Eb_;
	std::vector<Gisement*> Eg_;
	int etat_;
	int nbG_;
	int nbB_;
	int i_;
	int j_;
	int tick_;
	bool pb_;
};

#endif

