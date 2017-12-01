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
#include <ios>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <memory>

#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/names.h>
#include <FL/abi-version.h>
#include <FL/Fl_Export.H>
#include <FL/fl_types.h>
#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Help_View.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Tile.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Widget.H>

#include "material.hxx"
#include "request.hxx"
#include "collector.hxx"

using material = rss::material;
using request = rss::request;
using collector = rss::collector;

using namespace std;

class mainscreenheaderbar : public Fl_Widget {
        public:
                mainscreenheaderbar(int x, int y, int w, int h, const char *label = 0) : Fl_Widget(x, y, w, h, label) {
                        box(FL_FLAT_BOX);

                        return;
                }
        protected:
                void draw() {
                        /*
                                Thanks to Erco's Cheat Sheet page http://seriss.com/people/erco/fltk/
                                That page was far more clear than the FLTK page discussing drawing things.
                        */
                        string label_text = "RSS Reader";
                        fl_draw_box(FL_FLAT_BOX, x(), y(), w(), h(), fl_rgb_color(212, 85, 0));
                        
                        //cout << "draw() x/y/w/h " << x() << "/" << y() << "/" << w() << "/" << h() << "\n";
                        
                        fl_font(FL_HELVETICA, 72);
                        fl_color(fl_rgb_color(213, 255, 246));

                        int text_x = 0;
                        int text_y = 0;
                        int text_w = 0;
                        int text_h = 0;
                        
                        fl_text_extents(label_text.data(), text_x, text_y, text_w, text_h);

                        //cout << "draw() text x/y/w/h " << text_x << "/" << text_y << "/" << text_w << "/" << text_h << "\n";
                        
                        const int text_visual_x = 20;
                        int text_visual_y = h();
                        
                        if(abs(text_y) > 0) {
                                int text_y_offset = abs(h() - abs(text_y))/2;

                                text_visual_y = h()-text_y_offset;
                        }
                        
                        fl_draw(label_text.data(), text_visual_x, text_visual_y);

                        return;
                }
                int handle(int event) {
                        //redraw();
                        return 1;
                }
//                void hide() {
//                        return;
//                }
//                void resize(int x, int y, int w, int h) {
//                        return;
//                }
//                void show() {
//                        return;
//                }
};
class visualcallable {
        private:
                int _id = 0;
                int _x = 0;
                int _y = 0;
                int _w = 0;
                int _h = 0;
                string _label;
        public:
                visualcallable(int id) {
                        _id = id;
                        
                        return;
                }
                
                int id() {
                        return _id;
                }
                
                void id(int v) {
                        _id = v;
                        
                        return;
                }
                
                int x() {
                        return _x;
                }
                
                void x(int v) {
                        _x = v;
                        
                        return;
                }
                
                int y() {
                        return _y;
                }
                
                void y(int v) {
                        _y = v;
                        
                        return;
                }
                
                int w() {
                        return _w;
                }
                
                void w(int v) {
                        _w = v;
                        
                        return;
                }
                
                int h() {
                        return _h;
                }
                
                void h(int v) {
                        _h = v;
                        
                        return;
                }
                
                string label() {
                        return _label;
                }
                
                void label(string v) {
                        _label = v;
                        
                        return;
                }
};
auto get_callables(int screen_x, int screen_y, int screen_w, int screen_h) {
        vector<visualcallable> callables;
        
        int next_y = 0;
        int text_w = 0;
        int text_h = 0;
        int accumulated_h = 0;
        int remaining_h = 0;

        int x = 0;
        int y = 0;
        int w = 0;
        int h = 0;

        int rh = 0;

        const int max_elems = 6;

        for(int index = 0; index < max_elems; index++) {
                visualcallable callable(index);

                x = 0;
                y = next_y;
                w = screen_w;

                switch(index) {
                        case 0://RSS Reader Header
                        {
                                fl_font(FL_HELVETICA, 72);
                                
                                /*
                                        Usually results in a bounding box larger than  
                                        the text based on the same font size.
                                */
                                fl_measure("W", text_w, text_h, 1);

                                callable.label("RSS Reader");

                                h = text_h;
                        }
                        break;
                        case 1://RSS Reader Headlines
                        {
                                h = remaining_h / 2;
                        }
                        break;
                        case 2://RSS Reader article content
                        {
                                double dv = 1.4;
                                h = remaining_h / dv;
                        }
                        break;
                        case 3:
                        {
                                double dv = 3;
                                rh = remaining_h / dv;

                                h = rh;

                                callable.label("test 1");
                        }
                        break;
                        case 4:
                        {
                                h = rh;

                                callable.label("test 2");
                        }
                        break;
                        case 5:
                        {
                                h = rh;
                                
                                callable.label("test 3");
                        }
                        break;
                }

                callable.x(x);
                callable.y(y);
                callable.w(w);
                callable.h(h);
                
                accumulated_h = (accumulated_h + callable.h());
                next_y = (next_y + callable.h());
                //remaining_h = ((workarea_h - window_chrome_offset) - accumulated_h);
                remaining_h = (screen_h - accumulated_h);

                cout << "index: " << index << " ";
                cout << " h: " << h;
                cout << " accumulated_h: " << accumulated_h;
                cout << " remaining_h: " << remaining_h;
                cout << "\n";

                callables.push_back(callable);
        }
        
        return callables;
}
auto get_widgets(const vector<visualcallable> & callables) {
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

                cout << index << ": ";

                switch(index) {
                        case 0://RSS Reader Header
                        {
                                cout << "RSS Reader Header setup x/y/w/h " << x << "/" << y << "/" << w << "/" << h << "\n";
                                widgets.emplace_back(shared_ptr<Fl_Widget>(new mainscreenheaderbar(x, y, w, h, label_text)));
                        }
                        break;
                        case 1://RSS Reader Headlines
                        {
                                cout << "RSS Reader Headlines setup x/y/w/h " << x << "/" << y << "/" << w << "/" << h << "\n";
                                widgets.emplace_back(shared_ptr<Fl_Widget>(new Fl_Hold_Browser(x, y, w, h, label_text)));
                        }
                        break;
                        case 2://RSS Reader article content
                        {
                                cout << "RSS Reader Article setup x/y/w/h " << x << "/" << y << "/" << w << "/" << h << "\n";
                                widgets.emplace_back(shared_ptr<Fl_Widget>(new Fl_Help_View(x, y, w, h, label_text)));
                        }
                        break;
                        case 3:
                        {
                                cout << "RSS Reader Control Bar setup x/y/w/h " << x << "/" << y << "/" << w << "/" << h << "\n";
                                widgets.emplace_back(shared_ptr<Fl_Widget>(new Fl_Box(x, y, w, h, label_text)));
                                auto widget = widgets.back();
                                widget->box(FL_FLAT_BOX);
                                widget->color(fl_rgb_color(200, 113, 55));
                        }
                        break;
                        case 4:
                        {
                                cout << "RSS Reader RSS Change Bar setup x/y/w/h " << x << "/" << y << "/" << w << "/" << h << "\n";
                                widgets.emplace_back(shared_ptr<Fl_Widget>(new Fl_Box(x, y, w, h, label_text)));
                                auto widget = widgets.back();
                                widget->box(FL_FLAT_BOX);
                                widget->color(fl_rgb_color(83, 108, 83));
                        }
                        break;
                        case 5:
                        {
                                cout << "RSS Reader Feed Choice Bar setup x/y/w/h " << x << "/" << y << "/" << w << "/" << h << "\n";
                                widgets.emplace_back(shared_ptr<Fl_Widget>(new Fl_Box(x, y, w, h, label_text)));
                                auto widget = widgets.back();
                                widget->box(FL_FLAT_BOX);
                                widget->color(fl_rgb_color(255, 204, 170));
                        }
                        break;
                }

                fl_font(FL_HELVETICA, 20);
                fl_color(fl_rgb_color(213, 255, 246));
        }

        return widgets;
}
auto get_window(int x, int y, int w, int h, int w_lo, int h_lo, string label) {
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
int main() {
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

        auto visual_window = get_window(0, 0, workarea_w, workarea_h, 320, 480, "RSS Reader");

        visual_window->show();
        visual_window->redraw();

        int last_w = visual_window->w();
        int last_h = visual_window->h();

        cout << "last w/h before " << last_w << "/" << last_h << "\n";

        auto callables = get_callables(workarea_x, screen_y, workarea_w, (workarea_h - workarea_y));
        
        auto widgets = get_widgets(callables);

	while(Fl::check()) {
		const int new_w = visual_window->w();
		const int new_h = visual_window->h();

		if(last_h != new_h || last_w != new_w) {
			last_h = new_h;
			last_w = new_w;

//                        cout << " last w/h " << last_w << "/" << last_h << "\n";
//                        cout << "  new w/h " << new_w << "/" << new_h << "\n";

//                        Fl::screen_work_area(workarea_x, workarea_y, workarea_w, workarea_h);
//                        cout << "workarea x/y/w/h " << workarea_x << "/" << workarea_y << "/" << workarea_w << "/" << workarea_h << "\n";

                        clear_window_widgets(visual_window);

                        widgets.clear();

                        callables = get_callables(workarea_x, screen_y, new_w, new_h);
                        
                        widgets = get_widgets(callables);

                        for(auto widget : widgets) {
                                visual_window->add(widget.get());
                        }

			visual_window->redraw();
		}		
	}

        /*
                Segmentation fault if you don't do exactly this.
                You can model it by changing visual_window to a raw pointer
                use an compare visual_window to raw pointer and then 
                delete visual_window (or don't). Whether you delete or not,
                you will get a segmentation fault because the order of 
                deallocation may occur counter-intuitively.
                If vector<*> is cleared before FLTK window, then when 
                FLTK attempts to recursively deallocate, it accesses 
                pointers without checking they are valid.
                Anyway, this can be controlled by removing the widgets 
                from the window first (do not call window.clear()).
                Next, use clear on the vector to allow the standard 
                library to reclaim them (they were allocated through a 
                call to emplace_back after all).
                
                What this reveals is that smart pointers are not 100% smart 
                and some knowledge about raw pointers is still necessary.
        */
        clear_window_widgets(visual_window);

        widgets.clear();
        
	return 0;
}
