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
#include <cstring>

#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Button.H>

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

using namespace std;
using cls = rss::ui::mainscreengenerator;

cls* _self = nullptr;

void feed_items_callback_l(Fl_Widget* widget) {
        _self->feed_items_callback(widget);

        return;
}

void feed_source_callback_l(Fl_Widget* widget) {
        _self->feed_source_callback(widget);

        return;
}

void feed_contents_enlarge_callback_l(Fl_Widget* widget) {
        _self->feed_contents_enlarge_callback(widget);

        return;
}

void feed_setup_callback_l(Fl_Widget* widget) {
        _self->feed_setup_callback(widget);

        return;
}

visual_type_rss_header cls::get_visual_rss_header_display() {
        visual_type_rss_header header_region = new Fl_Output(0, 0, 0, 0);

        header_region->box(FL_FLAT_BOX);
        header_region->color(fl_rgb_color(212, 85, 0));
        header_region->value("RSS Reader");
        header_region->textfont(FL_HELVETICA);
        header_region->textsize(72);
        header_region->textcolor(fl_rgb_color(213, 255, 246));

        return header_region;
}

visual_type_rss_headlines cls::get_visual_rss_headlines_display() {
        visual_type_rss_headlines headlines_region = new Fl_Hold_Browser(0, 0, 0, 0);
        headlines_region->textfont(FL_HELVETICA);
        headlines_region->textsize(32);
        headlines_region->has_scrollbar(7);

        return headlines_region;
}

visual_type_rss_article_content cls::get_visual_rss_article_contents_display() {
        visual_type_rss_article_content article_contents_region = new Fl_Help_View(0, 0, 0, 0);
        article_contents_region->textfont(FL_HELVETICA);
        article_contents_region->textsize(36);

        return article_contents_region;
}

visual_type_rss_control_bar cls::get_visual_rss_control_bar_display() {
        visual_type_rss_control_bar rss_control_bar_region = new Fl_Group(0, 0, 0, 0);
        rss_control_bar_region->resizable(nullptr);

        return rss_control_bar_region;
}

visual_type_rss_change_bar cls::get_visual_rss_change_bar_display() {
        visual_type_rss_change_bar rss_change_bar_region = new Fl_Group(0, 0, 0, 0);
        rss_change_bar_region->resizable(nullptr);

        return rss_change_bar_region;
}

visual_type_rss_choice_bar cls::get_visual_rss_choice_display() {
        visual_type_rss_choice_bar rss_feed_choice_region = new Fl_Group(0, 0, 0, 0);
        rss_feed_choice_region->resizable(nullptr);

        return rss_feed_choice_region;
}

void cls::update_screen() {
        _visual_window->redraw();

        Fl::flush();

        return;
}

void cls::init() {
        _self = this;
        measure_screen();

        _visual_window = get_window(0, 0, _workarea_w, _workarea_h, _workarea_w/2, _workarea_h/2, "RSS Reader");
        _visual_window->color(fl_rgb_color(255, 153, 85));
	
        _workarea_region = new Fl_Pack(0, 0, _workarea_w, _workarea_h);

        _workarea_region->type(Fl_Pack::VERTICAL);

        _visual_window->add(_workarea_region);

        vector<visualcallable> callables = get_visual_definitions(_workarea_x, _screen_y, _workarea_w, _workarea_h);

        const int callable_size = callables.size();

        for(int callable_index; callable_index < callable_size; callable_index++) {
                switch(callable_index) {
                        case visual_index_rss_reader_region::header://RSS Reader Header
                        {
                                //cout << __FILE__ << " " << __func__ << ":" << __LINE__ << "\n";
                                _workarea_region->add(get_visual_rss_header_display());
                        }
                        break;
                        case visual_index_rss_reader_region::headlines://RSS Reader Headlines
                        {
                                //cout << __FILE__ << " " << __func__ << ":" << __LINE__ << "\n";
                                _workarea_region->add(get_visual_rss_headlines_display());
                        }
                        break;
                        case visual_index_rss_reader_region::article_content://RSS Reader article content
                        {
                                //cout << __FILE__ << " " << __func__ << ":" << __LINE__ << "\n";
                                _workarea_region->add(get_visual_rss_article_contents_display());
                        }
                        break;
                        case visual_index_rss_reader_region::control_bar://RSS Reader Control Bar
                        {
                                //cout << __FILE__ << " " << __func__ << ":" << __LINE__ << "\n";
                                _workarea_region->add(get_visual_rss_control_bar_display());
                        }
                        break;
                        case visual_index_rss_reader_region::change_bar://RSS Reader RSS Change Bar
                        {
                                //cout << __FILE__ << " " << __func__ << ":" << __LINE__ << "\n";
                                _workarea_region->add(get_visual_rss_change_bar_display());
                        }
                        break;
                        case visual_index_rss_reader_region::choice_bar://RSS Reader Feed Choice Bar
                        {
                                //cout << __FILE__ << " " << __func__ << ":" << __LINE__ << "\n";
                                _workarea_region->add(get_visual_rss_choice_display());
                        }
                        break;
                }
        }

        show();

        return;
}

void cls::generate() {
	while(Fl::wait()) {
                //cout << __FILE__ << " " << __func__ << ":" << __LINE__ << "\n";
	        const int new_w = _visual_window->w();
	        const int new_h = _visual_window->h();

	        if(_last_h != new_h || _last_w != new_w) {
                        //cout << __FILE__ << " " << __func__ << ":" << __LINE__ << "\n";
                        resize_workarea();
	        }
	}

        if(_visual_window) {
                //cout << __FILE__ << " " << __func__ << ":" << __LINE__ << "\n";
                clear(_visual_window->as_group());

                if(_workarea_region) {
                        delete _workarea_region;
                }

                delete _visual_window;
        }

        return;
}

void cls::resize_workarea() {
        measure_screen();

        const int new_w = _visual_window->w();
        const int new_h = _visual_window->h();

        const bool workarea_size_changed = (_last_h != new_h || _last_w != new_w);

        if(workarea_size_changed || _article_contents_enlarge_click) {
                //cout << " last w/h " << last_w << "/" << last_h << "\n";
	        _last_h = new_h;
	        _last_w = new_w;

                vector<visualcallable> callables = get_visual_definitions(_workarea_x, _screen_y, new_w, new_h);

                const int callable_size = callables.size();

                bool children_count_matches = (_workarea_region->children() == callable_size);

                for(int callable_index = 0; callable_index < callable_size; callable_index++) {
                        visualcallable callable = callables[callable_index];

                        string* label = new string(callable.label());

                        const char* label_text = label->data();

                        delete label;

                        const int x = callable.x();
                        const int y = callable.y();
                        const int w = callable.w();
                        const int h = callable.h();

                        switch(callable_index) {
                                case visual_index_rss_reader_region::header://RSS Reader Header
                                {
                                        //cout << __FILE__ << " " << __func__ << ":" << __LINE__ << "\n";
                                        _workarea_region->size(w, h);

                                        visual_type_rss_header
                                        header_region = (decltype(header_region))_workarea_region->child(callable_index);

                                        if(header_region) {
                                                header_region->size(w, h);
                                        }
                                }
                                break;
                                case visual_index_rss_reader_region::headlines://RSS Reader Headlines
                                {
                                        //cout << __FILE__ << " " << __func__ << ":" << __LINE__ << "\n";
                                        visual_type_rss_headlines 
                                        headlines_region = (decltype(headlines_region))_workarea_region->child(callable_index);

                                        if(headlines_region) {
                                                headlines_region->size(w, h);
                                        }
                                }
                                break;
                                case visual_index_rss_reader_region::article_content://RSS Reader article content
                                {
                                        //cout << __FILE__ << " " << __func__ << ":" << __LINE__ << "\n";
                                        visual_type_rss_article_content
                                        article_contents_region = (decltype(article_contents_region))_workarea_region->child(callable_index);

                                        if(article_contents_region){
                                                article_contents_region->size(w, h);
                                        }
                                }
                                break;
                                case visual_index_rss_reader_region::control_bar://RSS Reader Control Bar
                                {
                                        //cout << __FILE__ << " " << __func__ << ":" << __LINE__ << "\n";
                                        visual_type_rss_control_bar 
                                        rss_control_bar_region = (decltype(rss_control_bar_region))_workarea_region->child(callable_index);

                                        if(rss_control_bar_region){
                                                rss_control_bar_region->size(w, h);
                                        }

                                        if(rss_control_bar_region && rss_control_bar_region->children() == 0) {
                                                string label_text = "Enlarge";

                                                const int button_spacing = 12;

                                                const int button_x = 20;
                                                int button_w = 0;
                                                int button_h = 0;

                                                fl_font(FL_HELVETICA, 12);
                                                fl_measure((new string(label_text))->data(), button_w, button_h);

                                                button_w = measure_button_w(button_w, button_spacing);
                                                button_h = button_h + button_spacing;

                                                const int region_h = rss_control_bar_region->h();
                                                const int region_h_half = (region_h/2);
                                                const int button_y = measure_button_y(region_h_half, button_h);

                                                Fl_Button* enlarge_button = new Fl_Button(button_x, button_y, button_w, button_h);
                                                enlarge_button->copy_label((new string(label_text))->data());

                                                rss_control_bar_region->add(enlarge_button);
                                                enlarge_button->callback(feed_contents_enlarge_callback_l);
                                        }
                                }
                                break;
                                case visual_index_rss_reader_region::change_bar://RSS Reader RSS Change Bar
                                {
                                        //cout << __FILE__ << " " << __func__ << ":" << __LINE__ << "\n";
                                        visual_type_rss_change_bar 
                                        rss_change_bar_region = (decltype(rss_change_bar_region))_workarea_region->child(callable_index);

                                        if(rss_change_bar_region){
                                                rss_change_bar_region->size(w, h);
                                        }

                                        if(rss_change_bar_region && rss_change_bar_region->children() == 0) {
                                                const int region_w = rss_change_bar_region->w();
                                                const int region_h = rss_change_bar_region->h();
                                                const int region_h_half = (region_h/2);
                                                const int region_w_thrd = (region_w/3);

                                                const int button_spacing = 12;

                                                int widget_x = 20;
                                                int widget_y = measure_button_y(region_h_half, 32);
                                                int widget_w = region_w_thrd;
                                                int widget_h = 0;

                                                fl_font(FL_HELVETICA, 12);
                                                fl_measure("TEST WORD", widget_w, widget_h);

                                                widget_w = region_w_thrd;
                                                widget_h = widget_h + button_spacing;

                                                Fl_Input* feed_name_input = new Fl_Input(widget_x, widget_y, widget_w, widget_h);
                                                feed_name_input->maximum_size(1000);
                                                feed_name_input->value("name a new feed");

                                                widget_x = widget_x + widget_w + button_spacing;

                                                Fl_Input* feed_url_input = new Fl_Input(widget_x, widget_y, widget_w, widget_h);
                                                feed_url_input->maximum_size(100000);
                                                feed_url_input->value("new feed url");

                                                widget_x = widget_x + widget_w + button_spacing;                                        

                                                int button_x = widget_x;
                                                int button_w = 0;
                                                int button_h = 0;

                                                string label_text = "Update";

                                                fl_font(FL_HELVETICA, 12);
                                                fl_measure((new string(label_text))->data(), button_w, button_h);

                                                button_w = measure_button_w(button_w, button_spacing);
                                                button_h = button_h + button_spacing;

                                                const int button_y = measure_button_y(region_h_half, button_h);

                                                Fl_Button* update_button = new Fl_Button(button_x, button_y, button_w, button_h);
                                                update_button->copy_label((new string(label_text))->data());

                                                rss_change_bar_region->add(feed_name_input);
                                                rss_change_bar_region->add(feed_url_input);
                                                rss_change_bar_region->add(update_button);
                                                
                                                update_button->callback(feed_setup_callback_l);
                                        }
                                }
                                break;
                                case visual_index_rss_reader_region::choice_bar://RSS Reader Feed Choice Bar
                                {
                                        //cout << __FILE__ << " " << __func__ << ":" << __LINE__ << "\n";
                                        visual_type_rss_choice_bar 
                                        rss_feed_choice_region = (decltype(rss_feed_choice_region))_workarea_region->child(callable_index);

                                        if(rss_feed_choice_region) {
                                                rss_feed_choice_region->size(w, h);

                                                vector<request> feed_parameters = get_rss_feed_data(_feed_index);
                                                vector<string> feednames;
                                                
                                                const int feed_source_size = feed_parameters.size();

                                                for(int feed_source_index = 0; feed_source_index < feed_source_size; feed_source_index++) {
                                                        request feedsource = feed_parameters[feed_source_index];

                                                        feednames.push_back(feedsource.feedname);
                                                }
                                                
                                                display_feed_buttons(feednames);
                                        }
                                }
                                break;
                        }
                }
        }

        return;
}

vector<request> cls::display_feed_source_headlines(int feed_source_index) {
        //cout << __FILE__ << " " << __func__ << ":" << __LINE__ << "\n";
        visual_type_rss_headlines 
        headlines_region = (decltype(headlines_region))_workarea_region->child(visual_index_rss_reader_region::headlines);

        vector<request> feed_parameters;
        
        if(headlines_region && headlines_region->size() == 0) {
                //cout << "get_rss_feed_data()\n";
                feed_parameters = get_rss_feed_data(feed_source_index);

                if(_feed_articles.size() > 0) {
                        //cout << "headlines\n";

                        const int articles_size = _feed_articles.size();

                        for(int article_index = 0; article_index < articles_size; article_index++) {
                                auto feed_article_entry = _feed_articles[article_index];

                                headlines_region->add(string(feed_article_entry.headline).data());
                        }
                }
                else {
                        headlines_region->clear();
                }

                headlines_region->callback(feed_items_callback_l);
        }

        return feed_parameters;
}

void cls::feed_items_callback(Fl_Widget* widget) {
        if(widget) {
                visual_type_rss_headlines headlines_region = (Fl_Hold_Browser*)widget;

                if(headlines_region) {
                        const int headline_index = headlines_region->value();
                        
                        material article_info = _feed_articles[headline_index];
                        
                        visual_type_rss_article_content 
                        article_contents_region = (Fl_Help_View*)_workarea_region->child(visual_index_rss_reader_region::article_content);
                        
                        if(article_contents_region) {
                                const char* src = article_info.description.data();
                                
                                article_contents_region->value(src);
                        }
                }
        }

        return;
}

void cls::feed_source_callback(Fl_Widget* widget) {
        if(widget) {
                Fl_Button* feed_button = (Fl_Button*)widget;

                if(feed_button) {
                        const int feed_source_index = *((int*)feed_button->user_data());

                        _feed_index = feed_source_index;

                        visual_type_rss_headlines headlines_region = nullptr;
                        visual_type_rss_article_content article_contents_region = nullptr;

                        headlines_region = (decltype(headlines_region))_workarea_region->child(visual_index_rss_reader_region::headlines);
                        article_contents_region = (decltype(article_contents_region))_workarea_region->child(visual_index_rss_reader_region::article_content);

                        headlines_region->clear();
                        article_contents_region->value("");

                        display_feed_source_headlines(feed_source_index);
                }
        }

        return;
}

void cls::feed_contents_enlarge_callback(Fl_Widget* widget) {
        _article_contents_enlarge = !_article_contents_enlarge;
        _article_contents_enlarge_click = true;

        Fl_Button* button = (Fl_Button*)widget;
        string label_text = "";

        if(_article_contents_enlarge) {
                label_text = "Shrink";
        }
        else {
                label_text = "Enlarge";
        }

        button->copy_label(label_text.data());

        resize_workarea();

        return;
}

void cls::feed_setup_callback(Fl_Widget* widget) {
        visual_type_rss_change_bar 
        rss_change_bar_region = (decltype(rss_change_bar_region))_workarea_region->child(visual_index_rss_reader_region::change_bar);

        if(rss_change_bar_region->children() > 2) {
                Fl_Input* feed_name_input = (Fl_Input*)rss_change_bar_region->child(0);
                Fl_Input* feed_url_input = (Fl_Input*)rss_change_bar_region->child(1);

                string feedname(feed_name_input->value());
                string feedurl(feed_url_input->value());

                feed_name_input->value("");
                feed_url_input->value("");

                if((feedname != "name a new feed" && feedurl != "new feed url") && (!feedname.empty() && !feedurl.empty())) {
                        feedscycle feeds_group;
	                vector<string> added_feednames = feeds_group.set_feed_name_and_address(_feed_names_location, feedname, feedurl);

                        if(added_feednames.size() > 0) {
                                //cout << "clear rss_feed_choice_region\n";
                                //cout << "added_feednames" << added_feednames.size() << "\n";
                                //display_feed_buttons(feednames, rss_feed_choice_region);
                                vector<request> feed_parameters = get_rss_feed_data(_feed_index);
                                vector<string> feednames;
                                
                                const int feed_source_size = feed_parameters.size();

                                for(int feed_source_index = 0; feed_source_index < feed_source_size; feed_source_index++) {
                                        request feedsource = feed_parameters[feed_source_index];

                                        feednames.push_back(feedsource.feedname);
                                }
                                
                                display_feed_buttons(feednames);
                        }
                }
        }
        
        return;
}

void cls::display_feed_buttons(const vector<string> feednames) {
        //cout << __FILE__ << " " << __func__ << ":" << __LINE__ << "\n";
        if(_workarea_region->children() > 4) {
                _workarea_region->remove(visual_index_rss_reader_region::choice_bar);
                
                visual_type_rss_change_bar 
                widget = get_visual_rss_choice_display();

                _workarea_region->add(widget);
        }

        visual_type_rss_change_bar 
        rss_feed_choice_region = (decltype(rss_feed_choice_region))_workarea_region->child(visual_index_rss_reader_region::choice_bar);

        if(feednames.size() > 0) {
                const int button_spacing = 12;

                const int region_h = rss_feed_choice_region->h();
                const int region_h_half = (region_h/2);
                
                int button_x = 20;
                int button_w = 0;
                int button_h = 0;

                fl_font(FL_HELVETICA, 12);
                fl_measure("TEST WORD", button_w, button_h);

                button_h = button_h + button_spacing;
                const int button_y = measure_button_y(region_h_half, button_h);

                const int feed_source_size = feednames.size();

                for(int feed_source_index = 0; feed_source_index < feed_source_size; feed_source_index++) {
                        string feedname = feednames[feed_source_index];
                        //cout << feedname << "\n";

                        const char* label_text = (new string(feedname))->data();

                        int text_measure_w = 0;
                        int text_measure_h = 0;

                        fl_measure(label_text, text_measure_w, text_measure_h);
                        //cout << "measure text w/h " << text_measure_w << "/" << text_measure_h << "\n";

                        button_w = measure_button_w(text_measure_w, button_spacing);

                        Fl_Button* feed_button = new Fl_Button(button_x, button_y, button_w, button_h);
                        feed_button->copy_label(label_text);

                        rss_feed_choice_region->add(feed_button);

                        feed_button->user_data(new int(feed_source_index));
                        feed_button->callback(feed_source_callback_l);

                        button_x = (button_x + button_w) + button_spacing;
                }
        }
        
        return;
}

visual_type_window cls::get_window(int x, int y, int w, int h, int w_lo, int h_lo, string label) {
        visual_type_window visual_window = new Fl_Double_Window(x, y, w, h);
        visual_window->end();

	visual_window->size_range(w_lo, h_lo, w, h);
        visual_window->label(label.data());

        return visual_window;
}

void cls::show() {
        _visual_window->show();

        _last_w = _visual_window->w();
        _last_h = _visual_window->h();
        
        return;
}

void cls::clear(Fl_Group* grp) {
        const int widgetcount = grp->children();
        
        for(int index = 0; index < widgetcount; index++) {
                grp->remove(index);
                
                auto widget_sub = grp->child(index);

                if(widget_sub) {
                        auto widget_grp = widget_sub->as_group();

                        if(widget_grp) {
                                clear(widget_grp);
                        }
                }
        }

        return;
}

void cls::measure_screen() {
        Fl::screen_xywh(_screen_x, _screen_y, _screen_w, _screen_h);
        //cout << "screen x/y/w/h " << screen_x << "/" << screen_y << "/" << screen_w << "/" << screen_h << "\n";

        Fl::screen_work_area(_workarea_x, _workarea_y, _workarea_w, _workarea_h);
        //cout << "workarea x/y/w/h " << workarea_x << "/" << workarea_y << "/" << workarea_w << "/" << workarea_h << "\n";

        return;
}

vector<request> cls::get_rss_feed_data(int feed_source_index) {
        feedscycle feeds_group;
        
        vector<request> feed_parameters;
	feeds_group.get_feed_names_and_addresses(_feed_names_location, feed_parameters);

	if(feed_parameters.size() > 0) {
	        request feedsource = feed_parameters[feed_source_index];

                //cout << "feedsource " << feedsource.feedname << "\n";

                collector rss_requestor;
	        _feed_articles = rss_requestor.pull(feedsource);
	        
	        _feed_articles_requested = true;
	        
	        //cout << "_feed_articles.size() " << _feed_articles.size() << "\n";
	}

        return feed_parameters;
}

vector<visualcallable> cls::get_visual_definitions(int screen_x, int screen_y, int screen_w, int screen_h) {
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
                        case visual_index_rss_reader_region::header://RSS Reader Header
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
                        case visual_index_rss_reader_region::headlines://RSS Reader Headlines
                        {
                                h = remaining_h / 2;
                        }
                        break;
                        case visual_index_rss_reader_region::article_content://RSS Reader article content
                        {
                                double dv = 1.4;
                                h = remaining_h / dv;
                        }
                        break;
                        case visual_index_rss_reader_region::control_bar:
                        {
                                double dv = 3;
                                rh = remaining_h / dv;

                                h = rh;

                                callable.label("test 1");
                        }
                        break;
                        case visual_index_rss_reader_region::change_bar:
                        {
                                h = rh;

                                callable.label("test 2");
                        }
                        break;
                        case visual_index_rss_reader_region::choice_bar:
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

                //cout << "index: " << index << " ";
                //cout << " h: " << h;
                //cout << " accumulated_h: " << accumulated_h;
                //cout << " remaining_h: " << remaining_h;
                //cout << "\n";

                callables.push_back(callable);
        }

        if(_article_contents_enlarge) {
                const int region_h = callables[1].h() + callables[2].h();
                
                const int headlines_h = region_h/4;
                const int article_contents_h = region_h - headlines_h;
                
                callables[1].h(headlines_h);
                callables[2].h(article_contents_h);
        }
        
        return callables;
}
