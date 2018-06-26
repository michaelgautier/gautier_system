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
#include "rss_file_manager_feed_article.hxx"

using namespace std;
using cls = news::rss_file_manager_feed_article;

void cls::init(const string& file_location) {
    _file_location = file_location;

    return;
}

news::rss_data_feed_article_spec cls::get_spec(const news::rss_data_feed_headline_spec& feed_headline) {
    news::rss_data_feed_article_spec spec;
    spec.feed_headline = feed_headline;

    //Read the file, get the feed article, update the set.
    bool feed_match = false;
    bool headline_match = false;

    auto call_rss_line = [=,&spec,&feed_match,&headline_match](string& data) {
        if(data.size() < 1) {
            return;
        }

        //At least 1 tab expected most lines
        auto tab_pos = data.find_first_of(_tab_char);

        string first_char = string(&data[0]);

        if(first_char == _comment_char) {
            return;
        } else if (first_char == _feedname_start_char) {
            string name = data.substr(1, tab_pos);
            string last_checked_date_time = data.substr(tab_pos+1);

            feed_match = (feed_headline.feed_name.name == name);
        } else if (feed_match && first_char == _headline_start_char) {
            string headline = data.substr(1, tab_pos);
            string url = data.substr(tab_pos+1);

            if (headline == feed_headline.headline) {
                headline_match = true;
            }
        } else if (headline_match) {
            //
        }
    };

    rss_techconstruct::file rssfile;
    rssfile.read_file_into_string(_file_location, call_rss_line);

    return spec;
}

news::rss_consequence_set cls::save_spec(const news::rss_data_feed_article_spec& feed_article) {
    news::rss_consequence_set cs;

    //Read the feed articles and create/replace the file.
    auto call_rss_line = [=,&feed_article](ofstream& data) {
        data << feed_article.content << _newline_char;
    };

    rss_techconstruct::file rssfile;
    rssfile.persist_stream(_file_location, call_rss_line);

    return cs;
}

