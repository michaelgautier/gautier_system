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

#include "rss_set_feed_name.hxx"
#include "rss_set_feed_headline.hxx"

#include "rss_cycle_feed_name.hxx"
#include "rss_cycle_feed_headline.hxx"
#include "rss_cycle_feed_article.hxx"

using namespace std;
using cls = ::rss::ui::mainscreengenerator;

/*
        Various levels of initialization
        depending on the amount of preparation required
        before the user interface is laid out.
*/
cls::mainscreengenerator() {
    return;
}

void cls::init() {
    _inited = true;

    return;
}

void cls::generate() {
    if(_inited) {
        show_screen();
    }

    return;
}

/*
        Primary function that lays out the screen in the beginning.
        It is called by the generate function.
        The program will instantiate this class and after calling init, call generate.
*/
int cls::show_screen() {
    int gtk_app_err = 0;

    try {
        /*
                GTK+ starts with this call.
                The user interface is setup first and
                the app instance connects the user interface
                instructions to the operating system's
                graphics and interactivity processes.
        */
        auto app = Gtk::Application::create("gautier.rss");

        /*
                Sets up the size of the program screen.
                Initializes components for aesthetics.
                Calls setup_ui_region_layout_parameters() which
                sets up the initial geometry for the screen.

                The geometry is important because it provides
                more granular control over how the screen is laid out.

                While it is true GTK provides abundant automatic layout
                capability, you still need granular control to have the
                screen appear properly. Rather that hard set most of the
                geometry throughout the screen logic functions, the core
                geometry is consolidated into the setup_ui_region_layout_parameters() function.
        */
        setup_ui_layout_parameters();

        /*
                The window is where all the individual visual parts
                reside and the general context in which operations
                of an interactive nature are exercised.
        */
        create_ui_window();

        /*
                Visually represented program functions are defined within
                functional areas called regions. A region is a geometric
                area that defines the maximum visual placement of
                visually represented functions as well as the continued
                existence of those functions.

                Regions are generally created the same way and are then
                setup with specialized visual elements. Existence of visual elements
                are sustained by the existence of regions within a running window
                within an active GTK app instance.

                Each visual element in a region is specialized based on the
                intended purpose of the region. A region has an overall set of
                functions that each widget supports. It is the visual element
                in the region that maps the visual interface to the underlying
                logic/data interface at the application level.

                These are the unique, application specific layout functions for the RSS program.
        */
        create_ui_region_header();
        create_ui_region_headlines();
        create_ui_region_article_summary();
        create_ui_region_content();
        create_ui_region_feed_edit();
        create_ui_region_feed_names();

        /*
                A second pass at the screen geometry based on updates
                from the realization of all regions.

                The region geometry function is also defined separately
                as it is reapplied during externally triggered window/screen modifications.
        */
        setup_ui_region_layout_parameters();

        /*
                This does not really show anything, just flags the visual elements as ready for presentation on screen.
        */
        _gautier_rss_window->show_all();

        /*
                Get RSS data, apply logic, and ready visual elements with the information to show on screen.
        */
        show_feed(0);

        /*
                Once the following is called, the screen in its initial form is shown
                including any updates to the visual widgets. After this point,
                the program will hold here and any interactivity is dependent on GTK signaling system.
        */
        gtk_app_err = app->run(*_gautier_rss_window);

        /*
                The following exception handlers are primarily useful during development of the program
                to catch errors in setting up GTK or the RSS engine.

                When the program is setup correctly and barring any system interface issues, these exception
                handlers would not be called. A default handler is defined as a fallback to provide diagnostic
                information when the stated reliability expectation does not apply in a given situtation.
        */
    } catch(const Gtk::CssProviderError& ex) {
        std::cout << "CssProviderError, Gtk::CssProvider::load_from_path() failed: " << ex.what() << "\n";
    } catch(const Glib::Error& ex) {
        std::cout << "Error, Gtk::CssProvider::load_from_path() failed: " << ex.what() << "\n";
    } catch(const std::exception& e) {
        std::cout << e.what();
    }

    return gtk_app_err;
}

/*
        GTK provides a way to incorporate CSS directives
        into the program. This prevents loss of the CSS data
        and speeds up retrieval of this information.

        This function preloads the CSS information in order
        to systematically update appearance.

        Geometric size of the screen is also gathered to
        be used by the setup_ui_region_layout_parameters() function.

        Initial size of the regions, padding, and margins are
        defined through this function.
*/
void cls::setup_ui_layout_parameters() {
    /*GTK Styles*/
    _css_provider = Gtk::CssProvider::create();

    _css_provider->load_from_resource("/styles/styles.css");

    get_screen_wh();

    setup_ui_region_layout_parameters();

    return;
}

/*
        Primary visual element within which all other visual elements
        are defined, presented, and managed.

        This is not the only way to define a visual window
        and I chose a minimal construction to defer more to
        the default settings for the operating system for
        constructed windows. Extensive window customization is
        not necessary in this case.
*/
void cls::create_ui_window() {
    _gautier_rss_window = new Gtk::ApplicationWindow();
    _gautier_rss_window->set_title("Gautier RSS");

    _gautier_rss_window->set_default_size(_screen_w/2,_screen_h/2);
    _gautier_rss_window->maximize();

    _gautier_rss_area = new Gtk::Box(Gtk::Orientation::ORIENTATION_VERTICAL);

    _gautier_rss_window->add(*_gautier_rss_area);

    auto style_ctx_window = _gautier_rss_window->get_style_context();
    style_ctx_window->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    style_ctx_window->add_class("window");

    /*
    I left this here as a cautionary note of what not to do:

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

/*
        Geometry of the screen that contains the window.
        The maximum visual boundary used to define
        subsequent geometries. This function should be called
        first. Many user interfaces will have a function like
        this somewhere either overt (like here) or buried in an abstraction.
*/
void cls::get_screen_wh() {
    auto display = Gdk::Display::get_default();
    auto screen = display->get_default_screen();

    _screen_w = screen->get_width();
    _screen_h = screen->get_height();

    return;
}

/*

        The following functions setup the visual areas
        within the main window. Interactive visual elements
        for the program start within these functions.

        The functions themselves set the geometric boundaries
        for the visual elements contained. Initial triggers
        for some visual elements are also defined.

        However, main logic involving RSS feed data is fully
        separate and initiates in the show_feed() function.
        The show_feed() function then unites the RSS logic/data
        facilities with the visual representation of RSS operations
        involving network, file, and data.

        This is stated as the regions are not essential for
        RSS functionality but are part of the means to present
        RSS data and trigger RSS operations through visual interaction.

*/

/*
****************************START REGION SETUP
*/
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

    auto style_ctx = _region_headlines->get_style_context();
    style_ctx->add_provider(_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    style_ctx->add_class("headlines_region");

    return;
}

void cls::create_ui_region_article_summary() {
    _region_article_summary = new Gtk::Box(Gtk::Orientation::ORIENTATION_HORIZONTAL);

    _article_source_label = new Gtk::Label("");
    _article_link_button = new Gtk::LinkButton("http:://about:blank", "Open article page");
    _article_link_label = new Gtk::Label("No article yet");

    _region_article_summary->add(*_article_source_label);
    _region_article_summary->add(*_article_link_button);
    _region_article_summary->add(*_article_link_label);

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

    /*
        I left this here in case this proves useful in the future.

        Previously used to show the full article web page in the program.
        Worked well, but I decided that was not the best use of browser rendering capabilities.
        Better to allow full web browsers like Mozilla Firefox, Google Chrome, Chromium, and Microsoft Edge
        to show the content. The reasons to defer to full web browsers for rendering articles include:

                1)      Full use of browser extensions.
                2)      Security against web content injection attacks.
                3)      A complete security mode does not exist in just the rendering portion of the WebKit API.
                4)      User-Agent strings sent by web browsers are better recognized.

        //https://stackoverflow.com/questions/17039942/example-of-using-webkitgtk-with-gtkmm-3-0
        _article_content_web_backend = WEBKIT_WEB_VIEW( webkit_web_view_new() );
        _article_content_web = Glib::wrap( GTK_WIDGET( _article_content_web_backend ) );

        _region_content = _article_content_web;

        _region_content->add(*_article_content_web);

        _article_content_web_backend_settings = webkit_settings_new();

        webkit_settings_set_allow_modal_dialogs(_article_content_web_backend_settings, false);
        webkit_settings_set_auto_load_images (_article_content_web_backend_settings, false);
        webkit_settings_set_enable_developer_extras(_article_content_web_backend_settings, false);
        webkit_settings_set_enable_frame_flattening(_article_content_web_backend_settings, true);
        webkit_settings_set_enable_html5_database(_article_content_web_backend_settings, false);
        webkit_settings_set_enable_html5_local_storage(_article_content_web_backend_settings, false);
        webkit_settings_set_enable_javascript (_article_content_web_backend_settings, false);
        webkit_settings_set_enable_java (_article_content_web_backend_settings, false);
        webkit_settings_set_enable_plugins (_article_content_web_backend_settings, false);
        webkit_settings_set_enable_webaudio (_article_content_web_backend_settings, false);
        webkit_settings_set_enable_webgl (_article_content_web_backend_settings, false);
        webkit_settings_set_javascript_can_access_clipboard(_article_content_web_backend_settings, false);
        webkit_settings_set_javascript_can_open_windows_automatically(_article_content_web_backend_settings, false);
        webkit_settings_set_media_playback_allows_inline(_article_content_web_backend_settings, false);

        webkit_web_view_set_settings (_article_content_web_backend, _article_content_web_backend_settings);
    */
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
/*
****************************END OF REGION SETUP
*/

/*
        Primary screen layout function.
        Generally speaking, this function can work independent of
        GTK which means it applies to all user interface engines.

        Previous versions of this program had a similar function.
        This one has been calibrated to the particular use of GTK.

        However, the general principle is the same. Define
        geometric area in which user interface widgets present.
        That constrains the size and position of those widgets.

        While it is true GTK provides abundant automatic layout
        capability, you still need granular control to have the
        screen appear properly. Rather that hard set most of the
        geometry throughout the screen logic functions, the core
        geometry is consolidated into the setup_ui_region_layout_parameters() function.

        This function is also applied whenever the window/screen is updated
        through externally triggered changes.
*/
void cls::setup_ui_region_layout_parameters() {
    if(_last_window_h != _window_h || _last_window_w != _window_w) {
        int window_h = _screen_h;

        _region_header_w = 0;
        _region_header_h = _screen_w/2;

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

        double remaining_h1 = window_h;
        double remaining_h2 = 0;

        double height_ratio = 30;
        double height = 0;

        const int max_elems = 6;

        if(_window_h > 0) {
            window_h = _window_h;
        }

//cout << "remaining height start with " << remaining_h1 << "\n";
        for(int index = 0; index < max_elems; index++) {
            switch(index) {
            case 0: { //RSS Reader Header
                height_ratio = 30;
                remaining_h2 = remaining_h1 / height_ratio;

                _region_header_h = remaining_h2;
            }
            break;
            case 1: {//feed names
                height_ratio = 30;
                remaining_h2 = remaining_h1 / height_ratio;

                _region_feed_names_h = remaining_h2;
            }
            break;
            case 2: {//feed edit
                height_ratio = 30;
                remaining_h2 = remaining_h1 / height_ratio;

                _region_feed_edit_h = remaining_h2;
            }
            break;
            case 3: {//article headline and control
                height_ratio = 30;
                remaining_h2 = remaining_h1 / height_ratio;

                _region_article_summary_h = remaining_h2;
            }
            break;
            case 4: { //RSS Reader article content
                height_ratio = 8;
                remaining_h2 = remaining_h1 / height_ratio;

                _region_content_h = remaining_h2;
            }
            break;
            case 5: { //RSS Reader Headlines
                height_ratio = 1.3;
                remaining_h2 = remaining_h1 / height_ratio;

                _region_headlines_h = remaining_h2;
            }
            break;
            }
            //cout << "   index " << index << " " << "remaining h1 " << remaining_h1 << " remaining h2 " << remaining_h2 << "\n";

            remaining_h1 = (remaining_h1 - remaining_h2);

            //cout << "           remaining h1 final: " << remaining_h1 << "\n";
        }

        if(_region_header) {
            _region_header->set_size_request(_region_header_w, _region_header_h);
        }

        if(_region_content) {
            _region_content->set_size_request(_region_content_w, _region_content_h);
        }

        if(_region_headlines) {
            _region_headlines->set_size_request(_region_headlines_w, _region_headlines_h);

            resize_headlines();
        }

        _last_window_w = _window_w;
        _last_window_h = _window_h;
    }

    return;
}

/*
****************************START USER INTERFACE FUNCTIONALITY
*/
void cls::show_feed(const int& feed_index) {
    _feed_index = feed_index;

    if(_feed_index >= 0) {
        if(!_feed_names_location.empty()) {
            news::rss_cycle_feed_name rss_c_feed_name;

            rss_c_feed_name.init(_feed_names_location);
            news::rss_data_feed_name_spec feed_name = rss_c_feed_name.get_single_feed_name(_feed_index);

            _article_source_label->set_text(feed_name.name);
        }

        show_headlines();

        show_headline_description(0);
    }

    return;
}

void cls::remove_headlines() {
    auto headline_items = _headlines->get_children();
    auto headline_items_size = headline_items.size();

    for(int i = 0; i < headline_items_size; i++) {
        auto headlinebtn = (Gtk::Button*)headline_items[i];

        _headlines->remove(*headlinebtn);

        if(headlinebtn) {
            delete headlinebtn;
        }
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

    news::rss_cycle_feed_name rss_c_feed_name;

    rss_c_feed_name.init(_feed_names_location);
    news::rss_data_feed_name_spec feed_name = rss_c_feed_name.get_single_feed_name(_feed_index);

    news::rss_cycle_feed_headline rss_c_feed_headline;

    rss_c_feed_headline.init(feed_name.name + ".txt");
    news::rss_set_feed_headline feed_headline_set = rss_c_feed_headline.get_feed_headlines(feed_name);

    _feed_headlines = feed_headline_set.get_specs();

    /*
        Add an empty entry. Addresses a bug in the Gtk rendering process in which the last item added in a loop is not shown.
        Or, I am indirectly allocating the objects incorrectly.
        Either way, the faux entry allows all headlines to show. Since the last item added in the loop is not shown, the blank entry is ignored.
        This may not be the case in all situations, operating systems, etc., but I will start with what I can observe.
    */
    _feed_headlines.push_back(news::rss_data_feed_headline_spec());

    const int headlines_size = _feed_headlines.size();

    int headline_label_y = 0;
    int headline_height = 0;

    for(int headline_index = 0; headline_index < headlines_size; headline_index++) {
        string headline = _feed_headlines[headline_index].headline;

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
            show_headline_description(i);
        });
    }

    resize_headlines();

    return;
}

void cls::show_feed_names() {
    if(_feed_names_field) {
        _region_feed_names->remove_with_viewport();

        if(_feed_names_field) {
            delete _feed_names_field;
        }
    }

    _feed_names_field = new Gtk::Layout();

    _region_feed_names->add(*_feed_names_field);

    Gtk::Button feedname_label;

    news::rss_cycle_feed_name rss_c_feed_name;

    rss_c_feed_name.init(_feed_names_location);
    news::rss_set_feed_name feed_name_set = rss_c_feed_name.get_feed_names();

    vector<news::rss_data_feed_name_spec> feed_names = feed_name_set.get_specs();

    /*
        Add an empty entry. Addresses a bug in the Gtk rendering process in which the last item added in a loop is not shown.
        Or, I am indirectly allocating the objects incorrectly.
        Either way, the faux entry allows all feed names to show. Since the last item added in the loop is not shown, the blank entry is ignored.
        This may not be the case in all situations, operating systems, etc., but I will start with what I can observe.
    */
    feed_names.push_back(news::rss_data_feed_name_spec());

    const int feednames_size = feed_names.size();

    int feedname_label_width = 8;

    for(int feedname_index = 0; feedname_index < feednames_size; feedname_index++) {
        string feedname = feed_names[feedname_index].name;

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

void cls::show_headline_description(const int& headline_index) {
    news::rss_data_feed_headline_spec feed_headline_entry;

    int headlines_size = _feed_headlines.size();

    if(headline_index < headlines_size) {
        feed_headline_entry = _feed_headlines[headline_index];

        _article_link_button->set_uri(feed_headline_entry.url);
        _article_link_label->set_text(feed_headline_entry.url);
    } else {
        feed_headline_entry.url = "http://about:blank";
        feed_headline_entry.description = "No headlines yet";

        _article_link_button->set_uri(feed_headline_entry.url);
        _article_link_label->set_text(feed_headline_entry.description);
    }

    string headline_description = feed_headline_entry.url;

    if(headline_description.size() > _headline_description_max_chars) {
        headline_description = headline_description.substr(0, _headline_description_max_chars);
    }

    _article_content->set_single_line_mode(false);
    _article_content->set_text(feed_headline_entry.description);

    /*
        Previously used to show the full article web page within the screen.

        //https://stackoverflow.com/questions/17039942/example-of-using-webkitgtk-with-gtkmm-3-0
                webkit_web_view_load_uri(_article_content_web_backend, string(feed_headline_entry.url).data());
    */

    return;
}

void cls::show_headline_description_selected_row(Gtk::ListBoxRow* row) {
    if(row) {
        show_headline_description(row->get_index());
    }

    return;
}

void cls::resize_headlines() {
    if(_headlines) {
        auto pglyt = _headlines->create_pango_layout("H");

        int headline_width = 0;
        int headline_height = 0;

        pglyt->get_pixel_size(headline_width, headline_height);

        /*Gtk Scrolled Window does not handle exact heights in the content consistently but if you add a multiplier
        twice to double the height, it appears to truncate the viewport to items that can be displayed.*/
        const int height_multiplier = 2;

        int headlines_h = (_feed_headlines.size() * height_multiplier) * headline_height;

        _headlines->set_size(_screen_w, headlines_h);
    }

    return;
}

bool cls::update_feed_source() {
    bool feed_sources_updated = false;

    string feedname = _feed_name_edit->get_text();
    string feedurl = _feed_url_edit->get_text();

    if(!feedname.empty() && !feedurl.empty()) {
        news::rss_data_feed_name_spec feed_name_entry;

        feed_name_entry.name = feedname;
        feed_name_entry.url = feedurl;

        news::rss_cycle_feed_name rss_c_feed_name;

        rss_c_feed_name.init(_feed_names_location);
        news::rss_set_feed_name feed_name_set = rss_c_feed_name.get_feed_names();

        vector<news::rss_data_feed_name_spec> feed_names = feed_name_set.get_specs();

        bool feed_name_found = false;

        for(news::rss_data_feed_name_spec feed_name : feed_names) {
            feed_name_found = (feedname == feed_name.name);

            if(feed_name_found) {
                break;
            }
        }

        if(!feed_name_found) {
            news::rss_set_consequence cs = rss_c_feed_name.set_single_feed_name(feed_name_entry);

            feed_sources_updated = (cs.size() > 0 && cs[0].get_catalog_number() > 0);
        }
    }

    return feed_sources_updated;
}
/*
****************************END USER INTERFACE FUNCTIONALITY
*/

/*
        RESOURCE CLEANUP
        RAW POINTER DEALLOCATION

        C++ RAII Technique
        Automatic resource cleanup

        I rarely use raw pointers in this program.
        Discovering any pointers in the RSS library is nearly impossible.
        The primary exception in the RSS library is the network interface.

        However, the user interface uses several raw pointers as I find
        the lifetime of the visual elements is better managed that way.

        The many hours of tests I ran on the difference between using
        raw pointers versus scoped objects are not definitive. Rather,
        I observed more consistent user interface behavior in a
        C++ application on Fedora Linux under the Gnome 3 desktop with
        a user interface is defined in gtkmm where the widgets were
        referenced via raw pointer.

        I did not evaluate the use of smart pointers. I actually wanted
        precise determinism in the use of pointers. Some of the gtkmm
        widgets are themselves defined in a type of smart pointer and
        I needed those instances to automatically persist rather than
        automatically deallocate.

        As a result, I concluded that I can achieve appropriate
        raw pointer initialization at various stages in the
        user interface's construction. Such is the case in the
        single pass execution of the region construction functions.
        When those functions run, most raw pointers to widgets that
        define the overall user interface are established once and
        maintained at class scope. Many of them are deallocated in the
        following destructor function as the program moves beyond the
        point of using an instance of the class.

        The typical case for activation of the destructor is when the
        program ends. Since the primary region objects are often
        containers of other visual elements, they are checked for the
        existence of elements. Each visual element in a container that
        denotes a region is then systematically cleaned up followed by
        removal of the region itself from computer memory. It is often
        best to do it this way to ensure there are no dangling pointers
        to widgets previously associated with the region. Also, in some
        cases, a segmentation fault will occur on attempt to delete a
        region's raw pointer without first deallocating memory used for
        contained widgets.

        The overall process of allocation/deallocation has been vetted
        through various settings in Valgrind on Fedora Linux. That is
        not a definitive guarantee that memory consumed is comprehensively
        recovered, but the matter of memory acquire/release has been
        examined in detail sufficient to the software application.
*/
cls::~mainscreengenerator() {
    if(_region_feed_names) {
        _region_feed_names->remove_with_viewport();

        delete _region_feed_names;
    }

    if(_region_feed_edit) {
        auto region_items = _region_feed_edit->get_children();
        int region_items_size = region_items.size();

        for(int i = 0; i < region_items_size; i++) {
            auto region_item = (Gtk::Widget*)region_items[i];

            if(region_item) {
                _region_feed_edit->remove(*region_item);

                delete region_item;
            }
        }

        delete _region_feed_edit;
    }

    if(_region_content) {
        _region_content->remove_with_viewport();

        delete _region_content;
    }

    if(_region_article_summary) {
        auto region_items = _region_article_summary->get_children();
        int region_items_size = region_items.size();

        for(int i = 0; i < region_items_size; i++) {
            auto region_item = (Gtk::Widget*)region_items[i];

            if(region_item) {
                _region_article_summary->remove(*region_item);

                delete region_item;
            }
        }

        delete _region_article_summary;
    }

    if(_region_headlines) {
        _region_headlines->remove_with_viewport();

        delete _region_headlines;
    }

    if(_region_header) {
        auto region_items = _region_header->get_children();
        int region_items_size = region_items.size();

        for(int i = 0; i < region_items_size; i++) {
            auto region_item = (Gtk::Widget*)region_items[i];

            if(region_item) {
                _region_header->remove(*region_item);

                delete region_item;
            }
        }

        delete _region_header;
    }

    if(_gautier_rss_area) {
        auto region_items = _gautier_rss_area->get_children();
        int region_items_size = region_items.size();

        for(int i = 0; i < region_items_size; i++) {
            auto region_item = (Gtk::Widget*)region_items[i];

            if(region_item) {
                _gautier_rss_area->remove(*region_item);

                delete region_item;
            }
        }

        delete _gautier_rss_area;
    }

    if(_gautier_rss_window) {
        auto region_item = _gautier_rss_window->get_child();

        if(region_item) {
            delete region_item;
        }

        delete _gautier_rss_window;
    }

    return;
}

