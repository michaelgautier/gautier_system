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

/*WebKit*/
//#include <webkit2/webkit2.h>

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
#include <gtkmm/eventbox.h>
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

#include "rss_data_feed_name_spec.hxx"
#include "rss_data_feed_headline_spec.hxx"
#include "rss_data_feed_article_spec.hxx"

namespace rss {
namespace ui {
using namespace std;
class mainscreengenerator {
  public:
    mainscreengenerator();
    ~mainscreengenerator();

    void init();
    void generate();

  private:
    int _feed_index = 0;//Defaults to the first feed, if available;
    int _headline_index = -1;

    bool _article_contents_enlarge = false;

    string _feed_names_location = "feeds.txt";
    vector<news::rss_data_feed_headline_spec> _feed_headlines;

    bool update_feed_source();
    void get_rss_feed_names_and_articles();
    void get_screen_wh();

    Glib::RefPtr<Gtk::CssProvider> _css_provider;

    Gtk::EventBox* _events = nullptr;

    Gtk::Label* _article_content = nullptr;
    Gtk::Layout* _headlines = nullptr;

    Gtk::ApplicationWindow* _gautier_rss_window = nullptr;
    Gtk::Box* _gautier_rss_area = nullptr;
    Gtk::Box* _region_header = nullptr;
    Gtk::ScrolledWindow* _region_headlines = nullptr;
    Gtk::Label* _region_article_summary = nullptr;
    Gtk::ScrolledWindow* _region_content = nullptr;
    Gtk::Box* _region_feed_edit = nullptr;
    Gtk::ScrolledWindow* _region_feed_names = nullptr;

    Gtk::Label* _header_text = nullptr;
    Gtk::Label* _feed_name_label = nullptr;
    Gtk::Entry* _feed_name_edit = nullptr;
    Gtk::Label* _feed_url_label = nullptr;
    Gtk::Entry* _feed_url_edit = nullptr;
    Gtk::Button* _feed_edit_button = nullptr;
    Gtk::Layout* _feed_names_field = nullptr;

    /*
        //https://stackoverflow.com/questions/17039942/example-of-using-webkitgtk-with-gtkmm-3-0
        WebKitWebView* _article_content_web_backend = nullptr;
        Gtk::Widget* _article_content_web = nullptr;

        WebKitSettings* _article_content_web_backend_settings = nullptr;*/

    int _screen_w = 0;
    int _screen_h = 0;

    int _window_w = 0;
    int _window_h = 0;

    int _last_window_w = -1;
    int _last_window_h = -1;

    int _screen_max_w = 0;
    int _screen_max_h = 0;

    const int _feed_name_button_h = 34;

    const int _widget_xy_offset = 22;

    const int _headline_description_max_chars = 180;

    int _region_header_w = 0;
    int _region_header_h = 0;

    int _region_headlines_w = 0;
    int _region_headlines_h = 0;

    int _region_article_summary_w = 0;
    int _region_article_summary_h = 0;

    int _region_content_w = 0;
    int _region_content_h = 0;

    int _region_feed_edit_w = 0;
    int _region_feed_edit_h = 0;

    int _region_feed_names_w = 0;
    int _region_feed_names_h = 0;

    int show_screen();

    void show_feed(int feed_index);
    void show_feed_names();
    void show_headlines();
    void show_headline_description_selected_row(Gtk::ListBoxRow* row);
    void show_headline_description(int headline_index);

    void setup_ui_layout_parameters();
    void setup_ui_region_layout_parameters();

    void resize_headlines();
    void remove_headlines();

    void create_ui_window();
    void create_ui_region_header();
    void create_ui_region_headlines();
    void create_ui_region_article_summary();
    void create_ui_region_content();
    void create_ui_region_feed_edit();
    void create_ui_region_feed_names();
};
}
}
#endif
