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
#ifndef __rss_ui_mainscreengenerator__
#define __rss_ui_mainscreengenerator__
#include <vector>
#include <map>
#include <string>
#include <iostream>

#include <gdkmm/general.h>
#include <gtkmm/application.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/listbox.h>
#include <gtkmm/listboxrow.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/textview.h>
#include <gtkmm/entry.h>
#include <gtkmm/textbuffer.h>
#include <gtkmm/button.h>
#include <gtkmm/layout.h>
#include <gtkmm/viewport.h>
#include <gdkmm/rgba.h>
#include <gtkmm/cssprovider.h>
#include <gdkmm/screen.h>
#include <gdkmm/display.h>
#include <gdkmm/displaymanager.h>
#include <gtkmm/listviewtext.h>

#include "visualcallable.hxx"
#include "request.hxx"
#include "material.hxx"

namespace rss {
namespace ui {
using namespace std;
class mainscreengenerator {
  public:
    using material = ::rss::material;
    using request = ::rss::request;

    mainscreengenerator();
    ~mainscreengenerator();

    void init();
    void generate();

  private:
    int _feed_index = 0;//Defaults to the first feed, if available;
    int _headline_index = -1;

        bool _article_contents_enlarge = false;
        bool _feed_articles_requested = false;

    string _feed_names_location = "feeds.txt";

    vector<material> _feed_articles;
    vector<request> get_rss_feed_data(int feed_source_index, vector<material>& feed_articles);
    vector<string> _feednames;

    bool update_feed_source();
    void get_rss_feed_names_and_articles();
    void get_screen_wh();
    
    Gtk::Label* _article_content;
        
        int screen_w = 0;
        int screen_h = 0;
        int screen_max_w = 0;
        int screen_max_h = 0;

        int show_screen();

};
}
}
#endif
