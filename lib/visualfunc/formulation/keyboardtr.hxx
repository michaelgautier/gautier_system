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
#ifndef __visualfunc_formulation_keyboardtr__
#define __visualfunc_formulation_keyboardtr__
#include <iostream>
#include <allegro5/allegro.h>

#include "interactionstate.hxx"

namespace visualfunc {
namespace formulation {
struct keyboardtr {
  public:
    using interactionstate = ::visualfunc::formulation::InteractionState;

    ALLEGRO_EVENT_QUEUE*
    keyboard_evt_queue = nullptr;

    ALLEGRO_EVENT_SOURCE*
    keyboard_evt_src = nullptr;

    vector<textbuffer> texts;
    bool check_keyboard(interactionstate* interaction_ctx, const float wait_time);
    int process_keyboard(interactionstate* interaction_ctx, interactionstate* interaction_ctx_previous);
    int text_buffer_index = -1;
  private:
    ALLEGRO_EVENT
    _keyboard_event;
    const char get_al_char_from_keycode(int keycode, bool is_keyboard_caps_on);
};
}
}
#endif
