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
#include <FL/Fl_Group.H>

#include "mainscreengenerator.hxx"

#include "visualcallable.hxx"
#include "collector.hxx"
#include "request.hxx"
#include "material.hxx"
#include "feedscycle.hxx"

namespace rss {
namespace ui {
        using namespace std;
        class mainscreengenerator {
                public:
                void generate();

                private:
                /*Data*/
                vector<rss::request> feed_parameters;

                string feed_names_location = "feeds.txt";

                rss::collector rss_requestor;

                rss::feedscycle feeds_group;

                rss::request feedsource;
                vector<rss::material> feed_articles;

                int _workarea_x = 0;
                int _workarea_y = 0;
                int _workarea_w = 0;
                int _workarea_h = 0;

                int _screen_x = 0;
                int _screen_y = 0;
                int _screen_w = 0;
                int _screen_h = 0;

	        int _last_w = 0;
	        int _last_h = 0;

	        unique_ptr<Fl_Double_Window> _visual_window;

                unique_ptr<Fl_Double_Window> get_window(int x, int y, int w, int h, int w_lo, int h_lo, string label);
                vector<shared_ptr<Fl_Widget>> get_widgets(const vector<visualfunc::formulation::visualcallable> & callables);

                void clear(Fl_Group*);
                void update_screen();
                void show();
        };
}
}
#endif
