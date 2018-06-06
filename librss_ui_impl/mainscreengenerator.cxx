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

cls::mainscreengenerator() {
    return;
}

cls::~mainscreengenerator() {
    return;
}

void cls::init() {
    return;
}

void cls::generate() {
    _article_contents_enlarge = false;
    get_rss_feed_names_and_articles();

    _feed_articles_requested = true;

    show_screen();

    return;
}

void cls::get_rss_feed_names_and_articles() {
    vector<material> feed_articles;
    vector<request> feed_parameters = get_rss_feed_data(_feed_index, feed_articles);
    _feed_articles = feed_articles;

    const int feed_source_size = feed_parameters.size();

    _feednames.clear();

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

        collector rss_requestor;
        feed_articles = rss_requestor.pull(feedsource);
    }

    return feed_parameters;
}

void cls::show_feed(int feed_index) {
    int previous_articles_size = _feed_articles.size();

    _feed_index = feed_index;

    vector<material> feed_articles;
    get_rss_feed_data(_feed_index, feed_articles);
    _feed_articles = feed_articles;

    show_headlines();

    show_article_summary(0);

    return;
}

void cls::remove_headlines() {
    auto headline_items = _headlines->get_children();
    auto headline_items_size = headline_items.size();

    for(int i = 0; i < headline_items_size; i++) {
        auto headlinebtn = (Gtk::Button*)headline_items[i];

        _headlines->remove(*headlinebtn);
    }

    return;
}

void cls::show_headlines() {
    if(!_headlines) {
        _headlines = new Gtk::Layout();
        _region_headlines->add(*_headlines);

        _headlines->show();
    }

    if(_headlines) {
        //_region_headlines->remove_with_viewport();

        remove_headlines();
    }

    Gtk::EventBox headline_event;
    Gtk::Label headline_label;
    Gtk::Button headline_button;

    int articles_size = _feed_articles.size();

    int headline_label_y = 0;
    int headline_height = 0;

    for(int article_index = 0; article_index < articles_size; article_index++) {
        string headline = _feed_articles[article_index].headline;

        headline_label = Gtk::Label(headline, Gtk::Align::ALIGN_START);
        headline_label.show();

        headline_button = Gtk::Button();
        headline_button.add(headline_label);

        /*
                Measure the label pixels to layout them out horizontally.
                https://developer.gnome.org/gtkmm-tutorial/stable/sec-drawing-text.html.en
                https://developer.gnome.org/gtk3/stable/gtk-question-index.html
        */
        auto pglyt = headline_button.create_pango_layout(headline);

        if(headline_label_y < 1) {
            int headline_width = 0;

            pglyt->get_pixel_size(headline_width, headline_height);

            headline_height = headline_height + _widget_xy_offset;
        }

        _headlines->put(headline_button, 0, headline_label_y);

        headline_label_y = headline_label_y + headline_height;

        headline_button.set_size_request(_screen_w,headline_height);
        headline_button.show();

        /*GTK Styles*/
        auto style_ctx = headline_button.get_style_context();
        style_ctx->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx->add_class("headline_button");
    }

    auto headline_buttons = _headlines->get_children();
    auto headline_buttons_size = headline_buttons.size();

    for(int i = 0; i < headline_buttons_size; i++) {
        auto headlinebtn = (Gtk::Button*)headline_buttons[i];

        headlinebtn->signal_clicked().connect([=]() {
            show_article_summary(i);
        });
    }

    resize_headlines();
    _region_headlines->show_all();
//    _region_headlines->queue_draw();
//
//    _headlines->show_all_children(true);

    return;
}

bool cls::update_feed_source() {
    bool feed_sources_updated = false;

    string feedname = _feed_name_edit->get_text();
    string feedurl = _feed_url_edit->get_text();

    if(!feedname.empty() && !feedurl.empty()) {
        feedscycle feeds_group;
        vector<string> added_feednames = feeds_group.set_feed_name_and_address(_feed_names_location, feedname, feedurl);

        if(added_feednames.size() > 0) {
            get_rss_feed_names_and_articles();

            feed_sources_updated = true;
        }
    }

    return feed_sources_updated;
}

void cls::show_feed_names() {
    if(_feed_names_field) {
        _region_feed_names->remove_with_viewport();
    }

    _feed_names_field = new Gtk::Layout();

    _region_feed_names->add(*_feed_names_field);

    Gtk::Button feedname_label;

    /*
       Hack workaround for GTK. The GTK framework will not show the last button in the Layout instance.
       Problem noticed 6/1/2018.
    */
    _feednames.push_back("");

    int feednames_size = _feednames.size();

    int feedname_label_width = 8;

    for(int feedname_index = 0; feedname_index < feednames_size; feedname_index++) {
        string feedname = _feednames[feedname_index];

        feedname_label = Gtk::Button(feedname);

        /*
                Measure the label pixels to layout them out horizontally.
                https://developer.gnome.org/gtkmm-tutorial/stable/sec-drawing-text.html.en
                https://developer.gnome.org/gtk3/stable/gtk-question-index.html
        */
        auto pglyt = feedname_label.create_pango_layout(feedname);

        int feedtext_width = 0;
        int feedtext_height = 0;

        pglyt->get_pixel_size(feedtext_width, feedtext_height);

        _feed_names_field->put(feedname_label, feedname_label_width, 0);

        feedname_label_width = feedname_label_width + feedtext_width + _widget_xy_offset;

        /*GTK Styles*/
        auto style_ctx_feedname_button = feedname_label.get_style_context();
        style_ctx_feedname_button->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_feedname_button->add_class("feed_button");
    }

    _gautier_rss_window->show_all();

    /*You have to do this in this way because none but the last feedname_index value is captured in the previous loop.*/
    auto feedname_buttons = _feed_names_field->get_children();
    int feedname_buttons_size = feedname_buttons.size();

    /*You have to do this in this way because none but the last feedname_index value is captured in the previous loop.*/
    for(int i = 0; i < feedname_buttons_size; i++) {
        auto feedbtn = (Gtk::Button*)feedname_buttons[i];
        feedbtn->signal_clicked().connect([=]() {
            show_feed(i);
        });
    }

    return;
}

void cls::show_article_summary(int article_index) {
    auto feed_article_entry = _feed_articles[article_index];

    string headline_description = feed_article_entry.description;
    string article_content = feed_article_entry.content;

    _region_article_summary->set_text(headline_description);
    _article_content->set_text(article_content);

    return;
}

void cls::show_article_summary_selected_row(Gtk::ListBoxRow* row) {
    if(row) {
        show_article_summary(row->get_index());
    }

    return;
}

int cls::show_screen() {
    int gtk_app_err = 0;

    try {
        auto app = Gtk::Application::create("gautier.rss");

        setup_ui_layout_parameters();

        create_ui_window();

        create_ui_region_header();
        create_ui_region_headlines();
        create_ui_region_article_summary();
        create_ui_region_content();
        create_ui_region_feed_edit();
        create_ui_region_feed_names();

        setup_ui_region_layout_parameters();

        _gautier_rss_window->show_all();

        gtk_app_err = app->run(*_gautier_rss_window);
    } catch(const Gtk::CssProviderError& ex) {
        std::cout << "CssProviderError, Gtk::CssProvider::load_from_path() failed: " << ex.what() << "\n";
    } catch(const Glib::Error& ex) {
        std::cout << "Error, Gtk::CssProvider::load_from_path() failed: " << ex.what() << "\n";
    } catch(const std::exception& e) {
        std::cout << e.what();
    }

    return gtk_app_err;
}

void cls::get_screen_wh() {
    auto display = Gdk::Display::get_default();
    auto screen = display->get_default_screen();

    _screen_w = screen->get_width();
    _screen_h = screen->get_height();

    return;
}

void cls::setup_ui_layout_parameters() {
    /*GTK Styles*/
    _css_provider = Gtk::CssProvider::create();

    _css_provider->load_from_resource("/styles/styles.css");

    get_screen_wh();

    setup_ui_region_layout_parameters();

    return;
}

void cls::setup_ui_region_layout_parameters() {
    int window_h = _screen_h;

    _region_header_w = 0;
    _region_header_h = 0;

    _region_headlines_w = _screen_w/2;
    _region_headlines_h = 0;

    _region_article_summary_w = 0;
    _region_article_summary_h = 0;

    _region_content_w = _screen_w/2;
    _region_content_h = 0;

    _region_feed_edit_w = 0;
    _region_feed_edit_h = 0;

    _region_feed_names_w = _screen_w/2;
    _region_feed_names_h = 36;

    int next_y = 0;
    int accumulated_h = 0;
    int remaining_h = 0;

    int y = 0;
    int h = 0;

    int rh = 0;

    const int max_elems = 6;

    if(_window_h > 0) {
        window_h = _window_h;
    }

    for(int index = 0; index < max_elems; index++) {
        y = next_y;

        switch(index) {
        case 0: { //RSS Reader Header
        }
        break;
        case 1: { //RSS Reader Headlines
            h = remaining_h / 3;
            _region_headlines_h = h;
        }
        break;
        case 2: { //RSS Reader article content
            double dv = 2;
            h = remaining_h / dv;

            _region_content_h = h;
        }
        break;
        case 3: {//article headline and control
            double dv = 2;
            rh = remaining_h / dv;

            h = rh;
            _region_article_summary_h = h;
        }
        break;
        case 4: {//feed edit
            h = rh;

            _region_feed_edit_h = h;
        }
        break;
        case 5: {//feed names
            h = rh;

            _region_feed_names_h = h;
        }
        break;
        }

        accumulated_h = (accumulated_h + h);
        next_y = (next_y + h);

        remaining_h = (window_h - accumulated_h);
    }

    if(_region_content) {
        _region_content->set_size_request(_region_content_w,_region_content_h);
    }

    if(_region_headlines) {
        _region_headlines->set_size_request(_region_headlines_w, _region_headlines_h);

        resize_headlines();
    }
    //_feed_names_field->set_size(_region_feed_names_w,_region_feed_names_h);
    /*
    _feed_names_field->set_size(feedname_label_width + _widget_xy_offset, _feed_name_button_h);*/

    return;
}

void cls::resize_headlines() {
    auto pglyt = _headlines->create_pango_layout("H");

    int headline_width = 0;
    int headline_height = 0;

    pglyt->get_pixel_size(headline_width, headline_height);

    int headlines_h = _feed_articles.size() * headline_height;

    _headlines->set_size(_screen_w, headlines_h);

    return;
}

void cls::create_ui_window() {
    _gautier_rss_window = new Gtk::ApplicationWindow();
    _gautier_rss_window->set_title("Gautier RSS");

    _gautier_rss_window->set_default_size(_screen_w/2,_screen_h/2);
    _gautier_rss_window->maximize();

    _gautier_rss_area = new Gtk::Box(Gtk::Orientation::ORIENTATION_VERTICAL);

    _gautier_rss_window->add(*_gautier_rss_area);

    _events = new Gtk::EventBox();

    auto style_ctx_window = _gautier_rss_window->get_style_context();
    style_ctx_window->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    style_ctx_window->add_class("window");


    /*
    Causes real issues/problems
        The headlines area won't show correctly after when another feed is clicked.

        Don't do this.

    _gautier_rss_window->signal_size_allocate().connect([=](Gtk::Allocation& allo) {
        _window_w = _gautier_rss_window->get_width();
        _window_h = _gautier_rss_window->get_height();

        setup_ui_region_layout_parameters();
    });
    */

    //However, this works just fine.
    _gautier_rss_window->signal_check_resize().connect([=]() {
        _window_w = _gautier_rss_window->get_width();
        _window_h = _gautier_rss_window->get_height();

        setup_ui_region_layout_parameters();
    });

    return;
}

void cls::create_ui_region_header() {
    _region_header = new Gtk::Box(Gtk::Orientation::ORIENTATION_HORIZONTAL);

    _header_text = new Gtk::Label("Gautier RSS");
    _region_header->add(*_header_text);
    _gautier_rss_area->add(*_region_header);

    auto style_ctx = _region_header->get_style_context();
    style_ctx->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    style_ctx->add_class("header");

    return;
}

void cls::create_ui_region_headlines() {
    _region_headlines = new Gtk::ScrolledWindow();

    _gautier_rss_area->add(*_region_headlines);

    show_headlines();

    auto style_ctx = _region_headlines->get_style_context();
    style_ctx->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    style_ctx->add_class("headlines_region");

    return;
}

void cls::create_ui_region_article_summary() {
    _region_article_summary = new Gtk::Label();

    _gautier_rss_area->add(*_region_article_summary);

    auto style_ctx = _region_article_summary->get_style_context();
    style_ctx->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    style_ctx->add_class("article_summary_region");

    return;
}

void cls::create_ui_region_content() {
    _region_content = new Gtk::ScrolledWindow();

    _article_content = new Gtk::Label();
    _article_content->set_lines(-1);
    _article_content->set_line_wrap(true);
    _article_content->set_single_line_mode(false);

    _region_content->add(*_article_content);

    _gautier_rss_area->add(*_region_content);

    auto style_ctx = _region_content->get_style_context();
    style_ctx->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    style_ctx->add_class("contents_region");

    return;
}

void cls::create_ui_region_feed_edit() {
    _region_feed_edit = new Gtk::Box(Gtk::Orientation::ORIENTATION_HORIZONTAL);
    _feed_name_label = new Gtk::Label("Feed Name");
    _feed_name_edit = new Gtk::Entry();
    _feed_url_label = new Gtk::Label("Feed website address");
    _feed_url_edit = new Gtk::Entry();
    _feed_edit_button = new Gtk::Button("Save");

    _region_feed_edit->add(*_feed_name_label);
    _region_feed_edit->add(*_feed_name_edit);
    _region_feed_edit->add(*_feed_url_label);
    _region_feed_edit->add(*_feed_url_edit);
    _region_feed_edit->add(*_feed_edit_button);


    _feed_edit_button->signal_clicked().connect([=]() {
        update_feed_source();
        show_feed_names();
    });

    _gautier_rss_area->add(*_region_feed_edit);

    auto style_ctx_feed_changes_region = _region_feed_edit->get_style_context();
    style_ctx_feed_changes_region->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    style_ctx_feed_changes_region->add_class("feed_changes_region");

    auto style_ctx_feed_name_label = _feed_name_label->get_style_context();
    style_ctx_feed_name_label->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    style_ctx_feed_name_label->add_class("feed_change_label");

    auto style_ctx_feed_name_edit = _feed_name_edit->get_style_context();
    style_ctx_feed_name_edit->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    style_ctx_feed_name_edit->add_class("feed_change_field");

    auto style_ctx_feed_url_label = _feed_url_label->get_style_context();
    style_ctx_feed_url_label->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    style_ctx_feed_url_label->add_class("feed_change_label");

    auto style_ctx_feed_url_edit = _feed_url_edit->get_style_context();
    style_ctx_feed_url_edit->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    style_ctx_feed_url_edit->add_class("feed_change_field");
    _feed_url_edit->set_size_request(_screen_w/3);

    auto style_ctx_feed_change_button = _feed_edit_button->get_style_context();
    style_ctx_feed_change_button->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    style_ctx_feed_change_button->add_class("feed_change_button");

    return;
}

void cls::create_ui_region_feed_names() {
    _region_feed_names = new Gtk::ScrolledWindow();

    _gautier_rss_area->add(*_region_feed_names);

    show_feed_names();

    auto style_ctx_feeds_list_region = _region_feed_names->get_style_context();
    style_ctx_feeds_list_region->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    style_ctx_feeds_list_region->add_class("feeds_list_region");

    auto style_ctx_feeds_list = _feed_names_field->get_style_context();
    style_ctx_feeds_list->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    style_ctx_feeds_list->add_class("feeds_list");

    return;
}

