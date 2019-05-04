/*
Copyright 2019 Michael Gautier

This file is part of Gautier RSS System by Michael Gautier.

Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.

C++ Standard Library; Copyright 2018 Standard C++ Foundation.
*/
#ifndef __techconstruct_file__
#define __techconstruct_file__

#include <functional>
#include <string>
#include <fstream>

namespace rss_techconstruct {
class file {
  public:
    void get_stream(std::string location, std::string& output);
    void erase_stream(std::string location);
    void persist_stream(std::string location, std::string data_to_persist);
    void persist_stream(std::string location, std::function<void(std::ofstream&)> file_callable);
    void read_istream_into_string(std::istream& input, std::string& output);
    void read_file_into_string(std::string location, std::string& output);
    void read_file_into_string(std::string location, std::function<void(std::string&)> file_callable);
    void read_file_into_string(std::string location, std::function<void(std::string&,bool&)> file_callable);
};
}
#endif
