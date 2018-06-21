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

#ifndef __file__
#define __file__

#include <functional>
#include <string>
#include <fstream>

namespace rss_techconstruct {
using namespace std;

class file {
  public:
    void get_stream(string location, string& output);
    void persist_stream(string location, string data_to_persist);
    void persist_stream(string location, function<void(ofstream&)> file_callable);
    void read_istream_into_string(istream& input, string& output);
    void read_file_into_string(string location, string& output);
    void read_file_into_string(string location, function<void(string&)> file_callable);
};
}
#endif
