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

#include <FL/Fl_Widget.H>
#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Help_View.H>
#include <FL/Fl_Hold_Browser.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>

#include "mainscreengenerator.hxx"

#include "visualcallable.hxx"
#include "collector.hxx"
#include "request.hxx"
#include "material.hxx"
#include "feedscycle.hxx"

#include "mainscreenblueprint.hxx"

using collector = rss::collector;
using material = rss::material;
using request = rss::request;
using feedscycle = rss::feedscycle;
using visualcallable = visualfunc::formulation::visualcallable;
using mainscreenblueprint = rss::ui::mainscreenblueprint;

using namespace std;
using cls = rss::ui::mainscreengenerator;

vector<string> _strings;

Fl_Output* get_visual_rss_header_display() {
        Fl_Output* HeaderRegion = new Fl_Output(0, 0, 0, 0);

        HeaderRegion->box(FL_FLAT_BOX);
        HeaderRegion->color(fl_rgb_color(212, 85, 0));
        HeaderRegion->value("RSS Reader");
        HeaderRegion->textfont(FL_HELVETICA);
        HeaderRegion->textsize(72);
        HeaderRegion->textcolor(fl_rgb_color(213, 255, 246));

        return HeaderRegion;
}

Fl_Hold_Browser* get_visual_rss_headlines_display() {
        Fl_Hold_Browser* HeadlinesRegion = new Fl_Hold_Browser(0, 0, 0, 0);

        return HeadlinesRegion;
}

Fl_Help_View* get_visual_rss_article_contents_display() {
        Fl_Help_View* ArticleContentsRegion = new Fl_Help_View(0, 0, 0, 0);

        return ArticleContentsRegion;
}

Fl_Pack* get_visual_rss_control_bar_display() {
        Fl_Pack* RSSControlBarRegion = new Fl_Pack(0, 0, 0, 0);

        RSSControlBarRegion->type(Fl_Pack::HORIZONTAL);

        int text_x = 0;
        int text_y = 0;
        int text_w = 0;
        int text_h = 0;
        
        int text_wh_plus = 20;
        
        fl_font(FL_HELVETICA, 12);

        string enlarge_button_label_text = "Enlarge";
        fl_text_extents((new string(enlarge_button_label_text))->data(), text_x, text_y, text_w, text_h);
        //cout << "text_w " << text_w << "\n";

        Fl_Button* enlarge_button = new Fl_Button(0, 0, text_w+text_wh_plus, text_h+text_wh_plus);
        enlarge_button->copy_label((new string(enlarge_button_label_text))->data());

        RSSControlBarRegion->add(enlarge_button);

        return RSSControlBarRegion;
}

Fl_Pack* get_visual_rss_change_bar_display() {
        Fl_Pack* RSSChangeBarRegion = new Fl_Pack(0, 0, 0, 0);

        RSSChangeBarRegion->type(Fl_Pack::HORIZONTAL);

        int text_x = 0;
        int text_y = 0;
        int text_w = 0;
        int text_h = 0;
        
        int text_wh_plus = 20;

        fl_font(FL_HELVETICA, 12);

        string update_button_label_text = "Update";
        fl_text_extents((new string(update_button_label_text))->data(), text_x, text_y, text_w, text_h);
        //cout << "text_w " << text_w << "\n";

        Fl_Button* update_button = new Fl_Button(0, 0, text_w+text_wh_plus, text_h+text_wh_plus);
        update_button->copy_label(update_button_label_text.data());

        RSSChangeBarRegion->add(update_button);

        return RSSChangeBarRegion;
}

Fl_Pack* get_visual_rss_choice_display() {
        Fl_Pack* RSSFeedChoiceRegion = new Fl_Pack(0, 0, 0, 0);

        RSSFeedChoiceRegion->type(Fl_Pack::HORIZONTAL);

        return RSSFeedChoiceRegion;
}

void cls::generate() {
        const int w_lo = 320;
        const int h_lo = 480;
        string label = "RSS Reader";

        Fl::screen_xywh(_screen_x, _screen_y, _screen_w, _screen_h);
        //cout << "screen x/y/w/h " << screen_x << "/" << screen_y << "/" << screen_w << "/" << screen_h << "\n";

        Fl::screen_work_area(_workarea_x, _workarea_y, _workarea_w, _workarea_h);
        //cout << "workarea x/y/w/h " << workarea_x << "/" << workarea_y << "/" << workarea_w << "/" << workarea_h << "\n";

        _visual_window = get_window(0, 0, _workarea_w, _workarea_h, w_lo, h_lo, label);

        show();
	
	feeds_group.get_feed_namedaddresses(feed_names_location, feed_parameters);

	//When starting up, just get the first one.
	if(feed_parameters.size() > 0) {
	        feedsource = feed_parameters[0];

	        feed_articles = rss_requestor.pull(feedsource);
	}
	
	/*
	 links section and content sections
	 
	        when auto feedsource = feed_parameters[index];
	        
	        then 
	 
	        vector<material> feed_articles = rss_requestor.pull(feedsource);
	
	        for(auto feed_article_entry : feed_articles) {
	                show the headline in the links section
	        
		                string headline = feed_article_entry.headline;
	
	
	                hold onto the vector, when a headline is clicked, simply match by headline
	
		                string url = feed_article_entry.url;
		                string description = feed_article_entry.description;
		                string article_date = feed_article_entry.article_date;
	        }
	*/
	
	
        /*top bottom bar ... when the enlarge button is clicked, expand the content*/

        /*middle bottom bar ... just a feed adder ... later on*/

        /*bottom bar
        
	for(auto feedsource : feed_parameters) {
	        render a button for each of:
	        
	                feedsource.feedname


	}
        */

        Fl_Pack WorkAreaRegion(0, 0, _workarea_w, _workarea_h);
        WorkAreaRegion.type(Fl_Pack::VERTICAL);

        _visual_window->add(WorkAreaRegion);

        vector<visualcallable> callables;

	while(Fl::wait()) {
	        const int new_w = _visual_window->w();
	        const int new_h = _visual_window->h();

	        if(_last_h != new_h || _last_w != new_w) {
                        //cout << " last w/h " << last_w << "/" << last_h << "\n";
		        _last_h = new_h;
		        _last_w = new_w;

                        mainscreenblueprint visual_blueprint(_workarea_x, _screen_y, new_w, new_h);

                        callables = visual_blueprint.get_visual_definitions();

                        const int callable_size = callables.size();
                        
                        bool children_count_matches = (WorkAreaRegion.children() == callable_size);

                        for(int callable_index = 0; callable_index < callable_size; callable_index++) {
                                visualcallable callable = callables[callable_index];

                                auto label = new string(callable.label());

                                auto label_text = label->data();

                                const int x = callable.x();
                                const int y = callable.y();
                                const int w = callable.w();
                                const int h = callable.h();

                                switch(callable_index) {
                                        case 0://RSS Reader Header
                                        {
                                                Fl_Output* HeaderRegion = nullptr;

                                                if(children_count_matches) {
                                                        HeaderRegion = (decltype(HeaderRegion))WorkAreaRegion.child(callable_index);
                                                }
                                                else {
                                                        HeaderRegion = get_visual_rss_header_display();

                                                        WorkAreaRegion.add(HeaderRegion);
                                                }

                                                HeaderRegion->size(w, h);
                                        }
                                        break;
                                        case 1://RSS Reader Headlines
                                        {
                                                Fl_Hold_Browser* HeadlinesRegion = nullptr;

                                                if(children_count_matches) {
                                                        HeadlinesRegion = (decltype(HeadlinesRegion))WorkAreaRegion.child(callable_index);
                                                }
                                                else {
                                                        HeadlinesRegion = get_visual_rss_headlines_display();

                                                        WorkAreaRegion.add(HeadlinesRegion);
                                                }

                                                HeadlinesRegion->size(w, h);

                                                if(HeadlinesRegion->size() == 0) {
                                                        for(auto feed_article_entry : feed_articles) {
                                                              HeadlinesRegion->add(string(feed_article_entry.headline).data());
                                                        }
                                                }
                                        }
                                        break;
                                        case 2://RSS Reader article content
                                        {
                                                Fl_Help_View* ArticleContentsRegion = nullptr;

                                                if(children_count_matches) {
                                                        ArticleContentsRegion = (decltype(ArticleContentsRegion))WorkAreaRegion.child(callable_index);
                                                }
                                                else {
                                                        ArticleContentsRegion = get_visual_rss_article_contents_display();

                                                        WorkAreaRegion.add(ArticleContentsRegion);
                                                }

                                                ArticleContentsRegion->size(w, h);
                                        }
                                        break;
                                        case 3://RSS Reader Control Bar
                                        {
                                                Fl_Pack* RSSControlBarRegion = nullptr;

                                                if(children_count_matches) {
                                                        RSSControlBarRegion = (decltype(RSSControlBarRegion))WorkAreaRegion.child(callable_index);
                                                }
                                                else {
                                                        RSSControlBarRegion = get_visual_rss_control_bar_display();

                                                        WorkAreaRegion.add(RSSControlBarRegion);
                                                }

                                                RSSControlBarRegion->size(w, h);
                                        }
                                        break;
                                        case 4://RSS Reader RSS Change Bar
                                        {
                                                Fl_Pack* RSSChangeBarRegion = nullptr;

                                                if(children_count_matches) {
                                                        RSSChangeBarRegion = (decltype(RSSChangeBarRegion))WorkAreaRegion.child(callable_index);
                                                }
                                                else {
                                                        RSSChangeBarRegion = get_visual_rss_change_bar_display();

                                                        WorkAreaRegion.add(RSSChangeBarRegion);
                                                }

                                                RSSChangeBarRegion->size(w, h);
                                        }
                                        break;
                                        case 5://RSS Reader Feed Choice Bar
                                        {
                                                Fl_Pack* RSSFeedChoiceRegion = nullptr;

                                                if(children_count_matches) {
                                                        RSSFeedChoiceRegion = (decltype(RSSFeedChoiceRegion))WorkAreaRegion.child(callable_index);
                                                }
                                                else {
                                                        RSSFeedChoiceRegion = get_visual_rss_choice_display();

                                                        WorkAreaRegion.add(RSSFeedChoiceRegion);
                                                }

                                                RSSFeedChoiceRegion->size(w, h);

                                                if(RSSFeedChoiceRegion->children() == 0) {
                                                        for(auto feedsource : feed_parameters) {
                                                                auto feedname = feedsource.feedname;

                                                                int text_x = 0;
                                                                int text_y = 0;
                                                                int text_w = 0;
                                                                int text_h = 0;
                                                                
                                                                int text_wh_plus = 20;

                                                                fl_font(FL_HELVETICA, 12);

                                                                fl_text_extents(feedname.data(), text_x, text_y, text_w, text_h);
                                                                //cout << "text_w " << text_w << "\n";

                                                                Fl_Button* feed_button = new Fl_Button(0, 0, text_w+text_wh_plus, text_h+text_wh_plus);
                                                                feed_button->copy_label(feedname.data());

                                                                RSSFeedChoiceRegion->add(feed_button);
                                                        }
                                                }
                                        }
                                        break;
                                }
                        }

		        _visual_window->redraw();
	        }
	        
	        Fl::flush();
	}

        return;
}

unique_ptr<Fl_Double_Window> cls::get_window(int x, int y, int w, int h, int w_lo, int h_lo, string label) {
        unique_ptr<Fl_Double_Window> visual_window(new Fl_Double_Window(x, y, w, h));
        visual_window->end();

	visual_window->size_range(w_lo, h_lo, w, h);
        visual_window->label(label.data());

        return visual_window;
}

void cls::show() {
        _visual_window->show();
        _visual_window->redraw();

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
