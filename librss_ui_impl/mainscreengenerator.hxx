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

#ifndef __rss_ui_mainscreengenerator__
#define __rss_ui_mainscreengenerator__

#include <vector>
#include <memory>

#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Help_View.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Input.H>

#include "mainscreengenerator.hxx"

#include "visualcallable.hxx"
#include "collector.hxx"
#include "request.hxx"
#include "material.hxx"
#include "feedscycle.hxx"

using visual_type_window = Fl_Double_Window*;
using visual_type_rss_header = Fl_Output*;
using visual_type_rss_headlines = Fl_Hold_Browser*;
using visual_type_rss_article_content = Fl_Help_View*;
using visual_type_rss_control_bar = Fl_Group*;
using visual_type_rss_change_bar = Fl_Group*;
using visual_type_rss_choice_bar = Fl_Group*;
using visual_type_work_area_region = Fl_Pack*;

namespace rss {
namespace ui {
        using namespace std;
        class mainscreengenerator {
                public:
                void generate();
                void get_rss_feed_data(int feed_source_index);
                void resize_workarea();

                private:

                visual_type_window _visual_window;

                visual_type_window get_window(int x, int y, int w, int h, int w_lo, int h_lo, string label);
                vector<shared_ptr<Fl_Widget>> get_widgets(const vector<visualfunc::formulation::visualcallable> & callables);

                void measure_screen();
                vector<visualfunc::formulation::visualcallable> get_visual_definitions(int screen_x, int screen_y, int screen_w, int screen_h);

                void clear(Fl_Group*);
                void update_screen();
                void show();
        };
}
}
#endif
