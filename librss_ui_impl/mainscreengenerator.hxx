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

#ifndef __rss_ui_mainscreengenerator__
#define __rss_ui_mainscreengenerator__

#include <vector>

#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <dlib/geometry.h>

#include "visualcallable.hxx"
#include "request.hxx"
#include "material.hxx"

#include "interactionstate.hxx"

namespace rss {
namespace ui {
        using namespace std;
        class mainscreengenerator {
                public:
	        using interactionstate = visualfunc::formulation::InteractionState;
	        using InteractionCallBackType = void(*)(interactionstate&);

                mainscreengenerator();
                ~mainscreengenerator();
                void init();
                void generate();
                vector<rss::request> get_rss_feed_data(int feed_source_index);

                /*Application Logic*/

                void ProcessUpdates(interactionstate& interactionState);

                private:

                /*Graphics Engine*/
                ALLEGRO_DISPLAY* 
                        _WinCtx = nullptr;

                ALLEGRO_MONITOR_INFO 
                        _WinScreenInfo;

                ALLEGRO_EVENT_SOURCE* 
                        _WinMsgEvtSrc = nullptr;

                ALLEGRO_EVENT_SOURCE* 
                        _MouseEvtSrc = nullptr;

                ALLEGRO_EVENT_QUEUE* 
                        _WinMsgEvtQueue = nullptr;

                ALLEGRO_EVENT 
                        _winmsg_event;

                ALLEGRO_FONT* _Font = nullptr;

                interactionstate 
                        _InteractionState, 
                        _InteractionStateLast;

                InteractionCallBackType 
                        _InteractionCallBack;

                bool 
                        _IsAllegroInitialized, 
                        _IsAllegroUnInitialized,
                        _IsScreenDPICached;
                        
                int 
                        _FontBoxX, 
                        _FontBoxW, 
                        _FontBoxY, 
                        _FontBoxH;

                double 
                        _FontSize, 
                        _ScreenDPI;
                        
                const char* _FontPath = nullptr;

	        void Activate(InteractionCallBackType);
                void StartRenderGraphics();
                void EndRenderGraphics();
                void GetScreenDpi(double& screenDpi);

                bool GetIsVisualModelChanged(interactionstate const& old, interactionstate const& now);
                bool GetIsFontLoaded();
                void SetFontParameters(const char* fontPath, double const& fontSize);
                void GetFont(ALLEGRO_FONT*& font, int& fontBoxX, int& fontBoxY, int& fontBoxW, int& fontBoxH);

	        void Initialize();
	        void Release();

                void LoadFont();
                dlib::drectangle MeasureLineHeight(const char* str);

                void draw_region_background(const double x1, const double y1, const double x2, const double y2, ALLEGRO_COLOR& bkg_clr, ALLEGRO_COLOR& bdr_clr, const double bdr_width);

                void draw_scrollbar_right_background(const double x1, const double y1, const double x2, const double y2, ALLEGRO_COLOR& bkg_clr, ALLEGRO_COLOR& bdr_clr, const double bdr_width, const double scrollbar_width);

                void draw_left_aligned_widget(const double x1, const double y1, const double x2, const double y2, const double x_offset, double& next_x, ALLEGRO_COLOR& bkg_clr, ALLEGRO_COLOR& bdr_clr, const double bdr_width, string label_text, ALLEGRO_COLOR& label_color);

                void draw_right_aligned_button(const double x1, const double y1, const double x2, const double y2, ALLEGRO_COLOR& bkg_clr, ALLEGRO_COLOR& bdr_clr, const double bdr_width, string label_text, ALLEGRO_COLOR& label_color);

                /*Application Logic Implementation*/
                enum visual_index_rss_reader_region {
                        header = 0,//RSS Reader Header
                        headlines = 1,//RSS Reader Headlines
                        article_content = 2,//RSS Reader article content
                        control_bar = 3,//RSS Reader Control Bar
                        change_bar = 4,//RSS Reader RSS Change Bar
                        choice_bar = 5//RSS Reader Feed Choice Bar
                };

                enum visual_index_rss_reader_widget_type {
                        squared_region = 0,
                        left_aligned_button = 0,
                        right_aligned_button = 1,
                        text_field = 2,
                        vertical_scrollbar = 3
                };

                bool _article_contents_enlarge = false;
                bool _article_contents_enlarge_click = false;
                bool _feed_articles_requested = false;
                bool _render_is_requested = false;

                string _feed_names_location = "feeds.txt";

                vector<rss::material> _feed_articles;
                
                void BuildVisualModel(interactionstate& interaction_ctx);
                void ProcessInteractions(interactionstate& interaction_ctx);
                void UpdateVisualOutput(interactionstate& interaction_ctx);

                /*Widget geometry*/
                vector<visualfunc::formulation::visualcallable> _callables;

                const int _text_wh_plus = 20;

                int _text_x = 0;
                int _text_y = 0;
                int _text_w = 0;
                int _text_h = 0;

                int _workarea_x = 0;
                int _workarea_y = 0;
                int _workarea_w = 0;
                int _workarea_h = 0;

                int _screen_x = 0;
                int _screen_y = 0;
                int _screen_w = 0;
                int _screen_h = 0;

                int _last_w = 0;
                int _last_h = 0;
      
                constexpr double measure_widget_y(const double region_h_half, const double button_h) {
                        return (region_h_half - (button_h / 2));
                }

                void measure_screen(interactionstate& interaction_ctx);
                vector<visualfunc::formulation::visualcallable> get_visual_definitions(int screen_x, int screen_y, int screen_w, int screen_h);

                visualfunc::formulation::visualcallable build_visual_vertical_scrollbar(const double x1, const double y1, const double x2, const double y2, 
                const double bdr_width, const double scrollbar_width);

                visualfunc::formulation::visualcallable build_visual_left_aligned_widget(const double x1, const double y1, const double x2, const double y2, 
                const double x_offset, double& next_x, const double bdr_width, string label_text);

                visualfunc::formulation::visualcallable build_visual_right_aligned_button(const double x1, const double y1, const double x2, const double y2, 
                const double bdr_width, string label_text);
        };
}
}
#endif
