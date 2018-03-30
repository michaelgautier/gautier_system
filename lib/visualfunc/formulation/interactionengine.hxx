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
#ifndef __visualfunc_formulation_interactionengine__
#define __visualfunc_formulation_interactionengine__
#include <vector>
#include <map>

#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include <dlib/geometry.h>

#include "interactionengine.hxx"
#include "interactionstate.hxx"
#include "keyboardtr.hxx"
#include "textbuffer.hxx"
#include "visualcallable.hxx"

namespace visualfunc {
namespace formulation {
using namespace std;
class interactionengine {
  public:
    using interactionstate = ::visualfunc::formulation::InteractionState;
    using textbuffer = ::visualfunc::formulation::textbuffer;
    using visualcallable = ::visualfunc::formulation::visualcallable;
    using interaction_callback_type = void(*)(interactionstate*);

    enum widget_type {
        squared_region = 0,
        left_aligned_button = 1,
        right_aligned_button = 2,
        text_field = 3,
        vertical_scrollbar = 4
    };

    interactionstate* interactionstate_previous;
    int default_font_size = 10;

    double workarea_x = 0;
    double workarea_y = 0;
    double workarea_w = 0;
    double workarea_h = 0;

    double screen_x = 0;
    double screen_y = 0;
    double screen_w = 0;
    double screen_h = 0;

    interactionengine();
    ~interactionengine();

    void persist_interaction_state(interactionstate* interaction_ctx);

    void update_textfield(textbuffer* tb, visualcallable& vc, const int widget_type_id);

    double get_screen_dpi();

    ALLEGRO_FONT* load_sized_font(const int font_size, const char* font_file_location);

    dlib::drectangle measure_text_by_sized_font(const char* str, int font_size, const char* font_file_location);
    constexpr double measure_font_y_offset(const double y1, const double y2, const double h);
    constexpr double measure_widget_y(const double region_h_half, const double button_h);
    void measure_screen(interactionstate* interaction_ctx);

    void activate_allegro_graphics_engine(interactionstate* interaction_ctx, interaction_callback_type);

    void initialize_allegro_graphics_engine(interactionstate* interaction_ctx, keyboardtr* kytr);

    void shutdown_allegro_graphics_engine();

    visualcallable build_visual_vertical_scrollbar(const double x1,
            const double y1,
            const double x2,
            const double y2,
            const double bdr_width,
            const double scrollbar_width);

    visualcallable build_visual_left_aligned_widget(const double x1,
            const double y1,
            const double x2,
            const double y2,
            bool trim_to_label,
            const double x_offset,
            double& next_x,
            const double bdr_width,
            string label_text);

    visualcallable build_visual_right_aligned_button(const double x1,
            const double y1,
            const double x2,
            const double y2,
            bool trim_to_label,
            const double bdr_width,
            string label_text);

    visualcallable build_visual_vertical_widget(const double x1,
            const double y1,
            const double x2,
            const double y2,
            const double y_offset,
            double& next_y,
            const double bdr_width,
            string label_text);

    void draw_region_background(const double x1,
                                const double y1,
                                const double x2,
                                const double y2,
                                ALLEGRO_COLOR& bkg_clr,
                                ALLEGRO_COLOR& bdr_clr,
                                const double bdr_width);

    void draw_scrollbar_right_background(const double x1,
                                         const double y1,
                                         const double x2,
                                         const double y2,
                                         ALLEGRO_COLOR& bkg_clr,
                                         ALLEGRO_COLOR& bdr_clr,
                                         const double bdr_width,
                                         const double scrollbar_width);

    void draw_left_aligned_widget(const double x1,
                                  const double y1,
                                  const double x2,
                                  const double y2,
                                  ALLEGRO_COLOR& bkg_clr,
                                  ALLEGRO_COLOR& bdr_clr,
                                  const double bdr_width,
                                  string label_text,
                                  ALLEGRO_COLOR& label_color);

    void draw_right_aligned_button(const double x1,
                                   const double y1,
                                   const double x2,
                                   const double y2,
                                   ALLEGRO_COLOR& bkg_clr,
                                   ALLEGRO_COLOR& bdr_clr,
                                   const double bdr_width,
                                   string label_text,
                                   ALLEGRO_COLOR& label_color);

    void draw_visual_vertical_widget(const double x1,
                                     const double y1,
                                     const double x2,
                                     const double y2,
                                     ALLEGRO_COLOR& bkg_clr,
                                     ALLEGRO_COLOR& bdr_clr,
                                     const double bdr_width,
                                     string label_text,
                                     ALLEGRO_COLOR& label_color);

  private:
    bool _is_allegro_initialized = false;
    bool _is_allegro_uninitialized = false;

    std::string _default_window_title = "Gautier RSS";
    char* _font_file_location;

    int _default_widget_font_size = 12;
    double _default_label_margin_left;

    const double _avg_physical_screen_size = 13.667;
    const double _print_point_size = 72.0;

    double _screen_dpi_previous = 96;
    double _screen_dpi = 0;

    map<int, ALLEGRO_FONT*> _fonts;

    /*Graphics Engine*/
    ALLEGRO_DISPLAY*
    _win_ctx = nullptr;

    ALLEGRO_MONITOR_INFO
    _win_screen_info;

    ALLEGRO_EVENT_QUEUE*
    _win_msg_evt_queue = nullptr;

    ALLEGRO_EVENT_SOURCE*
    _win_msg_evt_src = nullptr;

    ALLEGRO_EVENT
    _winmsg_event;

    ALLEGRO_EVENT_QUEUE*
    _mouse_evt_queue = nullptr;

    ALLEGRO_EVENT_SOURCE*
    _mouse_evt_src = nullptr;

    ALLEGRO_EVENT
    _mouse_event;
};
}
}
#endif
