// graphic.h - v1
// Arthur CHANSEL - 324265
// Benoît VIGNON - 327737

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <gtkmm.h>
#include <cairomm/context.h>
#include <iostream>
#include <vector>

void set_para(double w, double h,double dim);
void set_color(int c);
void zone();
void dot(double x, double y, double r, int c=-1);
void circle(double x, double y, double r);
void line(double x1, double y1, double x2, double y2);
void rect(double x1, double y1, double x2, double y2, int c);
void cross(double x, double y, double size, int c);
void v(double x, double y, double size, int c);
void radar(double x, double y, double size, int c);

#endif
