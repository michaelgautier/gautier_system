/*
Copyright 2019 Michael Gautier

This file is part of Gautier RSS System by Michael Gautier.

Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.

C++ Standard Library; Copyright 2018 Standard C++ Foundation.
*/
#include "textfilter.hxx"

std::string rss_techconstruct::make_printable(const std::string& s) {
    std::string printable_text;

    const int s_text_length = s.size();

    for(int s_text_index = 0; s_text_index < s_text_length; s_text_index++) {
        auto letter = s[s_text_index];

        if(std::isprint(letter)) {
            printable_text.push_back(letter);
        }
    }

    return printable_text;
}

/*Based on example listed on cppreference.com on the page for std::toupper*/
std::string rss_techconstruct::text_toupper(std::string s) {
    std::string printable_text = make_printable(s);

    std::transform(printable_text.begin(), printable_text.end(), printable_text.begin(),
    [](unsigned char c) {
        return std::toupper(c);
    }
                  );

    return s;
}

/*Based on example listed on cppreference.com on the page for std::tolower*/
std::string rss_techconstruct::text_tolower(std::string s) {
    std::string printable_text = make_printable(s);

    std::transform(printable_text.begin(), printable_text.end(), printable_text.begin(),
    [](unsigned char c) {
        return std::tolower(c);
    }
                  );

    return s;
}
