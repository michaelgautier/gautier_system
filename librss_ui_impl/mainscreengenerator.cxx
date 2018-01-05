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

void UpdateDisplay(interactionstate interactionState);
void clear_to_background_color();

void cls::init() {

        return;
}

void cls::generate() {
        Activate(UpdateDisplay);
        
        return;
}

void UpdateDisplay(interactionstate interactionState) {
        _self->ProcessUpdates(interactionState);

	return;
}

void cls::ProcessUpdates(const interactionstate& interactionState) {
        bool IsVisualModelChanged = GetIsVisualModelChanged(_InteractionState, interactionState);

        if(IsVisualModelChanged) {
                if(!_Font) {
                        if(!GetIsFontLoaded()) {
                                cout << "loading font\n";
                                SetFontParameters("NotoSans-Regular.ttf", 10);
                                cout << "font loaded with size w/h " << _FontBoxW << "/" << _FontBoxH << "\n";
                        }
                }

                BuildVisualModel(interactionState);
                ProcessInteractions(interactionState);
                UpdateVisualOutput(interactionState);
	}

        _InteractionState = interactionState;

        return;
}

void cls::measure_screen(const interactionstate& interaction_ctx) {
        _workarea_x = interaction_ctx.WindowDimensions.left();
        _workarea_y = interaction_ctx.WindowDimensions.top();
        _workarea_w = interaction_ctx.WindowDimensions.right();
        _workarea_h = interaction_ctx.WindowDimensions.bottom();

        return;
}

void cls::BuildVisualModel(const interactionstate& interaction_ctx) {
        measure_screen(interaction_ctx);

        callables = get_visual_definitions(_workarea_x, _screen_y, _workarea_w, _workarea_h);

        return;
}

void cls::ProcessInteractions(const interactionstate& interaction_ctx) {
        return;
}

void cls::UpdateVisualOutput(const interactionstate& interaction_ctx) {
        const int callable_size = callables.size();

        clear_to_background_color();

        for(int callable_index = 0; callable_index < callable_size; callable_index++) {
                visualcallable callable = callables[callable_index];

                string* label = new string(callable.label());

                const char* label_text = label->data();

                delete label;

                const int x = callable.x();
                const int y = callable.y();
                const int w = callable.w();
                const int h = callable.h();

                //Allegro uses a x1/x2, y1,y2 coordinate pairs for some draw calls.
                const int x1 = x;
                const int x2 = w;
                
                const int y1 = y;
                const int y2 = y+h;

                //cout << "callable " << callable_index << " " << x << "/" << y << "/" << w << "/" << h << "\n";

                switch(callable_index) {
                        case visual_index_rss_reader_region::header://RSS Reader Header
                        {
                                ALLEGRO_COLOR background_color = al_map_rgb(212, 85, 0);
                                ALLEGRO_COLOR border_color = al_map_rgb(160, 44, 44);
                                double const  border_line_width = 1;

                                ALLEGRO_COLOR text_color = al_map_rgb(213, 255, 246);

	                        al_draw_filled_rectangle(x1, y1, x2, y2, background_color);
	                        al_draw_rectangle(x1, y1, x2, y2, border_color, border_line_width);
                        }
                        break;
                        case visual_index_rss_reader_region::headlines://RSS Reader Headlines
                        {
                                ALLEGRO_COLOR background_color = al_map_rgb(255, 255, 255);
                                ALLEGRO_COLOR border_color = al_map_rgb(160, 44, 44);
                                double const  border_line_width = 1;

                                ALLEGRO_COLOR text_color = al_map_rgb(213, 255, 246);

	                        al_draw_filled_rectangle(x1, y1, x2, y2, background_color);
	                        al_draw_rectangle(x1, y1, x2, y2, border_color, border_line_width);
                        }
                        break;
                        case visual_index_rss_reader_region::article_content://RSS Reader article content
                        {
                                ALLEGRO_COLOR background_color = al_map_rgb(255, 255, 255);
                                ALLEGRO_COLOR border_color = al_map_rgb(160, 44, 44);
                                double const  border_line_width = 1;

                                ALLEGRO_COLOR text_color = al_map_rgb(213, 255, 246);

	                        al_draw_filled_rectangle(x1, y1, x2, y2, background_color);
	                        al_draw_rectangle(x1, y1, x2, y2, border_color, border_line_width);
                        }
                        break;
                        case visual_index_rss_reader_region::control_bar://RSS Reader Control Bar
                        {
                                ALLEGRO_COLOR background_color = al_map_rgb(80, 68, 22);
                                ALLEGRO_COLOR border_color = al_map_rgb(160, 44, 44);
                                double const  border_line_width = 1;

                                ALLEGRO_COLOR text_color = al_map_rgb(213, 255, 246);

	                        al_draw_filled_rectangle(x1, y1, x2, y2, background_color);
	                        al_draw_rectangle(x1, y1, x2, y2, border_color, border_line_width);
                        }
                        break;
                        case visual_index_rss_reader_region::change_bar://RSS Reader RSS Change Bar
                        {
                                ALLEGRO_COLOR background_color = al_map_rgb(128, 51, 0);
                                ALLEGRO_COLOR border_color = al_map_rgb(160, 44, 44);
                                double const  border_line_width = 1;

                                ALLEGRO_COLOR text_color = al_map_rgb(213, 255, 246);

	                        al_draw_filled_rectangle(x1, y1, x2, y2, background_color);
	                        al_draw_rectangle(x1, y1, x2, y2, border_color, border_line_width);
                        }
                        break;
                        case visual_index_rss_reader_region::choice_bar://RSS Reader Feed Choice Bar
                        {
                                ALLEGRO_COLOR background_color = al_map_rgb(170, 68, 0);
                                ALLEGRO_COLOR border_color = al_map_rgb(160, 44, 44);
                                double const  border_line_width = 1;

                                ALLEGRO_COLOR text_color = al_map_rgb(213, 255, 246);

	                        al_draw_filled_rectangle(x1, y1, x2, y2, background_color);
	                        al_draw_rectangle(x1, y1, x2, y2, border_color, border_line_width);
                        }
                        break;
                }
        }

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
        if(_InteractionState.IsWindowOpen && _WinMsgEvtQueue) {
                StartRenderGraphics();
        }

        _InteractionCallBack = interactionCallBack;
	while(_InteractionState.IsWindowOpen && _WinMsgEvtQueue) {
                _InteractionStateLast = _InteractionState;

		ALLEGRO_EVENT_TYPE window_event_type = _winmsg_event.type;

		switch (window_event_type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				_InteractionState.IsWindowOpen = false;
			        break;
			case ALLEGRO_EVENT_DISPLAY_RESIZE:
			        clear_to_background_color();
			        EndRenderGraphics();//Will make this smoother later.
				al_acknowledge_resize(_WinCtx);
				
				_InteractionState.WindowWidth = _winmsg_event.display.width;
				_InteractionState.WindowHeight = _winmsg_event.display.height;
                                _InteractionState.WindowDimensions = dlib::drectangle(0,0,_InteractionState.WindowWidth, _InteractionState.WindowHeight);

				_InteractionState.IsWindowResized = true;	
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

                                bool IsVisualModelChanged = GetIsVisualModelChanged(_InteractionStateLast, _InteractionState);

                                if(IsVisualModelChanged) {
                                        interactionCallBack(_InteractionState);

                                        EndRenderGraphics();
                                }
	                }

                        _InteractionStateLast = _InteractionState;

                        al_wait_for_event_timed(_WinMsgEvtQueue, &_winmsg_event, 0.3);
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
        al_clear_to_color(al_map_rgb(255, 153, 85));

        return;
}

void cls::StartRenderGraphics() {
        clear_to_background_color();
        al_flip_display();

        return;
}
void cls::EndRenderGraphics() {
        al_flip_display();

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

void cls::MeasureLineHeight(const char* str) {
        if(_Font) {
                int FontBoxX = 0, FontBoxY = 0, FontBoxW = 0, FontBoxH = 0;
                
                al_get_text_dimensions(_Font, str, &FontBoxX, &FontBoxY, &FontBoxW, &FontBoxH);

                _FontBoxX = FontBoxX; _FontBoxW = FontBoxW;
                _FontBoxY = FontBoxY; _FontBoxH = FontBoxH;
        }

        return;
}

vector<visualcallable> cls::get_visual_definitions(int screen_x, int screen_y, int screen_w, int screen_h) {
        vector<visualcallable> callables;
        
        int next_y = 0;
        int text_w = 0;
        int text_h = _FontBoxH;
        int accumulated_h = 0;
        int remaining_h = 0;

        int x = 0;
        int y = 0;
        int w = 0;
        int h = 0;

        int rh = 0;

        const int max_elems = 6;

        for(int index = 0; index < max_elems; index++) {
                visualcallable callable(index);

                x = 0;
                y = next_y;
                w = screen_w;

                switch(index) {
                        case visual_index_rss_reader_region::header://RSS Reader Header
                        {
                                callable.label("RSS Reader");

                                if(_FontBoxH < 1) {
                                        cout << "measuring text for text_h which is at size: " << text_h << "\n";
                                        MeasureLineHeight("RSS Reader");
                                }

                                text_h = _FontBoxH;

                                h = text_h;
                        }
                        break;
                        case visual_index_rss_reader_region::headlines://RSS Reader Headlines
                        {
                                h = remaining_h / 2;
                        }
                        break;
                        case visual_index_rss_reader_region::article_content://RSS Reader article content
                        {
                                double dv = 1.4;
                                h = remaining_h / dv;
                        }
                        break;
                        case visual_index_rss_reader_region::control_bar:
                        {
                                double dv = 3;
                                rh = remaining_h / dv;

                                h = rh;

                                callable.label("test 1");
                        }
                        break;
                        case visual_index_rss_reader_region::change_bar:
                        {
                                h = rh;

                                callable.label("test 2");
                        }
                        break;
                        case visual_index_rss_reader_region::choice_bar:
                        {
                                h = rh;
                                
                                callable.label("test 3");
                        }
                        break;
                }

                callable.x(x);
                callable.y(y);
                callable.w(w);
                callable.h(h);

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

        if(_article_contents_enlarge) {
                cout << "enlarge content - adjusting callable\n";

                const int region_h = callables[1].h() + callables[2].h();
                
                const int headlines_h = region_h/4;
                const int article_contents_h = region_h - headlines_h;
                
                callables[1].h(headlines_h);
                callables[2].h(article_contents_h);
        }
        
        return callables;
}
