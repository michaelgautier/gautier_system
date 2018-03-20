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

#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>

#include <dlib/geometry.h>

#include "visualcallable.hxx"
#include "request.hxx"
#include "material.hxx"

#include "textbuffer.hxx"
#include "interactionstate.hxx"
#include "keyboardtr.hxx"
#include "interactionengine.hxx"

namespace rss {
namespace ui {
using namespace std;
class mainscreengenerator {
  public:
    using material = ::rss::material;
    using request = ::rss::request;

    using interactionstate = ::visualfunc::formulation::InteractionState;
    using textbuffer = ::visualfunc::formulation::textbuffer;
    using visualcallable = ::visualfunc::formulation::visualcallable;
    using keyboardtr = ::visualfunc::formulation::keyboardtr;
    using interactionengine = ::visualfunc::formulation::interactionengine;

    mainscreengenerator();
    ~mainscreengenerator();

    void init();
    void generate();

    /*Application Logic*/
    void process_updates(interactionstate* interaction_ctx);

  private:
    interactionstate* _interaction_ctx;
    interactionengine::interaction_callback_type _interaction_callback;
    interactionengine _uiengine;
    keyboardtr _keyboardtr;

    bool _article_contents_enlarge = false;
    bool _article_selected = false;
    bool _feed_articles_requested = false;
    bool _keyboard_field_active = false;
    bool _processing = false;
    bool _render_is_requested = false;

    const char* _font_file_location = "NotoSans-Regular.ttf";
    const double _default_label_margin_left = 4;
    const int _default_widget_font_size = 12;

    int _feed_index = 0;//Defaults to the first feed, if available;
    int _headline_index = -1;

    string _feed_names_location = "feeds.txt";

    vector<material> _feed_articles;
    vector<request> get_rss_feed_data(int feed_source_index, vector<material>& feed_articles);
    vector<string> _feednames;

    vector<visualcallable> _callables;
    vector<visualcallable> get_visual_definitions(int screen_x, int screen_y, int screen_w, int screen_h);

    void build_visual_model();
    void process_interactions(interactionstate* interaction_ctx);
    void update_visual_output();

    bool update_feed_source();
    void get_rss_feed_names_and_articles();

    /*Widget geometry and visualization*/
    enum visual_index_rss_reader_region {
        header = 0,//RSS Reader Header
        headlines = 1,//RSS Reader Headlines
        article_content = 2,//RSS Reader article content
        control_bar = 3,//RSS Reader Control Bar
        change_bar = 4,//RSS Reader RSS Change Bar
        choice_bar = 5//RSS Reader Feed Choice Bar
    };
};
}
}
#endif
