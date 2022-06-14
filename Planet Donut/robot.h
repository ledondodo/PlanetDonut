// robot.h - v1
// Arthur CHANSEL - 324265
// Benoît VIGNON - 327737

#ifndef ROBOT_H
#define ROBOT_H

#include "geomod.h"
#include "gisement.h"
#include "message.h"
#include "constantes.h"

void lire_bool(std::string txt, bool&b);

class Robot{
public:
	Robot () = default;
	Robot(double uid, double x, double y, double xp, double yp,
		  std::vector<Robot*>&Er, int color);
	virtual ~Robot();
	
	double getUid() const { return uid_; }
	bool getLink() const {return linked_;}
	Cercle getRange() const {return range_;}
	Point getCentre() const {return range_.getCentre();}
	double getDp() const {return dp_;}
	Point getBut() const {return but_;}
	char getType() const {return type_;}
	bool getDmd() const {return dmd_but_;}
	std::vector<Robot*> getReseau() const {return reseau_;}
	bool getFixe() const {return fixe_;}
	
	void new_uid(const std::vector<Robot*> &Er);
	bool same_uid(const std::vector<Robot*> &Er, bool& pb_) const;
	void mort();
	void deplacement();
	void setPause(bool a);
	void setLink (bool a);
	void setDp (double a);
	void setBut (Point pt);
	void setDmd (bool a);
	void setFixe(bool a);
	bool arrivee();
	void act();
	virtual void work() = 0;
	void ajt(Robot* rob);
	void reset();
	bool base() const;

protected:
	double uid_;
	Point but_;
	Cercle range_;
	double dp_;
	double cost_crea_;
	double dist_max_;
	bool atteint_;
	bool fixe_;
	bool linked_;
	bool pause_;
	char type_;
	bool dmd_but_;
	Point OG_;
	std::vector<Robot*> reseau_;
	int color_;
};

class Prospection : public Robot {
public:
	Prospection(double uid, double x, double y, double xp, double yp,
				std::vector<Robot*> &Er, int color);
	Prospection(std::istringstream &data, std::vector<Robot*> &Er,
				Point OG, bool& pb_);
	~Prospection() override;
	void decodagePRO(std::istringstream &data, std::vector<Robot*> &Er,
					 Point OG, bool& pb_);
	bool getFound() const {return found_;}
	void setFound(bool a);
	double getGisX() const {return pos_gis_.getX();}
	double getGisY() const {return pos_gis_.getY();}
	Point getPosGis() const {return pos_gis_;}
	double getRayG() const {return rayong_;}
	double getCapG() const {return capaciteg_;}
	bool getRetour() const {return retour_;}
	void erase();
	void detection (Gisement* gis);
	void work() override;
	
private:
	bool retour_;
	bool found_;
	Point pos_gis_;
	double rayong_;
	double capaciteg_;
};

class Foreur : public Robot {
public:
	Foreur(double uid, double x, double y, double xp, double yp,
		   std::vector<Robot*> &Er, int color);
	Foreur(std::istringstream &data, std::vector<Robot*> &Er, Point OG, bool& pb_);
	~Foreur() override;
	void decodageFOR(std::istringstream &data, std::vector<Robot*> &Er,
					 Point OG, bool& pb_);
	void setGis(Gisement* gis);
	void work() override;
	std::vector<Robot*> getAco() const {return acolyte_;}
	void ajtAco(Robot* tran);
	void rmvAco(Robot* tran);
	bool detection(Gisement* gis);
	Gisement* getGisVis() const {return gis_vis_;}
	bool getDmdT();
	
private: 
	Gisement* gis_vis_;
	std::vector<Robot*> acolyte_;	
};

class Transport : public Robot {
public: 
	Transport(double uid, double x, double y, double xp, double yp,
			  std::vector<Robot*> &Er, int color);
	Transport(std::istringstream &data, std::vector<Robot*> &Er,
			  Point OG, bool& pb_);
	~Transport() override;
	void decodageTRA(std::istringstream &data, std::vector<Robot*> &Er,
					 Point OG, bool& pb_);
	
	void work() override;
	void setAco(Robot* foreur); 
	Robot* getAco() const {return acolyte_;}
	bool getNb_aco() const {return aco_;}
	int getCharge() const{return charge_;}
	bool getRetour() const{return retour_;}
	void setNbAco(bool a);
	void fin_gis();
	
private:
	bool retour_;
	Robot* acolyte_;
	int charge_;
	bool aco_;
};

class Comm : public Robot {
public:
	Comm(double uid, double x, double y, double xp, double yp,
		 std::vector<Robot*> &Er, int color);
	Comm(std::istringstream &data, std::vector<Robot*> &Er, Point OG, bool& pb_);
	~Comm() override;
	void decodageCOM(std::istringstream &data, std::vector<Robot*> &Er,
					 Point OG, bool& pb_);
	void work() override;
};

#endif

