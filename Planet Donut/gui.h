// gui.h - v1
// Arthur CHANSEL - 324265
// Benoît VIGNON - 327737

#include <gtkmm.h>
#include <cairomm/context.h>
#include <iostream>
#include <cmath>
#include "geomod.h"
#include "graphic.h"
#include "simulation.h"

#ifndef GUI_H
#define GUI_H

void graphic_set_context(const Cairo::RefPtr<Cairo::Context> &cr);

class MyArea : public Gtk::DrawingArea
{
public:
	MyArea();
	virtual ~MyArea();
	void setToggleLink(bool togglelink);
	void setToggleRange(bool togglerange);
	
protected:
	void draw_simulation();
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
	
	double draw_size;
	double Xmin;
	double Xmax;
	double Ymin;
	double Ymax;
	
	
private:
	bool empty_;
	bool togglelink_;
	bool togglerange_;
};


class MyWin : public Gtk::Window
{
public:
	MyWin() = default;
	MyWin(int argc, char* f);
	virtual ~MyWin();
	void tree_view_update();
	
protected:
	// Signal handlers
	bool on_idle();
	void on_button_clicked_exit();
	void on_button_clicked_open();
	void on_button_clicked_save();
	void on_button_clicked_start();
	void on_button_clicked_step();
	void on_button_clicked_togglelink();
	void on_button_clicked_togglerange();
	bool on_key_press_event(GdkEventKey * key_event);
	
	// interface components
	Gtk::Box	m_Box, m_Box_Up, m_Box_Down, m_Box_Buttons,
				m_Box_General, m_Box_ToggleDisplay;
	MyArea 		m_Area;
	Gtk::Label	m_Label_General, m_Label_ToggleDisplay;
	Gtk::Button	m_Button_exit, m_Button_open, m_Button_save,
				m_Button_start, m_Button_step;
	Gtk::ToggleButton m_Button_togglelink, m_Button_togglerange;
	
private:
	bool started_;
	bool step_;
	void refresh();
	int counter_;

	// TreeView
	class Model_columns : public Gtk::TreeModel::ColumnRecord
	{
	public:
	 Model_columns(){
	   add(_col_id);
	   add(_col_nbP);
	   add(_col_nbF);
	   add(_col_nbT);
	   add(_col_nbC);
	   add(_col_resource);
	   add(_col_resource_percentage);
	 }
	 Gtk::TreeModelColumn<int> _col_id;
	 Gtk::TreeModelColumn<int> _col_nbP;
	 Gtk::TreeModelColumn<int> _col_nbF;
	 Gtk::TreeModelColumn<int> _col_nbT;
	 Gtk::TreeModelColumn<int> _col_nbC;
	 Gtk::TreeModelColumn<double> _col_resource;
	 Gtk::TreeModelColumn<int> _col_resource_percentage;
	};
	Model_columns _col;
	Gtk::ScrolledWindow _scrolled_window;
	Gtk::TreeView _tree_view;
	
};

#endif
