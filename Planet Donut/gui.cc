// gui.cc - v1
// Arthur CHANSEL - 324265
// Benoît VIGNON - 327737

#include "gui.h"
using namespace std;
namespace {
	Simulation s;
	Gtk::Allocation allocation;
	double width;
	double height;
}

struct SimData{
  unsigned nbP;
  unsigned nbF;
  unsigned nbT;
  unsigned nbC;
  double ressource;
  double ressource_p;
};

#define GTK_COMPATIBILITY_MODE

#ifdef GTK_COMPATIBILITY_MODE
namespace Gtk{
  template<class T, class... T_Args>
  auto make_managed(T_Args&&... args) -> T*
  {
    return manage(new T(std::forward<T_Args>(args)...));
  }
}
#endif

MyArea::MyArea()
: draw_size(0), Xmin(-dim_max), Xmax(dim_max), Ymin(-dim_max),
Ymax(dim_max), empty_(false)
{
	allocation = get_allocation();
	width = allocation.get_width();
	height = allocation.get_height();
}

MyArea::~MyArea()
{
}

void MyArea::setToggleLink(bool togglelink){togglelink_=togglelink;}

void MyArea::setToggleRange(bool togglerange){togglerange_=togglerange;}

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cr){
	graphic_set_context(cr); // indep graphic
	allocation = get_allocation();
	width = allocation.get_width();
	height = allocation.get_height();
	draw_size = ((width>height)?height:width);
	set_para(width, height,dim_max);
	zone();
	
	// Conversion de dimensions
	cr->translate(draw_size/2, draw_size/2);
	cr->scale(draw_size/(Xmax-Xmin),-draw_size/(Ymax-Ymin));
	cr->translate(-(Xmin+Xmax)/2,-(Ymin+Ymax)/2);
	
	if(not empty_) draw_simulation();
	else cout << "Empty !" << endl;
	
	return true;
}


void MyArea::draw_simulation(){
	s.draw_gis();
	s.draw_base(togglelink_,togglerange_);
}

MyWin::MyWin(int argc, char* f) :
	m_Box(Gtk::ORIENTATION_VERTICAL,10),
	m_Box_Up(Gtk::ORIENTATION_HORIZONTAL,10),
	m_Box_Down(Gtk::ORIENTATION_VERTICAL,10),
	m_Box_Buttons(Gtk::ORIENTATION_VERTICAL,10),
	m_Box_General(Gtk::ORIENTATION_VERTICAL,10),
	m_Box_ToggleDisplay(Gtk::ORIENTATION_VERTICAL,10),
	m_Label_General("General"), m_Label_ToggleDisplay("Toggle Display"),
	m_Button_exit("Exit"), m_Button_open("Open"), m_Button_save("Save"),
	m_Button_start("Start"), m_Button_step("Step"),
	m_Button_togglelink("Toggle Link"),
	m_Button_togglerange("Toggle Range"),
	started_(0), step_(0), counter_(0)
{
	set_title("GUI");
	set_border_width(0);
	add(m_Box);
	
	m_Box.pack_start(m_Box_Up);
	m_Box.pack_start(m_Box_Down);
	
	m_Box_Up.pack_start(m_Box_Buttons);
	m_Area.set_size_request(500,500);
	m_Box_Up.pack_start(m_Area);
	
	
	// =================== zone de bouttons =====================
	
	m_Box_Buttons.pack_start(m_Box_General);
	m_Box_Buttons.pack_start(m_Box_ToggleDisplay);
	
	m_Box_General.pack_start(m_Label_General);
	m_Box_General.pack_start(m_Button_exit);
	m_Box_General.pack_start(m_Button_open);
	m_Box_General.pack_start(m_Button_save);
	m_Box_General.pack_start(m_Button_start);
	m_Box_General.pack_start(m_Button_step);
	
	m_Box_ToggleDisplay.pack_start(m_Label_ToggleDisplay);
	m_Box_ToggleDisplay.pack_start(m_Button_togglelink);
	m_Box_ToggleDisplay.pack_start(m_Button_togglerange);
	
	m_Button_exit.signal_clicked().connect(sigc::mem_fun(*this,
              &MyWin::on_button_clicked_exit) );
	m_Button_open.signal_clicked().connect(sigc::mem_fun(*this,
              &MyWin::on_button_clicked_open) );
	m_Button_save.signal_clicked().connect(sigc::mem_fun(*this,
              &MyWin::on_button_clicked_save) );
	m_Button_start.signal_clicked().connect(sigc::mem_fun(*this,
              &MyWin::on_button_clicked_start) );
	m_Button_step.signal_clicked().connect(sigc::mem_fun(*this,
              &MyWin::on_button_clicked_step) );
	m_Button_togglelink.signal_clicked().connect(sigc::mem_fun(*this,
              &MyWin::on_button_clicked_togglelink) );
	m_Button_togglerange.signal_clicked().connect(sigc::mem_fun(*this,
              &MyWin::on_button_clicked_togglerange) ); 
	
	// ======================== pt3 ===========================
	
	m_Box_Down.pack_start(_scrolled_window);
	_scrolled_window.set_size_request(-1, 200);
	_scrolled_window.add(_tree_view);
	_scrolled_window.set_policy(Gtk::PolicyType::POLICY_AUTOMATIC,
							  Gtk::PolicyType::POLICY_AUTOMATIC);
	_scrolled_window.set_hexpand();

	_tree_view.append_column("nbP", _col._col_nbP);
	_tree_view.append_column("nbF", _col._col_nbF);
	_tree_view.append_column("nbT", _col._col_nbT);
	_tree_view.append_column("nbC", _col._col_nbC);
	_tree_view.append_column_numeric("Amount resource",
									 _col._col_resource, "%.2f");

	auto cell = Gtk::make_managed<Gtk::CellRendererProgress>();
	int cols_count = _tree_view.append_column("Mission completeness",
											  *cell);

	auto progress_col = _tree_view.get_column(cols_count - 1);
	if(progress_col)
	progress_col->add_attribute(cell->property_value(),
								_col._col_resource_percentage);

	
	
	Glib::signal_idle().connect(sigc::mem_fun(*this,&MyWin::on_idle));
	
	show_all_children();
	if (argc==2){
		s.init_sim(f);
		if(s.getPb()) s.clear_sim();
	}
}

MyWin::~MyWin()
{
}

void MyWin::refresh(){
	cout << "refreshing... " << ++counter_ << endl;
	m_Area.setToggleLink(m_Button_togglelink.get_active());
	m_Area.setToggleRange(m_Button_togglerange.get_active());
	auto win = get_window();
	if(win)
	{
		Gdk::Rectangle r(0,0, get_allocation().get_width(), 
						      get_allocation().get_height());					
		win->invalidate_rect(r,false);
	}
}


void MyWin::on_button_clicked_exit(){
	cout << "** Exit... **" << endl;
	exit(0);
}


void MyWin::on_button_clicked_open(){
	cout << "Open..." << endl;
	
	Gtk::FileChooserDialog dialog("Open",Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);
	dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("Open", Gtk::RESPONSE_OK);
	int result = dialog.run();
	
	switch(result){
	case(Gtk::RESPONSE_OK):
	{
	  cout << "Open clicked." << endl; // remove
	  s.clear_sim();
	  string filename = dialog.get_filename();
	  s.init_sim(filename);
	  if(s.getPb()) s.clear_sim();
	  break;
	}
	case(Gtk::RESPONSE_CANCEL):
	{
	  cout << "Cancel clicked." << endl; // remove
	  break;
	}
	default:
	{
	  cout << "Unexpected button clicked." << endl; // remove
	  break;
	}
	}
	
	refresh();
	tree_view_update();
}


void MyWin::on_button_clicked_save(){
	cout << "Save..." << endl;
	
	Gtk::FileChooserDialog dialog("Save",Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for(*this);
	dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("Save", Gtk::RESPONSE_OK);
	int result = dialog.run();
	
	switch(result){
	case(Gtk::RESPONSE_OK):
	{
	  cout << "Save clicked." << endl;
	  string filename = dialog.get_filename();
	  s.write_save(filename);
	  
	  break;
	}
	case(Gtk::RESPONSE_CANCEL):
	{
	  cout << "Cancel clicked." << endl;
	  break;
	}
	default:
	{
	  cout << "Unexpected button clicked." << endl;
	  break;
	}
	}
}


void MyWin::on_button_clicked_start(){
	if(!started_){
		cout << "Start..." << endl;
		m_Button_start.set_label("Stop");
	} else {
		cout << "Stop..." << endl;
		m_Button_start.set_label("Start");
	}
	
	started_=!started_;
}


void MyWin::on_button_clicked_step(){
	if(!started_){
		step_ = 1;
	} else {
		cout << "No step allowed..." << endl;
	}
}

void MyWin::on_button_clicked_togglelink(){
	cout << "Toggle Link" << m_Button_togglelink.get_active() << endl;
	refresh();
}

void MyWin::on_button_clicked_togglerange(){
	cout << "Toggle Range" << m_Button_togglerange.get_active() << endl;
	refresh();
}

// Keyboard signal handler:
bool MyWin::on_key_press_event(GdkEventKey * key_event){
	if(key_event->type == GDK_KEY_PRESS){
		switch(gdk_keyval_to_unicode(key_event->keyval)){
			case 's':
				on_button_clicked_start();
				break;
			case '1':
				on_button_clicked_step();
				break;
		}
	}
	
	return Gtk::Window::on_key_press_event(key_event);
}

bool MyWin::on_idle(){
	static unsigned count(0);
	if(started_){
		++count;
		s.update_sim();
		refresh();
		tree_view_update();
	}
	if(step_){
		step_ = 0;
		++count;
		s.update_sim();
		refresh();
		tree_view_update();
	}
	
	return true;  // return false when done
}

void MyWin::tree_view_update(){
	static std::vector<SimData> report;
	report.clear();
	
	Glib::RefPtr<Gtk::ListStore>ref_tree=Gtk::ListStore::create(_col);
	_tree_view.set_model(ref_tree);
	
	if(s.getEb().size()){
		for(unsigned int i(0) ; i<s.getEb().size() ; ++i){
			SimData line;
			line.nbP = s.getEb()[i]->getnbP();
			line.nbF = s.getEb()[i]->getnbF();
			line.nbT = s.getEb()[i]->getnbT();
			line.nbC = s.getEb()[i]->getnbC();
			line.ressource   = s.getEb()[i]->getStock();
			if(s.getEb()[i]->getStock()>=10000) line.ressource_p=100;
			else line.ressource_p = s.getEb()[i]->getStock()/100;
			report.push_back(line);

			auto row = *(ref_tree->append());
			row[_col._col_nbP] = line.nbP;
			row[_col._col_nbF] = line.nbF;
			row[_col._col_nbT] = line.nbT;
			row[_col._col_nbC] = line.nbC;
			row[_col._col_resource] = line.ressource;
			row[_col._col_resource_percentage] = line.ressource_p;
		}
	}
}

