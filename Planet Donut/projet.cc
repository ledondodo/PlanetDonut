// projet.cc - v3
// Arthur CHANSEL - 324265
// Benoît VIGNON - 327737

#include <iostream>
#include <gtkmm.h>
#include "simulation.h"
#include "gui.h"
using namespace std;

int main (int argc, char* argv[])
{	
	auto app = Gtk::Application::create();
	MyWin win(argc, argv[1]);
	
	return app->run(win);
}
