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
#include <map>

#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>

#include <dlib/geometry.h>

#include "visualcallable.hxx"
#include "request.hxx"
#include "material.hxx"

#include "interactionstate.hxx"

namespace rss {
namespace ui {
        using namespace std;
        class mainscreengenerator {
                public:
	        using interactionstate = visualfunc::formulation::InteractionState;
	        using interaction_callback_type = void(*)(interactionstate&);

                mainscreengenerator();
                ~mainscreengenerator();
                void init();
                void generate();

                /*Application Logic*/
                void process_updates(interactionstate& interactionState);

                private:
                /*Text Input*/
                string
                        _text_buffer_feed_name,
                        _text_buffer_feed_url
                        = "";

                string* 
                        _text_buffer_feed_entry = nullptr;

                int
                        _text_buffer_feed_edit_index = -1,

                        _text_buffer_feed_name_pos,
                        _text_buffer_feed_url_pos,
                        _text_buffer_feed_pos,

                        _text_buffer_feed_name_selection_pos1,
                        _text_buffer_feed_name_selection_pos2,
                        _text_buffer_feed_selection_pos1,

                        _text_buffer_feed_url_selection_pos1,
                        _text_buffer_feed_url_selection_pos2,
                        _text_buffer_feed_selection_pos2
                        = 0;

                double
                        _text_buffer_feed_name_x,
                        _text_buffer_feed_url_x,
                        _text_buffer_feed_x
                        = 0;

                /*Graphics Engine*/
                ALLEGRO_DISPLAY* 
                        _win_ctx = nullptr;

                ALLEGRO_MONITOR_INFO 
                        _win_screen_info;

                ALLEGRO_EVENT_QUEUE* 
                        _win_msg_evt_queue = nullptr;

                ALLEGRO_EVENT_SOURCE* 
                        _win_msg_evt_src = nullptr;

                ALLEGRO_EVENT
                        _winmsg_event;


                ALLEGRO_EVENT_QUEUE* 
                        _mouse_evt_queue = nullptr;

                ALLEGRO_EVENT_SOURCE* 
                        _mouse_evt_src = nullptr;

                ALLEGRO_EVENT
                        _mouse_event;


                ALLEGRO_EVENT_QUEUE* 
                        _keyboard_evt_queue = nullptr;
                        
                ALLEGRO_EVENT_SOURCE*
                        _keyboard_evt_src = nullptr;

                ALLEGRO_EVENT
                        _keyboard_event;

                interactionstate
                        _interaction_state_init;

                interactionstate 
                        _interaction_state_last;

                interaction_callback_type 
                        _interaction_callback;

                bool 
                        _is_allegro_initialized, 
                        _is_allegro_uninitialized
                        = false;

	        void activate_allegro_graphics_engine(interaction_callback_type);

	        void initialize_allegro_graphics_engine();
	        void shutdown_allegro_graphics_engine();

                /*Application Logic Implementation*/
                bool 
                        _article_contents_enlarge,
                        _feed_articles_requested,
                        _render_is_requested,
                        _article_selected
                        = false;

                int _feed_index = 0;//Defaults to the first feed, if available;

                string _feed_names_location = "feeds.txt";

                vector<string> _feednames;
                vector<rss::material> _feed_articles;
                vector<rss::request> get_rss_feed_data(int feed_source_index, vector<material>& feed_articles);
                void get_rss_feed_names_and_articles();
                bool update_feed_source();
                
                void process_interactions(interactionstate& interaction_ctx);
                void build_visual_model(interactionstate& interaction_ctx);
                void update_visual_output(interactionstate& interaction_ctx);

                void persist_interaction_state(const interactionstate& v);
                char get_al_char_from_keycode(int keycode, bool is_keyboard_caps_on);
                /*Widget geometry and visualization*/
                enum visual_index_rss_reader_region {
                        header = 0,//RSS Reader Header
                        headlines = 1,//RSS Reader Headlines
                        article_content = 2,//RSS Reader article content
                        control_bar = 3,//RSS Reader Control Bar
                        change_bar = 4,//RSS Reader RSS Change Bar
                        choice_bar = 5//RSS Reader Feed Choice Bar
                };

                enum visual_index_rss_reader_widget_type {
                        squared_region = 0,
                        left_aligned_button = 1,
                        right_aligned_button = 2,
                        text_field = 3,
                        vertical_scrollbar = 4
                };

                vector<visualfunc::formulation::visualcallable> _callables;

                double 
                _workarea_x,
                _workarea_y,
                _workarea_w,
                _workarea_h,

                _screen_x,
                _screen_y,
                _screen_w,
                _screen_h,

                _screen_dpi
                = 0;

                bool _processing = false;

                int 
                _headline_index = -1;

                const int _default_widget_font_size = 12;
                const double _default_label_margin_left = 4;

                const char* _font_file_location = "NotoSans-Regular.ttf";
                
                int _default_font_size = 10;

                map<int, ALLEGRO_FONT*> _fonts;

                double get_screen_dpi();

                ALLEGRO_FONT* load_sized_font(const int font_size, const char* font_file_location);

                dlib::drectangle measure_text_by_sized_font(const char* str, int font_size, const char* font_file_location);
                constexpr double measure_font_y_offset(const double y1, const double y2, const double h);
                constexpr double measure_widget_y(const double region_h_half, const double button_h);
                void measure_screen(interactionstate& interaction_ctx);

                vector<visualfunc::formulation::visualcallable> get_visual_definitions(int screen_x, int screen_y, int screen_w, int screen_h);

                visualfunc::formulation::visualcallable build_visual_vertical_scrollbar(const double x1, const double y1, const double x2, const double y2, 
                const double bdr_width, const double scrollbar_width);

                visualfunc::formulation::visualcallable build_visual_left_aligned_widget(const double x1, const double y1, const double x2, const double y2, 
                bool trim_to_label, const double x_offset, double& next_x, const double bdr_width, string label_text);

                visualfunc::formulation::visualcallable build_visual_right_aligned_button(const double x1, const double y1, const double x2, const double y2, 
                bool trim_to_label, const double bdr_width, string label_text);

                visualfunc::formulation::visualcallable build_visual_vertical_widget(const double x1, const double y1, const double x2, const double y2, 
                const double y_offset, double& next_y, const double bdr_width, string label_text);

                void draw_region_background(const double x1, const double y1, const double x2, const double y2, 
                ALLEGRO_COLOR& bkg_clr, ALLEGRO_COLOR& bdr_clr, const double bdr_width);

                void draw_scrollbar_right_background(const double x1, const double y1, const double x2, const double y2, 
                ALLEGRO_COLOR& bkg_clr, ALLEGRO_COLOR& bdr_clr, const double bdr_width, const double scrollbar_width);

                void draw_left_aligned_widget(const double x1, const double y1, const double x2, const double y2, 
                ALLEGRO_COLOR& bkg_clr, ALLEGRO_COLOR& bdr_clr, const double bdr_width, string label_text, ALLEGRO_COLOR& label_color);

                void draw_right_aligned_button(const double x1, const double y1, const double x2, const double y2, 
                ALLEGRO_COLOR& bkg_clr, ALLEGRO_COLOR& bdr_clr, const double bdr_width, string label_text, ALLEGRO_COLOR& label_color);

                void draw_visual_vertical_widget(const double x1, const double y1, const double x2, const double y2, 
                ALLEGRO_COLOR& bkg_clr, ALLEGRO_COLOR& bdr_clr, const double bdr_width, string label_text, ALLEGRO_COLOR& label_color);
        };
}
}
#endif
