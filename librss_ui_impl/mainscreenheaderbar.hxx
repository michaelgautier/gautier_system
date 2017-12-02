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

#ifndef __rss_ui_mainscreenheaderbar__
#define __rss_ui_mainscreenheaderbar__

#include <string>

#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/names.h>
#include <FL/abi-version.h>
#include <FL/Fl_Export.H>
#include <FL/fl_types.h>
#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Widget.H>

namespace rss {
namespace ui {
        using namespace std;
        class mainscreenheaderbar : public Fl_Widget {
                public:
                        mainscreenheaderbar(int x, int y, int w, int h, const char *label = 0) : Fl_Widget(x, y, w, h, label) {
                                box(FL_FLAT_BOX);

                                return;
                        }
                protected:
                        void draw();
                        int handle(int event);
        //                void hide();
        //                void resize(int x, int y, int w, int h);
        //                void show();
        };
}
}
#endif
