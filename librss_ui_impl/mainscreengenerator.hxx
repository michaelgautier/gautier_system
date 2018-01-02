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

#include <FL/Fl_Widget.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>

#include <FL/Fl_Output.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Help_View.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Pack.H>

#include "mainscreengenerator.hxx"

#include "visualcallable.hxx"
#include "request.hxx"
#include "material.hxx"

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
                void init();
                void generate();
                vector<rss::request> get_rss_feed_data(int feed_source_index);
                void resize_workarea();
                void update_screen();
                vector<rss::request> display_feed_source_headlines(int feed_source_index);
                void display_feed_buttons(const vector<string> feednames);

                enum visual_index_rss_reader_region {
                        header = 0,//RSS Reader Header
                        headlines = 1,//RSS Reader Headlines
                        article_content = 2,//RSS Reader article content
                        control_bar = 3,//RSS Reader Control Bar
                        change_bar = 4,//RSS Reader RSS Change Bar
                        choice_bar = 5//RSS Reader Feed Choice Bar
                };

                bool _article_contents_enlarge = false;
                bool _article_contents_enlarge_click = false;
                string _feed_names_location = "feeds.txt";

                vector<rss::material> _feed_articles;
                visual_type_work_area_region _workarea_region;
                int _feed_index = 0;

                void feed_items_callback(Fl_Widget* widget);
                void feed_source_callback(Fl_Widget* widget);
                void feed_contents_enlarge_callback(Fl_Widget* widget);
                void feed_setup_callback(Fl_Widget* widget);

                private:

                const int _text_wh_plus = 20;

                int _text_x = 0;
                int _text_y = 0;
                int _text_w = 0;
                int _text_h = 0;

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


                bool _feed_articles_requested = false;

                visual_type_window _visual_window;

                visual_type_window get_window(int x, int y, int w, int h, int w_lo, int h_lo, string label);
                vector<shared_ptr<Fl_Widget>> get_widgets(const vector<visualfunc::formulation::visualcallable> & callables);

                constexpr int measure_button_y(const int region_h_half, const int button_h) {
                        return (region_h_half - (button_h/2));
                }

                constexpr int measure_button_w(const int text_w, const int button_spacing) {
                        /*
                                FLTK does not properly measure text width in all cases.
                                On the machine I tested this on, 15.6" screen, 4K resolution,
                                I detected a pattern in which the count is missing another 1/4th pixels.
                        */
                        return (text_w + (text_w/4)) + button_spacing;
                }

                visual_type_rss_header get_visual_rss_header_display();
                visual_type_rss_headlines get_visual_rss_headlines_display();
                visual_type_rss_article_content get_visual_rss_article_contents_display();
                visual_type_rss_control_bar get_visual_rss_control_bar_display();
                visual_type_rss_change_bar get_visual_rss_change_bar_display();
                visual_type_rss_choice_bar get_visual_rss_choice_display();

                void measure_screen();
                vector<visualfunc::formulation::visualcallable> get_visual_definitions(int screen_x, int screen_y, int screen_w, int screen_h);

                void clear(Fl_Group*);
                void show();
        };
}
}
#endif
