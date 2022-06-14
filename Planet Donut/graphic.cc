// graphic.cc - v1
// Arthur CHANSEL - 324265
// Benoît VIGNON - 327737

#include "graphic.h"
using namespace std;

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);
static double width;
static double height;
static double dim_max;
static double draw_size;
static double offsetX_def;
static double offsetY_def;
static double offsetX;
static double offsetY;

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr) {
	ptcr = &cr;
}

void set_para(double w, double h, double dim){
	width = w;
	height = h;
	dim_max = dim;
	draw_size = ((width>height)?height:width);
	offsetX_def = (width-draw_size)/2;
	offsetY_def = (height-draw_size)/2;
	offsetX = (offsetX_def/width)*2*dim_max;
	offsetY = (offsetY_def/height)*2*dim_max;
}

void set_color (int c){
	switch(c){
	case -1:
		(*ptcr)->set_source_rgb(0,0,0);
		break;
	case 0:
		(*ptcr)->set_source_rgb(1,0,0);
		break;
	case 1:
		(*ptcr)->set_source_rgb(0,1,0);
		break;
	case 2:
		(*ptcr)->set_source_rgb(0,0,1);
		break;
	case 3:
		(*ptcr)->set_source_rgb(1,0.7,0);
		break;
	case 4:
		(*ptcr)->set_source_rgb(1,0,1);
		break;
	case 5:
		(*ptcr)->set_source_rgb(0,1,1);
		break;
	}
}

void zone(){
	(*ptcr)->set_line_width(2.0);
	(*ptcr)->set_source_rgb(0.5, 0.5, 0.5);
	
	(*ptcr)->set_source_rgb(0.5, 0.5, 0.5);
	(*ptcr)->rectangle(0,0,draw_size,draw_size);
	(*ptcr)->stroke();
}

void dot(double x, double y, double r, int c){
	set_color(c);
	(*ptcr)->set_line_width(0.5);
	
	(*ptcr)->move_to(x+r,y);
	(*ptcr)->arc(x,y,r,0,2*M_PI);
	(*ptcr)->fill_preserve();
	(*ptcr)->stroke();
}


void circle(double x, double y, double r){
	(*ptcr)->set_source_rgb(0.5,0.5,0.5);
	(*ptcr)->set_line_width(2.0);
	
	int div = 10;
	for (int i(0) ; i<div ; ++i){
		(*ptcr)->move_to(x+r*cos(i*M_PI/5),y+r*sin(i*M_PI/5));
		(*ptcr)->arc(x,y,r,i*M_PI/5,(i+0.5)*M_PI/5);
	}
	(*ptcr)->stroke();
}

void line(double x1, double y1, double x2, double y2){
	(*ptcr)->set_source_rgb(1,0,0.5);
	(*ptcr)->set_line_width(5.0);
	
	(*ptcr)->move_to(x1,y1);
	(*ptcr)->line_to(x2,y2);
	(*ptcr)->stroke();
}

void rect(double x1, double y1, double x2, double y2, int c){
	set_color(c);
	(*ptcr)->set_line_width(4.0);
	
	(*ptcr)->move_to(x1,y1);
	(*ptcr)->line_to(x1,y2);
	(*ptcr)->line_to(x2,y2);
	(*ptcr)->line_to(x2,y1);
	(*ptcr)->line_to(x1,y1);
	(*ptcr)->stroke();
}

void cross(double x, double y, double size, int c){
	set_color(c);
	(*ptcr)->set_line_width(4.0);
	
	(*ptcr)->move_to(x-size,y+size);
	(*ptcr)->line_to(x+size,y-size);
	(*ptcr)->move_to(x-size,y-size);
	(*ptcr)->line_to(x+size,y+size);
	(*ptcr)->stroke();
}

void v(double x, double y, double size, int c){
	set_color(c);
	(*ptcr)->set_line_width(4.0);
	
	(*ptcr)->move_to(x,y);
	(*ptcr)->line_to(x-size,y-size);
	(*ptcr)->move_to(x,y);
	(*ptcr)->line_to(x+size,y-size);
	(*ptcr)->stroke();
}

void radar(double x, double y, double size, int c){
	set_color(c);
	(*ptcr)->set_line_width(4.0);
	
	(*ptcr)->move_to(x+size*cos(M_PI/4),y+size*sin(M_PI/4));
	(*ptcr)->arc(x,y,size,M_PI/4,3*M_PI/4);
	(*ptcr)->move_to(x+2*size*cos(M_PI/4),y+2*size*sin(M_PI/4));
	(*ptcr)->arc(x,y,2*size,M_PI/4,3*M_PI/4);
	(*ptcr)->stroke();
}
