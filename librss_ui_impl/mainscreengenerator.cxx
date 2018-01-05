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
#include <iostream>
#include <chrono>
#include <ctime>

#include "mainscreengenerator.hxx"

#include "visualcallable.hxx"
#include "collector.hxx"
#include "request.hxx"
#include "material.hxx"
#include "feedscycle.hxx"

using collector = rss::collector;
using material = rss::material;
using request = rss::request;
using feedscycle = rss::feedscycle;
using visualcallable = visualfunc::formulation::visualcallable;

using interactionstate = visualfunc::formulation::InteractionState;

using namespace std;
using cls = rss::ui::mainscreengenerator;

static std::string 
        _DefaultWindowTitle = "Gautier RSS";

static constexpr double 
        _AvgPhysicalScreenSize = 13.667, _PrintPointSize = 72.0;

static double 
        _ScreenDpiLast = 96;

cls* _self = nullptr;

void UpdateDisplay(interactionstate& interactionState);
void clear_to_background_color();

void cls::init() {
        _render_is_requested = true;

        return;
}

void cls::generate() {
        Activate(UpdateDisplay);
        
        return;
}

void UpdateDisplay(interactionstate& interactionState) {
        _self->ProcessUpdates(interactionState);

	return;
}

void cls::measure_screen(interactionstate& interaction_ctx) {
        _workarea_x = interaction_ctx.WindowDimensions.left();
        _workarea_y = interaction_ctx.WindowDimensions.top();
        _workarea_w = interaction_ctx.WindowDimensions.right();
        _workarea_h = interaction_ctx.WindowDimensions.bottom();

        return;
}

void clear_interaction_state_events(interactionstate& interaction_ctx) {
        interaction_ctx.IsWindowResized = false;
        interaction_ctx.IsMouseDown = false;
        interaction_ctx.IsMouseUp = false;
        interaction_ctx.IsVisualModelChanged = false;
        interaction_ctx.IsDisplayUpdated = false;

        return;
}

void cls::ProcessUpdates(interactionstate& interaction_ctx) {
        bool IsVisualModelChanged = GetIsVisualModelChanged(_InteractionState, interaction_ctx);

        if(IsVisualModelChanged) {
                if(!_Font) {
                        if(!GetIsFontLoaded()) {
                                cout << "loading font\n";
                                SetFontParameters("NotoSans-Regular.ttf", 10);
                                cout << "font loaded with size w/h " << _FontBoxW << "/" << _FontBoxH << "\n";
                        }
                }

                BuildVisualModel(interaction_ctx);
                ProcessInteractions(interaction_ctx);
                
                if(_render_is_requested) {
                        _render_is_requested = false;
                        cout << __func__ << " call UpdateVisualOutput, line: " << __LINE__ << "\n";
                        UpdateVisualOutput(interaction_ctx);
                        UpdateVisualOutput(interaction_ctx);
                }
                
                clear_interaction_state_events(interaction_ctx);
                _InteractionState = interaction_ctx;
                _InteractionStateLast = _InteractionState;
	}

        return;
}

void cls::BuildVisualModel(interactionstate& interaction_ctx) {
        measure_screen(interaction_ctx);

        _callables = get_visual_definitions(_workarea_x, _screen_y, _workarea_w, _workarea_h);

        const int callable_size = _callables.size();

        if(_article_contents_enlarge) {
                cout << __func__ << " enlarge button click implementation, line: " << __LINE__ << "\n";

                visualcallable headline_region = _callables[visual_index_rss_reader_region::headlines];
                visualcallable article_content = _callables[visual_index_rss_reader_region::article_content];

                const int region_h = headline_region.h() + article_content.h();
                
                const int headlines_h = region_h/4;
                const int article_contents_h = region_h - headlines_h;
                
                headline_region.h(headlines_h);
                article_content.h(article_contents_h);
        }

        double previous_line_stroke_width = 0;

        double const scrollbar_width = 42;

        for(int callable_index = 0; callable_index < callable_size; callable_index++) {
                visualcallable callable = _callables[callable_index];

                string* label = new string(callable.label());

                const char* label_text = label->data();

                delete label;

                const double x = callable.x();
                const double y = callable.y();
                const double w = callable.w();
                const double h = callable.h();

                //Allegro uses a x1/x2, y1,y2 coordinate pairs for some draw calls.
                const double x1 = callable.x1();
                const double x2 = callable.x2();
                
                const double y1 = callable.y1();
                const double y2 = callable.y2();

                //cout << "callable " << callable_index << " " << x << "/" << y << "/" << w << "/" << h << "\n";

                double const  border_line_width = callable.line_stroke_width();

                switch(callable_index) {
                        case visual_index_rss_reader_region::header://RSS Reader Header
                        {
                        }
                        break;
                        case visual_index_rss_reader_region::headlines://RSS Reader Headlines
                        {
                                build_visual_vertical_scrollbar(x1, y1, x2, y2, 1, scrollbar_width);
                        }
                        break;
                        case visual_index_rss_reader_region::article_content://RSS Reader article content
                        {
                                build_visual_vertical_scrollbar(x1, y1, x2, y2, 1, scrollbar_width);
                        }
                        break;
                        case visual_index_rss_reader_region::control_bar://RSS Reader Control Bar
                        {
                                const double button_border_line_width = 1;

                                build_visual_right_aligned_button(x1, y1, x2, y2, button_border_line_width, callable.label());
                        }
                        break;
                        case visual_index_rss_reader_region::change_bar://RSS Reader RSS Change Bar
                        {
                                vector<string> widget_texts = {"THE LONGEST FEED NAME EVER WWWWWWWWWWWWWWW", "HTTPS://WWWWWWWWWWWWWWWWWWWWWWWW.COM", "Update"};

                                double next_x = 20;
                                const double x_offset = 20;
                                
                                const double widget_border_line_width = 1;

                                for(int widget_index = 0; widget_index < widget_texts.size(); widget_index++) {
                                        string label_text = widget_texts[widget_index];

                                        build_visual_left_aligned_widget(x1, y1, x2, y2, x_offset, next_x, widget_border_line_width, label_text);
                                }
                        }
                        break;
                        case visual_index_rss_reader_region::choice_bar://RSS Reader Feed Choice Bar
                        {
                                vector<string> button_texts = {"TEST BUTTON 1", "TEST BUTTON 2", "TEST BUTTON 3", "TEST BUTTON 4"};

                                double next_x = 20;
                                const double x_offset = 20;
                                
                                const double button_border_line_width = 1;

                                for(int button_index = 0; button_index < button_texts.size(); button_index++) {
                                        string label_text = button_texts[button_index];

                                        build_visual_left_aligned_widget(x1, y1, x2, y2, x_offset, next_x, button_border_line_width, label_text);
                                }
                        }
                        break;
                }

                previous_line_stroke_width = border_line_width;
        }


        return;
}

void cls::ProcessInteractions(interactionstate& interaction_ctx) {
        bool visual_model_changed = GetIsVisualModelChanged(_InteractionStateLast, interaction_ctx);
        bool window_resized = interaction_ctx.IsWindowResized;
        bool display_updated = interaction_ctx.IsDisplayUpdated;

        if(visual_model_changed && window_resized) {
                _render_is_requested = true;
                cout << "window_resized " << window_resized << "\n";
        }

        return;
}

void cls::draw_region_background(const double x1, const double y1, const double x2, const double y2, ALLEGRO_COLOR& bkg_clr, ALLEGRO_COLOR& bdr_clr, const double bdr_width) {
        const double rect_x1 = x1 + 0.5;
        const double rect_x2 = x2 + 0.5;
        const double rect_y1 = y1 + 0.5;
        const double rect_y2 = y2 + 0.5;

        al_draw_filled_rectangle(rect_x1, rect_y1, rect_x2, rect_y2, bkg_clr);
        al_draw_rectangle(rect_x1, rect_y1, rect_x2, rect_y2, bdr_clr, bdr_width);

        return;
}

void cls::draw_scrollbar_right_background(const double x1, const double y1, const double x2, const double y2, ALLEGRO_COLOR& bkg_clr, ALLEGRO_COLOR& bdr_clr, const double bdr_width, const double scrollbar_width) {
        ALLEGRO_COLOR vertical_scrollbar_background_color = bkg_clr;
        ALLEGRO_COLOR vertical_scrollbar_border_color = bdr_clr;

        /*scrollbar aligned to the right.*/

        const double vertical_scrollbar_border_line_width = bdr_width;

        const double vertical_scrollbar_h = y2;
        const double vertical_scrollbar_w = x2;

        const double vertical_scrollbar_x = (x2 - scrollbar_width);
        const double vertical_scrollbar_y = y1;

        draw_region_background(vertical_scrollbar_x, vertical_scrollbar_y, vertical_scrollbar_w, vertical_scrollbar_h, 
                vertical_scrollbar_background_color, vertical_scrollbar_border_color, vertical_scrollbar_border_line_width);

        return;
}

void cls::draw_left_aligned_widget(const double x1, const double y1, const double x2, const double y2, const double x_offset, double& next_x, ALLEGRO_COLOR& bkg_clr, ALLEGRO_COLOR& bdr_clr, const double bdr_width, string label_text, ALLEGRO_COLOR& label_color) {
        ALLEGRO_COLOR widget_background_color = bkg_clr;
        ALLEGRO_COLOR widget_border_color = bdr_clr;
        ALLEGRO_COLOR widget_text_color = label_color;

        const double widget_border_line_width = bdr_width;

        dlib::drectangle widget_dimensions = MeasureLineHeight(label_text.data());

        /*Widgets aligned to the left.*/

        const double widget_h = (widget_dimensions.bottom() + y1);

        const double widget_y_offset = measure_widget_y(((y2 - y1) / 2.0), (y2 - widget_h));
        const double widget_x_offset = x_offset;

        const double widget_x = (next_x + widget_x_offset);
        const double widget_y = (widget_y_offset + y1);
        const double widget_w = widget_x + widget_dimensions.right();

        next_x = widget_w;

        draw_region_background(widget_x, widget_y, widget_w, widget_h, 
                widget_background_color, widget_border_color, widget_border_line_width);

        return;
}

void cls::draw_right_aligned_button(const double x1, const double y1, const double x2, const double y2, ALLEGRO_COLOR& bkg_clr, ALLEGRO_COLOR& bdr_clr, const double bdr_width, string label_text, ALLEGRO_COLOR& label_color) {

        dlib::drectangle button_dimensions = MeasureLineHeight(label_text.data());

        ALLEGRO_COLOR button_background_color = bkg_clr;
        ALLEGRO_COLOR button_border_color = bdr_clr;
        ALLEGRO_COLOR button_text_color = label_color;

        /*Button aligned to the right.*/

        const double button_border_line_width = bdr_width;

        const double button_h = (button_dimensions.bottom() + y1);

        const double button_y_offset = measure_widget_y(((y2 - y1) / 2.0), (y2 - button_h));
        const double button_x_offset = (x2 - 20);

        const double button_x = (button_x_offset - button_dimensions.right());
        const double button_y = (button_y_offset + y1);
        const double button_w = button_x_offset;

        draw_region_background(button_x, button_y, button_w, button_h, 
                button_background_color, button_border_color, button_border_line_width);

        return;
}

void cls::UpdateVisualOutput(interactionstate& interaction_ctx) {
        const int callable_size = _callables.size();

        double previous_line_stroke_width = 0;

        ALLEGRO_COLOR background_color = al_map_rgb(0, 0, 0);
        ALLEGRO_COLOR border_color = al_map_rgb(0, 0, 0);
        ALLEGRO_COLOR text_color = al_map_rgb(0, 0, 0);

        double const scrollbar_width = 42;

        clear_to_background_color();

        for(int callable_index = 0; callable_index < callable_size; callable_index++) {
                visualcallable callable = _callables[callable_index];

                string* label = new string(callable.label());

                const char* label_text = label->data();

                delete label;

                const double x = callable.x();
                const double y = callable.y();
                const double w = callable.w();
                const double h = callable.h();

                //Allegro uses a x1/x2, y1,y2 coordinate pairs for some draw calls.
                const double x1 = callable.x1();
                const double x2 = callable.x2();
                
                const double y1 = callable.y1();
                const double y2 = callable.y2();

                //cout << "callable " << callable_index << " " << x << "/" << y << "/" << w << "/" << h << "\n";

                double const  border_line_width = callable.line_stroke_width();

                switch(callable_index) {
                        case visual_index_rss_reader_region::header://RSS Reader Header
                        {
                                background_color = al_map_rgb(212, 85, 0);
                                border_color = al_map_rgb(255, 127, 42);
                                text_color = al_map_rgb(213, 255, 246);

                                draw_region_background(x1, y1, x2, y2, background_color, border_color, border_line_width);
                        }
                        break;
                        case visual_index_rss_reader_region::headlines://RSS Reader Headlines
                        {
                                background_color = al_map_rgb(255, 255, 255);
                                border_color = al_map_rgb(160, 44, 44);
                                text_color = al_map_rgb(213, 255, 246);

                                draw_region_background(x1, y1, x2, y2, background_color, border_color, border_line_width);

                                ALLEGRO_COLOR vertical_scrollbar_background_color = al_map_rgb(0, 0, 0);
                                ALLEGRO_COLOR vertical_scrollbar_border_color = al_map_rgb(0, 0, 0);

                                draw_scrollbar_right_background(x1, y1, x2, y2, 
                                        vertical_scrollbar_background_color, vertical_scrollbar_border_color, 1, scrollbar_width);
                        }
                        break;
                        case visual_index_rss_reader_region::article_content://RSS Reader article content
                        {
                                background_color = al_map_rgb(255, 255, 255);
                                border_color = al_map_rgb(160, 44, 44);
                                text_color = al_map_rgb(213, 255, 246);

                                draw_region_background(x1, y1, x2, y2, background_color, border_color, border_line_width);

                                ALLEGRO_COLOR vertical_scrollbar_background_color = al_map_rgb(0, 0, 0);
                                ALLEGRO_COLOR vertical_scrollbar_border_color = al_map_rgb(0, 0, 0);

                                draw_scrollbar_right_background(x1, y1, x2, y2, 
                                        vertical_scrollbar_background_color, vertical_scrollbar_border_color, 1, scrollbar_width);
                        }
                        break;
                        case visual_index_rss_reader_region::control_bar://RSS Reader Control Bar
                        {
                                background_color = al_map_rgb(80, 68, 22);
                                border_color = al_map_rgb(128, 102, 0);
                                text_color = al_map_rgb(213, 255, 246);

                                draw_region_background(x1, y1, x2, y2, background_color, border_color, border_line_width);

                                ALLEGRO_COLOR button_background_color = al_map_rgb(0,0,0);
                                ALLEGRO_COLOR button_border_color = al_map_rgb(0,0,0);
                                ALLEGRO_COLOR button_text_color = al_map_rgb(0,0,0);

                                const double button_border_line_width = 1;

                                string label_text = "Enlarge";

                                draw_right_aligned_button(x1, y1, x2, y2, 
                                        button_background_color, button_border_color, button_border_line_width, label_text, button_text_color);
                        }
                        break;
                        case visual_index_rss_reader_region::change_bar://RSS Reader RSS Change Bar
                        {
                                background_color = al_map_rgb(128, 51, 0);
                                border_color = al_map_rgb(170, 68, 0);
                                text_color = al_map_rgb(213, 255, 246);

                                draw_region_background(x1, y1, x2, y2, background_color, border_color, border_line_width);

                                vector<string> widget_texts = {"THE LONGEST FEED NAME EVER WWWWWWWWWWWWWWW", "HTTPS://WWWWWWWWWWWWWWWWWWWWWWWW.COM", "Update"};

                                double next_x = 20;
                                const double x_offset = 20;
                                
                                ALLEGRO_COLOR widget_background_color = al_map_rgb(0, 0, 0);
                                ALLEGRO_COLOR widget_border_color = al_map_rgb(0, 0, 0);
                                ALLEGRO_COLOR widget_text_color = al_map_rgb(0, 0, 0);

                                const double widget_border_line_width = 1;

                                for(int widget_index = 0; widget_index < widget_texts.size(); widget_index++) {
                                        string label_text = widget_texts[widget_index];

                                        draw_left_aligned_widget(x1, y1, x2, y2, x_offset, next_x, 
                                                widget_background_color, widget_border_color, widget_border_line_width, label_text, widget_text_color);
                                }
                        }
                        break;
                        case visual_index_rss_reader_region::choice_bar://RSS Reader Feed Choice Bar
                        {
                                background_color = al_map_rgb(170, 68, 0);
                                border_color = al_map_rgb(255, 102, 0);
                                text_color = al_map_rgb(213, 255, 246);

                                draw_region_background(x1, y1, x2, y2, background_color, border_color, border_line_width);

                                vector<string> button_texts = {"TEST BUTTON 1", "TEST BUTTON 2", "TEST BUTTON 3", "TEST BUTTON 4"};

                                double next_x = 20;
                                const double x_offset = 20;
                                
                                ALLEGRO_COLOR button_background_color = al_map_rgb(0, 0, 0);
                                ALLEGRO_COLOR button_border_color = al_map_rgb(0, 0, 0);
                                ALLEGRO_COLOR button_text_color = al_map_rgb(0, 0, 0);

                                const double button_border_line_width = 1;

                                for(int button_index = 0; button_index < button_texts.size(); button_index++) {
                                        string label_text = button_texts[button_index];

                                        draw_left_aligned_widget(x1, y1, x2, y2, x_offset, next_x, 
                                                button_background_color, button_border_color, button_border_line_width, label_text, button_text_color);
                                }
                        }
                        break;
                }

                previous_line_stroke_width = border_line_width;
        }

        EndRenderGraphics();
        interaction_ctx.IsDisplayUpdated = true;

        return;
}

vector<rss::request> cls::get_rss_feed_data(int feed_source_index) {
        feedscycle feeds_group;
        
        vector<request> feed_parameters;
	feeds_group.get_feed_names_and_addresses(_feed_names_location, feed_parameters);

	if(feed_parameters.size() > 0) {
	        request feedsource = feed_parameters[feed_source_index];

                //cout << "feedsource " << feedsource.feedname << "\n";

                collector rss_requestor;
	        _feed_articles = rss_requestor.pull(feedsource);
	        
	        _feed_articles_requested = true;
	        
	        //cout << "_feed_articles.size() " << _feed_articles.size() << "\n";
	}

        return feed_parameters;
}


/*private member implementation*/
cls::mainscreengenerator() {
        _self = this;
        Initialize();
        
        return;
}

cls::~mainscreengenerator() {
        Release();
        
        return;
}

void cls::Initialize() {
        _InteractionState.WindowPosition = dlib::dpoint(0, 0);
        _InteractionState.WindowDimensions = dlib::drectangle(0, 0, 0, 0);

	_IsAllegroInitialized = al_init();

	if(_IsAllegroInitialized) {
	        al_init_primitives_addon();
		al_init_font_addon();
		al_init_ttf_addon();

		al_set_new_display_flags(ALLEGRO_RESIZABLE | ALLEGRO_MAXIMIZED | ALLEGRO_GENERATE_EXPOSE_EVENTS);
		al_set_new_display_option(ALLEGRO_FLOAT_COLOR, true, ALLEGRO_SUGGEST);
		
		bool IsMonitorInfoAvailable = al_get_monitor_info(0, &_WinScreenInfo);

		if(IsMonitorInfoAvailable) {
			_InteractionState.MonitorWidth = _WinScreenInfo.x2-_WinScreenInfo.x1;
			_InteractionState.MonitorHeight = _WinScreenInfo.y2-_WinScreenInfo.y1;

			_WinCtx = al_create_display(_InteractionState.MonitorWidth, _InteractionState.MonitorHeight);

                        if(_WinCtx) {
			        _InteractionState.WindowWidth = al_get_display_width(_WinCtx);		
			        _InteractionState.WindowHeight = al_get_display_height(_WinCtx);

                                _InteractionState.WindowDimensions = dlib::drectangle(0,0,_InteractionState.WindowWidth, _InteractionState.WindowHeight);

			        al_set_window_title(_WinCtx, _DefaultWindowTitle.data());
			        al_set_window_position(_WinCtx, _InteractionState.WindowPosition.x(), _InteractionState.WindowPosition.y());

			        _WinMsgEvtSrc = al_get_display_event_source(_WinCtx);
			        _WinMsgEvtQueue = al_create_event_queue();

			        al_register_event_source(_WinMsgEvtQueue, _WinMsgEvtSrc);

		                if(!al_is_mouse_installed()) {
			                al_install_mouse();

			                _MouseEvtSrc = al_get_mouse_event_source();

			                al_register_event_source(_WinMsgEvtQueue, _MouseEvtSrc);
		                }
                        }
                        else {
                                std::cout << __FILE__ " " << __func__ << " " << "(" << __LINE__ << ") ";
                                std::cout << "window could not be created.\r\n";
                        }
		}
		
		if(_WinCtx) {
			_InteractionState.IsWindowOpen = true;
			_InteractionState.IsVisualModelChanged = true;
			_InteractionStateLast = _InteractionState;
		}
	}

	return;
}
void cls::Activate(InteractionCallBackType interactionCallBack) {
        cout << __func__ << " line: " << __LINE__ << "\n";
        if(_InteractionState.IsWindowOpen && _WinMsgEvtQueue) {
                StartRenderGraphics();
        }

        _InteractionCallBack = interactionCallBack;
	while(_InteractionState.IsWindowOpen && _WinMsgEvtQueue) {
		ALLEGRO_EVENT_TYPE window_event_type = _winmsg_event.type;

		switch (window_event_type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				_InteractionState.IsWindowOpen = false;
			        break;
			case ALLEGRO_EVENT_DISPLAY_RESIZE:
				al_acknowledge_resize(_WinCtx);
				
				_InteractionState.WindowWidth = _winmsg_event.display.width;
				_InteractionState.WindowHeight = _winmsg_event.display.height;
                                _InteractionState.WindowDimensions = dlib::drectangle(0,0,_InteractionState.WindowWidth, _InteractionState.WindowHeight);

                                /*Allegro will replay the same event over and over again.
                                        You cannot merely set a flag based on the event.
                                        You have to check the values since flushing the queue does not remove subsequent duplicate events.
                                */
                                //cout << "resize before w/h " << _InteractionStateLast.WindowWidth << "/" << _InteractionStateLast.WindowHeight << "\n";
                                //cout << "resize after  w/h " << _InteractionState.WindowWidth << "/" << _InteractionState.WindowHeight << "\n";

				_InteractionState.IsWindowResized = (   _InteractionState.WindowWidth != _InteractionStateLast.WindowWidth || 
				                                        _InteractionState.WindowHeight != _InteractionStateLast.WindowHeight);
			        break;
	                case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
	                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
	                        _InteractionState.IsMouseUp = (window_event_type == ALLEGRO_EVENT_MOUSE_BUTTON_UP);
	                        _InteractionState.IsMouseDown = (window_event_type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN);
	                        break;
	                case ALLEGRO_EVENT_MOUSE_AXES:
	                        _InteractionState.MouseDirection = _winmsg_event.mouse.dz;
	                        break;
                }

                _InteractionState.MousePosition = dlib::dpoint(_winmsg_event.mouse.x, _winmsg_event.mouse.y);
                _InteractionState.MouseButton = _winmsg_event.mouse.button;

                if(_InteractionState.IsWindowOpen) {
                        if(_WinCtx) {
                                al_flush_event_queue(_WinMsgEvtQueue);
                                interactionCallBack(_InteractionState);
	                }

                        al_wait_for_event_timed(_WinMsgEvtQueue, &_winmsg_event, 0.2);
                }
                else {
		        Release();
                }
	}
	
	return;
}
void cls::Release() {
        if(!_IsAllegroUnInitialized) {
                if(_WinMsgEvtQueue) {
                        if(_MouseEvtSrc) {
	                        al_unregister_event_source(_WinMsgEvtQueue, _MouseEvtSrc);
	                }

                        if(_WinMsgEvtSrc) {
	                        al_unregister_event_source(_WinMsgEvtQueue, _WinMsgEvtSrc);
	                }

	                al_destroy_event_queue(_WinMsgEvtQueue);
	        }

                if(_WinCtx) {
	                al_destroy_display(_WinCtx);
	        }

	        _IsAllegroUnInitialized = true;
	        _IsAllegroInitialized = false;
	}
	
	return;
}
void cls::GetScreenDpi(double& screenDpi) {
	double scrdpi = 0;
	/*
	 * 	Calculate true screen resolution, PPI  (see wikipedia  Pixel Density
	 * 
	 * 		diagonal resolution in pixels   =   square root of ( (screen_w)^2  +  (screen_h)^2  )
	 * 
	 * 		diag_res / diagonal screen size in inches (the average of (11, 12, 13.3, 14, 15.6, and 17) )
	 */		
	if(_IsScreenDPICached) {
		scrdpi = _ScreenDpiLast;
	}
	else {
		double diagres = std::hypot(_InteractionState.MonitorWidth, _InteractionState.MonitorHeight);
		double scrtmp = diagres/_AvgPhysicalScreenSize; 

		scrdpi = _ScreenDpiLast = scrtmp;

		_IsScreenDPICached = true;
	}
	
	screenDpi = scrdpi;
	
	return;
}
bool cls::GetIsVisualModelChanged(interactionstate const& old, interactionstate const& now) {
        bool IsChanged = (now.IsVisualModelChanged || now.IsWindowResized || now.IsMouseDown || now.IsMouseUp);

        if(!IsChanged) {
                IsChanged = (old != now);
        }
        
        return IsChanged;
}

void clear_to_background_color() {
        cout << __func__ << " line: " << __LINE__ << "\n";
        al_clear_to_color(al_map_rgb(255, 153, 85));

        return;
}

void cls::StartRenderGraphics() {
        cout << __func__ << " line: " << __LINE__ << "\n";
        clear_to_background_color();
        al_flip_display();

        return;
}
void cls::EndRenderGraphics() {
        cout << __func__ << " line: " << __LINE__ << "\n";
        al_flip_display();

        std::chrono::time_point<std::chrono::system_clock> current_clock_time = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(current_clock_time);
        string end_time_text(std::ctime(&end_time));

        cout << "display flipped " << end_time_text << "\n";

        return;
}
bool cls::GetIsFontLoaded() {
        return _Font != nullptr;
}
void cls::SetFontParameters(const char* fontPath, double const& fontSize) {
        _FontPath = fontPath;
        _FontSize = fontSize;

        LoadFont();

        return;
}
void cls::GetFont(ALLEGRO_FONT*& font, int& fontBoxX, int& fontBoxY, int& fontBoxW, int& fontBoxH) {
        font = _Font;
        fontBoxX = _FontBoxX;
        fontBoxY = _FontBoxY;
        fontBoxW = _FontBoxW;
        fontBoxH = _FontBoxH;

        return;
}
void cls::LoadFont() {
        if(!_Font) {
                cout << "font load\n";
                /*
                        Font scaling based on Paragraph #4 in the Article DPI and Device-Independent Pixels at: 
                        https://msdn.microsoft.com/en-us/library/windows/desktop/ff684173(v=vs.85).aspx
                */
                double ScreenDpi;

                GetScreenDpi(ScreenDpi);

                const double ScaledFontSizeD = (_FontSize/_PrintPointSize) * ScreenDpi;

                _Font = al_load_font(_FontPath, ScaledFontSizeD, 0);

                if(_Font) {
                        char
                                TextToMeasure[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','z','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','Z','Y','Z','1','2','3','4','5','6','8','9','0','~','!','@','#','$','%','^','&','*','(',')','_','+','-','=','{','}','[',']','|','\\',':',';','<','>','?',',','.','/','\0'}; 

                        const char* 
                                SampleText = TextToMeasure;

                        MeasureLineHeight(SampleText);

                        int FontBoxX = 0, FontBoxY = 0, FontBoxW = 0, FontBoxH = 0;

                        std::vector<double>
                                FontBoxWs;

                        double FontBoxWm = 0;

                        for(auto txt : TextToMeasure) {
                                char Letter[] = {txt, '\0'};

                                al_get_text_dimensions(_Font, Letter, &FontBoxX, &FontBoxY, &FontBoxW, &FontBoxH);

                                FontBoxWm += FontBoxW;

                                FontBoxWs.push_back(FontBoxW);
                        }

                        _FontBoxW = FontBoxWm/FontBoxWs.size();
                }
                else {
                        std::cout << __FILE__ " " << __func__ << " " << "(" << __LINE__ << ") ";
                        std::cout << "could not load font\r\n";
                }
        }
        else {
                cout << "font already loaded\n";
        }

        return;
}

dlib::drectangle cls::MeasureLineHeight(const char* str) {
        dlib::drectangle rect;

        if(_Font) {
                int FontBoxX = 0, FontBoxY = 0, FontBoxW = 0, FontBoxH = 0;
                
                al_get_text_dimensions(_Font, str, &FontBoxX, &FontBoxY, &FontBoxW, &FontBoxH);

                rect = dlib::drectangle(FontBoxX, FontBoxY, FontBoxW, FontBoxH);
        }

        return rect;
}

visualcallable cls::build_visual_vertical_scrollbar(const double x1, const double y1, const double x2, const double y2, const double bdr_width, const double scrollbar_width) {
        /*scrollbar aligned to the right.*/

        const double vertical_scrollbar_border_line_width = bdr_width;

        const double vertical_scrollbar_h = y2;
        const double vertical_scrollbar_w = x2;

        const double vertical_scrollbar_x = (x2 - scrollbar_width);
        const double vertical_scrollbar_y = y1;

        visualcallable callable(0);
        callable.type_id(visual_index_rss_reader_widget_type::vertical_scrollbar);
        callable.x(vertical_scrollbar_x);
        callable.y(vertical_scrollbar_y);
        callable.w(vertical_scrollbar_w);
        callable.h(vertical_scrollbar_h);

        return callable;
}

visualcallable cls::build_visual_left_aligned_widget(const double x1, const double y1, const double x2, const double y2, const double x_offset, double& next_x, const double bdr_width, string label_text) {
        const double widget_border_line_width = bdr_width;

        dlib::drectangle widget_dimensions = MeasureLineHeight(label_text.data());

        /*Widgets aligned to the left.*/

        const double widget_h = (widget_dimensions.bottom() + y1);

        const double widget_y_offset = measure_widget_y(((y2 - y1) / 2.0), (y2 - widget_h));
        const double widget_x_offset = x_offset;

        const double widget_x = (next_x + widget_x_offset);
        const double widget_y = (widget_y_offset + y1);
        const double widget_w = widget_x + widget_dimensions.right();

        next_x = widget_w;

        visualcallable callable(0);
        callable.type_id(visual_index_rss_reader_widget_type::left_aligned_button);
        callable.x(widget_x);
        callable.y(widget_y);
        callable.w(widget_w);
        callable.h(widget_h);

        return callable;
}

visualcallable cls::build_visual_right_aligned_button(const double x1, const double y1, const double x2, const double y2, const double bdr_width, string label_text) {
        dlib::drectangle button_dimensions = MeasureLineHeight(label_text.data());

        /*Button aligned to the right.*/

        const double button_border_line_width = bdr_width;

        const double button_h = (button_dimensions.bottom() + y1);

        const double button_y_offset = measure_widget_y(((y2 - y1) / 2.0), (y2 - button_h));
        const double button_x_offset = (x2 - 20);

        const double button_x = (button_x_offset - button_dimensions.right());
        const double button_y = (button_y_offset + y1);
        const double button_w = button_x_offset;

        visualcallable callable(0);
        callable.type_id(visual_index_rss_reader_widget_type::right_aligned_button);
        callable.x(button_x);
        callable.y(button_y);
        callable.w(button_w);
        callable.h(button_h);

        return callable;
}

vector<visualcallable> cls::get_visual_definitions(int screen_x, int screen_y, int screen_w, int screen_h) {
        vector<visualcallable> callables;
        
        double next_y = 0;
        double text_w = 0;
        double text_h = _FontBoxH;
        double accumulated_h = 0;
        double remaining_h = 0;

        double x = 0;
        double y = 0;
        double w = 0;
        double h = 0;

        double stroke_width = 1;

        double rh = 0;

        const int max_elems = 6;

        for(int index = 0; index < max_elems; index++) {
                visualcallable callable(index);
                callable.type_id(visual_index_rss_reader_widget_type::squared_region);

                x = 0;
                y = next_y;
                w = screen_w;

                switch(index) {
                        case visual_index_rss_reader_region::header://RSS Reader Header
                        {
                                stroke_width = 1;
                                
                                string label_text = "RSS Reader";
                                
                                callable.label(label_text);

                                dlib::drectangle text_dimensions = MeasureLineHeight(label_text.data());

                                text_h = text_dimensions.bottom();

                                h = text_h;
                        }
                        break;
                        case visual_index_rss_reader_region::headlines://RSS Reader Headlines
                        {
                                stroke_width = 4;
                                h = remaining_h / 2;
                        }
                        break;
                        case visual_index_rss_reader_region::article_content://RSS Reader article content
                        {
                                stroke_width = 4;
                                double dv = 1.4;
                                h = remaining_h / dv;
                        }
                        break;
                        case visual_index_rss_reader_region::control_bar:
                        {
                                stroke_width = 2;
                                double dv = 3;
                                rh = remaining_h / dv;

                                h = rh;

                                callable.label("test 1");
                        }
                        break;
                        case visual_index_rss_reader_region::change_bar:
                        {
                                stroke_width = 1.5;
                                h = rh;

                                callable.label("test 2");
                        }
                        break;
                        case visual_index_rss_reader_region::choice_bar:
                        {
                                stroke_width = 1;
                                h = rh;
                                
                                callable.label("test 3");
                        }
                        break;
                }

                callable.x(x);
                callable.y(y);
                callable.w(w);
                callable.h(h);
                callable.line_stroke_width(stroke_width);

                accumulated_h = (accumulated_h + callable.h());
                next_y = (next_y + callable.h());
                //remaining_h = ((workarea_h - window_chrome_offset) - accumulated_h);
                remaining_h = (screen_h - accumulated_h);

                /*cout << "index: " << index << " ";
                cout << " h: " << h;
                cout << " accumulated_h: " << accumulated_h;
                cout << " remaining_h: " << remaining_h;
                cout << "\n";*/

                callables.push_back(callable);

                /*cout << "index: " << index << " x/y/w/h";
                cout << " : " << callable.x();
                cout << " / " << callable.y();
                cout << " / " << callable.w();
                cout << " / " << callable.h();
                cout << "\n";*/
        }
        
        return callables;
}
