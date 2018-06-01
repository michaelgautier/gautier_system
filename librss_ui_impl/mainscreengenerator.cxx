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

void cls::show_headlines() {
    auto headline_buttons = _headlines->get_children();
    int headline_buttons_size = headline_buttons.size();

    for(int i = 0; i < headline_buttons_size; i++) {
        auto headlinebtn = headline_buttons[i];

        headlinebtn->hide();

        _headlines->remove(*headlinebtn);
    }

    /*GTK ListBox is extremely unreliable.*/
    Gtk::Button headline_label;

    const int articles_size = _feed_articles.size();

    int headline_label_y = 0;
    int headline_height = 0;

    for(int article_index = 0; article_index < articles_size; article_index++) {
        string headline = _feed_articles[article_index].headline;

        headline_label = Gtk::Button(headline);

        /*
                Measure the label pixels to layout them out horizontally.
                https://developer.gnome.org/gtkmm-tutorial/stable/sec-drawing-text.html.en
                https://developer.gnome.org/gtk3/stable/gtk-question-index.html
        */
        auto pglyt = headline_label.create_pango_layout(headline);

        if(headline_label_y < 1) {
            int headline_width = 0;

            pglyt->get_pixel_size(headline_width, headline_height);

            headline_height = headline_height + 22;
        }

        _headlines->put(headline_label, 0, headline_label_y);

        headline_label_y = headline_label_y + headline_height;

        headline_label.set_size_request(screen_w,headline_height);
        headline_label.show();
        /*GTK Styles*/
        auto style_ctx_headline_button = headline_label.get_style_context();
        style_ctx_headline_button->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_headline_button->add_class("headline_button");
    }

    _headlines->set_size(screen_w, headline_label_y);

    headline_buttons = _headlines->get_children();
    headline_buttons_size = headline_buttons.size();

    for(int i = 0; i < headline_buttons_size; i++) {
        auto headlinebtn = (Gtk::Button*)headline_buttons[i];

        headlinebtn->signal_clicked().connect([=]() {
            show_article_summary(i);
        });
    }

    return;
}

void cls::show_article_summary(int article_index) {
    auto feed_article_entry = _feed_articles[article_index];

    string article_content = feed_article_entry.description;

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

        /*GTK Styles*/
        _css_provider = Gtk::CssProvider::create();

        /*Use this when the css is final for the compile.*/
        _css_provider->load_from_resource("/styles/styles.css");

        /*Use to quickly test out css changes.*/
        //_css_provider->load_from_path("styles.css");

        _gautier_rss_window = new Gtk::ApplicationWindow();

        get_screen_wh();

        int headline_region_h = 0;
        int contents_region_h = 0;

        headline_region_h = screen_h / 3;

        contents_region_h = ((screen_h - headline_region_h) - (headline_region_h - (headline_region_h / 3) ) );








        _gautier_rss_window->set_title("Gautier RSS");

        _gautier_rss_window->set_default_size(screen_max_w,screen_max_h);
        _gautier_rss_window->maximize();

        _gautier_rss_area = new Gtk::Box(Gtk::Orientation::ORIENTATION_VERTICAL);
        /*

        ---------------------------------------------------------------------

        5/31/2018

        I decided to use gtkmm as the software framework for the user interface.
        At the time this was written, I only use Linux operating systems in my private life.
        I do not use Microsoft Windows or Apple MacOS.

        Previously, I was using Allegro as the software framework. I preferred Allegro or similar
        but the requirements to use Allegro was too granular for the intended result.

        My original intent was to use one software framework that could be applied across all UI operating system environments.
        FLTK, Allegro, SFML, and SDL allows this, but each had tradeoffs that worked well for some situations but not others.

        GTK+ is not better than those, it is simply more convenient to use as a software framework under Linux.
        That said, GTK+ has problems.
                - I do not like the CSS (Cascading Style Sheets) approach to the user interface.
                - I am beyond accepting of CSS as a suppliment to HTML but it is not a solid implementation in GTK+.
                - The CSS approach is misleading because it is approximately 70% of the CSS specification
                        and simple things like width, height does not work well.
                - Style sheet errors will actually cause the program to abruptly render the UI as if no styles are active.
                - The official manual/tutorial "Programming with GTKMM" seems incomplete and does not fully explain certain aspects.
                - No good books in early/mid 2018 on GTK. The most recent book is 10 years old.
                - Too much of an emphasis on web-based documentation. Some people "need" to code without a constant Internet connection.
                - Inconsistent API when it comes to list style controls compared to similar controls in other UI toolkits.
                - Unexplainable visual rendering problems as in ListBox widget showing list items at regular size in the beginning
                        but when you clear those items out and add new items, the new items are rendered at a height so small
                        that rows almost seems invisible.
                - The [biggest] problem is that GTK+ seems designed to nudge you to design UI software a certain way.
                        * Subclassing. GTK+ documentation/tutorials strongly slanted towards inheriting from Widgets.
                        * Overcomplicating data binding with TreeModels when what you need is a simple list with a selection event.
                        * Signals on widgets that have very weak callback signatures. You have to concoct hacks using C++11 lambda capturing.
                        * Noticable widget rendering issues depending on if the widget is heap vs stack allocated.
                        * Moving you in the direction of not using pointers but then using RefPtr types that require use of the indirection operator.

                - Despite those issues, the GTK+ software framework is useable. Definitely useful in a Linux/UNIX environment.
                        I most certainly think that GTK+ sucks but that it is at the same time convenient. As a result, I have
                        made a practical decision to use GTK+.

        ---------------------------------------------------------------------

                Original comments*****

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

        _gautier_rss_window->add(*_gautier_rss_area);

        /*
                Original comments*****

                5/27/2018
                You only get a modicum of a background color when you add content. It should not be that way.
                You should be able to get background color on just the window itself or at least a container
                that auto sizes to the width and height of the container. GTK provides no direct, intuitive way to achieve this.
                At its core, all GTK offers is auto widget arrangement and basic widgets but not a straightforward path to making slick UI.
        */
        //auto feed_button = Gtk::Button("test");
        //gautier_rss_area.add(feed_button);

        /*Header*/
        _header_region = new Gtk::Box(Gtk::Orientation::ORIENTATION_HORIZONTAL);
        //header_region.override_background_color(Gdk::RGBA("255,153,85"));
        auto header_text = Gtk::Label("Gautier RSS");
        _header_region->add(header_text);
        _gautier_rss_area->add(*_header_region);

        /*Headlines*/
        _headlines_region = new Gtk::ScrolledWindow();
        _headlines_region->set_size_request(screen_max_w,headline_region_h);

        _headlines = new Gtk::Layout();
        _headlines_region->add(*_headlines);

        show_headlines();

        _gautier_rss_area->add(*_headlines_region);

        /*Content*/
        _content_region = new Gtk::ScrolledWindow();
        _content_region->set_size_request(screen_max_w,contents_region_h);

        _article_content = new Gtk::Label();
        _article_content->set_lines(-1);
        _article_content->set_line_wrap(true);
        _article_content->set_single_line_mode(false);

        _content_region->add(*_article_content);

        _gautier_rss_area->add(*_content_region);

        /*RSS Feed Edit*/
        _feed_edit_region = new Gtk::Box(Gtk::Orientation::ORIENTATION_HORIZONTAL);
        auto feed_name_label = Gtk::Label("Feed Name");
        auto feed_name_edit = Gtk::Entry();
        auto feed_url_label = Gtk::Label("Feed website address");
        auto feed_url_edit = Gtk::Entry();
        auto feed_edit_button = Gtk::Button("Save");

        _feed_edit_region->add(feed_name_label);
        _feed_edit_region->add(feed_name_edit);
        _feed_edit_region->add(feed_url_label);
        _feed_edit_region->add(feed_url_edit);
        _feed_edit_region->add(feed_edit_button);

        _gautier_rss_area->add(*_feed_edit_region);

        /*RSS feed names*/
        _feed_names_region = new Gtk::ScrolledWindow();
        auto feed_names_field = Gtk::Layout();

        _feed_names_region->add(feed_names_field);

        Gtk::Button feedname_label;

        const int feednames_size = _feednames.size();

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

            feed_names_field.put(feedname_label, feedname_label_width, 0);

            feedname_label_width = feedname_label_width + feedtext_width + 22;

            /*GTK Styles*/
            auto style_ctx_feedname_button = feedname_label.get_style_context();
            style_ctx_feedname_button->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
            style_ctx_feedname_button->add_class("feed_button");
        }

        /*You have to do this in this way because none but the last feedname_index value is captured in the previous loop.*/
        auto feedname_buttons = feed_names_field.get_children();
        int feedname_buttons_size = feedname_buttons.size();

        /*You have to do this in this way because none but the last feedname_index value is captured in the previous loop.*/
        for(int i = 0; i < feedname_buttons_size; i++) {
            auto feedbtn = (Gtk::Button*)feedname_buttons[i];
            feedbtn->signal_clicked().connect([=]() {
                show_feed(i);
            });
        }

        _gautier_rss_area->add(*_feed_names_region);

        /*GTK Styles*/
        auto style_ctx_window = _gautier_rss_window->get_style_context();
        style_ctx_window->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_window->add_class("window");

        auto style_ctx_header = _header_region->get_style_context();
        style_ctx_header->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_header->add_class("header");

        auto style_ctx_headlines_region = _headlines_region->get_style_context();
        style_ctx_headlines_region->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_headlines_region->add_class("headlines_region");

        auto style_ctx_contents_region = _content_region->get_style_context();
        style_ctx_contents_region->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_contents_region->add_class("contents_region");

        auto style_ctx_feed_changes_region = _feed_edit_region->get_style_context();
        style_ctx_feed_changes_region->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_feed_changes_region->add_class("feed_changes_region");

        auto style_ctx_feed_name_label = feed_name_label.get_style_context();
        style_ctx_feed_name_label->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_feed_name_label->add_class("feed_change_label");

        auto style_ctx_feed_name_edit = feed_name_edit.get_style_context();
        style_ctx_feed_name_edit->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_feed_name_edit->add_class("feed_change_field");

        auto style_ctx_feed_url_label = feed_url_label.get_style_context();
        style_ctx_feed_url_label->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_feed_url_label->add_class("feed_change_label");

        auto style_ctx_feed_url_edit = feed_url_edit.get_style_context();
        style_ctx_feed_url_edit->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_feed_url_edit->add_class("feed_change_field");
        feed_url_edit.set_size_request(screen_w/3);

        auto style_ctx_feed_change_button = feed_edit_button.get_style_context();
        style_ctx_feed_change_button->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_feed_change_button->add_class("feed_change_button");

        auto style_ctx_feeds_list_region = _feed_names_region->get_style_context();
        style_ctx_feeds_list_region->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_feeds_list_region->add_class("feeds_list_region");

        auto style_ctx_feeds_list = feed_names_field.get_style_context();
        style_ctx_feeds_list->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        style_ctx_feeds_list->add_class("feeds_list");

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
