// geomod.cc - v2
// Arthur CHANSEL - 324265
// Benoît VIGNON - 327737

#include "geomod.h"
using namespace std;

namespace {
	double epsilon_zero;
}

bool equal_zero (double x) {
	if ( abs(x) < epsilon_zero ) return 1;
	return 0;
}

void setEps () {
	epsilon_zero = dim_max*pow(10,-10);
	setprecision(10);
}

double getEps() { return epsilon_zero; }

void Point::setX(double x) { x_ = x; }

void Point::setY(double y) { y_ = y; }

Vecteur::Vecteur(double x, double y) : x_(x), y_(y) {
		setNorme();
}

void Vecteur::setX(double x) { x_ = x; }

void Vecteur::setY(double y) { y_ = y; }

void Vecteur::setNorme() { norme_ = sqrt(x_*x_ + y_*y_); }

Cercle::Cercle(double x, double y, double r) : rayon_(r) {
	setCentre(x, y);
}

void Cercle::setCentre (double x, double y) {
	centre_.setX(x);
	centre_.setY(y);
}

void Cercle::setRayon (double r) { rayon_=r; }

double normalize (double a) {
	if (a>dim_max) {
		a = int(a)%int(2*dim_max);
		if (a>dim_max) a -= 2*dim_max;
	}
	if (a<-dim_max) {
		a = int(a)%int(2*dim_max);
		if (a<-dim_max) a += 2*dim_max;
	}
	
	return a;
}

double normalize (Vecteur &v) {
	v.setX(normalize(v.getX()));
	v.setY(normalize(v.getY()));
	v.setNorme();
	
	return v.getNorme();
}

double normalize (Point &p) {
	p.setX(normalize(p.getX()));
	p.setY(normalize(p.getY()));
	
	return sqrt(p.getX()*p.getX() + p.getY()*p.getY());
}

double normalize (Cercle &c) {
	Point centre(c.getCentre());
	normalize(centre);
	c.setCentre(centre.getX(), centre.getY());
	
	return sqrt(c.getCentre().getX()*c.getCentre().getX()
				+ c.getCentre().getY()*c.getCentre().getY());
};

double new_vect (Point depart, Point arrivee, Vecteur &v) {
	double dx(2*dim_max);
	double dy(2*dim_max);
	
	for (int kx(-1) ; kx < 2 ; ++kx){
		for (int ky(-1) ; ky < 2 ; ++ky){
			double dy_temp(0);
			dy_temp = ( arrivee.getY() + ky*2*dim_max ) - depart.getY();
			if (abs(dy) > abs(dy_temp)) {
				dy = dy_temp;
			}
		}
		double dx_temp(0);
		dx_temp = ( arrivee.getX() + kx*2*dim_max ) - depart.getX();
		if (abs(dx) > abs(dx_temp)) {
			dx = dx_temp;
		}
	}
	v = Vecteur (dx,dy);
	
	return v.getNorme();
}

bool egalite_pts (Point a, Point b) {
	Vecteur v;
	if ( new_vect(a, b, v) < epsilon_zero ) return 1;
	return 0;
}

bool pt_cercle (Point a, Cercle c) {
	Vecteur v;
	new_vect(a, c.getCentre(), v);
	if ( v.getNorme() <= (c.getRayon())+epsilon_zero ) return 1;
	return 0;
}

bool inter_cercles (Cercle a, Cercle b) {
	Vecteur v;
	new_vect(a.getCentre(), b.getCentre(), v);
	if(v.getNorme()<a.getRayon()+b.getRayon()-epsilon_zero ) return 1;
	return 0;
}

void draw_dot (double x, double y, double r, int c) {
	dot(x,y,r,c);
	if ( (x+r-dim_max)>epsilon_zero ){ // droite
		dot(x-2*dim_max,y,r,c);
		if ( (y+r-dim_max)>epsilon_zero ){ // coin haut
			dot(x,y-2*dim_max,r,c);
			dot(x-2*dim_max,y-2*dim_max,r,c);
		}
		if ( (y-r+dim_max)<epsilon_zero ){ // coin bas
			dot(x,y+2*dim_max,r,c);
			dot(x-2*dim_max,y+2*dim_max,r,c);
		}
	}
	if ( (y+r-dim_max)>epsilon_zero ) dot(x,y-2*dim_max,r,c); // haut
	if ( (x-r+dim_max)<epsilon_zero ){ // gauche
		dot(x+2*dim_max,y,r,c);
		if ( (y+r-dim_max)>epsilon_zero ){ // coin haut
			dot(x,y-2*dim_max,r,c);
			dot(x+2*dim_max,y-2*dim_max,r,c);
		}
		if ( (y-r+dim_max)<epsilon_zero ){ // coin bas
			dot(x,y+2*dim_max,r,c);
			dot(x+2*dim_max,y+2*dim_max,r,c);
		}
	}
	if ( (y-r+dim_max)<epsilon_zero ) dot(x,y+2*dim_max,r,c); // bas
}

void draw_circle (double x, double y, double r) {
	circle(x,y,r);
	if ( (x+r-dim_max)>epsilon_zero ){ // droite
		circle(x-2*dim_max,y,r);
		if ( (y+r-dim_max)>epsilon_zero ){ // coin haut
			circle(x,y-2*dim_max,r);
			circle(x-2*dim_max,y-2*dim_max,r);
		}
		if ( (y-r+dim_max)<epsilon_zero ){ // coin bas
			circle(x,y+2*dim_max,r);
			circle(x-2*dim_max,y+2*dim_max,r);
		}
	}
	if ( (y+r-dim_max)>epsilon_zero ) circle(x,y-2*dim_max,r); // haut
	if ( (x-r+dim_max)<epsilon_zero ){ // gauche
		circle(x+2*dim_max,y,r);
		if ( (y+r-dim_max)>epsilon_zero ){ // coin haut
			circle(x,y-2*dim_max,r);
			circle(x+2*dim_max,y-2*dim_max,r);
		}
		if ( (y-r+dim_max)<epsilon_zero ){ // coin bas
			circle(x,y+2*dim_max,r);
			circle(x+2*dim_max,y+2*dim_max,r);
		}
	}
	if ( (y-r+dim_max)<epsilon_zero ) circle(x,y+2*dim_max,r); // bas
}


void draw_pro (double x, double y, int c) {
	cross(x,y+30,12, c); // ajouter color
	dot(x,y,display_rayon_robot, c); // ajouter color
}

void draw_for (double x, double y, int c) {
	v(x,y+30,15, c); // ajouter color
	dot(x,y,display_rayon_robot, c); // ajouter color
}

void draw_tra (double x, double y, int c) {
	rect(x-16,y-16,x+16,y+16, c); // ajouter color
	dot(x,y,display_rayon_robot, c); // ajouter color
}

void draw_com (double x, double y, int c) {
	radar(x,y,20, c); // ajouter color
	dot(x,y,display_rayon_robot, c); // ajouter color
}

void draw_link (Point a, Point b){
	line(a.getX(),a.getY(),b.getX(),b.getY());
}

