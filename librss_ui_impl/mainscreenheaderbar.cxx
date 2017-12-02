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
#include "mainscreenheaderbar.hxx"

using namespace std;
using cls = rss::ui::mainscreenheaderbar;

void cls::draw() {
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
int cls::handle(int event) {
        //redraw();
        return 1;
}
//                void cls::hide() {
//                        return;
//                }
//                void cls::resize(int x, int y, int w, int h) {
//                        return;
//                }
//                void cls::show() {
//                        return;
//                }

