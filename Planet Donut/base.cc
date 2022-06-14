// base.cc - v1
// Arthur CHANSEL - 324265
// Benoît VIGNON - 327737

#include "base.h"
using namespace std;

Base::Base(double x, double y)
: stock_(iniR), fin_(0), tick_(0), etat_urgence_(0)
{
	range_.setCentre(normalize(x), normalize(y));
	range_.setRayon(rayon_base);
}

Base::Base(istringstream &data,vector<Base*> &Eb,vector<Gisement*> &Eg,
		   int ct, bool& pb_)
: stock_(iniR), color_(ct%6)
{
	decodageBASE(data, Eb, Eg, pb_);
}

void Base::decodageBASE(istringstream &data, vector<Base*> &Eb,
						vector<Gisement*> &Eg, bool& pb_) {
	double x(0), y(0);
	data>>x>>y>>stock_>>nbP_>>nbF_>>nbT_>>nbC_;
	range_.setCentre(normalize(x), normalize(y));
	range_.setRayon(rayon_base);
	if (stock_ <= 400) etat_urgence_ = 1;
	etat_urgence_ = 1;
	if(!superpose_BB(Eb, pb_))
		if(!(superpose_BG(Eg, pb_))) Eb.push_back( this );
}

void Base::createPRO(istringstream &data, bool& pb_){ 
	new Prospection(data, Er_, range_.getCentre(), pb_);
}

void Base::createFOR(istringstream &data, bool& pb_){
	new Foreur(data, Er_, range_.getCentre(), pb_);
	}

void Base::createTRA(istringstream &data, bool& pb_){
	new Transport(data, Er_, range_.getCentre(), pb_);
	}

void Base::createCOM(istringstream &data, bool& pb_){
	new Comm(data, Er_, range_.getCentre(), pb_);
}

void Base::createPRO(int uid){
	new Prospection(uid, range_.getCentre().getX(),
					range_.getCentre().getY(),range_.getCentre().getX(),
					range_.getCentre().getY(), Er_, color_);
}

void Base::createFOR(int uid){
	new Foreur(uid,range_.getCentre().getX(),range_.getCentre().getY(),
			   range_.getCentre().getX(),range_.getCentre().getY(),Er_,
			   color_);
}

void Base::createTRA(int uid){
	new Transport(uid,range_.getCentre().getX(),
				  range_.getCentre().getY(),range_.getCentre().getX(),
				  range_.getCentre().getY(), Er_, color_);
}

void Base::createCOM(int uid){
	new Comm(uid, range_.getCentre().getX(), range_.getCentre().getY(),
			 range_.getCentre().getX(), range_.getCentre().getY(), Er_,
			 color_);
}

bool Base::superpose_BG(const vector<Gisement*> &Eg, bool& pb_) const {
	double xb(range_.getCentre().getX());
	double yb(range_.getCentre().getY());
	for (unsigned i(0); i < Eg.size(); i++){
		double xg(Eg[i]->getRange().getCentre().getX());
		double yg(Eg[i]->getRange().getCentre().getY());
		if (inter_cercles(Eg[i]->getRange(), range_)){
			cout << message::base_field_superposition(xb,yb,xg,yg);
			pb_ = 1;
			return 1;
		}
	}
	return 0;
}

bool Base::superpose_BB(const vector<Base*> &Eb, bool& pb_) const {
	double x1(range_.getCentre().getX());
	double y1(range_.getCentre().getY());
	for (unsigned i(0); i < Eb.size(); i++){
		double x2(Eb[i]->getRange().getCentre().getX());
		double y2(Eb[i]->getRange().getCentre().getY());
		if(Eb[i]!=this){
			if (inter_cercles(Eb[i]->getRange(), range_)){
				cout << message::base_superposition(x1,y1,x2,y2);
				pb_ = 1;
				return 1;
			}
		}
	}
	return 0;
}

void Base::reset(){
	Eg_known_.clear();
	Reseau_tot_.clear();
	tick_++;
	for (unsigned int i(0); i < Er_.size(); i++){	
		Er_[i]->reset();
	}
}

int Base::find_depart (){
	for (unsigned i(0); i < Er_.size(); i++)
		if (egalite_pts(Er_[i]->getRange().getCentre(),
			range_.getCentre())) return i;
	return -1;
}

void Base::repair(unsigned int i){
	if ((egalite_pts(Er_[i]->getRange().getCentre(),range_.getCentre()))
		and (Er_[i]->arrivee())) {
		stock_ -= (Er_[i]->getDp() * cost_repair);
		Er_[i]->setDp(0);
		Er_[i]->setDmd(1);
	}	
}

void Base::dec_but(unsigned int i){
	switch(Er_[i]->getType()){
	case 'P':
		dec_p(Er_[i]);
		break;
	case 'F':
		dec_f(Er_[i]);
		break;
	case 'T':
		dec_t(Er_[i]);
		break;
	case 'C':
		dec_c(Er_[i]);
		break;
	}
}

void Base::dec_p(Robot* rob){
	Vecteur v;
	rob->setDmd(0);
	if (pt_P_.empty()) points_P();
	
	double dist_base (new_vect(rob->getRange().getCentre(),
					  range_.getCentre(), v));
	double dist_PT_Base (new_vect(pt_P_[0], range_.getCentre(), v));
	double dist_PT_Rob (new_vect(pt_P_[0],rob->getRange().getCentre(),
						v));
	double dist_nextPT(dist_PT_Rob+dist_PT_Base);
	if (((maxD_prosp - rob->getDp()) > dist_base)
		and ((maxD_prosp - rob->getDp()) < dist_nextPT)){
		rob->setBut(range_.getCentre());
	} else {
		normalize(pt_P_.back());
		rob->setBut(pt_P_.back());
		pt_P_.pop_back();
	}
}

void Base::dec_f(Robot* rob){
	int best_gis(0);
	rob->setDmd(0);
	for (unsigned int i (0); i < Eg_known_.size(); i++){
		if (Eg_known_[i].note > Eg_known_[best_gis].note) best_gis = i;
	} 	
	double xb (range_.getCentre().getX());
	double yb (range_.getCentre().getY());
	double xg (Eg_known_[best_gis].pos.getX());
	double yg (Eg_known_[best_gis].pos.getY());
	double r (Eg_known_[best_gis].rayg);
	double theta (atan((yg-yb)/(xg-xb)));
	if (xg > xb) theta += M_PI;
	Point but(xg+r*cos(theta), yg+r*sin(theta));
	normalize(but);
	rob->setBut(but);
	Eg_known_[best_gis].note -= 3; 
}

void Base::dec_t(Robot* rob){
	Transport *tran = static_cast<Transport*>(rob);
	if(tran->getCharge()){
		tran->setBut(range_.getCentre());
		tran->setDmd(0);
	}else if(!(tran->getNb_aco())){
		for (unsigned int i(0); i<Er_.size(); i++){
			if (Er_[i]->getType() == 'F'){
				Foreur *foreur = static_cast<Foreur*>(Er_[i]);
				if ((!(foreur->getDmd()))and(foreur->getAco().size()<=5)
					and (!tran->getNb_aco()) and (!foreur->getFixe())){ 
					foreur->ajtAco(rob);
					tran->setAco(foreur);
					tran->setNbAco(1);
					tran->setBut(foreur->getBut());
					tran->setDmd(0);
				}
			}
		}
	} else {
		rob->setBut(range_.getCentre());
	}
}

void Base::dec_c(Robot* rob){
	if (egalite_pts(rob->getBut(),range_.getCentre())){
		rob->setFixe(1);
		rob->setDmd(0);
	} else {
		rob->setDmd(0);
		if (pt_C_.empty()){points_C();}
		normalize(pt_C_[0]);
		rob->setBut(pt_C_[0]);
		pt_C_.erase(pt_C_.begin());
	}
}

void Base::update_gis(unsigned int i){
	Prospection *prosp = static_cast<Prospection*>(Er_[i]);
	if (prosp->getFound()) {
		data_gis gis = {prosp->getPosGis(), prosp->getRayG(),
						noter(prosp->getGisX(),prosp->getGisY(),
						prosp->getCapG(), prosp->getRayG())};
		Eg_known_.push_back(gis);
		prosp->erase();
	}
}
		
void Base::points_P(){
	static int n (0);
	int r (rayon_base + (2*n+1)*rayon_comm + 2*rayon_min);
	double peri (2*M_PI*r);
	double nb_pts (floor(peri/(2*rayon_comm))+1);
	for (unsigned int i(0); i<nb_pts; i++){
		Point pt ((range_.getCentre().getX()+r*cos(2*M_PI*i/nb_pts)),
				  (range_.getCentre().getY()+r*sin(2*M_PI*i/nb_pts)));
		pt_P_.push_back(pt);
	}
	n++;
}	

void Base::points_C(){
	static int n (1);
	int r (n*rayon_comm*sqrt(3));
	double nb_pts (6*n);
	
	for (unsigned int i(0); i<nb_pts; i++){
		Point pt ((range_.getCentre().getX()+r*cos(2*M_PI*i/nb_pts)),
				  (range_.getCentre().getY()+r*sin(2*M_PI*i/nb_pts)));
		pt_C_.push_back(pt);
	}
	n++;
}	

double Base::noter(double x, double y, double cap, double ray){
	double cap_max (1000.*(rayon_max/rayon_min)*(rayon_max/rayon_min));
	double note_cap (cap*10/cap_max);
	int coef (0);
	Vecteur v;
	Point pt (x, y);
	Cercle c(x, y, ray);
	double dist (new_vect(range_.getCentre(), pt, v));
	double note_dist = 10 - (dist*10/(1.414*dim_max));
	for (unsigned int i(0); i<Er_.size(); i++)
		if(pt_cercle(Er_[i]->getBut(), c)and(Er_[i]->getType() == 'F')){
			coef++;
		}
	return ((note_cap+note_dist)/2 - 4*coef);
}

void Base::discharge(unsigned int i){
	Transport *tran = static_cast<Transport*>(Er_[i]);
	if((egalite_pts(Er_[i]->getRange().getCentre(), range_.getCentre()))
	   and (egalite_pts(Er_[i]->getBut(), range_.getCentre()))){
		stock_ += tran->getCharge();
		tran->work();
		if (stock_ >= finR){
			fin_ = 1;
		}
	}
	tran->fin_gis();
}

void Base::creer_rob(){
	int rob_ct (0);
	if (stock_ >= 50){
		if (Eg_known_.size() > 0){
			for (int i(0); i<worth(); i++){
				if ((alive(cost_forage)) and (rob_ct < 3)){
					createFOR(Er_.size());
					stock_ -= cost_forage;
					rob_ct ++;
					dec_f(Er_.back());
					Er_.back()->act();
					nbF_++;
				}
				if ((besoin_t()) and (nbT_<12)){
					if ((alive(cost_transp)) and (rob_ct < 3)){
						createTRA(Er_.size());
						stock_ -= cost_transp;
						rob_ct ++;
						dec_t(Er_.back());
						Er_.back()->act();
						nbT_++;
					}
				}
			}
		}
		if ((tick_%(150+(150*etat_urgence_)) == 0) and (rob_ct < 3)
			and (nbP_ < (10-3*etat_urgence_))){
			alive(cost_prosp);
			createPRO(Er_.size()); 
			stock_ -= cost_prosp; 
			rob_ct ++;
			dec_p(Er_.back());
			Er_.back()->act();
			nbP_++;
		}
		if ((tick_%(50+(50*etat_urgence_)) == 0) and (rob_ct < 3)
			and (nbC_<(15-5*etat_urgence_))){
			alive(cost_com);
			createCOM(Er_.size());
			stock_ -= cost_com;
			rob_ct ++;
			dec_c(Er_.back());
			Er_.back()->act();
			nbC_++;
		}
	}
}

bool Base::alive(double cost){
	if (stock_<= 300-cost){ 
		etat_urgence_ = 1;
		return 0;
	}
	etat_urgence_ = 0;
	return 1;
}

bool Base::besoin_t(){
	for (unsigned int i(0); i < Er_.size(); i++)
		if ((Er_[i]->getType() == 'T') and (Er_[i]->getDmd())) return 0;
	return 1;	
}

bool Base::salvation(){
	for (unsigned int i(0); i < Er_.size(); i++){
		if ((Er_[i]->getType() == 'T') and (Er_[i]->getLink())){
			Transport *tran = static_cast<Transport*>(Er_[i]);
			if (tran->getCharge()){
				return 1;
			}
		}
	}
	return 0;
}

int Base::worth(){
	int seuil (2);
	if (tick_ > 5000) seuil = 0;
	if (stock_ > 4000) seuil = 0;
	int ct(0);
	for (unsigned int i(0); i<Eg_known_.size(); i++)
		if (Eg_known_[i].note > seuil) ct++;
		
	return ct;
}

void Base::connexion(int index){
	Er_[index]->setLink(1);
	Reseau_tot_.push_back(Er_[index]);
	for (unsigned int i(0); i<Er_.size(); i++){
		if((inter_cercles(Er_[index]->getRange(), Er_[i]->getRange()))
		   and (!(Er_[i]->getLink()))){
			connexion(i);	
		}
	}
}

Base::~Base(){
	for (unsigned int i(0); i<Er_.size(); i++){
		delete Er_[i];
		Er_[i] = nullptr;
	}
}

