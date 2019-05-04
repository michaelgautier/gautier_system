/*
Copyright 2019 Michael Gautier

This file is part of Gautier RSS System by Michael Gautier.

Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.

C++ Standard Library; Copyright 2018 Standard C++ Foundation.
*/
#ifndef __techconstruct_textfilter__
#define __techconstruct_textfilter__

#include <algorithm>
#include <string>

namespace rss_techconstruct {
<<<<<<< HEAD
std::string make_printable(const std::string& s);
std::string text_toupper(std::string s);
std::string text_tolower(std::string s);
=======
/*Based on example listed on cppreference.com on the page for std::toupper*/
std::string text_toupper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
    [](unsigned char c) {
        return std::toupper(c);
    }
                  );

    return s;
}

/*Based on example listed on cppreference.com on the page for std::tolower*/
std::string text_tolower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
    [](unsigned char c) {
        return std::tolower(c);
    }
                  );

    return s;
}
>>>>>>> b7588274ce3f10ef4cb8f2d7331c970e1677002c
}
#endif
