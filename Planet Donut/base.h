// base.h - v1
// Arthur CHANSEL - 324265
// Benoît VIGNON - 327737

#ifndef BASE_H
#define BASE_H

#include "geomod.h"
#include "robot.h"
#include "gisement.h"
#include "message.h"
#include "constantes.h"
#include <cmath>

struct data_gis {
	Point pos;
	double rayg;
	double note;	
};

class Base {
public:
	Base(double x=0, double y=0);
	Base(std::istringstream &data, std::vector<Base*> &Eb,
		 std::vector<Gisement*> &Eg, int ct, bool& pb_);
	~Base();
	
	Cercle getRange() const { return range_;}
	Point getCentre() const {return range_.getCentre();}
	int getnbP() const { return nbP_; }
	int getnbF() const { return nbF_; }
	int getnbT() const { return nbT_; }
	int getnbC() const { return nbC_; }
	std::vector<Robot*> getEr() const { return Er_; }
	double getStock() const { return stock_; }
	int getColor() const { return color_; }
	std::vector<Robot*> getReseauTot() const { return Reseau_tot_; }
	
	void decodageBASE(std::istringstream &data, std::vector<Base*> &Eb,
					  std::vector<Gisement*> &Eg, bool& pb_);
	void createPRO(std::istringstream &data, bool& pb_);
	void createFOR(std::istringstream &data, bool& pb_);
	void createTRA(std::istringstream &data, bool& pb_);
	void createCOM(std::istringstream &data, bool& pb_);
	void createPRO(int uid);
	void createFOR(int uid);
	void createTRA(int uid);
	void createCOM(int uid);
	
	bool superpose_BG(const std::vector<Gisement*> &Eg, bool& pb_) const;
	bool superpose_BB(const std::vector<Base*> &Eb, bool& pb_) const;
	
	void reset();
	void repair(unsigned int i);
	void discharge(unsigned int i);
	void dec_but(unsigned int i);
	void update_gis(unsigned int i);
	void creer_rob();
	void points_P();
	void points_C();
	double noter(double x, double y, double cap, double ray);
	bool alive(double cost);
	bool besoin_t();
	bool salvation();
	int worth();
	
	void dec_p (Robot* rob);
	void dec_f (Robot* rob);
	void dec_t (Robot* rob);
	void dec_c (Robot* rob);
	
	int find_index (int uid);
	int find_depart ();
	void parcours_DFS(unsigned depart);
	void rec_DFS (std::vector<bool>& visited, unsigned index);
	bool getFin() const {return fin_;}
	void connexion(int index);
	
private:
	Cercle range_;
	double stock_;
	int nbP_;
	int nbF_;
	int nbT_;
	int nbC_;
	bool fin_;
	int tick_;
	bool etat_urgence_;
	int color_;
	
	std::vector<Robot*> Er_;
	std::vector<data_gis> Eg_known_;
	std::vector<Point> pt_P_;
	std::vector<Point> pt_C_;
	std::vector<Robot*> Reseau_tot_;
};

#endif
