// geomod.h - v2
// Arthur CHANSEL - 324265
// Benoît VIGNON - 327737

#ifndef GEOMOD_H
#define GEOMOD_H

#include <iostream>
#include <iomanip>
#include <cmath>
#include "constantes.h"
#include "graphic.h"

class Point {
public:
	Point(double x=0, double y=0) : x_(x), y_(y) {}
	void setX(double x);
	void setY(double y);
	double getX() const { return x_; }
	double getY() const { return y_; }
private:
	double x_;
	double y_;
};

class Vecteur {
public:
	Vecteur(double x=0, double y=0);
	void setX(double x);
	void setY(double y);
	void setNorme();
	double getX() const { return x_; }
	double getY() const { return y_; }
	double getNorme() const { return norme_; }
private:
	double x_;
	double y_;
	double norme_;
};

class Cercle {
public:
	Cercle(double x=0, double y=0, double r=1);
	void setCentre (double x, double y);
	Point getCentre () const { return centre_; }
	void setRayon (double r);
	double getRayon () const { return rayon_; }
private:
	Point centre_;
	double rayon_;
};

bool equal_zero (double x);
void setMax (double x);
void setEps ();
double getMax();
double getEps ();

double normalize (double a);
double normalize (Vecteur &v);
double normalize (Point &p);
double normalize (Cercle &c);
double new_vect (Point depart, Point arrivee, Vecteur &v);
bool egalite_pts (Point a, Point b);
bool pt_cercle (Point a, Cercle c);
bool inter_cercles (Cercle a, Cercle b);

void draw_dot (double x, double y, double r, int c=-1);
void draw_circle (double x, double y, double r);
void draw_pro (double x, double y, int c);
void draw_for (double x, double y, int c);
void draw_tra (double x, double y, int c);
void draw_com (double x, double y, int c);
void draw_link (Point a, Point b);

#endif
