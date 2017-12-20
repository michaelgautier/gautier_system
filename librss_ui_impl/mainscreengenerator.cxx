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
#include <FL/Fl_Tile.H>

#include "mainscreengenerator.hxx"

#include "visualcallable.hxx"
#include "collector.hxx"
#include "request.hxx"
#include "material.hxx"
#include "feedscycle.hxx"

#include "mainscreenblueprint.hxx"
//#include "mainscreenheaderbar.hxx"

using collector = rss::collector;
using material = rss::material;
using request = rss::request;
using feedscycle = rss::feedscycle;
using visualcallable = visualfunc::formulation::visualcallable;
using mainscreenblueprint = rss::ui::mainscreenblueprint;

using namespace std;
using cls = rss::ui::mainscreengenerator;

vector<string> _strings;

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

        Fl_Output HeaderRegion(0, 0, 0, 0);
        Fl_Hold_Browser HeadlinesRegion(0, 0, 0, 0);
        Fl_Help_View ArticleContentsRegion(0, 0, 0, 0);
        Fl_Pack RSSControlBarRegion(0, 0, 0, 0);
        Fl_Pack RSSChangeBarRegion(0, 0, 0, 0);
        Fl_Pack RSSFeedChoiceRegion(0, 0, 0, 0);

        WorkAreaRegion.add(HeaderRegion);
        WorkAreaRegion.add(HeadlinesRegion);
        WorkAreaRegion.add(ArticleContentsRegion);
        WorkAreaRegion.add(RSSControlBarRegion);
        WorkAreaRegion.add(RSSChangeBarRegion);
        WorkAreaRegion.add(RSSFeedChoiceRegion);

        HeaderRegion.box(FL_FLAT_BOX);
        HeaderRegion.color(fl_rgb_color(212, 85, 0));
        HeaderRegion.value("RSS Reader");
        HeaderRegion.textfont(FL_HELVETICA);
        HeaderRegion.textsize(72);
        HeaderRegion.textcolor(fl_rgb_color(213, 255, 246));

        RSSControlBarRegion.type(Fl_Pack::HORIZONTAL);
        RSSChangeBarRegion.type(Fl_Pack::HORIZONTAL);
        RSSFeedChoiceRegion.type(Fl_Pack::HORIZONTAL);

        int text_x = 0;
        int text_y = 0;
        int text_w = 0;
        int text_h = 0;
        
        int text_wh_plus = 20;
        
        fl_font(FL_HELVETICA, 12);

        string enlarge_button_label_text = "Enlarge";
        fl_text_extents((new string(enlarge_button_label_text))->data(), text_x, text_y, text_w, text_h);
        cout << "text_w " << text_w << "\n";

        Fl_Button* enlarge_button = new Fl_Button(0, 0, text_w+text_wh_plus, text_h+text_wh_plus);
        enlarge_button->copy_label((new string(enlarge_button_label_text))->data());

        RSSControlBarRegion.add(enlarge_button);

        string update_button_label_text = "Update";
        fl_text_extents((new string(update_button_label_text))->data(), text_x, text_y, text_w, text_h);
        cout << "text_w " << text_w << "\n";

        Fl_Button* update_button = new Fl_Button(0, 0, text_w+text_wh_plus, text_h+text_wh_plus);
        update_button->copy_label(update_button_label_text.data());

        RSSChangeBarRegion.add(update_button);
        
        for(auto feed_article_entry : feed_articles) {
              HeadlinesRegion.add(string(feed_article_entry.headline).data());
        }

        for(auto feedsource : feed_parameters) {
                auto feedname = feedsource.feedname;

                fl_text_extents(feedname.data(), text_x, text_y, text_w, text_h);
                cout << "text_w " << text_w << "\n";

                Fl_Button* feed_button = new Fl_Button(0, 0, text_w+text_wh_plus, text_h+text_wh_plus);
                feed_button->copy_label(feedname.data());

                RSSFeedChoiceRegion.add(feed_button);
        }

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

                        for(visualcallable callable : callables) {
                                const int index = callable.id();

                                auto label = new string(callable.label());

                                auto label_text = label->data();

                                const int x = callable.x();
                                const int y = callable.y();
                                const int w = callable.w();
                                const int h = callable.h();

                                switch(index) {
                                        case 0://RSS Reader Header
                                        {
                                                HeaderRegion.size(w, h);
                                        }
                                        break;
                                        case 1://RSS Reader Headlines
                                        {
                                                HeadlinesRegion.size(w, h);
                                        }
                                        break;
                                        case 2://RSS Reader article content
                                        {
                                                ArticleContentsRegion.size(w, h);
                                        }
                                        break;
                                        case 3://RSS Reader Control Bar
                                        {
                                                RSSControlBarRegion.size(w, h);
                                        }
                                        break;
                                        case 4://RSS Reader RSS Change Bar
                                        {
                                                RSSChangeBarRegion.size(w, h);
                                        }
                                        break;
                                        case 5://RSS Reader Feed Choice Bar
                                        {
                                                RSSFeedChoiceRegion.size(w, h);
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

//vector<shared_ptr<Fl_Widget>> cls::get_widgets(const vector<visualcallable> & callables) {
//        vector<shared_ptr<Fl_Widget>> widgets;

//        for(visualcallable callable : callables) {
//                const int index = callable.id();

//                /*
//                        Raw pointer necessary.
//                        Quickly tried both C++14 smart pointers and both 
//                        resulted in text that was missing.
//                        
//                        Without a raw pointer the text is garbled.
//                        
//                        FLTK is accessing string data such that this approach 
//                        is required.
//                */
//                auto label = new string(callable.label());

//                auto label_text = label->data();

//                const int x = callable.x();
//                const int y = callable.y();
//                const int w = callable.w();
//                const int h = callable.h();

//                switch(index) {
//                        case 0://RSS Reader Header
//                        {
//                                widgets.emplace_back(shared_ptr<Fl_Widget>(new mainscreenheaderbar(x, y, w, h, label_text)));
//                        }
//                        break;
//                        case 1://RSS Reader Headlines
//                        {
//                                auto widget = new Fl_Hold_Browser(x, y, w, h, label_text);
//                                widget->textsize(60);
//                                widgets.push_back(shared_ptr<Fl_Widget>(widget));
//                        }
//                        break;
//                        case 2://RSS Reader article content
//                        {
//                                widgets.emplace_back(shared_ptr<Fl_Widget>(new Fl_Help_View(x, y, w, h, label_text)));
//                        }
//                        break;
//                        case 3://RSS Reader Control Bar
//                        {
//                                widgets.emplace_back(shared_ptr<Fl_Widget>(new Fl_Box(x, y, w, h, label_text)));
//                                auto widget = widgets.back();
//                                widget->box(FL_FLAT_BOX);
//                                widget->color(fl_rgb_color(200, 113, 55));
//                        }
//                        break;
//                        case 4://RSS Reader RSS Change Bar
//                        {
//                                widgets.emplace_back(shared_ptr<Fl_Widget>(new Fl_Box(x, y, w, h, label_text)));
//                                auto widget = widgets.back();
//                                widget->box(FL_FLAT_BOX);
//                                widget->color(fl_rgb_color(83, 108, 83));
//                        }
//                        break;
//                        case 5://RSS Reader Feed Choice Bar
//                        {
//                                widgets.emplace_back(shared_ptr<Fl_Widget>(new Fl_Box(x, y, w, h, label_text)));
//                                auto widget = widgets.back();
//                                widget->box(FL_FLAT_BOX);
//                                widget->color(fl_rgb_color(255, 204, 170));
//                        }
//                        break;
//                }

//                //cout << index << ": ";
//                //cout << "x/y/w/h " << x << "/" << y << "/" << w << "/" << h << "\n";

//                fl_font(FL_HELVETICA, 20);
//                fl_color(fl_rgb_color(213, 255, 246));
//        }

//        return widgets;
//}

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
