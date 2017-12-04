/*
Copyright 2017 Michael Gautier

This file is part of Gautier RSS System by Michael Gautier.

Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.

Portions of the POCO C++ Libraries utilize the following copyrighted material, the use of which is hereby acknowledged.
POCO C++ Libraries released under the Boost Software License; Copyright 2017, Applied Informatics Software Engineering GmbH and Contributors; 
C++ Standard Library; Copyright 2017 Standard C++ Foundation.
*/

#include <string>
#include <ios>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <memory>

#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>

#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Help_View.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Tile.H>

#include "visualcallable.hxx"
#include "mainscreenblueprint.hxx"
#include "mainscreengenerator.hxx"
#include "mainscreenheaderbar.hxx"

using namespace std;
using cls = rss::ui::mainscreengenerator;

using visualcallable = visualfunc::formulation::visualcallable;
using mainscreenblueprint = rss::ui::mainscreenblueprint;
using mainscreenheaderbar = rss::ui::mainscreenheaderbar;

vector<visualcallable> get_visual_areas(int screen_x, int screen_y, int screen_w, int screen_h);
vector<shared_ptr<Fl_Widget>> get_widgets(const vector<visualcallable> & callables);
unique_ptr<Fl_Double_Window> get_window(int x, int y, int w, int h, int w_lo, int h_lo, string label);
void clear_window_widgets(unique_ptr<Fl_Double_Window>& visual_window);

void cls::generate() {
	int workarea_x = 0;
	int workarea_y = 0;
	int workarea_w = 0;
	int workarea_h = 0;

        int screen_x = 0;
        int screen_y = 0;
        int screen_w = 0;
        int screen_h = 0;

        Fl::screen_xywh(screen_x, screen_y, screen_w, screen_h);
        //cout << "screen x/y/w/h " << screen_x << "/" << screen_y << "/" << screen_w << "/" << screen_h << "\n";

        Fl::screen_work_area(workarea_x, workarea_y, workarea_w, workarea_h);
        //cout << "workarea x/y/w/h " << workarea_x << "/" << workarea_y << "/" << workarea_w << "/" << workarea_h << "\n";

        unique_ptr<Fl_Double_Window> visual_window = get_window(0, 0, workarea_w, workarea_h, 320, 480, "RSS Reader");

        visual_window->show();
        visual_window->redraw();

        int last_w = visual_window->w();
        int last_h = visual_window->h();

        //cout << "last w/h before " << last_w << "/" << last_h << "\n";

        vector<visualcallable> callables;
        
        vector<shared_ptr<Fl_Widget>> widgets;

	while(Fl::check()) {
		const int new_w = visual_window->w();
		const int new_h = visual_window->h();

		if(last_h != new_h || last_w != new_w) {
			last_h = new_h;
			last_w = new_w;

                        //cout << " last w/h " << last_w << "/" << last_h << "\n";
                        //cout << "  new w/h " << new_w << "/" << new_h << "\n";

                        //Fl::screen_work_area(workarea_x, workarea_y, workarea_w, workarea_h);
                        //cout << "workarea x/y/w/h " << workarea_x << "/" << workarea_y << "/" << workarea_w << "/" << workarea_h << "\n";

                        clear_window_widgets(visual_window);

                        widgets.clear();

                        mainscreenblueprint visual_blueprint(workarea_x, screen_y, new_w, new_h);
                        
                        callables = visual_blueprint.get_visual_definitions();

                        widgets = get_widgets(callables);

                        for(auto widget : widgets) {
                                visual_window->add(widget.get());
                        }

			visual_window->redraw();
		}		
	}

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
        clear_window_widgets(visual_window);

        widgets.clear();
}

vector<shared_ptr<Fl_Widget>> get_widgets(const vector<visualcallable> & callables) {
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

unique_ptr<Fl_Double_Window> get_window(int x, int y, int w, int h, int w_lo, int h_lo, string label) {
        unique_ptr<Fl_Double_Window> visual_window(new Fl_Double_Window(x, y, w, h));
        visual_window->end();

	visual_window->size_range(w_lo, h_lo, w, h);
        visual_window->label(label.data());

        return visual_window;
}

void clear_window_widgets(unique_ptr<Fl_Double_Window>& visual_window) {
        const int widgetcount = visual_window->children();
        
        for(int index = 0; index < widgetcount; index++) {
                visual_window->remove(index);
        }

        return;
}
