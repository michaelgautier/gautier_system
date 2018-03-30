#include "interactionengine.hxx"

using cls = ::visualfunc::formulation::interactionengine;

cls::interactionengine() {
    interactionstate_previous = new interactionstate();

    return;
}

cls::~interactionengine() {
    shutdown_allegro_graphics_engine();

    delete interactionstate_previous;

    return;
}

void cls::measure_screen(interactionstate* interaction_ctx) {
    const double x = interaction_ctx->WindowDimensions.left();
    const double y = interaction_ctx->WindowDimensions.top();
    const double w = interaction_ctx->WindowWidth;
    const double h = interaction_ctx->WindowHeight;

    workarea_x = x;
    workarea_y = y;
    workarea_w = w;
    workarea_h = h;

    /*cout << " workarea w   workarea h  " << workarea_w << " " << workarea_h << " \n";*/

    return;
}

void cls::persist_interaction_state(interactionstate* interaction_ctx) {
    if(interactionstate_previous) {
        delete interactionstate_previous;
    }

    interactionstate_previous = new interactionstate(*interaction_ctx);

    return;
}

void cls::update_textfield(textbuffer* tb, visualcallable& vc, const int widget_type_id) {
    dlib::drectangle font_dimensions;

    if(tb->buffer_visual_width < 1) {
        tb->buffer_visual_width = vc.x2() - vc.x1();

        font_dimensions = measure_text_by_sized_font("W", (_default_widget_font_size / 2.0), _font_file_location);

        const int font_w = font_dimensions.right();

        tb->letter_width = font_w;
    }

    string letters = tb->text;

    const int letter_count = letters.size();
    const int buffer_visual_width = tb->buffer_visual_width;
    const int letter_width = tb->letter_width;

    if(letter_count != tb->letter_count_previous) {
        font_dimensions = measure_text_by_sized_font(letters.data(), (_default_widget_font_size / 2.0), _font_file_location);

        const int buffer_actual_width = font_dimensions.right();

        if(buffer_actual_width != tb->buffer_actual_width) {
            if(buffer_actual_width > tb->buffer_visual_width) {
                tb->buffer_next_index = tb->buffer_next_index + 1;
            } else if(buffer_actual_width < tb->buffer_visual_width && tb->buffer_next_index > 0) {
                tb->buffer_next_index = tb->buffer_next_index - 1;
            }
        }

        /*cout << "check buffer width ------------------------ \n";
        cout << "letter count " << letter_count << "\n";
        cout << "actual width " << buffer_actual_width << "\n";
        cout << "max width " << tb->buffer_visual_width << "\n";
        cout << "next index " << tb->buffer_next_index << "\n";*/

        tb->buffer_actual_width = font_dimensions.right();

        letters = letters.substr(tb->buffer_next_index, letter_count);

        tb->letter_count_previous = letter_count;
    }

    vc.type_id(widget_type_id);
    vc.label(letters);

    return;
}

void cls::draw_region_background(const double x1,
                                 const double y1,
                                 const double x2,
                                 const double y2,
                                 ALLEGRO_COLOR& bkg_clr,
                                 ALLEGRO_COLOR& bdr_clr,
                                 const double bdr_width) {
    const double rect_x1 = x1 + 0.5;
    const double rect_x2 = x2 + 0.5;
    const double rect_y1 = y1 + 0.5;
    const double rect_y2 = y2 + 0.5;

    al_draw_filled_rectangle(rect_x1, rect_y1, rect_x2, rect_y2, bkg_clr);
    al_draw_rectangle(rect_x1, rect_y1, rect_x2, rect_y2, bdr_clr, bdr_width);

    /*if(y1 < 4) {
            cout << __func__ << " line " << __LINE__ << ", x/y/w/h " << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
    }*/

    return;
}

cls::visualcallable cls::build_visual_vertical_scrollbar(const double x1,
        const double y1,
        const double x2,
        const double y2,
        const double bdr_width,
        const double scrollbar_width) {
    /*scrollbar aligned to the right.*/

    const double vertical_scrollbar_border_line_width = bdr_width;

    const double vertical_scrollbar_h = y2;
    const double vertical_scrollbar_w = x2;

    const double vertical_scrollbar_x = (x2 - scrollbar_width);
    const double vertical_scrollbar_y = y1;

    visualcallable callable(0);
    callable.type_id(widget_type::vertical_scrollbar);
    callable.x(vertical_scrollbar_x);
    callable.y(vertical_scrollbar_y);
    callable.w(vertical_scrollbar_w);
    callable.h(vertical_scrollbar_h);

    return callable;
}

void cls::draw_scrollbar_right_background(const double x1,
        const double y1,
        const double x2,
        const double y2,
        ALLEGRO_COLOR& bkg_clr,
        ALLEGRO_COLOR& bdr_clr,
        const double bdr_width,
        const double scrollbar_width) {
    ALLEGRO_COLOR vertical_scrollbar_background_color = bkg_clr;
    ALLEGRO_COLOR vertical_scrollbar_border_color = bdr_clr;

    /*scrollbar aligned to the right.*/

    draw_region_background(x1, y1, x2, y2,
                           vertical_scrollbar_background_color, vertical_scrollbar_border_color, bdr_width);

    /*if(y1 < 4) {
            cout << __func__ << " line " << __LINE__ << ", x/y/w/h " << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
    }*/

    return;
}

cls::visualcallable cls::build_visual_left_aligned_widget(const double x1,
        const double y1,
        const double x2,
        const double y2,
        bool trim_to_label,
        const double x_offset,
        double& next_x,
        const double bdr_width,
        string label_text) {
    /*Widgets aligned to the left.*/

    const double widget_border_line_width = bdr_width;

    dlib::drectangle widget_dimensions = measure_text_by_sized_font(label_text.data(), _default_widget_font_size, _font_file_location);

    const double widget_h = (widget_dimensions.bottom() + y1);

    const double widget_y_offset = measure_widget_y(((y2 - y1) / 2.0), (y2 - widget_h));
    const double widget_x_offset = x_offset;

    const double widget_x = (next_x + widget_x_offset);
    const double widget_y = (widget_y_offset + y1);
    double widget_w = widget_x + widget_dimensions.right();

    if(trim_to_label && !label_text.empty()) {
        dlib::drectangle font_dimensions = measure_text_by_sized_font(label_text.data(), _default_widget_font_size/2, _font_file_location);

        const double font_y = measure_font_y_offset(y1, y2, font_dimensions.bottom());
        const double font_w = font_dimensions.right();

        //ALLEGRO_FONT* Font = load_sized_font(_default_widget_font_size/2, _font_file_location);

        widget_w = widget_x + font_w + (_default_label_margin_left * 2.0);
    }

    next_x = widget_w;

    visualcallable callable(0);
    callable.type_id(widget_type::left_aligned_button);
    callable.label(label_text);
    callable.x(widget_x);
    callable.y(widget_y);
    callable.w(widget_w);
    callable.h(widget_h);

    return callable;
}

void cls::draw_left_aligned_widget(const double x1,
                                   const double y1,
                                   const double x2,
                                   const double y2,
                                   ALLEGRO_COLOR& bkg_clr,
                                   ALLEGRO_COLOR& bdr_clr,
                                   const double bdr_width,
                                   string label_text,
                                   ALLEGRO_COLOR& label_color) {
    ALLEGRO_COLOR widget_background_color = bkg_clr;
    ALLEGRO_COLOR widget_border_color = bdr_clr;
    ALLEGRO_COLOR widget_text_color = label_color;

    draw_region_background(x1, y1, x2, y2,
                           widget_background_color, widget_border_color, bdr_width);

    if(!label_text.empty()) {
        dlib::drectangle font_dimensions = measure_text_by_sized_font(label_text.data(), _default_widget_font_size/2, _font_file_location);

        const double font_y = measure_font_y_offset(y1, y2, font_dimensions.bottom());
        const double font_x = (x1 + _default_label_margin_left);

        //cout << __func__ << " " << __LINE__ << " \n";
        ALLEGRO_FONT* Font = load_sized_font(_default_widget_font_size/2, _font_file_location);

        if(Font) {
            al_draw_text(Font, label_color, font_x, font_y, ALLEGRO_ALIGN_LEFT, label_text.data());
        } else {
            cout << __func__ << " " << __LINE__ << " font not loaded\n";
        }
    }

    /*if(y1 < 4) {
            cout << __func__ << " line " << __LINE__ << ", x/y/w/h " << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
    }*/

    return;
}

cls::visualcallable cls::build_visual_vertical_widget(const double x1,
        const double y1,
        const double x2,
        const double y2,
        const double y_offset,
        double& next_y,
        const double bdr_width,
        string label_text) {
    /*Widgets aligned to the left.*/

    const double widget_border_line_width = bdr_width;

    dlib::drectangle widget_dimensions = measure_text_by_sized_font(label_text.data(), _default_widget_font_size, _font_file_location);

    const double widget_x = x1;
    const double widget_y = (y_offset + y1 + next_y);
    const double widget_h = (widget_dimensions.bottom() + (widget_y));

    //const double widget_y_offset = measure_widget_y(((y2 - y1) / 2.0), (y2 - widget_h));

    const double widget_w = widget_x + widget_dimensions.right();

    next_y = widget_y;

    visualcallable callable(0);
    callable.type_id(widget_type::text_field);
    callable.label(label_text);
    callable.x(widget_x);
    callable.y(widget_y);
    callable.w(widget_w);
    callable.h(widget_h);

    //cout << "built callable x/y/w/h " << callable.x() << "/" << callable.y() << "/" << callable.w() << "/" << callable.h() << "\n";

    return callable;
}

void cls::draw_visual_vertical_widget(const double x1,
                                      const double y1,
                                      const double x2,
                                      const double y2,
                                      ALLEGRO_COLOR& bkg_clr,
                                      ALLEGRO_COLOR& bdr_clr,
                                      const double bdr_width,
                                      string label_text,
                                      ALLEGRO_COLOR& label_color) {
    ALLEGRO_COLOR widget_background_color = bkg_clr;
    ALLEGRO_COLOR widget_border_color = bdr_clr;
    ALLEGRO_COLOR widget_text_color = label_color;

    draw_region_background(x1, y1, x2, y2,
                           widget_background_color, widget_border_color, bdr_width);

    if(!label_text.empty()) {
        const double font_x = (x1 + _default_label_margin_left);

        ALLEGRO_FONT* Font = load_sized_font(default_font_size, _font_file_location);

        if(Font) {
            al_draw_text(Font, label_color, font_x, y1, ALLEGRO_ALIGN_LEFT, label_text.data());
        } else {
            cout << __func__ << " " << __LINE__ << " font not loaded\n";
        }
    }

    /*if(y1 < 4) {
            cout << __func__ << " line " << __LINE__ << ", x/y/w/h " << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
    }*/

    return;
}

cls::visualcallable cls::build_visual_right_aligned_button(const double x1,
        const double y1,
        const double x2,
        const double y2,
        bool trim_to_label,
        const double bdr_width,
        string label_text) {
    /*Button aligned to the right.*/

    const double button_border_line_width = bdr_width;

    dlib::drectangle button_dimensions = measure_text_by_sized_font(label_text.data(), _default_widget_font_size, _font_file_location);

    const double button_h = (button_dimensions.bottom() + y1);

    const double button_y_offset = measure_widget_y(((y2 - y1) / 2.0), (y2 - button_h));
    const double button_y = (button_y_offset + y1);

    const double button_x_offset = (x2 - 20);

    double button_x = (button_x_offset - button_dimensions.right());
    double button_w = button_x_offset;

    if(trim_to_label && !label_text.empty()) {
        dlib::drectangle font_dimensions = measure_text_by_sized_font(label_text.data(), _default_widget_font_size/2, _font_file_location);

        const double font_w = font_dimensions.right();

        //ALLEGRO_FONT* Font = load_sized_font(_default_widget_font_size/2, _font_file_location);

        button_x = ((x2 - 60) - font_w);
        button_w = (font_w + button_x + (_default_label_margin_left * 2.0));
    }

    visualcallable callable(0);
    callable.type_id(widget_type::right_aligned_button);
    callable.label(label_text);
    callable.x(button_x);
    callable.y(button_y);
    callable.w(button_w);
    callable.h(button_h);

    return callable;
}

void cls::draw_right_aligned_button(const double x1,
                                    const double y1,
                                    const double x2,
                                    const double y2,
                                    ALLEGRO_COLOR& bkg_clr,
                                    ALLEGRO_COLOR& bdr_clr,
                                    const double bdr_width,
                                    string label_text,
                                    ALLEGRO_COLOR& label_color) {
    dlib::drectangle button_dimensions = measure_text_by_sized_font(label_text.data(), _default_widget_font_size, _font_file_location);

    ALLEGRO_COLOR button_background_color = bkg_clr;
    ALLEGRO_COLOR button_border_color = bdr_clr;
    ALLEGRO_COLOR button_text_color = label_color;

    /*Button aligned to the right.*/

    draw_region_background(x1, y1, x2, y2,
                           button_background_color, button_border_color, bdr_width);

    if(!label_text.empty()) {
        dlib::drectangle font_dimensions = measure_text_by_sized_font(label_text.data(), _default_widget_font_size/2, _font_file_location);

        const double font_y = measure_font_y_offset(y1, y2, font_dimensions.bottom());
        const double font_x = (x1 + _default_label_margin_left);

        //cout << __func__ << " " << __LINE__ << " \n";
        ALLEGRO_FONT* Font = load_sized_font(_default_widget_font_size/2, _font_file_location);

        if(Font) {
            al_draw_text(Font, label_color, font_x, font_y, ALLEGRO_ALIGN_LEFT, label_text.data());
        } else {
            cout << __func__ << " " << __LINE__ << " font not loaded\n";
        }
    }

    /*if(y1 < 4) {
            cout << __func__ << " line " << __LINE__ << ", x/y/w/h " << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
    }*/

    return;
}

void cls::initialize_allegro_graphics_engine(interactionstate* interaction_ctx, keyboardtr* kytr) {
    interaction_ctx->WindowPosition = dlib::dpoint(0, 0);
    interaction_ctx->WindowDimensions = dlib::drectangle(0, 0, 0, 0);

    _is_allegro_initialized = al_init();

    if(_is_allegro_initialized) {
        al_init_primitives_addon();
        al_init_font_addon();
        al_init_ttf_addon();
        al_init_native_dialog_addon();

        al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE | ALLEGRO_GENERATE_EXPOSE_EVENTS);
        al_set_new_display_option(ALLEGRO_FLOAT_COLOR, 1, ALLEGRO_SUGGEST);
        al_set_new_display_option(ALLEGRO_RENDER_METHOD, 1, ALLEGRO_SUGGEST);
        al_set_new_display_option(ALLEGRO_SINGLE_BUFFER, 0, ALLEGRO_SUGGEST);
        al_set_new_display_option(ALLEGRO_SWAP_METHOD, 2, ALLEGRO_SUGGEST);
        al_set_new_display_option(ALLEGRO_UPDATE_DISPLAY_REGION, 0, ALLEGRO_SUGGEST);
        al_set_new_display_option(ALLEGRO_CAN_DRAW_INTO_BITMAP, 0, ALLEGRO_SUGGEST);

        bool IsMonitorInfoAvailable = al_get_monitor_info(0, &_win_screen_info);

        if(IsMonitorInfoAvailable) {
            screen_w = _win_screen_info.x2 - _win_screen_info.x1;
            screen_h = _win_screen_info.y2 - _win_screen_info.y1;
            interaction_ctx->MonitorWidth = screen_w;
            interaction_ctx->MonitorHeight = screen_h;

            _win_ctx = al_create_display(interaction_ctx->MonitorWidth, interaction_ctx->MonitorHeight);

            if(_win_ctx) {
                interaction_ctx->WindowWidth = al_get_display_width(_win_ctx);
                interaction_ctx->WindowHeight = al_get_display_height(_win_ctx);

                interaction_ctx->WindowDimensions = dlib::drectangle(0, 0, interaction_ctx->WindowWidth, interaction_ctx->WindowHeight);

                al_set_window_title(_win_ctx, _default_window_title.data());
                al_set_window_position(_win_ctx, interaction_ctx->WindowPosition.x(), interaction_ctx->WindowPosition.y());

                _win_msg_evt_src = al_get_display_event_source(_win_ctx);
                _win_msg_evt_queue = al_create_event_queue();
                kytr->keyboard_evt_queue = al_create_event_queue();
                _mouse_evt_queue = al_create_event_queue();

                al_register_event_source(_win_msg_evt_queue, _win_msg_evt_src);

                if(!al_is_mouse_installed()) {
                    al_install_mouse();

                    _mouse_evt_src = al_get_mouse_event_source();

                    al_register_event_source(_mouse_evt_queue, _mouse_evt_src);
                }

                if(!al_is_keyboard_installed()) {
                    al_install_keyboard();

                    kytr->keyboard_evt_src = al_get_keyboard_event_source();

                    al_register_event_source(kytr->keyboard_evt_queue, kytr->keyboard_evt_src);
                }
            } else {
                std::cout << __FILE__ " " << __func__ << " " << "(" << __LINE__ << ") ";
                std::cout << "window could not be created.\r\n";
            }
        }

        if(_win_ctx) {
            interaction_ctx->IsWindowOpen = true;
            interaction_ctx->IsVisualModelChanged = true;
        }
    }

    return;
}

void cls::activate_allegro_graphics_engine(interactionstate* interaction_ctx, interaction_callback_type interactionCallBack) {
    while(interaction_ctx && interaction_ctx->IsWindowOpen && _win_msg_evt_queue) {
        const float wait_time = 0.01;
        const bool has_window_event = al_wait_for_event_timed(_win_msg_evt_queue, &_winmsg_event, wait_time);

        if(has_window_event) {
            ALLEGRO_EVENT_TYPE window_event_type = _winmsg_event.type;

            switch (window_event_type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                interaction_ctx->IsWindowOpen = false;
                break;
            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                al_acknowledge_resize(_win_ctx);
                al_flush_event_queue(_win_msg_evt_queue);

                interaction_ctx->WindowWidth = _winmsg_event.display.width;
                interaction_ctx->WindowHeight = _winmsg_event.display.height;
                interaction_ctx->WindowDimensions = dlib::drectangle(0, 0, interaction_ctx->WindowWidth, interaction_ctx->WindowHeight);

                interaction_ctx->IsWindowResized = (interaction_ctx->WindowWidth != interactionstate_previous->WindowWidth ||
                                                    interaction_ctx->WindowHeight != interactionstate_previous->WindowHeight);
                //cout << "resize " << interaction_ctx->IsWindowResized << "\n";
                break;
            }
        }

        const bool has_mouse_event = al_wait_for_event_timed(_mouse_evt_queue, &_mouse_event, wait_time);

        if(has_mouse_event) {
            ALLEGRO_EVENT_TYPE mouse_event_type = _mouse_event.type;

            switch (mouse_event_type) {
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                interaction_ctx->IsMouseUp = (mouse_event_type == ALLEGRO_EVENT_MOUSE_BUTTON_UP);
                interaction_ctx->IsMouseDown = (mouse_event_type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN);
                interaction_ctx->MousePosition = dlib::dpoint(_mouse_event.mouse.x, _mouse_event.mouse.y);
                interaction_ctx->MouseButton = _mouse_event.mouse.button;
                break;
            case ALLEGRO_EVENT_MOUSE_AXES:
                interaction_ctx->MouseDirection = _mouse_event.mouse.dz;
                break;
            }
        }

        if(!interaction_ctx->IsWindowOpen) {
            break;
        } else if(_win_ctx) {
            interactionCallBack(interaction_ctx);

            interaction_ctx->IsWindowResized = false;
            interaction_ctx->IsMouseUp = true;
            interaction_ctx->IsMouseDown = false;
            interaction_ctx->MouseDirection = -1;
            interaction_ctx->IsKeyAvailable = false;
            interaction_ctx->KeyboardKeyCode = -1;
            interaction_ctx->IsKeyDown = false;
            interaction_ctx->IsKeyUp = false;
            interaction_ctx->KeyModifiers = -1;
        }
    }

    shutdown_allegro_graphics_engine();

    return;
}

void cls::shutdown_allegro_graphics_engine() {
    if(!_is_allegro_uninitialized) {
        if(_win_msg_evt_queue) {
            if(_mouse_evt_src) {
                al_unregister_event_source(_win_msg_evt_queue, _mouse_evt_src);
            }

            if(_win_msg_evt_src) {
                al_unregister_event_source(_win_msg_evt_queue, _win_msg_evt_src);
            }

            al_destroy_event_queue(_win_msg_evt_queue);
        }

        if(_win_ctx) {
            al_destroy_display(_win_ctx);
        }

        _is_allegro_uninitialized = true;
        _is_allegro_initialized = false;
    }

    return;
}

double cls::get_screen_dpi() {
    /*
     * 	Calculate true screen resolution, PPI  (see wikipedia  Pixel Density
     *
     * 		diagonal resolution in pixels   =   square root of ( (screen_w)^2  +  (screen_h)^2  )
     *
     * 		diag_res / diagonal screen size in inches (the average of (11, 12, 13.3, 14, 15.6, and 17) )
     */
    double diagres = std::hypot(screen_w, screen_h);
    const double scrdpi = diagres/_avg_physical_screen_size;

    return scrdpi;
}

ALLEGRO_FONT* cls::load_sized_font(const int font_size, const char* font_file_location) {
    /*
            Font scaling based on Paragraph #4 in the Article DPI and Device-Independent Pixels at:
            https://msdn.microsoft.com/en-us/library/windows/desktop/ff684173(v=vs.85).aspx
    */
    ALLEGRO_FONT* Font = nullptr;

    auto font_count = _fonts.count(font_size);

    if(font_count < 1) {
        //cout << "adding font to _fonts\n";
        const double screen_dpi = get_screen_dpi();

        const double scaled_font_size = (font_size / _print_point_size) * screen_dpi;

        Font = al_load_font(font_file_location, scaled_font_size, 0);

        if(Font) {
            _fonts[font_size] = Font;
        } else {
            //cout << "loaded font invalid\n";
            _fonts.erase(font_size);
            //cout << __func__ << " " << __LINE__ << " \n";
            load_sized_font(font_size, font_file_location);
        }
    } else {
        Font = _fonts[font_size];

        if(!Font) {
            //cout << "cached font invalid\n";
            _fonts.erase(font_size);
            //cout << __func__ << " " << __LINE__ << " \n";
            load_sized_font(font_size, font_file_location);
        }
    }

    return Font;
}

constexpr double cls::measure_widget_y(const double region_h_half, const double button_h) {
    return (region_h_half - (button_h / 2));
}

dlib::drectangle cls::measure_text_by_sized_font(const char* str, int font_size, const char* font_file_location) {
    dlib::drectangle rect;

    ALLEGRO_FONT* Font = nullptr;

    do {
        Font = load_sized_font(font_size, font_file_location);
    } while(!Font);

    if(Font) {
        int FontBoxX = 0, FontBoxY = 0, FontBoxW = 0, FontBoxH = 0;

        al_get_text_dimensions(Font, str, &FontBoxX, &FontBoxY, &FontBoxW, &FontBoxH);

        rect = dlib::drectangle(FontBoxX, FontBoxY, FontBoxW, FontBoxH);
    } else {
        cout << __func__ << " " << __LINE__ << " Font not loaded \n";
    }

    return rect;
}

constexpr double cls::measure_font_y_offset(const double y1, const double y2, const double h) {
    const double font_y_diff = y2 - y1;
    const double font_y1 = h;
    const double font_y2 = abs((y2 - y1) - h);

    double font_y = y1;

    if(font_y1 > font_y_diff && font_y_diff == font_y2) {
        font_y = (y1 - (font_y2 / 2.0));
    }

    return font_y;
}
