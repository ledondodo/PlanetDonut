// gisement.h - v1
// Arthur CHANSEL - 324265
// Benoît VIGNON - 327737

#ifndef GISEMENT_H
#define GISEMENT_H

#include "geomod.h"
#include "message.h"
#include "constantes.h"

#include <vector>
#include <iostream>
#include <sstream>


class Gisement {
public:
	Gisement(double x=0, double y=0, double r=1, double cap=0);
	Gisement(std::istringstream &data, std::vector<Gisement*> &Eg, bool& pb_);
	void decodageGIS(std::istringstream &data,
					 std::vector<Gisement*> &Eg, bool& pb_);
	Cercle getRange() const { return range_; }
	Point getCentre() const {return range_.getCentre();}
	double getCap() const { return cap_; }
	
	bool superpose_GG(std::vector<Gisement*> &Eg, bool& pb_) const; 
	
	void extract();
	
private:
	Cercle range_;
	double cap_;
};

#endif
