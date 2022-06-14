// robot.cc - v1
// Arthur CHANSEL - 324265
// Benoît VIGNON - 327737

#include "robot.h"
using namespace std;

void lire_bool(string txt, bool&b) {
	if(txt=="true") b=1;
	else if(txt=="false") b=0;
}

bool Robot::same_uid(const vector<Robot*> &Er, bool& pb_) const {
	for(unsigned int i(0) ; i<Er.size() ; ++i){
		if(uid_==Er[i]->uid_ and Er[i]!=this){
			cout << message::identical_robot_uid(uid_);
			pb_ = 1;
			return 1;
		}
	}
	return 0;
}

Robot::Robot (double uid, double x, double y, double xp, double yp,
			  vector<Robot*> &Er, int color)
: uid_(uid), but_(x,y), range_(xp, yp, rayon_comm), dp_(0), atteint_(0),
fixe_(0), linked_(0), pause_(0), dmd_but_(1), color_(color)
{
	new_uid(Er);
	Er.push_back(this);
}

void Robot::new_uid(const vector<Robot*> &Er){
	int max_uid(0);
	for(unsigned int i(0);i<Er.size();i++)
		if(Er[i]->uid_>=max_uid) max_uid=Er[i]->uid_+1;
	uid_=max_uid;
}

Prospection::Prospection(double uid, double x, double y, double xp,
						 double yp, vector<Robot*> &Er, int color)
: Robot (uid, x,y, xp, yp, Er, color), retour_(0), found_(0)
{
	cost_crea_ = cost_prosp;
	dist_max_ = maxD_prosp;
	type_ = 'P';
	OG_ = Point (xp, yp);
	fixe_ = 0;
	dmd_but_ = 1;
}

Prospection::Prospection(istringstream &data, vector<Robot*> &Er,
						 Point OG, bool& pb_) {
	decodagePRO(data, Er, OG, pb_);
}

void Prospection::decodagePRO(istringstream &data, vector<Robot*> &Er,
							  Point OG, bool& pb_) {
	double x(0), y(0), xb(0), yb(0), xg(0), yg(0);
	string a, b, c;
	
	data>>uid_>>dp_>>x>>y>>xb>>yb>>a>>b>>c;
	lire_bool(a,atteint_);
	lire_bool(b,retour_);
	lire_bool(c,found_);
	
	if(found_) data>>xg>>yg>>rayong_>>capaciteg_;
	
	range_.setCentre(normalize(x),normalize(y));
	range_.setRayon(rayon_comm);
	but_.setX(normalize(xb));
	but_.setY(normalize(yb));
	pos_gis_.setX(normalize(xg));
	pos_gis_.setY(normalize(yg));
	cost_crea_ = cost_prosp;
	dist_max_ = maxD_prosp;
	type_ = 'P';
	OG_ = OG;
	fixe_ = 0;
	dmd_but_ = 0;
	
	if(!same_uid(Er, pb_)) Er.push_back( this );
}

Foreur::Foreur(double uid, double x, double y, double xp, double yp,
			   vector<Robot*> &Er, int color)
: Robot (uid, x,y, xp, yp, Er, color), gis_vis_(nullptr)
{
	cost_crea_ = cost_forage;
	dist_max_ = maxD_forage;
	type_ = 'F';
	OG_ = Point (xp, yp);
	fixe_ = 0;
	dmd_but_ = 1;
}

Foreur::Foreur(istringstream &data, vector<Robot*> &Er, Point OG, bool& pb_) {
	decodageFOR(data, Er, OG, pb_);
}

void Foreur::decodageFOR(istringstream &data, vector<Robot*> &Er,
						 Point OG, bool& pb_) {
	double x(0), y(0), xb(0), yb(0);
	string a;
	
	data>>uid_>>dp_>>x>>y>>xb>>yb>>a;
	
	lire_bool(a,atteint_);
	range_.setCentre(normalize(x),normalize(y));
	range_.setRayon(rayon_comm);
	but_.setX(normalize(xb));
	but_.setY(normalize(yb));
	cost_crea_ = cost_forage;
	dist_max_ = maxD_forage;
	type_ = 'F';
	OG_ = OG;
	gis_vis_=nullptr;
	fixe_ = 0;
	dmd_but_ = 0;
	
	if(!same_uid(Er, pb_)) Er.push_back( this );
}


Transport::Transport (double uid, double x, double y, double xp,
					  double yp, vector<Robot*> &Er, int color)
: Robot (uid, x,y, xp, yp, Er, color), retour_(0), acolyte_(nullptr),
charge_(0), aco_(0)
{
	cost_crea_ = cost_transp;
	dist_max_ = maxD_transp;
	type_ = 'T';
	OG_ = Point (xp, yp);
	fixe_ = 0;
	dmd_but_ = 1;
}

Transport::Transport(istringstream &data, vector<Robot*> &Er, Point OG, bool& pb_){
	decodageTRA(data,Er, OG, pb_);
}

void Transport::decodageTRA(istringstream &data, vector<Robot*> &Er,
							Point OG, bool& pb_) {
	double x(0), y(0), xb(0), yb(0);
	string a, b;
	
	data>>uid_>>dp_>>x>>y>>xb>>yb>>a>>b;
	
	lire_bool(a,atteint_);
	lire_bool(b,retour_);
	range_.setCentre(normalize(x),normalize(y));
	range_.setRayon(rayon_comm);
	but_.setX(normalize(xb));
	but_.setY(normalize(yb));
	cost_crea_ = cost_transp;
	dist_max_ = maxD_transp;
	type_ = 'T';
	OG_ = OG;
	aco_ = 0;
	acolyte_ = nullptr;
	fixe_ = 0;
	dmd_but_ = 1;
	if (egalite_pts(OG_, but_)){
		charge_ = deltaR;
	} else {
		charge_ = 0;
	}
	
	if(!same_uid(Er, pb_)) Er.push_back( this );
}

Comm::Comm(double uid, double x, double y, double xp, double yp,
		   vector<Robot*> &Er, int color)
: Robot (uid, x,y, xp, yp, Er, color)
{
	cost_crea_ = cost_com;
	dist_max_ = maxD_com;
	type_ = 'C';
	OG_ = Point (xp, yp);
	but_ = Point(xp+1, yp+1);
	fixe_ = 0;
	dmd_but_ = 1;
}

Comm::Comm(istringstream &data, vector<Robot*> &Er, Point OG, bool& pb_) {
	decodageCOM(data, Er, OG, pb_);
}

void Comm::decodageCOM(istringstream &data,vector<Robot*> &Er,Point OG, bool& pb_){
	double x(0), y(0), xb(0), yb(0);
	string a;
	
	data>>uid_>>dp_>>x>>y>>xb>>yb>>a;
	
	lire_bool(a,atteint_);
	range_.setCentre(normalize(x),normalize(y));
	range_.setRayon(rayon_comm);
	but_.setX(normalize(xb));
	but_.setY(normalize(yb));
	cost_crea_ = cost_com;
	dist_max_ = maxD_com;
	type_ = 'C';
	OG_ = OG;
	fixe_ = 0;
	dmd_but_ = 0;
	
	if(!same_uid(Er, pb_)) Er.push_back( this );
}

void Robot::deplacement() {
	normalize (but_);
	Vecteur dir (but_.getX()-range_.getCentre().getX(),
				 but_.getY()-range_.getCentre().getY());
	normalize (dir);
	Vecteur unit (dir.getX()/dir.getNorme(), dir.getY()/dir.getNorme());
	int i(0);
	if (dir.getNorme() <= deltaD){
		range_.setCentre(but_.getX(), but_.getY());
		dp_ += dir.getNorme();
	} else {
		while(!((i++ >= deltaD) or (arrivee()) or (fixe_))){
			range_.setCentre(range_.getCentre().getX()+ unit.getX(),
								range_.getCentre().getY()+ unit.getY());
			dp_ += unit.getNorme();
			if (dp_ >= dist_max_) fixe_ = 1;
		}
	}
	pause_ = 1;
}

void Robot::setPause(bool a){
	pause_ = a;
}

void Robot::setLink(bool a){
	linked_ = a;
}

void Robot::setDp(double a){
	dp_ = a;
}

void Robot::setFixe (bool a){
	fixe_ = a;
}

bool Robot::arrivee(){
	if (egalite_pts(range_.getCentre(), but_)) {
		atteint_ = 1;
		return 1;
	}
	return 0;
}

void Robot::act(){
	if (!fixe_){
		if (!(arrivee())){
			deplacement();
		} else {
			if (!(type_ == 'T')) work();
		}
	}
}

void Prospection::work(){
	dmd_but_ = 1;
	if (!linked_) {
		but_ = OG_;
		retour_ = 1;
	}
}

void Foreur::work(){
	if ((!(gis_vis_ == nullptr)) and (acolyte_.size())){
		for (unsigned int i(0); i<acolyte_.size(); i++){
			if ((pt_cercle(range_.getCentre(), gis_vis_->getRange()))
				and (egalite_pts(acolyte_[i]->getRange().getCentre(),
				range_.getCentre()))) {
				gis_vis_->extract();
				if (gis_vis_->getCap() <= deltaR) {
					fixe_ = 1;	
					acolyte_[i]->work();
					gis_vis_ = nullptr;
					acolyte_.clear();
				} else {
					acolyte_[i]->work();
					pause_ = 1;
				}
			}
		}
	}
}

void Transport::work(){
	if ((charge_) and (egalite_pts(range_.getCentre(), OG_))){
		charge_ = 0;
		pause_ = 1;
		dmd_but_ = 1;
	}
	if ((aco_) and (egalite_pts(range_.getCentre(),
		acolyte_->getRange().getCentre()))){
		charge_ = deltaR;
		pause_ = 1;
		dmd_but_ = 0;
		but_ = OG_;	
		Foreur *foreur = static_cast<Foreur*>(acolyte_);
		foreur->rmvAco(this);
		aco_ = 0;
		acolyte_ = nullptr;	
	}
}

void Comm::work(){
	fixe_ = 1;
}

void Foreur::setGis(Gisement* gis){
	gis_vis_ = gis;
}

void Foreur::ajtAco(Robot* tran){
	acolyte_.push_back(tran);
}

void Foreur::rmvAco(Robot* tran){
	for (unsigned int i(0); i<acolyte_.size(); i++){
		if (egalite_pts(tran->getRange().getCentre(),
			acolyte_[i]->getRange().getCentre())){
			acolyte_[i] = nullptr;
			swap(acolyte_[i], acolyte_.back());
			acolyte_.pop_back();
		}
	}
}

void Transport::setAco(Robot* foreur){
	acolyte_ = foreur;
}

void Prospection::setFound(bool a){
	found_ = a;
}

void Robot::setBut(Point pt){
	but_ = pt;
}

void Robot::setDmd(bool a){
	dmd_but_ = a;
}

void Prospection::erase(){
	rayong_ = 0;
	capaciteg_ = 0;
	pos_gis_ = Point(0, 0);
	found_ = 0;
}

void Transport::setNbAco(bool a){
	aco_ = a;
}

void Robot::ajt(Robot* rob){
	reseau_.push_back(rob);
}

void Robot::reset(){
	reseau_.clear();
	linked_ = 0;
	pause_ = 0;
}

Robot::~Robot(){
}

Prospection::~Prospection(){
	for (unsigned int i(0); i<reseau_.size(); i++){
		reseau_[i] = nullptr;
	}
}

Transport::~Transport(){
	acolyte_ = nullptr;
	for (unsigned int i(0); i<reseau_.size(); i++){
		reseau_[i] = nullptr;
	}
}

Foreur::~Foreur(){
	for (unsigned int i(0); i< acolyte_.size(); i++)
		acolyte_[i] = nullptr;
	acolyte_.clear();
	gis_vis_ = nullptr;
	for (unsigned int i(0); i<reseau_.size(); i++){
		reseau_[i] = nullptr;
	}
}

Comm::~Comm(){
	for (unsigned int i(0); i<reseau_.size(); i++){
		reseau_[i] = nullptr;
	}
}

bool Foreur::detection (Gisement* gis){
	if (pt_cercle(range_.getCentre(), gis->getRange())){
		this->gis_vis_ = gis;
		return 1;
	}
	return 0;
}

void Prospection::detection (Gisement* gis){
	if (inter_cercles(range_, gis->getRange())){
		pos_gis_ = gis->getRange().getCentre();
		capaciteg_ = gis->getCap();
		rayong_ = gis->getRange().getRayon();
		found_ = 1;
	}
}

void Transport::fin_gis(){
	if ((aco_) and (acolyte_->getFixe())){
		but_ = OG_;
		dmd_but_ = 1;
		acolyte_ = nullptr;
		aco_ = 0;
	}
}
