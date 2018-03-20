/*
Copyright 2018 Michael Gautier

This file is part of Gautier RSS System by Michael Gautier.

Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.

Portions of the POCO C++ Libraries utilize the following copyrighted material, the use of which is hereby acknowledged.
POCO C++ Libraries released under the Boost Software License; Copyright 2018, Applied Informatics Software Engineering GmbH and Contributors;
C++ Standard Library; Copyright 2018 Standard C++ Foundation.
*/
#include <iostream>
#include <chrono>
#include <ctime>
#include <cctype>

#include "mainscreengenerator.hxx"

#include "collector.hxx"
#include "feedscycle.hxx"

using collector = ::rss::collector;
using feedscycle = ::rss::feedscycle;

using namespace std;
using cls = ::rss::ui::mainscreengenerator;

cls* _self = nullptr;

void UpdateDisplay(::visualfunc::formulation::InteractionState* interaction_ctx);
void clear_to_background_color();

cls::mainscreengenerator() {
    _self = this;

    _interaction_ctx = new interactionstate();

    _keyboardtr._texts.emplace_back(textbuffer());
    _keyboardtr._texts.emplace_back(textbuffer());

    return;
}

cls::~mainscreengenerator() {
    return;
}

void cls::init() {
    _render_is_requested = true;

    return;
}

void cls::generate() {
    _article_contents_enlarge = false;
    get_rss_feed_names_and_articles();

    _feed_articles_requested = true;

    _uiengine.initialize_allegro_graphics_engine(_interaction_ctx, &_keyboardtr);
    _uiengine.activate_allegro_graphics_engine(_interaction_ctx, UpdateDisplay);

    return;
}

void UpdateDisplay(::visualfunc::formulation::InteractionState* interaction_ctx) {
    _self->process_updates(interaction_ctx);

    return;
}

void cls::process_updates(interactionstate* interaction_ctx) {
    if(!_processing) {
        _processing = true;

        if(interaction_ctx->IsWindowResized) {
            interaction_ctx->IsWindowResized = false;
            _render_is_requested = true;
        } else {
            process_interactions(interaction_ctx);
        }

        if(_render_is_requested) {
            _render_is_requested = false;

            _uiengine.measure_screen(interaction_ctx);

            //cout << __func__ << " call UpdateVisualOutput, line: " << __LINE__ << "\n";
            build_visual_model();
            update_visual_output();
        }

        _processing = false;
    }

    return;
}

void cls::process_interactions(interactionstate* interaction_ctx) {
    if(!_feed_articles_requested) {
        get_rss_feed_names_and_articles();

        _feed_articles_requested = true;
    }

    const int
    mouse_direction = interaction_ctx->MouseDirection,
    mouse_button = interaction_ctx->MouseButton;

    const bool
    is_mouse_click = (interaction_ctx->IsMouseUp && _uiengine._interactionstate_previous->IsMouseDown),
    is_mouse_down = interaction_ctx->IsMouseDown,
    is_mouse_button_left = (mouse_button == 1);

    const bool
    interactions_occured = (is_mouse_click || is_mouse_down);

    if(interactions_occured) {
        /*cout << "((interaction_ctx->IsKeyUp && _inter_sts_lst->IsKeyDown) && (interaction_ctx->KeyboardKeyCode == _inter_sts_lst->KeyboardKeyCode))\n";
        cout << interaction_ctx->IsKeyUp << " " << _inter_sts_lst->IsKeyDown << " " << interaction_ctx->KeyboardKeyCode << " " << _inter_sts_lst->KeyboardKeyCode << "\n";*/

        _uiengine.persist_interaction_state(interaction_ctx);
    }

    dlib::dpoint
    mouse_position = interaction_ctx->MousePosition;

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
            dlib::drectangle button_dimensions = _uiengine.measure_text_by_sized_font("WWWWWWWWWWWWWWWW", _uiengine._default_font_size, _font_file_location);

            const double headline_h = button_dimensions.bottom();

            int headline_index = 0;

            for(double y = headline_y_start; y <= headline_y_end; y = y + headline_h) {
                dlib::drectangle headline_region(0, y, _uiengine._workarea_w, (y + headline_h));

                if(headline_region.contains(mouse_position)) {
                    _headline_index = headline_index;
                    _article_selected = true;
                    _render_is_requested = true;
                    _keyboard_field_active = false;
                    break;
                }

                headline_index++;
            }
        } else if(mouse_y >= control_bar_y_start && mouse_y <= control_bar_y_end && ctrl_bar_callable.callables().size() > 0) {
            visualcallable descendant_callable = ctrl_bar_callable.callables()[0];
            dlib::drectangle region(descendant_callable.x(), descendant_callable.y(), descendant_callable.w(), descendant_callable.h());

            if(region.contains(mouse_position)) {
                cout << "enlarge/shrink clicked\n";
                _article_contents_enlarge = !_article_contents_enlarge;
                _render_is_requested = true;
                _keyboard_field_active = false;
            }
        } else if(mouse_y >= change_bar_y_start && mouse_y <= change_bar_y_end && chng_bar_callable.callables().size() > 0) {
            int widget_index = -1;

            for(auto& descendant_callable : chng_bar_callable.callables()) {
                widget_index++;

                dlib::drectangle region(descendant_callable.x(), descendant_callable.y(), descendant_callable.w(), descendant_callable.h());

                if(region.contains(mouse_position)) {
                    if(widget_index > -1 && widget_index < 2) {
                        _keyboardtr._text_buffer_index = widget_index;
                        (&_keyboardtr._texts[_keyboardtr._text_buffer_index])->buffer_x = mouse_x;
                        _keyboard_field_active = true;
                    } else if (widget_index == 2) {
                        bool feed_sources_updated = update_feed_source();

                        if(feed_sources_updated) {

                        }
                        _keyboard_field_active = false;
                    }

                    _render_is_requested = (widget_index > -1 && widget_index < 3);
                }
            }
        } else if(mouse_y >= feed_bar_y_start && mouse_y <= feed_bar_y_end && feed_bar_callable.callables().size() > 0) {
            int feed_name_index = -1;
            bool feed_name_clicked = false;

            for(auto& descendant_callable : feed_bar_callable.callables()) {
                feed_name_index++;

                dlib::drectangle region(descendant_callable.x(), descendant_callable.y(), descendant_callable.w(), descendant_callable.h());

                if(region.contains(mouse_position)) {
                    feed_name_clicked = true;
                    _keyboard_field_active = false;
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
    }

    if(!_keyboard_field_active && _keyboardtr._text_buffer_index > -1) {
        _keyboardtr._text_buffer_index = -1;
    } else if(_keyboard_field_active) {
        int key_update_count = 0;
        /*The following technique is not perfect but it works in the limited testing conducted.*/
        const float wait_time = 0.0006;
        float elapsed_time = 0;

        while(elapsed_time < 1000000 && _keyboardtr.check_keyboard(interaction_ctx, wait_time)) {
            elapsed_time += wait_time;

            key_update_count = _keyboardtr.process_keyboard(interaction_ctx,_uiengine._interactionstate_previous);

            _uiengine.persist_interaction_state(interaction_ctx);
        }

        if(key_update_count > 0) {
            _render_is_requested = true;
        }
    }

    if(!_keyboard_field_active && interactions_occured) {
        _uiengine.persist_interaction_state(interaction_ctx);
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

void cls::build_visual_model() {
    //cache fonts
    if(_callables.empty()) {
        string text_value = "WWWWWWWWWWWWWWWWWWWWWWWW";

        for(int font_size = 2; font_size < 18; font_size++) {
            dlib::drectangle text_dimensions = _uiengine.measure_text_by_sized_font(text_value.data(), font_size, _font_file_location);
        }
    }

    _callables = get_visual_definitions(_uiengine._workarea_x, _uiengine._screen_y, _uiengine._workarea_w, _uiengine._workarea_h);

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
        case visual_index_rss_reader_region::header: { //RSS Reader Header
        }
        break;
        case visual_index_rss_reader_region::headlines: { //RSS Reader Headlines
            const int articles_size = _feed_articles.size();

            double next_y = 0;
            const double y_offset = 0;

            for(int article_index = 0; article_index < articles_size; article_index++) {
                auto feed_article_entry = _feed_articles[article_index];

                string headline = feed_article_entry.headline;

                const double widget_border_line_width = 1;

                visualcallable descendant_callable = _uiengine.build_visual_vertical_widget(x1, y1, x2, y2,
                                                     y_offset, next_y,
                                                     widget_border_line_width, headline);

                descendant_callable.type_id(interactionengine::widget_type::text_field);

                callable->add_descendant(descendant_callable);
            }

            visualcallable scrollbar_callable = _uiengine.build_visual_vertical_scrollbar(x1, y1, x2, y2, 1, scrollbar_width);
            scrollbar_callable.type_id(interactionengine::widget_type::vertical_scrollbar);

            callable->add_descendant(scrollbar_callable);
        }
        break;
        case visual_index_rss_reader_region::article_content: { //RSS Reader article content
            const int articles_size = _feed_articles.size();
            const int article_index = _headline_index;

            if(_article_selected && article_index > -1 && article_index < articles_size) {
                _article_selected = false;

                double next_x = 0;
                const double x_offset = 0;

                auto feed_article_entry = _feed_articles[article_index];

                string article_content = feed_article_entry.description;

                //cout << "build visual model " << article_content << "\n";

                visualcallable descendant_callable = _uiengine.build_visual_left_aligned_widget(x1, y1, x2, y2,
                                                     false, x_offset, next_x,
                                                     0, article_content);

                descendant_callable.type_id(interactionengine::widget_type::text_field);

                callable->add_descendant(descendant_callable);
            }

            visualcallable scrollbar_callable = _uiengine.build_visual_vertical_scrollbar(x1, y1, x2, y2, 1, scrollbar_width);
            scrollbar_callable.type_id(interactionengine::widget_type::vertical_scrollbar);

            callable->add_descendant(scrollbar_callable);
        }
        break;
        case visual_index_rss_reader_region::control_bar: { //RSS Reader Control Bar
            const double button_border_line_width = 1;

            string button_text = "Enlarge";

            if(_article_contents_enlarge) {
                button_text = "Shrink";
            }

            visualcallable descendant_callable = _uiengine.build_visual_right_aligned_button(x1, y1, x2, y2, true,
                                                 button_border_line_width, button_text);

            descendant_callable.type_id(interactionengine::widget_type::right_aligned_button);

            callable->add_descendant(descendant_callable);
        }
        break;
        case visual_index_rss_reader_region::change_bar: { //RSS Reader RSS Change Bar
            vector<string> widget_texts = {"THE LONGEST FEED NAME EVER", "HTTPS://WWWWWWWWWWWWWWWW.COM", "Update"};

            double next_x = 20;
            const double x_offset = 20;

            const double widget_border_line_width = 1;

            for(int widget_index = 0; widget_index < widget_texts.size(); widget_index++) {
                string label_text = widget_texts[widget_index];

                visualcallable descendant_callable = _uiengine.build_visual_left_aligned_widget(x1, y1, x2, y2,
                                                     (widget_index == 2), x_offset, next_x,
                                                     widget_border_line_width, label_text);

                switch(widget_index) {
                case 0:
                case 1:
                    _uiengine.update_textfield(&_keyboardtr._texts[widget_index], descendant_callable, interactionengine::widget_type::text_field);
                    break;
                case 2:
                    descendant_callable.type_id(interactionengine::widget_type::left_aligned_button);
                    break;
                }

                callable->add_descendant(descendant_callable);
            }
        }
        break;
        case visual_index_rss_reader_region::choice_bar: { //RSS Reader Feed Choice Bar
            double next_x = 20;
            const double x_offset = 20;

            const double button_border_line_width = 1;

            for(int button_index = 0; button_index < _feednames.size(); button_index++) {
                string label_text = _feednames[button_index];

                visualcallable descendant_callable = _uiengine.build_visual_left_aligned_widget(x1, y1, x2, y2,
                                                     true, x_offset, next_x,
                                                     button_border_line_width, label_text);

                descendant_callable.type_id(interactionengine::widget_type::left_aligned_button);

                callable->add_descendant(descendant_callable);
            }
        }
        break;
        }

        previous_line_stroke_width = border_line_width;
    }

    return;
}

void cls::update_visual_output() {
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
        case visual_index_rss_reader_region::header: { //RSS Reader Header
            background_color = al_map_rgb(212, 85, 0);
            border_color = al_map_rgb(255, 127, 42);
            text_color = al_map_rgb(213, 255, 246);

            _uiengine.draw_region_background(x1, y1, x2, y2, background_color, border_color, border_line_width);
        }
        break;
        case visual_index_rss_reader_region::headlines: { //RSS Reader Headlines
            background_color = al_map_rgb(255, 255, 255);
            border_color = al_map_rgb(160, 44, 44);
            text_color = al_map_rgb(213, 255, 246);

            _uiengine.draw_region_background(x1, y1, x2, y2, background_color, border_color, border_line_width);

            vector<visualcallable> descendant_callables = callable->callables();

            ALLEGRO_COLOR headline_background_color = al_map_rgb(249, 249, 249);
            ALLEGRO_COLOR headline_border_color = al_map_rgb(227, 219, 219);
            ALLEGRO_COLOR headline_text_color = al_map_rgb(0, 0, 0);

            for(auto descendant_callable : descendant_callables) {
                const int descendant_type = descendant_callable.type_id();

                if(descendant_type == interactionengine::widget_type::text_field) {
                    const double headline_border_line_width = descendant_callable.line_stroke_width();

                    const double c_x1 = descendant_callable.x();
                    const double c_x2 = _uiengine._workarea_w;
                    const double c_y1 = descendant_callable.y();
                    const double c_y2 = descendant_callable.h();

                    string headline = descendant_callable.label();

                    _uiengine.draw_visual_vertical_widget(c_x1, c_y1, c_x2, c_y2,
                                                          headline_background_color, headline_border_color, headline_border_line_width,
                                                          headline, headline_text_color);
                }
            }

            ALLEGRO_COLOR vertical_scrollbar_background_color = al_map_rgb(244, 227, 215);
            ALLEGRO_COLOR vertical_scrollbar_border_color = al_map_rgb(222, 170, 135);

            for(auto descendant_callable : descendant_callables) {
                const int descendant_type = descendant_callable.type_id();

                if(descendant_type == interactionengine::widget_type::vertical_scrollbar) {
                    const double vertical_scrollbar_border_line_width = descendant_callable.line_stroke_width();

                    const double c_x1 = descendant_callable.x();
                    const double c_x2 = descendant_callable.w();
                    const double c_y1 = descendant_callable.y();
                    const double c_y2 = descendant_callable.h();

                    _uiengine.draw_scrollbar_right_background(c_x1, c_y1, c_x2, c_y2,
                            vertical_scrollbar_background_color, vertical_scrollbar_border_color,
                            vertical_scrollbar_border_line_width, scrollbar_width);
                }
            }
        }
        break;
        case visual_index_rss_reader_region::article_content: { //RSS Reader article content
            background_color = al_map_rgb(255, 255, 255);
            border_color = al_map_rgb(160, 44, 44);
            text_color = al_map_rgb(213, 255, 246);

            _uiengine.draw_region_background(x1, y1, x2, y2, background_color, border_color, border_line_width);

            ALLEGRO_COLOR widget_background_color = al_map_rgb(255, 255, 255);
            ALLEGRO_COLOR widget_border_color = al_map_rgb(255, 255, 255);
            ALLEGRO_COLOR widget_text_color = al_map_rgb(0, 0, 0);

            vector<visualcallable> descendant_callables = callable->callables();

            for(auto descendant_callable : descendant_callables) {
                const int descendant_type = descendant_callable.type_id();

                if(descendant_type == interactionengine::widget_type::text_field) {
                    const double widget_border_line_width = 0;

                    const double c_x1 = descendant_callable.x();
                    const double c_x2 = descendant_callable.w();
                    const double c_y1 = descendant_callable.y();
                    const double c_y2 = descendant_callable.h();

                    string widget_text = descendant_callable.label();

                    //cout << "draw text " << widget_text << "\n";

                    _uiengine.draw_left_aligned_widget(c_x1, c_y1, c_x2, c_y2,
                                                       widget_background_color, widget_border_color, widget_border_line_width,
                                                       widget_text, widget_text_color);
                }
            }

            ALLEGRO_COLOR vertical_scrollbar_background_color = al_map_rgb(244, 227, 215);
            ALLEGRO_COLOR vertical_scrollbar_border_color = al_map_rgb(222, 170, 135);

            for(auto descendant_callable : descendant_callables) {
                const int descendant_type = descendant_callable.type_id();

                if(descendant_type == interactionengine::widget_type::vertical_scrollbar) {
                    const double vertical_scrollbar_border_line_width = descendant_callable.line_stroke_width();

                    const double c_x1 = descendant_callable.x();
                    const double c_x2 = descendant_callable.w();
                    const double c_y1 = descendant_callable.y();
                    const double c_y2 = descendant_callable.h();

                    _uiengine.draw_scrollbar_right_background(c_x1, c_y1, c_x2, c_y2,
                            vertical_scrollbar_background_color, vertical_scrollbar_border_color,
                            vertical_scrollbar_border_line_width, scrollbar_width);
                }
            }
        }
        break;
        case visual_index_rss_reader_region::control_bar: { //RSS Reader Control Bar
            background_color = al_map_rgb(80, 68, 22);
            border_color = al_map_rgb(128, 102, 0);
            text_color = al_map_rgb(213, 255, 246);

            _uiengine.draw_region_background(x1, y1, x2, y2, background_color, border_color, border_line_width);

            ALLEGRO_COLOR button_background_color = al_map_rgb(222, 170, 135);
            ALLEGRO_COLOR button_border_color = al_map_rgb(0,0,0);
            ALLEGRO_COLOR button_text_color = al_map_rgb(0,0,0);

            vector<visualcallable> descendant_callables = callable->callables();

            for(auto descendant_callable : descendant_callables) {
                const int descendant_type = descendant_callable.type_id();

                if(descendant_type == interactionengine::widget_type::right_aligned_button) {
                    const double button_border_line_width = descendant_callable.line_stroke_width();

                    const double c_x1 = descendant_callable.x();
                    const double c_x2 = descendant_callable.w();
                    const double c_y1 = descendant_callable.y();
                    const double c_y2 = descendant_callable.h();

                    string button_text = descendant_callable.label();

                    _uiengine.draw_right_aligned_button(c_x1, c_y1, c_x2, c_y2,
                                                        button_background_color, button_border_color, button_border_line_width, button_text, button_text_color);
                }
            }
        }
        break;
        case visual_index_rss_reader_region::change_bar: { //RSS Reader RSS Change Bar
            background_color = al_map_rgb(128, 51, 0);
            border_color = al_map_rgb(170, 68, 0);
            text_color = al_map_rgb(213, 255, 246);

            _uiengine.draw_region_background(x1, y1, x2, y2, background_color, border_color, border_line_width);

            ALLEGRO_COLOR widget_background_color = al_map_rgb(0, 0, 0);
            ALLEGRO_COLOR widget_border_color = al_map_rgb(0, 0, 0);
            ALLEGRO_COLOR widget_text_color = al_map_rgb(0, 0, 0);

            vector<visualcallable> descendant_callables = callable->callables();

            int text_field_edit_index = -1;

            for(auto descendant_callable : descendant_callables) {
                const int descendant_type = descendant_callable.type_id();

                switch(descendant_type) {
                case interactionengine::widget_type::left_aligned_button:
                case interactionengine::widget_type::text_field:

                    const double widget_border_line_width = descendant_callable.line_stroke_width();

                    const double c_x1 = descendant_callable.x();
                    const double c_x2 = descendant_callable.w();
                    const double c_y1 = descendant_callable.y();
                    const double c_y2 = descendant_callable.h();

                    string widget_text;
                    bool text_field_highlight = false;
                    bool text_field_blank = true;

                    switch(descendant_type) {
                    case interactionengine::widget_type::left_aligned_button:
                        widget_background_color = al_map_rgb(222, 170, 135);
                        widget_text = descendant_callable.label();
                        break;
                    case interactionengine::widget_type::text_field:
                        text_field_edit_index++;

                        widget_background_color = al_map_rgb(255, 255, 255);
                        widget_text = descendant_callable.label();

                        if(text_field_edit_index == _keyboardtr._text_buffer_index) {
                            textbuffer* text_buffer = &_keyboardtr._texts[_keyboardtr._text_buffer_index];

                            ALLEGRO_COLOR highlight_background_color = al_map_rgb(255, 246, 213);
                            widget_background_color = highlight_background_color;

                            text_field_highlight = true;
                            text_field_blank = text_buffer->text.empty();
                        }
                        break;
                    }

                    _uiengine.draw_left_aligned_widget(c_x1, c_y1, c_x2, c_y2,
                                                       widget_background_color, widget_border_color, widget_border_line_width,
                                                       widget_text, widget_text_color);

                    if(text_field_highlight) {
                        textbuffer* text_buffer = &_keyboardtr._texts[_keyboardtr._text_buffer_index];

                        ALLEGRO_COLOR vertical_line_color = al_map_rgb(0, 43, 34);

                        double widget_vertical_line_x = text_buffer->buffer_x;
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
        case visual_index_rss_reader_region::choice_bar: { //RSS Reader Feed Choice Bar
            background_color = al_map_rgb(170, 68, 0);
            border_color = al_map_rgb(255, 102, 0);
            text_color = al_map_rgb(213, 255, 246);

            _uiengine.draw_region_background(x1, y1, x2, y2, background_color, border_color, border_line_width);

            ALLEGRO_COLOR button_background_color = al_map_rgb(222, 170, 135);
            ALLEGRO_COLOR button_border_color = al_map_rgb(0, 0, 0);
            ALLEGRO_COLOR button_text_color = al_map_rgb(0, 0, 0);

            vector<visualcallable> descendant_callables = callable->callables();

            for(auto descendant_callable : descendant_callables) {
                const int descendant_type = descendant_callable.type_id();

                if(descendant_type == interactionengine::widget_type::left_aligned_button) {
                    const double button_border_line_width = descendant_callable.line_stroke_width();

                    const double c_x1 = descendant_callable.x();
                    const double c_x2 = descendant_callable.w();
                    const double c_y1 = descendant_callable.y();
                    const double c_y2 = descendant_callable.h();

                    string feed_name = descendant_callable.label();

                    _uiengine.draw_left_aligned_widget(c_x1, c_y1, c_x2, c_y2,
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

vector<cls::visualcallable> cls::get_visual_definitions(int screen_x, int screen_y, int screen_w, int screen_h) {
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
        callable.type_id(interactionengine::widget_type::squared_region);

        x = 0;
        y = next_y;
        w = screen_w;

        switch(index) {
        case visual_index_rss_reader_region::header: { //RSS Reader Header
            stroke_width = 1;

            callable.label("RSS Reader");

            dlib::drectangle text_dimensions = _uiengine.measure_text_by_sized_font(callable.label().data(), _uiengine._default_font_size, _font_file_location);

            text_h = text_dimensions.bottom();

            h = text_h;
        }
        break;
        case visual_index_rss_reader_region::headlines: { //RSS Reader Headlines
            stroke_width = 4;
            h = remaining_h / 2;
        }
        break;
        case visual_index_rss_reader_region::article_content: { //RSS Reader article content
            stroke_width = 4;
            double dv = 1.4;
            h = remaining_h / dv;
        }
        break;
        case visual_index_rss_reader_region::control_bar: {
            stroke_width = 2;
            double dv = 3;
            rh = remaining_h / dv;

            h = rh;

            callable.label("test 1");
        }
        break;
        case visual_index_rss_reader_region::change_bar: {
            stroke_width = 1.5;
            h = rh;

            callable.label("test 2");
        }
        break;
        case visual_index_rss_reader_region::choice_bar: {
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
