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

#ifndef __rss_ui_mainscreenblueprint__
#define __rss_ui_mainscreenblueprint__

#include <vector>
#include "visualcallable.hxx"

namespace rss {
namespace ui {
        using namespace std;
	class mainscreenblueprint {
	        public:
	        mainscreenblueprint(int screen_x, int screen_y, int screen_w, int screen_h) {
	                _screen_x = screen_x;
	                _screen_y = screen_y;
	                _screen_w = screen_w;
	                _screen_h = screen_h;

	                return;
	        }
	        vector<visualfunc::formulation::visualcallable> get_visual_definitions();
	        private:
                int _screen_x = 0;
                int _screen_y = 0;
                int _screen_w = 0;
                int _screen_h = 0;
	};
}
}
#endif
