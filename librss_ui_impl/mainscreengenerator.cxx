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

void cls::get_screen_wh() {
    auto display = Gdk::Display::get_default();
    auto screen = display->get_default_screen();

    screen_w = screen->get_width();
    screen_h = screen->get_height();

    screen_max_w = (screen_w - (screen_w / 8));
    screen_max_h = (screen_h - (screen_h / 8));

    return;
}

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

int cls::show_screen() {
    int gtk_app_err = 0;

    try {
        auto app = Gtk::Application::create("gautier.rss");

        auto gautier_rss_window = Gtk::ApplicationWindow();

        get_screen_wh();

        int headline_region_h = 0;
        int contents_region_h = 0;

        headline_region_h = screen_h / 3;

        contents_region_h = ((screen_h - headline_region_h) - (headline_region_h - (headline_region_h / 3) ) );








        gautier_rss_window.set_title("Gautier RSS");

        gautier_rss_window.set_default_size(screen_max_w,screen_max_h);
        gautier_rss_window.maximize();

        auto gautier_rss_area = Gtk::Box(Gtk::Orientation::ORIENTATION_VERTICAL);
        /*
                5/27/2018
                Unlike other UI toolkits, GTK will NOT show a background color on an empty widget.
                Other issues:
                        - Cannot directly change visual appearance with high granularity.
                        - Forces you to use CSS.
                        - Not really a software developer friendly API.
                        - It starts out good (make a hello world window), but GTK has usability problems not overcome by tutorials.
                        - A UI toolkit more like FLTK in that you can make basic UI fairly easily, but is far behind
                                other UI toolkits in the expedient creation of even a moderately sophisticated UI.
                        - The toolkit is solid at a technical level but fails to drive visually compelling UI by foregoing
                                many of the conventions and results apparent in other UI toolkits.
        */
        //gautier_rss_area.override_background_color(Gdk::RGBA("60,122,158"));

        gautier_rss_window.add(gautier_rss_area);

        /*
                5/27/2018
                You only get a modicum of a background color when you add content. It should not be that way.
                You should be able to get background color on just the window itself or at least a container
                that auto sizes to the width and height of the container. GTK provides no direct, intuitive way to achieve this.
                At its core, all GTK offers is auto widget arrangement and basic widgets but not a straightforward path to making slick UI.
        */
        //auto feed_button = Gtk::Button("test");
        //gautier_rss_area.add(feed_button);

        /*Header*/
        auto header_region = Gtk::Box(Gtk::Orientation::ORIENTATION_HORIZONTAL);
        //header_region.override_background_color(Gdk::RGBA("255,153,85"));
        auto header_text = Gtk::Label("Gautier RSS");
        header_region.add(header_text);
        gautier_rss_area.add(header_region);

        /*Headlines*/
        auto headlines_region = Gtk::ScrolledWindow();
        headlines_region.set_size_request(screen_max_w,headline_region_h);

        auto headlines = Gtk::ListBox();
        headlines_region.add(headlines);

        Gtk::Label headline_label;

        /*for(int headline_index = 0; headline_index < 100; headline_index++) {
                auto test_row_text = "test " + std::to_string(headline_index+1);
                headline_label = Gtk::Label(test_row_text);
                headlines.append(headline_label);
        }*/

        const int articles_size = _feed_articles.size();

        int headline_index = 0;

        for(int article_index = 0; article_index < articles_size; article_index++) {
            auto feed_article_entry = _feed_articles[article_index];

            string headline = feed_article_entry.headline;

            headline_label = Gtk::Label(headline, Gtk::Align::ALIGN_START);
            headlines.append(headline_label);
        }

        gautier_rss_area.add(headlines_region);

        /*Content*/
        auto content_region = Gtk::ScrolledWindow();
        content_region.set_size_request(screen_max_w,contents_region_h);

        auto content = Gtk::Label();
        _article_content = &content;
        content.set_lines(-1);
        content.set_line_wrap(true);
        content.set_single_line_mode(false);

        content_region.add(content);

        gautier_rss_area.add(content_region);

        /*RSS Feed Edit*/
        auto feed_edit_region = Gtk::Box(Gtk::Orientation::ORIENTATION_HORIZONTAL);
        auto feed_name_label = Gtk::Label("Feed Name");
        auto feed_name_edit = Gtk::Entry();
        auto feed_url_label = Gtk::Label("Feed website address");
        auto feed_url_edit = Gtk::Entry();
        auto feed_edit_button = Gtk::Button("Save");

        feed_edit_region.add(feed_name_label);
        feed_edit_region.add(feed_name_edit);
        feed_edit_region.add(feed_url_label);
        feed_edit_region.add(feed_url_edit);
        feed_edit_region.add(feed_edit_button);

        gautier_rss_area.add(feed_edit_region);


        /*Signals*/
        headlines.signal_row_selected().connect([=](Gtk::ListBoxRow* row) {
            if(row) {
                int article_index = row->get_index();

                auto feed_article_entry = _feed_articles[article_index];

                string article_content = feed_article_entry.description;

                _article_content->set_text(article_content);
            }
        });


        /*GTK Styles*/
        auto css_provider = Gtk::CssProvider::create();

        auto style_ctx_window = gautier_rss_window.get_style_context();
        style_ctx_window->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_window->add_class("window");

        auto style_ctx_header = header_region.get_style_context();
        style_ctx_header->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_header->add_class("header");

        auto style_ctx_headlines_region = headlines_region.get_style_context();
        style_ctx_headlines_region->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_headlines_region->add_class("headlines_region");

        auto style_ctx_contents_region = content_region.get_style_context();
        style_ctx_contents_region->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_contents_region->add_class("contents_region");

        auto style_ctx_feed_changes_region = feed_edit_region.get_style_context();
        style_ctx_feed_changes_region->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_feed_changes_region->add_class("feed_changes_region");

        auto style_ctx_feed_name_label = feed_name_label.get_style_context();
        style_ctx_feed_name_label->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_feed_name_label->add_class("feed_change_label");

        auto style_ctx_feed_name_edit = feed_name_edit.get_style_context();
        style_ctx_feed_name_edit->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_feed_name_edit->add_class("feed_change_field");

        auto style_ctx_feed_url_label = feed_url_label.get_style_context();
        style_ctx_feed_url_label->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_feed_url_label->add_class("feed_change_label");

        auto style_ctx_feed_url_edit = feed_url_edit.get_style_context();
        style_ctx_feed_url_edit->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_feed_url_edit->add_class("feed_change_field");
        feed_url_edit.set_size_request(screen_w/3);

        auto style_ctx_feed_change_button = feed_edit_button.get_style_context();
        style_ctx_feed_change_button->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_feed_change_button->add_class("feed_change_button");

        /*auto style_ctx_feeds_list_region = .get_style_context();
        style_ctx_feeds_list_region->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_feeds_list_region->add_class("feeds_list_region");*/

        /*Use this when the css is final for the compile.*/
        css_provider->load_from_resource("/styles/styles.css");

        /*Use to quickly test out css changes.*/
        //css_provider->load_from_path("styles.css");

        gautier_rss_window.show_all();

        gtk_app_err = app->run(gautier_rss_window);
    } catch(const Gtk::CssProviderError& ex) {
        std::cout << "CssProviderError, Gtk::CssProvider::load_from_path() failed: " << ex.what() << "\n";
    } catch(const Glib::Error& ex) {
        std::cout << "Error, Gtk::CssProvider::load_from_path() failed: " << ex.what() << "\n";
    } catch(const std::exception& e) {
        std::cout << e.what();
    }

    return gtk_app_err;
}
