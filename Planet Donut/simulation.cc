// simulation.cc - v1
// Arthur CHANSEL - 324265
// Benoît VIGNON - 327737

#include "simulation.h"
using namespace std;

namespace {
	enum etat_lecture {NBG,GISEMENTS,NBB,BASES,PRO,FOR,TRA,COM};
}

void Simulation::init_sim(string f) {
	setEps();
	fi_ = f;
	etat_ = NBG;
	nbG_ = 0;
	nbB_ = 0;
	i_ = 0;
	j_ = 0;
	tick_ = 0;
	pb_ =  0;
	lecture(fi_);
}

bool Simulation::test_comm(int a) {
	double x(Eb_[a]->getRange().getCentre().getX());
	double y(Eb_[a]->getRange().getCentre().getY());
	
	if (Eb_[a]->getnbC() == 0){
		cout << message::missing_robot_communication(x,y);
		pb_ = 1;
		return 1;
	}
	
	int i_0 = Eb_[a]->getnbP() + Eb_[a]->getnbF() + Eb_[a]->getnbT();
	bool same(0);
	
	for(int i(i_0) ; i<(i_0+Eb_[a]->getnbC()) ; ++i){
		if(egalite_pts((Eb_[a]->getEr())[i]->getRange().getCentre(),
					   Eb_[a]->getRange().getCentre())){
			same = 1;
		}
	}
	if(!same){
		cout << message::missing_robot_communication(x,y);
		pb_ = 1;
		return 1;
	}
	return 0;
}

void Simulation::lecture(string nom_fichier) {
	string line;
	ifstream fichier(nom_fichier);
	if (!fichier.fail()){
		while((getline(fichier>>ws, line)) and (!pb_)){
			if (line[0]=='#') continue;
			decodage(line);
		}
	}
	if (!pb_) cout << message::success();
}

void Simulation::decodageNBG(istringstream &data) {
	data>>nbG_;
	i_=0;
	if(nbG_==0)  etat_=NBB;
	else 		 etat_=GISEMENTS;
}

void Simulation::newGIS(istringstream &data) {
	new Gisement(data, Eg_, pb_);
	++i_;
	if(i_==nbG_) etat_=NBB;
}

void Simulation::decodageNBB(istringstream &data) {
	data>>nbB_;
	i_=0;
	etat_=BASES;
}

void Simulation::newBASE(istringstream &data) {
	static int ct = 0;
	ct++;
	new Base(data, Eb_, Eg_, ct, pb_);
	
	if(Eb_[i_]->getnbP()){
		etat_=PRO;
		return;
	} else if(Eb_[i_]->getnbF()){
		etat_=FOR;
		return;
	} else if(Eb_[i_]->getnbT()){
		etat_=TRA;
		return;
	} else if(Eb_[i_]->getnbC()){
		etat_=COM;
		return;
	}	
	++i_;
}

void Simulation::newPRO(istringstream &data) {
	Eb_[i_]->createPRO(data, pb_);
	
	++j_;
	if(j_==Eb_[i_]->getnbP()){
		j_=0;
		if(Eb_[i_]->getnbF()){
			etat_=FOR;
			return;
		} else if(Eb_[i_]->getnbT()){
			etat_=TRA;
			return;
		} else if(Eb_[i_]->getnbC()){
			etat_=COM;
			return;
		} else {
			etat_=BASES;
		}
	}
}

void Simulation::newFOR(istringstream &data) {
	Eb_[i_]->createFOR(data, pb_);
	
	++j_;
	if(j_==Eb_[i_]->getnbF()){
		j_=0;
		if(Eb_[i_]->getnbT()){
			etat_=TRA;
			return;
		} else if(Eb_[i_]->getnbC()){
			etat_=COM;
			return;
		} else {
			etat_=BASES;
		}
	}
}

void Simulation::newTRA(istringstream &data) {
	Eb_[i_]->createTRA(data, pb_);
	
	++j_;
	if(j_==Eb_[i_]->getnbT()){
		j_=0;
		if(Eb_[i_]->getnbC()){
			etat_=COM;
			return;
		} else {
			etat_=BASES;
		}
	}
}

void Simulation::newCOM(istringstream &data) {
	Eb_[i_]->createCOM(data, pb_);
	++j_;
	if(j_==Eb_[i_]->getnbC()){
		j_=0;
		test_comm(i_);
		etat_=BASES;
		++i_;
	}
}

void Simulation::decodage(string line) {
	istringstream data(line);
	switch(etat_){
	case NBG:
		decodageNBG(data);
		break;
	case GISEMENTS:
		newGIS(data);	
		break;
	case NBB:
		decodageNBB(data);
		break;
	case BASES:
		newBASE(data);
		break;
	case PRO:
		newPRO(data);
		break;
	case FOR:
		newFOR(data);
		break;
	case TRA:
		newTRA(data);
		break;
	case COM:
		newCOM(data);
		break;
	}
}

void Simulation::reset_base(){
	for (unsigned int i (0); i < Eb_.size(); i++){
		Eb_[i]->reset();
	}
}

void Simulation::update_links(int i){
	for (unsigned int j(0); j<Eb_.size(); j++){
		for (unsigned int k(0); k<Eb_.size(); k++){
			if (i == 0){
				update_voisin(Eb_[j], Eb_[k]);
			}	
		}		
	}
	Eb_[i]->connexion(Eb_[i]->find_depart());
}

void Simulation::update_detection(int i){
	for (unsigned int j(0); j<Eb_[i]->getEr().size(); j++){
		if ((Eb_[i]->getEr()[j]->getType() == 'F')
			and (Eb_[i]->getEr()[j]->arrivee())){
			Foreur *foreur = static_cast<Foreur*>(Eb_[i]->getEr()[j]);
			bool detecte (0);
			for (unsigned int k (0); k< Eg_.size(); k++){
				if(foreur->detection(Eg_[k])) detecte = 1;
			}
			if (!detecte){
				foreur->setFixe(1);
			}
		}
		if ((Eb_[i]->getEr()[j]->getType() == 'P')
			and (Eb_[i]->getEr()[j]->arrivee())){
			Robot* r(Eb_[i]->getEr()[j]);
			Prospection *prosp = static_cast<Prospection*>(r);
			for (unsigned int k (0); k< Eg_.size(); k++){
				prosp->detection(Eg_[k]);
			}
		}
	}
}

void Simulation::delete_base(int i){
	if((Eb_[i]->getStock())<=0){
		swap(Eb_.back(), Eb_[i]);
		delete Eb_.back();
		Eb_.pop_back();
		nbB_--;
	}
}

void Simulation::delete_gis(){
	for (unsigned int i (0); i < Eg_.size(); i++){
		if(Eg_[i]->getCap()<=deltaR){
			delete Eg_[i];
			Eg_[i] = nullptr;
			swap(Eg_[i], Eg_.back());
			Eg_.pop_back();
			nbG_--;
		}
	}
}

void Simulation::update_sim(){
	reset_base();
	for (unsigned int i (0); i < Eb_.size(); i++){
		update_links(i);
		if(!(Eb_[i]->getFin())) {
			update_detection(i);
			update_base(Eb_[i]);
		}
		delete_base(i);
	}
	delete_gis();
}

void Simulation::update_voisin(Base* source, Base* voisin){
	for (unsigned int i(0); i<source->getEr().size(); i++){
		for (unsigned int j(0); j<voisin->getEr().size(); j++){
			Cercle c1(source->getEr()[i]->getRange());
			Cercle c2(voisin->getEr()[j]->getRange());
			if ((inter_cercles(c1,c2))
				and (source->getEr()[i] != voisin->getEr()[j])){
				source->getEr()[i]->ajt(voisin->getEr()[j]);
			}	
		}
	}
}

void Simulation::update_base(Base* base){
	for (unsigned int j (0); j < base->getEr().size(); j++){
		if (base->getEr()[j]->getLink()){
			base->repair(j);
			if (base->getEr()[j]->getDmd()) base->dec_but(j);
			base->getEr()[j]->act();
			if (base->getEr()[j]->getType() == 'P') base->update_gis(j);
			if (base->getEr()[j]->getType() == 'T') base->discharge(j);
		} else {
			base->getEr()[j]->act();
		}
	}
	base->creer_rob();
}

void Simulation::draw_gis(){
	for(unsigned int i (0); i<Eg_.size(); i++){
		double xb (Eg_[i]->getRange().getCentre().getX());
		double yb (Eg_[i]->getRange().getCentre().getY());
		double rb (Eg_[i]->getRange().getRayon());
		draw_dot(xb, yb, rb,-1);
	}
}

void Simulation::draw_base(bool link, bool range){
	for(unsigned int i (0); i<Eb_.size(); i++){
		double xb (Eb_[i]->getRange().getCentre().getX());
		double yb (Eb_[i]->getRange().getCentre().getY());
		double rb (Eb_[i]->getRange().getRayon());
		int c (Eb_[i]->getColor());
		draw_dot(xb, yb, 10*rb, c);
		
		for (unsigned int j(0); j<Eb_[i]->getEr().size(); j++){
			double xr(Eb_[i]->getEr()[j]->getCentre().getX());
			double yr(Eb_[i]->getEr()[j]->getCentre().getY());
			int c (Eb_[i]->getColor());
			draw_dot(xr, yr, display_rayon_robot, c);
			if(range) draw_circle(xr,yr,rayon_comm);
			if(Eb_[i]->getEr()[j]->getType() == 'P') draw_pro(xr,yr,c);
			if(Eb_[i]->getEr()[j]->getType() == 'F') draw_for(xr,yr,c);
			if(Eb_[i]->getEr()[j]->getType() == 'T') draw_tra(xr,yr,c);
			if(Eb_[i]->getEr()[j]->getType() == 'C') draw_com(xr,yr,c);
		}
		
		if(link){
			for(unsigned int j(0) ; j<Eb_[i]->getEr().size(); j++){
				for(unsigned int k(0);k<(Eb_[i]->getReseauTot().size());
					++k){
					Point p1(Eb_[i]->getEr()[j]->getCentre());
					Point p2(Eb_[i]->getReseauTot()[k]->getCentre());
					if(Eb_[i]->getEr()[j]->getLink()) draw_link(p1,p2);
				}
			}
		}
	}
}

void Simulation::clear_sim(){
	for (unsigned int i(0); i<Eg_.size(); i++){
		delete Eg_[i];
	}
	Eg_.clear();
	
	for (unsigned int i(0); i<Eb_.size(); i++){
		delete Eb_[i];
	}
	Eb_.clear();
}

void Simulation::write_save(string f){
	string line;
	ofstream fichier(f, ofstream::out);
	if (!fichier.fail()) {
		fichier << "# Sauvegarde" << "\n" << "\n";
		write_gis(fichier);
		write_base(fichier);
	}	
}

string Simulation::write_bool (bool a){
	if (a) return "true";
	return "false";
}

void Simulation::write_gis(ofstream &fichier){
	fichier <<"# Gisement"<<"\n"<<nbG_<<"\n"<<"# Gisements:"<<"\n";
	for (unsigned int i(0); i<Eg_.size(); i++){
		double xg (Eg_[i]->getRange().getCentre().getX());
		double yg (Eg_[i]->getRange().getCentre().getY());
		double rg (Eg_[i]->getRange().getRayon());
		double cap (Eg_[i]->getCap());
		fichier<<"	"<<xg<<" "<<yg<<" "<<rg<<" "<<cap<<"\n";
	}
}

void Simulation::write_base(ofstream &fichier){
	fichier <<"\n"<<"# nb Base"<<"\n"<<nbB_<<"\n"<<"# Bases:"<<"\n";
	for (unsigned int i(0); i<Eb_.size(); i++){
		double xb (Eb_[i]->getRange().getCentre().getX());
		double yb (Eb_[i]->getRange().getCentre().getY());
		double sto (Eb_[i]->getStock());
		int nbP (Eb_[i]->getnbP());
		int nbF (Eb_[i]->getnbF());
		int nbT (Eb_[i]->getnbT());
		int nbC (Eb_[i]->getnbC());
		fichier <<"	"<<xb<<" "<<yb<<" "<<sto<<" "<<nbP<<" "<<nbF
				<<" "<<nbT<<" "<<nbC<<"\n";
		
		write_pro(fichier, i, nbP);
		write_for(fichier, i, nbF);
		write_tra(fichier, i, nbT);
		write_comm(fichier, i, nbC);
		fichier << "\n" << "\n";
	}
}

void Simulation::write_pro(ofstream &fichier, unsigned int i, int nbP){
	if (nbP){
		fichier <<"	# Prospector:"<<"\n";
		for (unsigned int j(0); j<Eb_[i]->getEr().size(); j++){
			if (Eb_[i]->getEr()[j]->getType() == 'P') {
				Robot* r(Eb_[i]->getEr()[j]);
				Prospection *prosp = dynamic_cast<Prospection*>(r);
				double uid (prosp->getUid());
				double dp (prosp->getDp());
				double x (prosp->getRange().getCentre().getX());
				double y (prosp->getRange().getCentre().getY());
				double xb (prosp->getBut().getX());
				double yb (prosp->getBut().getY());
				bool atteint (prosp->arrivee());
				bool retour (prosp->getRetour());
				bool found (prosp->getFound());
				fichier <<"	"<<uid <<" "<<dp<<" "<<x<<" "<<y<<" "<<xb
						<<" "<<yb<<" "<<write_bool(atteint)<<" "
						<<write_bool(retour)<<" "
						<<write_bool(found)<<" ";
				if (found){
					double xg (prosp->getGisX());
					double yg (prosp->getGisY());
					double rg (prosp->getRayG());
					double cg (prosp->getCapG());
					fichier <<xg<<" "<<yg<<" "<<rg<<" "<<cg<<"\n";
				} else {
					fichier <<"\n";
				}
			}
		}
	}
}

void Simulation::write_for(ofstream &fichier, unsigned int i, int nbF){
	if (nbF){
		fichier <<"	# Drilling:"<<"\n";
		for (unsigned int j(0); j<Eb_[i]->getEr().size(); j++){
			if (Eb_[i]->getEr()[j]->getType() == 'F'){
				Robot* r(Eb_[i]->getEr()[j]);
				Foreur *foreur = dynamic_cast<Foreur*>(r);
				double uid (foreur->getUid());
				double dp (foreur->getDp());
				double x (foreur->getRange().getCentre().getX());
				double y (foreur->getRange().getCentre().getY());
				double xb (foreur->getBut().getX());
				double yb (foreur->getBut().getY());
				bool atteint (foreur->arrivee());
				fichier<<"	"<<uid<<" "<<dp<<" "<<x<<" "<<y<<" "<<xb
					   <<" "<<yb<<" "<<write_bool(atteint)<<"\n";
			}
		}
	}
}

void Simulation::write_tra(ofstream &fichier, unsigned int i, int nbT){
	if (nbT){
		fichier << "	#Transport:" << "\n";
		for (unsigned int j(0); j<Eb_[i]->getEr().size(); j++){
			if (Eb_[i]->getEr()[j]->getType() == 'T'){
				Robot* r(Eb_[i]->getEr()[j]);
				Transport *transp = dynamic_cast<Transport*>(r);
				double uid (transp->getUid());
				double dp (transp->getDp());
				double x (transp->getRange().getCentre().getX());
				double y (transp->getRange().getCentre().getY());
				double xb (transp->getBut().getX());
				double yb (transp->getBut().getY());
				bool atteint (transp->arrivee());
				bool retour (transp->getRetour());
				fichier<<"	"<<uid<<" "<<dp<<" "<<x<<" "<<y<<" "<<xb
					   <<" "<<yb<<" "<<write_bool(atteint)<<" "
					   <<write_bool(retour)<<"\n";
			}
		}
	}
}

void Simulation::write_comm(ofstream &fichier, unsigned int i, int nbC){
	if (nbC) {
		fichier << "	#Communication:" << "\n";
		for (unsigned int j(0); j<Eb_[i]->getEr().size(); j++){
			if (Eb_[i]->getEr()[j]->getType() == 'C'){
				Comm *comm = dynamic_cast<Comm*>(Eb_[i]->getEr()[j]);
				double uid (comm->getUid());
				double dp (comm->getDp());
				double x (comm->getRange().getCentre().getX());
				double y (comm->getRange().getCentre().getY());
				double xb (comm->getBut().getX());
				double yb (comm->getBut().getY());
				bool atteint (comm->arrivee());
				fichier<<"	"<<uid<<" "<<dp<<" "<<x<<" "<<y<<" "<<xb
					   <<" "<<yb<<" "<<write_bool(atteint)<<"\n";
			}
		}
	}
}

