// gisement.cc - v1
// Arthur CHANSEL - 324265
// Benoît VIGNON - 327737

#include "gisement.h"

using namespace std;

Gisement::Gisement(double x, double y, double r, double cap) {
	range_.setCentre(normalize(x), normalize(y));
	range_.setRayon(r);
	cap_ = cap;
}

Gisement::Gisement(istringstream &data, vector<Gisement*> &Eg, bool& pb_) {
	decodageGIS(data, Eg, pb_);
}

void Gisement::decodageGIS(istringstream &data, vector<Gisement*> &Eg, bool& pb_) {
	double x(0), y(0), r(0), cap(0);
	
	data>>x>>y>>r>>cap;
	range_.setCentre(normalize(x), normalize(y));
	range_.setRayon(r);
	cap_ = cap;
	if(!superpose_GG(Eg, pb_)) Eg.push_back( this );
}

bool Gisement::superpose_GG(vector<Gisement*> &Eg, bool& pb_) const {
	for (unsigned i(0); i < Eg.size(); i++){
		if (inter_cercles(Eg[i]->getRange(), range_) and Eg[i]!=this){
			double x1(range_.getCentre().getX());
			double y1(range_.getCentre().getY());
			double x2(Eg[i]->getRange().getCentre().getX());
			double y2(Eg[i]->getRange().getCentre().getY());
			cout << message::field_superposition(x1,y1,x2,y2);
			pb_ = 1;
			return 1;
		}
	}																			
	return 0;
}

void Gisement::extract(){
	if (cap_ > deltaR) cap_ -= deltaR;
	else cap_ = 0;
}
