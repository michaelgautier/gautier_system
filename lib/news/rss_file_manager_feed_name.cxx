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
#include "rss_file_manager_feed_name.hxx"

using namespace std;
using cls = news::rss_file_manager_feed_name;

void cls::init(const string& file_location) {
    _file_location = file_location;

    return;
}

news::rss_set_feed_name cls::get_set() {
    news::rss_set_feed_name fn_set;

    //Read the file, get the feed name/feed url combinations, update the set.
    auto call_rss_line = [=,&fn_set](string& data) {
        if(data.size() > 1 && string(&data[0]) == _comment_char) {
            return;
        }

        const auto tab_pos = data.find_first_of(_tab_char, 0);

        if(tab_pos == string::npos) {
            return;
        }

        const string name = data.substr(0, tab_pos);
        const string url = data.substr(tab_pos+1);

        news::rss_data_feed_name_spec spec;

        spec.name = name;
        spec.url = url;

        fn_set.add(spec);
    };

    rss_techconstruct::file rssfile;
    rssfile.read_file_into_string(_file_location, call_rss_line);

    return fn_set;
}

news::rss_set_consequence cls::save_set(news::rss_set_feed_name& rss_set) {
    news::rss_set_consequence cs;

    //Read the feed name/feed url combinations and create/replace the file.
    vector<news::rss_data_feed_name_spec> v = rss_set.get_specs();

    auto call_rss_line = [=,&v](ofstream& data) {
        for(news::rss_data_feed_name_spec& spec : v) {
            data << spec.name << _tab_char << spec.url << _newline_char;
        }
    };

    rss_techconstruct::file rssfile;
    rssfile.persist_stream(_file_location, call_rss_line);

    return cs;
}

