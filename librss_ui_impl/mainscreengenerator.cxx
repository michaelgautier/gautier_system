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
#include <iostream>
#include <chrono>
#include <ctime>

#include "mainscreengenerator.hxx"

#include "visualcallable.hxx"
#include "collector.hxx"
#include "request.hxx"
#include "material.hxx"
#include "feedscycle.hxx"

using collector = rss::collector;
using material = rss::material;
using request = rss::request;
using feedscycle = rss::feedscycle;
using visualcallable = visualfunc::formulation::visualcallable;

using interactionstate = visualfunc::formulation::InteractionState;

using namespace std;
using cls = rss::ui::mainscreengenerator;

static std::string 
        _DefaultWindowTitle = "Gautier RSS";

static constexpr double 
        _AvgPhysicalScreenSize = 13.667, _PrintPointSize = 72.0;

static double 
        _ScreenDpiLast = 96;

cls* _self = nullptr;

void UpdateDisplay(interactionstate& interactionState);
void clear_to_background_color();

void cls::init() {
        _render_is_requested = true;

        return;
}

void cls::generate() {
        _article_contents_enlarge = false;
        get_rss_feed_names_and_articles();

        _feed_articles_requested = true;

        activate_allegro_graphics_engine(UpdateDisplay);
        
        return;
}

void UpdateDisplay(interactionstate& interactionState) {
        _self->process_updates(interactionState);

	return;
}

void cls::measure_screen(interactionstate& interaction_ctx) {
        const double x = interaction_ctx.WindowDimensions.left();
        const double y = interaction_ctx.WindowDimensions.top();
        const double w = interaction_ctx.WindowWidth;
        const double h = interaction_ctx.WindowHeight;

        _workarea_x = x;
        _workarea_y = y;
        _workarea_w = w;
        _workarea_h = h;

        //cout << " workarea w   workarea h  " << _workarea_w << " " << _workarea_h << " \n";

        return;
}

void cls::process_updates(interactionstate& interaction_ctx) {
        if(!interaction_ctx.IsDisplayUpdating) {
                interaction_ctx.IsDisplayUpdating = true;

                if(interaction_ctx.IsWindowResized) {
                        interaction_ctx.IsWindowResized = false;
                        _render_is_requested = true;
                }
                else {
                        process_interactions(interaction_ctx);
                }

                if(_render_is_requested) {
                        _render_is_requested = false;
                        //cout << __func__ << " call UpdateVisualOutput, line: " << __LINE__ << "\n";
                        build_visual_model(interaction_ctx);
                        update_visual_output(interaction_ctx);
                }

                interaction_ctx.IsDisplayUpdating = false;

                _interaction_state_last = interaction_ctx;
        }

        return;
}

void cls::process_interactions(interactionstate& interaction_ctx) {
        if(!_feed_articles_requested) {
                get_rss_feed_names_and_articles();

                _feed_articles_requested = true;
        }

        const int 
                mouse_direction = interaction_ctx.MouseDirection,
                mouse_button = interaction_ctx.MouseButton;

        const bool 
                is_mouse_click = (interaction_ctx.IsMouseUp && _interaction_state_last.IsMouseDown),
                is_mouse_down = interaction_ctx.IsMouseDown,
                is_mouse_button_left = (mouse_button == 1);

        dlib::dpoint 
                mouse_position = interaction_ctx.MousePosition;

        if(is_mouse_click && _callables.size() > 4) {
                const double mouse_x = mouse_position.x();
                const double mouse_y = mouse_position.y();

                visualcallable headline_callable = _callables[visual_index_rss_reader_region::headlines];
                visualcallable ctrl_bar_callable = _callables[visual_index_rss_reader_region::control_bar];
                visualcallable chng_bar_callable = _callables[visual_index_rss_reader_region::change_bar];
                visualcallable feed_bar_callable = _callables[visual_index_rss_reader_region::choice_bar];

                const double headline_y_start = headline_callable.y();
                const double headline_y_end = (headline_callable.y() + headline_callable.h());

                const double control_bar_y_start = ctrl_bar_callable.y();
                const double control_bar_y_end = control_bar_y_start + ctrl_bar_callable.h();

                const double change_bar_y_start = chng_bar_callable.y();
                const double change_bar_y_end = change_bar_y_start + chng_bar_callable.h();

                const double feed_bar_y_start = feed_bar_callable.y();
                const double feed_bar_y_end = feed_bar_y_start + feed_bar_callable.h();

                if(mouse_y >= headline_y_start && mouse_y <= headline_y_end) {
                        dlib::drectangle button_dimensions = measure_text_by_sized_font("WWWWWWWWWWWWWWWW", _default_font_size, _font_file_location);

                        const double headline_h = button_dimensions.bottom();

                        int headline_index = 0;

                        for(double y = headline_y_start; y <= headline_y_end; y = y + headline_h) {
                                dlib::drectangle headline_region(0, y, _workarea_w, (y + headline_h));

                                if(headline_region.contains(mouse_position)) {
                                        _headline_index = headline_index;
                                        _article_selected = true;
                                        _render_is_requested = true;
                                        break;               
                                }

                                headline_index++;
                        }
                }
                else if(mouse_y >= control_bar_y_start && mouse_y <= control_bar_y_end && ctrl_bar_callable.callables().size() > 0) {
                        visualcallable descendant_callable = ctrl_bar_callable.callables()[0];
                        dlib::drectangle region(descendant_callable.x(), descendant_callable.y(), descendant_callable.w(), descendant_callable.h());
                        
                        if(region.contains(mouse_position)) {
                                cout << "enlarge/shrink clicked\n";
                                _article_contents_enlarge = !_article_contents_enlarge;
                                _render_is_requested = true;
                        }
                }
                else if(mouse_y >= change_bar_y_start && mouse_y <= change_bar_y_end && chng_bar_callable.callables().size() > 0) {
                        int widget_index = -1;

                        for(auto& descendant_callable : chng_bar_callable.callables()) {
                                widget_index++;

                                dlib::drectangle region(descendant_callable.x(), descendant_callable.y(), descendant_callable.w(), descendant_callable.h());
                                
                                if(region.contains(mouse_position)) {
                                        switch(widget_index) {
                                                case 0:
                                                        cout << "feed name text field clicked\n";
                                                        _text_buffer_feed_edit_index = 0;
                                                        _text_buffer_feed_name_x = mouse_x;

                                                        _text_buffer_feed_pos = _text_buffer_feed_name_pos;

                                                        _text_buffer_feed_selection_pos1 = _text_buffer_feed_name_selection_pos1;
                                                        _text_buffer_feed_selection_pos2 = _text_buffer_feed_name_selection_pos2;

                                                        _text_buffer_feed_x = _text_buffer_feed_name_x;
                                                        
                                                        _text_buffer_feed_entry = _text_buffer_feed_name;

                                                        _render_is_requested = true;
                                                break;
                                                case 1:
                                                        cout << "feed url text field clicked\n";
                                                        _text_buffer_feed_edit_index = 1;
                                                        _text_buffer_feed_url_x = mouse_x;

                                                        _text_buffer_feed_pos = _text_buffer_feed_url_pos;

                                                        _text_buffer_feed_selection_pos1 = _text_buffer_feed_url_selection_pos1;
                                                        _text_buffer_feed_selection_pos2 = _text_buffer_feed_url_selection_pos2;

                                                        _text_buffer_feed_x = _text_buffer_feed_url_x;
                                                        
                                                        _text_buffer_feed_entry = _text_buffer_feed_url;

                                                        _render_is_requested = true;
                                                break;
                                                case 2:
                                                        cout << "feed name/url update clicked\n";
                                                        bool feed_sources_updated = update_feed_source();
                                                        
                                                        if(feed_sources_updated) {
                                                                _render_is_requested = true;
                                                        }
                                                break;
                                        }
                                }
                        }
                }
                else if(mouse_y >= feed_bar_y_start && mouse_y <= feed_bar_y_end && feed_bar_callable.callables().size() > 0) {
                        int feed_name_index = -1;
                        bool feed_name_clicked = false;

                        for(auto& descendant_callable : feed_bar_callable.callables()) {
                                feed_name_index++;

                                dlib::drectangle region(descendant_callable.x(), descendant_callable.y(), descendant_callable.w(), descendant_callable.h());
                                
                                if(region.contains(mouse_position)) {
                                        feed_name_clicked = true;
                                        break;
                                }
                        }

                        if(feed_name_clicked && feed_name_index > -1 && feed_name_index < _feednames.size()) {
                                if(_feed_index != feed_name_index) {
                                        _feed_index = feed_name_index;

                                        vector<material> feed_articles;
                                        get_rss_feed_data(_feed_index, feed_articles);
                                        _feed_articles = feed_articles;

                                        _render_is_requested = true;
                                }
                        }
                }
                else {
                        if(_text_buffer_feed_edit_index != -1) {
                                _text_buffer_feed_edit_index = -1;
                        }
                }
        }
        
        return;
}

bool cls::update_feed_source() {
        bool feed_sources_updated = false;

        string feedname;
        string feedurl;

        if((feedname != "name a new feed" && feedurl != "new feed url") && (!feedname.empty() && !feedurl.empty())) {
                feedscycle feeds_group;
                vector<string> added_feednames = feeds_group.set_feed_name_and_address(_feed_names_location, feedname, feedurl);

                if(added_feednames.size() > 0) {
                        get_rss_feed_names_and_articles();
                        
                        feed_sources_updated = true;
                }
        }

        return feed_sources_updated;
}

void cls::build_visual_model(interactionstate& interaction_ctx) {
        measure_screen(interaction_ctx);

        //cache fonts
        if(_callables.empty()) {
                string text_value = "WWWWWWWWWWWWWWWWWWWWWWWW";

                for(int font_size = 2; font_size < 18; font_size++) {
                        dlib::drectangle text_dimensions = measure_text_by_sized_font(text_value.data(), font_size, _font_file_location);                
                }
        }

        _callables = get_visual_definitions(_workarea_x, _screen_y, _workarea_w, _workarea_h);

        const int callable_size = _callables.size();

        if(_article_contents_enlarge) {
                //cout << __func__ << " enlarge button click implementation, line: " << __LINE__ << "\n";

                visualcallable* headline_region = &_callables[visual_index_rss_reader_region::headlines];
                visualcallable* article_content = &_callables[visual_index_rss_reader_region::article_content];

                const int region_h = headline_region->h() + article_content->h();
                
                const int headlines_h = region_h/4;
                const int article_contents_h = region_h - headlines_h;
                
                headline_region->h(headlines_h);
                article_content->y(headline_region->y() + headlines_h);
                article_content->h(article_contents_h);
        }

        double previous_line_stroke_width = 0;

        double const scrollbar_width = 42;

        for(int callable_index = 0; callable_index < callable_size; callable_index++) {
                visualcallable* callable = &_callables[callable_index];

                //Allegro uses a x1/x2, y1,y2 coordinate pairs for some draw calls.
                const double x1 = callable->x1();
                const double x2 = callable->x2();
                
                const double y1 = callable->y1();
                const double y2 = callable->y2();

                //cout << "callable " << callable_index << " " << x << "/" << y << "/" << w << "/" << h << "\n";

                double const  border_line_width = callable->line_stroke_width();

                switch(callable_index) {
                        case visual_index_rss_reader_region::header://RSS Reader Header
                        {
                        }
                        break;
                        case visual_index_rss_reader_region::headlines://RSS Reader Headlines
                        {
                                const int articles_size = _feed_articles.size();

                                double next_y = 0;
                                const double y_offset = 0;

                                for(int article_index = 0; article_index < articles_size; article_index++) {
                                        auto feed_article_entry = _feed_articles[article_index];

                                        string headline = feed_article_entry.headline;
                                        
                                        const double widget_border_line_width = 1;
                                        
                                        visualcallable descendant_callable = build_visual_vertical_widget(x1, y1, x2, y2, 
                                                y_offset, next_y,
                                                 widget_border_line_width, headline);

                                        descendant_callable.type_id(visual_index_rss_reader_widget_type::text_field);

                                        callable->add_descendant(descendant_callable);                                        
                                }

                                visualcallable scrollbar_callable = build_visual_vertical_scrollbar(x1, y1, x2, y2, 1, scrollbar_width);
                                scrollbar_callable.type_id(visual_index_rss_reader_widget_type::vertical_scrollbar);
                                
                                callable->add_descendant(scrollbar_callable);
                        }
                        break;
                        case visual_index_rss_reader_region::article_content://RSS Reader article content
                        {
                                const int articles_size = _feed_articles.size();
                                const int article_index = _headline_index;

                                if(_article_selected && article_index > -1 && article_index < articles_size) {
                                        _article_selected = false;

                                        double next_x = 0;
                                        const double x_offset = 0;

                                        auto feed_article_entry = _feed_articles[article_index];

                                        string article_content = feed_article_entry.description;

                                        //cout << "build visual model " << article_content << "\n";

                                        visualcallable descendant_callable = build_visual_left_aligned_widget(x1, y1, x2, y2, 
                                                false, x_offset, next_x,
                                                0, article_content);

                                        descendant_callable.type_id(visual_index_rss_reader_widget_type::text_field);

                                        callable->add_descendant(descendant_callable);
                                }
                        
                                visualcallable scrollbar_callable = build_visual_vertical_scrollbar(x1, y1, x2, y2, 1, scrollbar_width);
                                scrollbar_callable.type_id(visual_index_rss_reader_widget_type::vertical_scrollbar);
                                
                                callable->add_descendant(scrollbar_callable);
                        }
                        break;
                        case visual_index_rss_reader_region::control_bar://RSS Reader Control Bar
                        {
                                const double button_border_line_width = 1;

                                string button_text = "Enlarge";
                                
                                if(_article_contents_enlarge) {
                                        button_text = "Shrink";
                                }

                                visualcallable descendant_callable = build_visual_right_aligned_button(x1, y1, x2, y2, true, button_border_line_width, button_text);
                                descendant_callable.type_id(visual_index_rss_reader_widget_type::right_aligned_button);
                                
                                callable->add_descendant(descendant_callable);
                        }
                        break;
                        case visual_index_rss_reader_region::change_bar://RSS Reader RSS Change Bar
                        {
                                vector<string> widget_texts = {"THE LONGEST FEED NAME EVER", "HTTPS://WWWWWWWWWWWWWWWW.COM", "Update"};

                                double next_x = 20;
                                const double x_offset = 20;
                                
                                const double widget_border_line_width = 1;

                                for(int widget_index = 0; widget_index < widget_texts.size(); widget_index++) {
                                        string label_text = widget_texts[widget_index];

                                        visualcallable descendant_callable = build_visual_left_aligned_widget(x1, y1, x2, y2, 
                                                (widget_index == 2), x_offset, next_x,
                                                widget_border_line_width, label_text);

                                        switch(widget_index) {
                                                case 0:
                                                        descendant_callable.type_id(visual_index_rss_reader_widget_type::text_field);
                                                        descendant_callable.label(_text_buffer_feed_name);
                                                break;
                                                case 1:
                                                        descendant_callable.type_id(visual_index_rss_reader_widget_type::text_field);
                                                        descendant_callable.label(_text_buffer_feed_url);                                                
                                                break;
                                                case 2:
                                                        descendant_callable.type_id(visual_index_rss_reader_widget_type::left_aligned_button);
                                                break;
                                        }

                                        callable->add_descendant(descendant_callable);
                                }
                        }
                        break;
                        case visual_index_rss_reader_region::choice_bar://RSS Reader Feed Choice Bar
                        {
                                double next_x = 20;
                                const double x_offset = 20;
                                
                                const double button_border_line_width = 1;

                                for(int button_index = 0; button_index < _feednames.size(); button_index++) {
                                        string label_text = _feednames[button_index];

                                        visualcallable descendant_callable = build_visual_left_aligned_widget(x1, y1, x2, y2, 
                                                true, x_offset, next_x,
                                                button_border_line_width, label_text);

                                        descendant_callable.type_id(visual_index_rss_reader_widget_type::left_aligned_button);

                                        callable->add_descendant(descendant_callable);
                                }
                        }
                        break;
                }

                previous_line_stroke_width = border_line_width;
        }

        return;
}

void cls::update_visual_output(interactionstate& interaction_ctx) {
        const int callable_size = _callables.size();

        double previous_line_stroke_width = 0;

        ALLEGRO_COLOR background_color = al_map_rgb(0, 0, 0);
        ALLEGRO_COLOR border_color = al_map_rgb(0, 0, 0);
        ALLEGRO_COLOR text_color = al_map_rgb(0, 0, 0);

        double const scrollbar_width = 38;

        al_clear_to_color(al_map_rgb(255, 153, 85));
        al_flip_display();

        for(int callable_index = 0; callable_index < callable_size; callable_index++) {
                visualcallable* callable = &_callables[callable_index];

                string* label = new string(callable->label());

                const char* label_text = label->data();

                delete label;

                const double x = callable->x();
                const double y = callable->y();
                const double w = callable->w();
                const double h = callable->h();

                //Allegro uses a x1/x2, y1,y2 coordinate pairs for some draw calls.
                const double x1 = callable->x1();
                const double x2 = callable->x2();
                
                const double y1 = callable->y1();
                const double y2 = callable->y2();

                //cout << "(" << callable_index << ") " << __func__ << " line " << __LINE__ << ", x/y/w/h " << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";

                double const  border_line_width = callable->line_stroke_width();

                switch(callable_index) {
                        case visual_index_rss_reader_region::header://RSS Reader Header
                        {
                                background_color = al_map_rgb(212, 85, 0);
                                border_color = al_map_rgb(255, 127, 42);
                                text_color = al_map_rgb(213, 255, 246);

                                draw_region_background(x1, y1, x2, y2, background_color, border_color, border_line_width);
                        }
                        break;
                        case visual_index_rss_reader_region::headlines://RSS Reader Headlines
                        {
                                background_color = al_map_rgb(255, 255, 255);
                                border_color = al_map_rgb(160, 44, 44);
                                text_color = al_map_rgb(213, 255, 246);

                                draw_region_background(x1, y1, x2, y2, background_color, border_color, border_line_width);

                                vector<visualcallable> descendant_callables = callable->callables();

                                ALLEGRO_COLOR headline_background_color = al_map_rgb(249, 249, 249);
                                ALLEGRO_COLOR headline_border_color = al_map_rgb(227, 219, 219);
                                ALLEGRO_COLOR headline_text_color = al_map_rgb(0, 0, 0);

                                for(auto descendant_callable : descendant_callables) {
                                        const int descendant_type = descendant_callable.type_id();

                                        if(descendant_type == visual_index_rss_reader_widget_type::text_field) {
                                                const double headline_border_line_width = descendant_callable.line_stroke_width();

                                                const double c_x1 = descendant_callable.x();
                                                const double c_x2 = _workarea_w;
                                                const double c_y1 = descendant_callable.y();
                                                const double c_y2 = descendant_callable.h();

                                                string headline = descendant_callable.label();

                                                draw_visual_vertical_widget(c_x1, c_y1, c_x2, c_y2, 
                                                        headline_background_color, headline_border_color, headline_border_line_width, 
                                                        headline, headline_text_color);
                                        }
                                }

                                ALLEGRO_COLOR vertical_scrollbar_background_color = al_map_rgb(244, 227, 215);
                                ALLEGRO_COLOR vertical_scrollbar_border_color = al_map_rgb(222, 170, 135);

                                for(auto descendant_callable : descendant_callables) {
                                        const int descendant_type = descendant_callable.type_id();

                                        if(descendant_type == visual_index_rss_reader_widget_type::vertical_scrollbar) {
                                                const double vertical_scrollbar_border_line_width = descendant_callable.line_stroke_width();

                                                const double c_x1 = descendant_callable.x();
                                                const double c_x2 = descendant_callable.w();
                                                const double c_y1 = descendant_callable.y();
                                                const double c_y2 = descendant_callable.h();

                                                draw_scrollbar_right_background(c_x1, c_y1, c_x2, c_y2, 
                                                        vertical_scrollbar_background_color, vertical_scrollbar_border_color,
                                                        vertical_scrollbar_border_line_width, scrollbar_width);
                                        }
                                }
                        }
                        break;
                        case visual_index_rss_reader_region::article_content://RSS Reader article content
                        {
                                background_color = al_map_rgb(255, 255, 255);
                                border_color = al_map_rgb(160, 44, 44);
                                text_color = al_map_rgb(213, 255, 246);

                                draw_region_background(x1, y1, x2, y2, background_color, border_color, border_line_width);

                                ALLEGRO_COLOR widget_background_color = al_map_rgb(255, 255, 255);
                                ALLEGRO_COLOR widget_border_color = al_map_rgb(255, 255, 255);
                                ALLEGRO_COLOR widget_text_color = al_map_rgb(0, 0, 0);

                                vector<visualcallable> descendant_callables = callable->callables();

                                for(auto descendant_callable : descendant_callables) {
                                        const int descendant_type = descendant_callable.type_id();

                                        if(descendant_type == visual_index_rss_reader_widget_type::text_field) {
                                                const double widget_border_line_width = 0;

                                                const double c_x1 = descendant_callable.x();
                                                const double c_x2 = descendant_callable.w();
                                                const double c_y1 = descendant_callable.y();
                                                const double c_y2 = descendant_callable.h();

                                                string widget_text = descendant_callable.label();

                                                //cout << "draw text " << widget_text << "\n";

                                                draw_left_aligned_widget(c_x1, c_y1, c_x2, c_y2, 
                                                        widget_background_color, widget_border_color, widget_border_line_width, 
                                                        widget_text, widget_text_color);
                                        }
                                }

                                ALLEGRO_COLOR vertical_scrollbar_background_color = al_map_rgb(244, 227, 215);
                                ALLEGRO_COLOR vertical_scrollbar_border_color = al_map_rgb(222, 170, 135);

                                for(auto descendant_callable : descendant_callables) {
                                        const int descendant_type = descendant_callable.type_id();

                                        if(descendant_type == visual_index_rss_reader_widget_type::vertical_scrollbar) {
                                                const double vertical_scrollbar_border_line_width = descendant_callable.line_stroke_width();

                                                const double c_x1 = descendant_callable.x();
                                                const double c_x2 = descendant_callable.w();
                                                const double c_y1 = descendant_callable.y();
                                                const double c_y2 = descendant_callable.h();

                                                draw_scrollbar_right_background(c_x1, c_y1, c_x2, c_y2, 
                                                        vertical_scrollbar_background_color, vertical_scrollbar_border_color,
                                                        vertical_scrollbar_border_line_width, scrollbar_width);
                                        }
                                }
                        }
                        break;
                        case visual_index_rss_reader_region::control_bar://RSS Reader Control Bar
                        {
                                background_color = al_map_rgb(80, 68, 22);
                                border_color = al_map_rgb(128, 102, 0);
                                text_color = al_map_rgb(213, 255, 246);

                                draw_region_background(x1, y1, x2, y2, background_color, border_color, border_line_width);

                                ALLEGRO_COLOR button_background_color = al_map_rgb(222, 170, 135);
                                ALLEGRO_COLOR button_border_color = al_map_rgb(0,0,0);
                                ALLEGRO_COLOR button_text_color = al_map_rgb(0,0,0);

                                vector<visualcallable> descendant_callables = callable->callables();

                                for(auto descendant_callable : descendant_callables) {
                                        const int descendant_type = descendant_callable.type_id();

                                        if(descendant_type == visual_index_rss_reader_widget_type::right_aligned_button) {
                                                const double button_border_line_width = descendant_callable.line_stroke_width();

                                                const double c_x1 = descendant_callable.x();
                                                const double c_x2 = descendant_callable.w();
                                                const double c_y1 = descendant_callable.y();
                                                const double c_y2 = descendant_callable.h();

                                                string button_text = descendant_callable.label();

                                                draw_right_aligned_button(c_x1, c_y1, c_x2, c_y2, 
                                                        button_background_color, button_border_color, button_border_line_width, button_text, button_text_color);
                                        }
                                }
                        }
                        break;
                        case visual_index_rss_reader_region::change_bar://RSS Reader RSS Change Bar
                        {
                                background_color = al_map_rgb(128, 51, 0);
                                border_color = al_map_rgb(170, 68, 0);
                                text_color = al_map_rgb(213, 255, 246);

                                draw_region_background(x1, y1, x2, y2, background_color, border_color, border_line_width);

                                ALLEGRO_COLOR widget_background_color = al_map_rgb(0, 0, 0);
                                ALLEGRO_COLOR widget_border_color = al_map_rgb(0, 0, 0);
                                ALLEGRO_COLOR widget_text_color = al_map_rgb(0, 0, 0);

                                vector<visualcallable> descendant_callables = callable->callables();

                                int text_field_edit_index = -1;

                                for(auto descendant_callable : descendant_callables) {
                                        const int descendant_type = descendant_callable.type_id();

                                        switch(descendant_type) {
                                                case visual_index_rss_reader_widget_type::left_aligned_button:
                                                case visual_index_rss_reader_widget_type::text_field:

                                                        const double widget_border_line_width = descendant_callable.line_stroke_width();

                                                        const double c_x1 = descendant_callable.x();
                                                        const double c_x2 = descendant_callable.w();
                                                        const double c_y1 = descendant_callable.y();
                                                        const double c_y2 = descendant_callable.h();

                                                        string widget_text;
                                                        bool text_field_highlight = false;
                                                        bool text_field_blank = true;

                                                        switch(descendant_type) {
                                                                case visual_index_rss_reader_widget_type::left_aligned_button:
                                                                        widget_background_color = al_map_rgb(222, 170, 135);
                                                                        widget_text = descendant_callable.label();
                                                                        break;
                                                                case visual_index_rss_reader_widget_type::text_field:
                                                                        text_field_edit_index++;

                                                                        widget_background_color = al_map_rgb(255, 255, 255);
                                                                        widget_text = descendant_callable.label();

                                                                        if(text_field_edit_index == _text_buffer_feed_edit_index) {
                                                                                ALLEGRO_COLOR highlight_background_color = al_map_rgb(255, 246, 213);
                                                                                widget_background_color = highlight_background_color;

                                                                                text_field_highlight = true;
                                                                                text_field_blank = _text_buffer_feed_entry.empty();
                                                                                //_text_buffer_feed_pos = _text_buffer_feed_name_pos;

                                                                                //_text_buffer_feed_selection_pos1 = _text_buffer_feed_name_selection_pos1;
                                                                                //_text_buffer_feed_selection_pos2 = _text_buffer_feed_name_selection_pos2;

                                                                                //_text_buffer_feed_x = _text_buffer_feed_name_x;
                                                                                
                                                                                //_text_buffer_feed_entry = _text_buffer_feed_name;

                                                                        }
                                                                        break;
                                                        }

                                                        draw_left_aligned_widget(c_x1, c_y1, c_x2, c_y2, 
                                                                widget_background_color, widget_border_color, widget_border_line_width, 
                                                                widget_text, widget_text_color);

                                                        if(text_field_highlight) {
                                                                ALLEGRO_COLOR vertical_line_color = al_map_rgb(0, 43, 34);

                                                                double widget_vertical_line_x = _text_buffer_feed_x;
                                                                double widget_vertical_line_w = 1;

                                                                if(text_field_blank) {
                                                                        vertical_line_color = al_map_rgb(0, 85, 68);
                                                                        widget_vertical_line_w = 8;
                                                                        widget_vertical_line_x = c_x1 + 8;
                                                                }

                                                                al_draw_line(widget_vertical_line_x, c_y1, widget_vertical_line_x, c_y2,
                                                                vertical_line_color, widget_vertical_line_w);
                                                        }
                                                
                                                break;
                                        }
                                }
                        }
                        break;
                        case visual_index_rss_reader_region::choice_bar://RSS Reader Feed Choice Bar
                        {
                                background_color = al_map_rgb(170, 68, 0);
                                border_color = al_map_rgb(255, 102, 0);
                                text_color = al_map_rgb(213, 255, 246);

                                draw_region_background(x1, y1, x2, y2, background_color, border_color, border_line_width);
                                
                                ALLEGRO_COLOR button_background_color = al_map_rgb(222, 170, 135);
                                ALLEGRO_COLOR button_border_color = al_map_rgb(0, 0, 0);
                                ALLEGRO_COLOR button_text_color = al_map_rgb(0, 0, 0);

                                vector<visualcallable> descendant_callables = callable->callables();

                                for(auto descendant_callable : descendant_callables) {
                                        const int descendant_type = descendant_callable.type_id();

                                        if(descendant_type == visual_index_rss_reader_widget_type::left_aligned_button) {
                                                const double button_border_line_width = descendant_callable.line_stroke_width();

                                                const double c_x1 = descendant_callable.x();
                                                const double c_x2 = descendant_callable.w();
                                                const double c_y1 = descendant_callable.y();
                                                const double c_y2 = descendant_callable.h();

                                                string feed_name = descendant_callable.label();

                                                draw_left_aligned_widget(c_x1, c_y1, c_x2, c_y2, 
                                                        button_background_color, button_border_color, button_border_line_width, 
                                                        feed_name, button_text_color);
                                        }
                                }
                        }
                        break;
                }

                previous_line_stroke_width = border_line_width;
        }

        al_flip_display();
        interaction_ctx.IsDisplayUpdated = true;

        return;
}

void cls::draw_region_background(const double x1, const double y1, const double x2, const double y2, 
        ALLEGRO_COLOR& bkg_clr, ALLEGRO_COLOR& bdr_clr, const double bdr_width) {
        const double rect_x1 = x1 + 0.5;
        const double rect_x2 = x2 + 0.5;
        const double rect_y1 = y1 + 0.5;
        const double rect_y2 = y2 + 0.5;

        al_draw_filled_rectangle(rect_x1, rect_y1, rect_x2, rect_y2, bkg_clr);
        al_draw_rectangle(rect_x1, rect_y1, rect_x2, rect_y2, bdr_clr, bdr_width);

//        if(y1 < 4) {
//                cout << __func__ << " line " << __LINE__ << ", x/y/w/h " << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
//        }

        return;
}

visualcallable cls::build_visual_vertical_scrollbar(const double x1, const double y1, const double x2, const double y2, const double bdr_width, const double scrollbar_width) {
        /*scrollbar aligned to the right.*/

        const double vertical_scrollbar_border_line_width = bdr_width;

        const double vertical_scrollbar_h = y2;
        const double vertical_scrollbar_w = x2;

        const double vertical_scrollbar_x = (x2 - scrollbar_width);
        const double vertical_scrollbar_y = y1;

        visualcallable callable(0);
        callable.type_id(visual_index_rss_reader_widget_type::vertical_scrollbar);
        callable.x(vertical_scrollbar_x);
        callable.y(vertical_scrollbar_y);
        callable.w(vertical_scrollbar_w);
        callable.h(vertical_scrollbar_h);

        return callable;
}

void cls::draw_scrollbar_right_background(const double x1, const double y1, const double x2, const double y2, 
        ALLEGRO_COLOR& bkg_clr, ALLEGRO_COLOR& bdr_clr, const double bdr_width, const double scrollbar_width) {
        ALLEGRO_COLOR vertical_scrollbar_background_color = bkg_clr;
        ALLEGRO_COLOR vertical_scrollbar_border_color = bdr_clr;

        /*scrollbar aligned to the right.*/

        draw_region_background(x1, y1, x2, y2, 
                vertical_scrollbar_background_color, vertical_scrollbar_border_color, bdr_width);

//        if(y1 < 4) {
//                cout << __func__ << " line " << __LINE__ << ", x/y/w/h " << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
//        }

        return;
}

visualcallable cls::build_visual_left_aligned_widget(const double x1, const double y1, const double x2, const double y2, 
bool trim_to_label, const double x_offset, double& next_x, const double bdr_width, string label_text) {
        /*Widgets aligned to the left.*/

        const double widget_border_line_width = bdr_width;

        dlib::drectangle widget_dimensions = measure_text_by_sized_font(label_text.data(), _default_widget_font_size, _font_file_location);

        const double widget_h = (widget_dimensions.bottom() + y1);

        const double widget_y_offset = measure_widget_y(((y2 - y1) / 2.0), (y2 - widget_h));
        const double widget_x_offset = x_offset;

        const double widget_x = (next_x + widget_x_offset);
        const double widget_y = (widget_y_offset + y1);
        double widget_w = widget_x + widget_dimensions.right();

        if(trim_to_label && !label_text.empty()) {
                dlib::drectangle font_dimensions = measure_text_by_sized_font(label_text.data(), _default_widget_font_size/2, _font_file_location);

                const double font_y = measure_font_y_offset(y1, y2, font_dimensions.bottom());
                const double font_w = font_dimensions.right();

                //ALLEGRO_FONT* Font = load_sized_font(_default_widget_font_size/2, _font_file_location);

                widget_w = widget_x + font_w + (_default_label_margin_left * 2.0);
        }

        next_x = widget_w;

        visualcallable callable(0);
        callable.type_id(visual_index_rss_reader_widget_type::left_aligned_button);
        callable.label(label_text);
        callable.x(widget_x);
        callable.y(widget_y);
        callable.w(widget_w);
        callable.h(widget_h);

        return callable;
}

void cls::draw_left_aligned_widget(const double x1, const double y1, const double x2, const double y2,
        ALLEGRO_COLOR& bkg_clr, ALLEGRO_COLOR& bdr_clr, const double bdr_width, string label_text, ALLEGRO_COLOR& label_color) {
        ALLEGRO_COLOR widget_background_color = bkg_clr;
        ALLEGRO_COLOR widget_border_color = bdr_clr;
        ALLEGRO_COLOR widget_text_color = label_color;

        draw_region_background(x1, y1, x2, y2, 
                widget_background_color, widget_border_color, bdr_width);

        if(!label_text.empty()) {
                dlib::drectangle font_dimensions = measure_text_by_sized_font(label_text.data(), _default_widget_font_size/2, _font_file_location);

                const double font_y = measure_font_y_offset(y1, y2, font_dimensions.bottom());
                const double font_x = (x1 + _default_label_margin_left);

                //cout << __func__ << " " << __LINE__ << " \n";
                ALLEGRO_FONT* Font = load_sized_font(_default_widget_font_size/2, _font_file_location);

                if(Font) {
                        al_draw_text(Font, label_color, font_x, font_y, ALLEGRO_ALIGN_LEFT, label_text.data());
                }
                else {
                        cout << __func__ << " " << __LINE__ << " font not loaded\n";
                }
        }

//        if(y1 < 4) {
//                cout << __func__ << " line " << __LINE__ << ", x/y/w/h " << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
//        }

        return;
}

visualcallable cls::build_visual_vertical_widget(const double x1, const double y1, const double x2, const double y2, const double y_offset, double& next_y, const double bdr_width, string label_text) {
        /*Widgets aligned to the left.*/

        const double widget_border_line_width = bdr_width;

        dlib::drectangle widget_dimensions = measure_text_by_sized_font(label_text.data(), _default_widget_font_size, _font_file_location);

        const double widget_x = x1;
        const double widget_y = (y_offset + y1 + next_y);
        const double widget_h = (widget_dimensions.bottom() + (widget_y));

        //const double widget_y_offset = measure_widget_y(((y2 - y1) / 2.0), (y2 - widget_h));

        const double widget_w = widget_x + widget_dimensions.right();

        next_y = widget_y;

        visualcallable callable(0);
        callable.type_id(visual_index_rss_reader_widget_type::text_field);
        callable.label(label_text);
        callable.x(widget_x);
        callable.y(widget_y);
        callable.w(widget_w);
        callable.h(widget_h);

        //cout << "built callable x/y/w/h " << callable.x() << "/" << callable.y() << "/" << callable.w() << "/" << callable.h() << "\n";

        return callable;
}

void cls::draw_visual_vertical_widget(const double x1, const double y1, const double x2, const double y2, 
        ALLEGRO_COLOR& bkg_clr, ALLEGRO_COLOR& bdr_clr, const double bdr_width, string label_text, ALLEGRO_COLOR& label_color) {
        ALLEGRO_COLOR widget_background_color = bkg_clr;
        ALLEGRO_COLOR widget_border_color = bdr_clr;
        ALLEGRO_COLOR widget_text_color = label_color;

        draw_region_background(x1, y1, x2, y2, 
                widget_background_color, widget_border_color, bdr_width);

        if(!label_text.empty()) {
                const double font_x = (x1 + _default_label_margin_left);

                ALLEGRO_FONT* Font = load_sized_font(_default_font_size, _font_file_location);

                if(Font) {
                        al_draw_text(Font, label_color, font_x, y1, ALLEGRO_ALIGN_LEFT, label_text.data());
                }
                else {
                        cout << __func__ << " " << __LINE__ << " font not loaded\n";
                }
        }

//        if(y1 < 4) {
//                cout << __func__ << " line " << __LINE__ << ", x/y/w/h " << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
//        }

        return;
}

visualcallable cls::build_visual_right_aligned_button(const double x1, const double y1, const double x2, const double y2, 
bool trim_to_label, const double bdr_width, string label_text) {
        /*Button aligned to the right.*/

        const double button_border_line_width = bdr_width;

        dlib::drectangle button_dimensions = measure_text_by_sized_font(label_text.data(), _default_widget_font_size, _font_file_location);

        const double button_h = (button_dimensions.bottom() + y1);

        const double button_y_offset = measure_widget_y(((y2 - y1) / 2.0), (y2 - button_h));
        const double button_y = (button_y_offset + y1);

        const double button_x_offset = (x2 - 20);

        double button_x = (button_x_offset - button_dimensions.right());
        double button_w = button_x_offset;

        if(trim_to_label && !label_text.empty()) {
                dlib::drectangle font_dimensions = measure_text_by_sized_font(label_text.data(), _default_widget_font_size/2, _font_file_location);

                const double font_w = font_dimensions.right();

                //ALLEGRO_FONT* Font = load_sized_font(_default_widget_font_size/2, _font_file_location);

                button_x = ((x2 - 60) - font_w);
                button_w = (font_w + button_x) + (_default_label_margin_left * 2.0);
        }

        visualcallable callable(0);
        callable.type_id(visual_index_rss_reader_widget_type::right_aligned_button);
        callable.label(label_text);
        callable.x(button_x);
        callable.y(button_y);
        callable.w(button_w);
        callable.h(button_h);

        return callable;
}

void cls::draw_right_aligned_button(const double x1, const double y1, const double x2, const double y2, 
        ALLEGRO_COLOR& bkg_clr, ALLEGRO_COLOR& bdr_clr, const double bdr_width, string label_text, ALLEGRO_COLOR& label_color) {
        dlib::drectangle button_dimensions = measure_text_by_sized_font(label_text.data(), _default_widget_font_size, _font_file_location);

        ALLEGRO_COLOR button_background_color = bkg_clr;
        ALLEGRO_COLOR button_border_color = bdr_clr;
        ALLEGRO_COLOR button_text_color = label_color;

        /*Button aligned to the right.*/

        draw_region_background(x1, y1, x2, y2, 
                button_background_color, button_border_color, bdr_width);

        if(!label_text.empty()) {
                dlib::drectangle font_dimensions = measure_text_by_sized_font(label_text.data(), _default_widget_font_size/2, _font_file_location);

                const double font_y = measure_font_y_offset(y1, y2, font_dimensions.bottom());
                const double font_x = (x1 + _default_label_margin_left);

                //cout << __func__ << " " << __LINE__ << " \n";
                ALLEGRO_FONT* Font = load_sized_font(_default_widget_font_size/2, _font_file_location);

                if(Font) {
                        al_draw_text(Font, label_color, font_x, font_y, ALLEGRO_ALIGN_LEFT, label_text.data());
                }
                else {
                        cout << __func__ << " " << __LINE__ << " font not loaded\n";
                }
        }

//        if(y1 < 4) {
//                cout << __func__ << " line " << __LINE__ << ", x/y/w/h " << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
//        }

        return;
}

void cls::get_rss_feed_names_and_articles() {
        vector<material> feed_articles;
        vector<request> feed_parameters = get_rss_feed_data(_feed_index, feed_articles);
        _feed_articles = feed_articles;

        const int feed_source_size = feed_parameters.size();

        for(int feed_source_index = 0; feed_source_index < feed_source_size; feed_source_index++) {
                request feedsource = feed_parameters[feed_source_index];

                _feednames.push_back(feedsource.feedname);
        }

        return;
}

vector<rss::request> cls::get_rss_feed_data(int feed_source_index, vector<material>& feed_articles) {
        feedscycle feeds_group;
        
        vector<request> feed_parameters;
	feeds_group.get_feed_names_and_addresses(_feed_names_location, feed_parameters);

	if(feed_parameters.size() > 0) {
	        request feedsource = feed_parameters[feed_source_index];

                //cout << "feedsource " << feedsource.feedname << "\n";

                collector rss_requestor;
	        feed_articles = rss_requestor.pull(feedsource);
	        
	        //cout << "_feed_articles.size() " << _feed_articles.size() << "\n";
	}

        return feed_parameters;
}

/*private member implementation*/
cls::mainscreengenerator() {
        _self = this;
        initialize_allegro_graphics_engine();
        
        return;
}

cls::~mainscreengenerator() {
        shutdown_allegro_graphics_engine();
        
        return;
}

void cls::initialize_allegro_graphics_engine() {
        _interaction_state_init.WindowPosition = dlib::dpoint(0, 0);
        _interaction_state_init.WindowDimensions = dlib::drectangle(0, 0, 0, 0);

	_is_allegro_initialized = al_init();

	if(_is_allegro_initialized) {
	        al_init_primitives_addon();
		al_init_font_addon();
		al_init_ttf_addon();
		al_init_native_dialog_addon();

		al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_GENERATE_EXPOSE_EVENTS);
		al_set_new_display_option(ALLEGRO_FLOAT_COLOR, 1, ALLEGRO_SUGGEST);
		al_set_new_display_option(ALLEGRO_RENDER_METHOD, 1, ALLEGRO_SUGGEST);
		al_set_new_display_option(ALLEGRO_SINGLE_BUFFER, 0, ALLEGRO_SUGGEST);
		al_set_new_display_option(ALLEGRO_SWAP_METHOD, 2, ALLEGRO_SUGGEST);
		al_set_new_display_option(ALLEGRO_UPDATE_DISPLAY_REGION, 0, ALLEGRO_SUGGEST);
		al_set_new_display_option(ALLEGRO_CAN_DRAW_INTO_BITMAP, 0, ALLEGRO_SUGGEST);
		
		bool IsMonitorInfoAvailable = al_get_monitor_info(0, &_win_screen_info);

		if(IsMonitorInfoAvailable) {
		        _screen_w = _win_screen_info.x2 - _win_screen_info.x1;
		        _screen_h = _win_screen_info.y2 - _win_screen_info.y1;
			_interaction_state_init.MonitorWidth = _screen_w;
			_interaction_state_init.MonitorHeight = _screen_h;

			_win_ctx = al_create_display(_interaction_state_init.MonitorWidth, _interaction_state_init.MonitorHeight);

                        if(_win_ctx) {
			        _interaction_state_init.WindowWidth = al_get_display_width(_win_ctx);		
			        _interaction_state_init.WindowHeight = al_get_display_height(_win_ctx);

                                _interaction_state_init.WindowDimensions = dlib::drectangle(0, 0, _interaction_state_init.WindowWidth, _interaction_state_init.WindowHeight);

			        al_set_window_title(_win_ctx, _DefaultWindowTitle.data());
			        al_set_window_position(_win_ctx, _interaction_state_init.WindowPosition.x(), _interaction_state_init.WindowPosition.y());

			        _win_msg_evt_src = al_get_display_event_source(_win_ctx);
			        _win_msg_evt_queue = al_create_event_queue();

			        al_register_event_source(_win_msg_evt_queue, _win_msg_evt_src);

		                if(!al_is_mouse_installed()) {
			                al_install_mouse();

			                _mouse_evt_src = al_get_mouse_event_source();

			                al_register_event_source(_win_msg_evt_queue, _mouse_evt_src);
		                }
                        }
                        else {
                                std::cout << __FILE__ " " << __func__ << " " << "(" << __LINE__ << ") ";
                                std::cout << "window could not be created.\r\n";
                        }
		}
		
		if(_win_ctx) {
			_interaction_state_init.IsWindowOpen = true;
			_interaction_state_init.IsVisualModelChanged = true;
		}
	}

	return;
}

void cls::activate_allegro_graphics_engine(interaction_callback_type interactionCallBack) {
        //cout << __func__ << " line: " << __LINE__ << "\n";
        if(_interaction_state_init.IsWindowOpen && _win_msg_evt_queue) {
                _interaction_state_last = _interaction_state_init;
        }

        _interaction_callback = interactionCallBack;

        interactionstate interaction_ctx = _interaction_state_init;
        
	while(interaction_ctx.IsWindowOpen && _win_msg_evt_queue) {
                al_wait_for_event(_win_msg_evt_queue, &_winmsg_event);
	
		ALLEGRO_EVENT_TYPE window_event_type = _winmsg_event.type;

		switch (window_event_type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				interaction_ctx.IsWindowOpen = false;
			        break;
			case ALLEGRO_EVENT_DISPLAY_RESIZE:
				al_acknowledge_resize(_win_ctx);
	                        al_flush_event_queue(_win_msg_evt_queue);

				interaction_ctx.WindowWidth = _winmsg_event.display.width;
				interaction_ctx.WindowHeight = _winmsg_event.display.height;
                                interaction_ctx.WindowDimensions = dlib::drectangle(0, 0, interaction_ctx.WindowWidth, interaction_ctx.WindowHeight);

				interaction_ctx.IsWindowResized = (interaction_ctx.WindowWidth != _interaction_state_last.WindowWidth || 
				                                   interaction_ctx.WindowHeight != _interaction_state_last.WindowHeight);
				//cout << "resize " << interaction_ctx.IsWindowResized << "\n";
			        break;
	                case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
	                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
	                        interaction_ctx.IsMouseUp = (window_event_type == ALLEGRO_EVENT_MOUSE_BUTTON_UP);
	                        interaction_ctx.IsMouseDown = (window_event_type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN);
                                interaction_ctx.MousePosition = dlib::dpoint(_winmsg_event.mouse.x, _winmsg_event.mouse.y);
                                interaction_ctx.MouseButton = _winmsg_event.mouse.button;
	                        break;
	                case ALLEGRO_EVENT_MOUSE_AXES:
	                        interaction_ctx.MouseDirection = _winmsg_event.mouse.dz;
	                        break;
                }

                if(!interaction_ctx.IsWindowOpen) {
                        break;
                }
                else if(_win_ctx) {
                        interactionCallBack(interaction_ctx);
	                al_flush_event_queue(_win_msg_evt_queue);

                        interaction_ctx.IsWindowResized = false;
                        interaction_ctx.IsMouseUp = true;
                        interaction_ctx.IsMouseDown = false;
                        interaction_ctx.MouseDirection = -1;                        
                }
	}

        shutdown_allegro_graphics_engine();

	return;
}

void cls::shutdown_allegro_graphics_engine() {
        if(!_is_allegro_uninitialized) {
                if(_win_msg_evt_queue) {
                        if(_mouse_evt_src) {
	                        al_unregister_event_source(_win_msg_evt_queue, _mouse_evt_src);
	                }

                        if(_win_msg_evt_src) {
	                        al_unregister_event_source(_win_msg_evt_queue, _win_msg_evt_src);
	                }

	                al_destroy_event_queue(_win_msg_evt_queue);
	        }

                if(_win_ctx) {
	                al_destroy_display(_win_ctx);
	        }

	        _is_allegro_uninitialized = true;
	        _is_allegro_initialized = false;
	}
	
	return;
}

double cls::get_screen_dpi() {
	/*
	 * 	Calculate true screen resolution, PPI  (see wikipedia  Pixel Density
	 * 
	 * 		diagonal resolution in pixels   =   square root of ( (screen_w)^2  +  (screen_h)^2  )
	 * 
	 * 		diag_res / diagonal screen size in inches (the average of (11, 12, 13.3, 14, 15.6, and 17) )
	 */
	double diagres = std::hypot(_screen_w, _screen_h);
	const double scrdpi = diagres/_AvgPhysicalScreenSize; 

	return scrdpi;
}

ALLEGRO_FONT* cls::load_sized_font(const int font_size, const char* font_file_location) {
        /*
                Font scaling based on Paragraph #4 in the Article DPI and Device-Independent Pixels at: 
                https://msdn.microsoft.com/en-us/library/windows/desktop/ff684173(v=vs.85).aspx
        */
        ALLEGRO_FONT* Font = nullptr;

        auto font_count = _fonts.count(font_size);

        if(font_count < 1) {
                //cout << "adding font to _fonts\n";
                const double screen_dpi = get_screen_dpi();

                const double scaled_font_size = (font_size / _PrintPointSize) * screen_dpi;

                Font = al_load_font(font_file_location, scaled_font_size, 0);

                if(Font) {
                        _fonts[font_size] = Font;
                }
                else {
                        //cout << "loaded font invalid\n";
                        _fonts.erase(font_size);
                        //cout << __func__ << " " << __LINE__ << " \n";
                        load_sized_font(font_size, font_file_location);
                }
        }
        else {
                Font = _fonts[font_size];

                if(!Font) {
                        //cout << "cached font invalid\n";
                        _fonts.erase(font_size);
                        //cout << __func__ << " " << __LINE__ << " \n";
                        load_sized_font(font_size, font_file_location);
                }
        }

        return Font;
}

constexpr double cls::measure_widget_y(const double region_h_half, const double button_h) {
        return (region_h_half - (button_h / 2));
}

dlib::drectangle cls::measure_text_by_sized_font(const char* str, int font_size, const char* font_file_location) {
        dlib::drectangle rect;

        ALLEGRO_FONT* Font = nullptr;

        do {
                Font = load_sized_font(font_size, font_file_location);
        } while(!Font);

        if(Font) {
                int FontBoxX = 0, FontBoxY = 0, FontBoxW = 0, FontBoxH = 0;

                al_get_text_dimensions(Font, str, &FontBoxX, &FontBoxY, &FontBoxW, &FontBoxH);

                rect = dlib::drectangle(FontBoxX, FontBoxY, FontBoxW, FontBoxH);
        }
        else {
                cout << __func__ << " " << __LINE__ << " Font not loaded \n";
        }

        return rect;
}

constexpr double cls::measure_font_y_offset(const double y1, const double y2, const double h) {
        const double font_y_diff = y2 - y1;
        const double font_y1 = h;
        const double font_y2 = abs((y2 - y1) - h);

        double font_y = y1;
        
        if(font_y1 > font_y_diff && font_y_diff == font_y2) {
                font_y = (y1 - (font_y2 / 2.0));
        }

        return font_y;
}

vector<visualcallable> cls::get_visual_definitions(int screen_x, int screen_y, int screen_w, int screen_h) {
        vector<visualcallable> callables;
        
        double next_y = 0;
        double text_w = 0;
        double text_h = 0;
        double accumulated_h = 0;
        double remaining_h = 0;

        double x = 0;
        double y = 0;
        double w = 0;
        double h = 0;

        double stroke_width = 1;

        double rh = 0;

        const int max_elems = 6;

        for(int index = 0; index < max_elems; index++) {
                visualcallable callable(index);
                callable.type_id(visual_index_rss_reader_widget_type::squared_region);

                x = 0;
                y = next_y;
                w = screen_w;

                switch(index) {
                        case visual_index_rss_reader_region::header://RSS Reader Header
                        {
                                stroke_width = 1;
                                
                                callable.label("RSS Reader");

                                dlib::drectangle text_dimensions = measure_text_by_sized_font(callable.label().data(), _default_font_size, _font_file_location);

                                text_h = text_dimensions.bottom();

                                h = text_h;
                        }
                        break;
                        case visual_index_rss_reader_region::headlines://RSS Reader Headlines
                        {
                                stroke_width = 4;
                                h = remaining_h / 2;
                        }
                        break;
                        case visual_index_rss_reader_region::article_content://RSS Reader article content
                        {
                                stroke_width = 4;
                                double dv = 1.4;
                                h = remaining_h / dv;
                        }
                        break;
                        case visual_index_rss_reader_region::control_bar:
                        {
                                stroke_width = 2;
                                double dv = 3;
                                rh = remaining_h / dv;

                                h = rh;

                                callable.label("test 1");
                        }
                        break;
                        case visual_index_rss_reader_region::change_bar:
                        {
                                stroke_width = 1.5;
                                h = rh;

                                callable.label("test 2");
                        }
                        break;
                        case visual_index_rss_reader_region::choice_bar:
                        {
                                stroke_width = 1;
                                h = rh;
                                
                                callable.label("test 3");
                        }
                        break;
                }

                callable.x(x);
                callable.y(y);
                callable.w(w);
                callable.h(h);
                callable.line_stroke_width(stroke_width);

                accumulated_h = (accumulated_h + callable.h());
                next_y = (next_y + callable.h());
                //remaining_h = ((workarea_h - window_chrome_offset) - accumulated_h);
                remaining_h = (screen_h - accumulated_h);

                /*cout << "index: " << index << " ";
                cout << " h: " << h;
                cout << " accumulated_h: " << accumulated_h;
                cout << " remaining_h: " << remaining_h;
                cout << "\n";*/

                callables.push_back(callable);

                /*cout << "(" << index << ") " << __func__ << " line " << __LINE__ << ", x/y/w/h";
                cout << " " << callable.x();
                cout << " " << callable.y();
                cout << " " << callable.w();
                cout << " " << callable.h();
                cout << "\n";*/
        }
        
        return callables;
}
