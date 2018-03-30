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
#ifndef __visualfunc_formulation_textbuffer__
#define __visualfunc_formulation_textbuffer__
#include <string>

namespace visualfunc {
namespace formulation {
using namespace std;
class textbuffer {
  public:
    int buffer_x = 0;
    int buffer_visual_width = 0;
    int letter_width = 0;
    int letter_count = 0;
    int letter_count_previous = -1;

    int buffer_actual_width = 0;
    int buffer_next_index = 0;

    int buffer_insert_pos = 0;
    int buffer_select_end_pos = 0;

    int last_char_size = 0;//When this differs from text.size(), then redraw. That way, text doesn't have to be real-time.

    string text;
};
}
}
#endif
