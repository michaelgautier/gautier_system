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


/*GDK*/
#include <gdkmm/display.h>
#include <gdkmm/displaymanager.h>
#include <gdkmm/general.h>
#include <gdkmm/rgba.h>
#include <gdkmm/screen.h>

/*GLib*/
#include <glibmm/ustring.h>

/*GTK*/
#include <gtkmm/application.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/entry.h>
#include <gtkmm/flowbox.h>
#include <gtkmm/label.h>
#include <gtkmm/layout.h>
#include <gtkmm/linkbutton.h>
#include <gtkmm/listbox.h>
#include <gtkmm/listboxrow.h>
#include <gtkmm/listviewtext.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/textbuffer.h>
#include <gtkmm/textview.h>
#include <gtkmm/viewport.h>

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
    Gtk::Layout* _headlines;

    Glib::RefPtr<Gtk::CssProvider> _css_provider;

    Gtk::ApplicationWindow* _gautier_rss_window;
    Gtk::Box* _gautier_rss_area;
    Gtk::Box* _header_region;
    Gtk::ScrolledWindow* _headlines_region;
    Gtk::ScrolledWindow* _content_region;
    Gtk::Box* _feed_edit_region;
    Gtk::ScrolledWindow* _feed_names_region;

    Gtk::Label* _header_text;

    int _screen_w = 0;
    int _screen_h = 0;
    int _screen_max_w = 0;
    int _screen_max_h = 0;
    int _headline_region_h = 0;
    int _contents_region_h = 0;

    int show_screen();

    void show_feed(int feed_index);
    void show_headlines();
    void show_article_summary_selected_row(Gtk::ListBoxRow* row);
    void show_article_summary(int article_index);

    void setup_ui_layout_parameters();

    void create_ui_window();
    void create_ui_header();
    void create_ui_headline_region();
    void create_ui_article_content_region();
    void create_ui_feed_edit_region();
    void create_ui_feed_names_region();
};
}
}
#endif
