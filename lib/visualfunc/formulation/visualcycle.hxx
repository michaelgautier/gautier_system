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

#ifndef __visualfunc_formulation_visualcycle__
#define __visualfunc_formulation_visualcycle__

#include <vector>
#include "visualcallable.hxx"
#include "visualsheet.hxx"
#include "visualsheetconfigparam.hxx"
#include "visualcallableproto.hxx"

namespace visualfunc {
namespace formulation {
        using namespace std;
        vector<visualsheetconfigparam> get_visual_sheet_config();
        vector<visualcallableproto> get_visual_callable_prototypes();
        visualsheet make_visual_sheet(vector<visualsheetconfigparam> parameters);
        vector<visualcallable> make_visual_callables(vector<visualcallableproto> prototypes);
}
}
#endif
