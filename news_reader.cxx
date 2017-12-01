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
                        
                        int text_visual_x = 20;
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
int main() {
	const int xy = 0;

	const int workarea_w = Fl::w();
	const int workarea_h = Fl::h();

        /*
                Not using any of this but is a good way to 
                determine how much of the system generated 
                title bar is used up in vertical pixels.
        */
        //int screen_x = 0;
        //int screen_y = 0;
        //int screen_w = 0;
        //int screen_h = 0;

        //Fl::screen_xywh(screen_x, screen_y, screen_w, screen_h);

        //int window_chrome_offset = screen_h - workarea_h;

        vector<visualcallable> callables;
        
        int next_y = 0;
        int text_w = 0;
        int text_h = 0;
        int accumulated_h = 0;

        int x = 0;
        int y = 0;
        int w = 0;
        int h = 0;

        int rh = 0;

        const int max_elems = 6;
        cout << "workarea_h " << workarea_h << "\n";
        for(int index = 0; index < max_elems; index++) {
                visualcallable callable(index);

                x = 0;
                y = next_y;
                w = workarea_w;

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
                                h = (workarea_h) / 2;
                        }
                        break;
                        case 2://RSS Reader article content
                        {
                                double dv = 1.4;
                                h = (workarea_h - accumulated_h)/dv;
                        }
                        break;
                        case 3:
                        {
                                double dv = 4.6;
                                rh = (workarea_h - accumulated_h)/dv;
                                h = rh;
                                cout << "rh " << rh << "\n";
                                callable.label("test 1");
                        }
                        break;
                        case 4:
                        {
                                int dv = 3;
                                h = rh;
                                callable.label("test 2");
                        }
                        break;
                        case 5:
                        {
                                int dv = 3;
                                h = rh;
                                
                                if((accumulated_h + h) < workarea_h) {
                                        cout << "compare workarea_h to h + accumulated_h " << workarea_h << " " << (accumulated_h + h) << "\n";
                                        int additional_area_h = workarea_h-accumulated_h;

                                        cout << "additional_area " << additional_area_h << " last_h " << h << "\n";
                                        
                                        h = additional_area_h;

                                        cout << "final last h " << h << "\n";
                                }
                                
                                callable.label("test 3");
                        }
                        break;
                }

                callable.x(x);
                callable.y(y);
                callable.w(w);
                callable.h(abs(h));
                
                accumulated_h = (accumulated_h + callable.h());
                next_y = (next_y + callable.h());

                cout << index << ": accumulated_h " << accumulated_h << "\n";

                callables.push_back(callable);
        }

        Fl_Double_Window visual_window(xy, xy, workarea_w, workarea_h);
        visual_window.end();

	visual_window.size_range(480, 320, workarea_w, workarea_h);
        visual_window.label("RSS Reader");
        
        vector<shared_ptr<Fl_Widget>> widgets;
        
        for(visualcallable callable : callables) {
                int index = callable.id();
                string label = callable.label();
                auto label_text = label.data();

                x = callable.x();
                y = callable.y();
                w = callable.w();
                h = callable.h();
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

        for(auto widget : widgets) {
                visual_window.add(widget.get());
        }

        visual_window.show();
        visual_window.redraw();

        int last_w = visual_window.w();
        int last_h = visual_window.h();

	while(Fl::check()) {
		int new_w = visual_window.w();
		int new_h = visual_window.h();

		if(last_h != new_h || last_w != new_w) {
			last_h = new_h;
			last_w = new_w;

			visual_window.redraw();
		}		
	}
        
	return 0;
}
