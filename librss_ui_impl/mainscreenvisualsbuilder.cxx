#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/fl_draw.H>

#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Help_View.H>
#include <FL/Fl_Hold_Browser.H>

#include "visualcallable.hxx"
#include "mainscreenvisualsbuilder.hxx"
#include "mainscreenblueprint.hxx"
#include "mainscreenheaderbar.hxx"

using namespace std;
using cls = rss::ui::mainscreenvisualsbuilder;
using visualcallable = visualfunc::formulation::visualcallable;
using mainscreenblueprint = rss::ui::mainscreenblueprint;

vector<shared_ptr<Fl_Widget>> cls::get_widgets(const vector<visualcallable> & callables) {
        vector<shared_ptr<Fl_Widget>> widgets;

        for(visualcallable callable : callables) {
                const int index = callable.id();

                /*
                        Raw pointer necessary.
                        Quickly tried both C++14 smart pointers and both 
                        resulted in text that was missing.
                        
                        Without a raw pointer the text is garbled.
                        
                        FLTK is accessing string data such that this approach 
                        is required.
                */
                auto label = new string(callable.label());

                auto label_text = label->data();

                const int x = callable.x();
                const int y = callable.y();
                const int w = callable.w();
                const int h = callable.h();

                switch(index) {
                        case 0://RSS Reader Header
                        {
                                widgets.emplace_back(shared_ptr<Fl_Widget>(new mainscreenheaderbar(x, y, w, h, label_text)));
                        }
                        break;
                        case 1://RSS Reader Headlines
                        {
                                widgets.emplace_back(shared_ptr<Fl_Widget>(new Fl_Hold_Browser(x, y, w, h, label_text)));
                        }
                        break;
                        case 2://RSS Reader article content
                        {
                                widgets.emplace_back(shared_ptr<Fl_Widget>(new Fl_Help_View(x, y, w, h, label_text)));
                        }
                        break;
                        case 3://RSS Reader Control Bar
                        {
                                widgets.emplace_back(shared_ptr<Fl_Widget>(new Fl_Box(x, y, w, h, label_text)));
                                auto widget = widgets.back();
                                widget->box(FL_FLAT_BOX);
                                widget->color(fl_rgb_color(200, 113, 55));
                        }
                        break;
                        case 4://RSS Reader RSS Change Bar
                        {
                                widgets.emplace_back(shared_ptr<Fl_Widget>(new Fl_Box(x, y, w, h, label_text)));
                                auto widget = widgets.back();
                                widget->box(FL_FLAT_BOX);
                                widget->color(fl_rgb_color(83, 108, 83));
                        }
                        break;
                        case 5://RSS Reader Feed Choice Bar
                        {
                                widgets.emplace_back(shared_ptr<Fl_Widget>(new Fl_Box(x, y, w, h, label_text)));
                                auto widget = widgets.back();
                                widget->box(FL_FLAT_BOX);
                                widget->color(fl_rgb_color(255, 204, 170));
                        }
                        break;
                }

                //cout << index << ": ";
                //cout << "x/y/w/h " << x << "/" << y << "/" << w << "/" << h << "\n";

                fl_font(FL_HELVETICA, 20);
                fl_color(fl_rgb_color(213, 255, 246));
        }

        return widgets;
}

void cls::update_screen() {
	const int new_w = _visual_window->w();
	const int new_h = _visual_window->h();

	if(_last_h != new_h || _last_w != new_w) {
                //cout << " last w/h " << last_w << "/" << last_h << "\n";
		_last_h = new_h;
		_last_w = new_w;

                //cout << "  new w/h " << new_w << "/" << new_h << "\n";

                //Fl::screen_work_area(workarea_x, workarea_y, workarea_w, workarea_h);
                //cout << "workarea x/y/w/h " << workarea_x << "/" << workarea_y << "/" << workarea_w << "/" << workarea_h << "\n";

                clear();

                _widgets.clear();

                mainscreenblueprint visual_blueprint(_workarea_x, _screen_y, new_w, new_h);
                
                _callables = visual_blueprint.get_visual_definitions();

                _widgets = get_widgets(_callables);

                for(auto widget : _widgets) {
                        _visual_window->add(widget.get());
                }

		_visual_window->redraw();
	}

        return;
}

unique_ptr<Fl_Double_Window> cls::get_window(int x, int y, int w, int h, int w_lo, int h_lo, string label) {
        unique_ptr<Fl_Double_Window> visual_window(new Fl_Double_Window(x, y, w, h));
        visual_window->end();

	visual_window->size_range(w_lo, h_lo, w, h);
        visual_window->label(label.data());

        return visual_window;
}

void cls::show() {
        _visual_window->show();
        _visual_window->redraw();

        _last_w = _visual_window->w();
        _last_h = _visual_window->h();
        
        return;
}

void cls::clear() {
        /*
                Segmentation fault if you don't do exactly this. You can model 
                it by changing visual_window to a raw pointer compare 
                visual_window to raw pointer and then delete visual_window 
                (or don't). Whether you delete or not, you will get a 
                segmentation fault because the order of deallocation may occur 
                counter-intuitively. If vector<*> is cleared before 
                FLTK window, then when FLTK attempts to recursively 
                deallocate, it accesses pointers without checking they are 
                valid. Anyway, this can be controlled by removing the widgets 
                from the window first (do not call window.clear()). Next, 
                use clear on the vector to allow the standard library to 
                reclaim them (they were allocated through a call to 
                emplace_back after all).
                
                What this reveals is that smart pointers are not 100% smart 
                and some knowledge about raw pointers is still necessary. You 
                cannot just code without thinking about the memory model once 
                you include third-party libraries that may work differently 
                than the latest espoused practice in C++ coding.
        */
        const int widgetcount = _visual_window->children();
        
        for(int index = 0; index < widgetcount; index++) {
                _visual_window->remove(index);
        }

        return;
}
