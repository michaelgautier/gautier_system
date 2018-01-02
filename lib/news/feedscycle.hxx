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

#ifndef __feedscycle__
#define __feedscycle__

#include <string>
#include <vector>

namespace rss {
        using namespace std;
	class feedscycle {
		public:
                void get_feed_names_and_addresses(string& location, vector<request>& feed_parameters);
                void get_feed_parameter_lines_from_config(string& location, vector<string>& feed_parameter_lines);
                vector<string> set_feed_name_and_address(string& location, string& feed_name, string& feed_url);
	};
}
#endif
