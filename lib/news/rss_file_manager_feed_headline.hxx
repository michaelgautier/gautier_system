/*
Copyright 2019 Michael Gautier

This file is part of Gautier RSS System by Michael Gautier.

Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.

C++ Standard Library; Copyright 2018 Standard C++ Foundation.
*/
#ifndef __news_rss_file_manager_feed_headline__
#define __news_rss_file_manager_feed_headline__
#include <string>
#include <ctime>
#include <iostream>

#include "libxml/parser.h"
#include "libxml/tree.h"

#include "rss_data_feed_name_spec.hxx"
#include "rss_set_feed_headline.hxx"
#include "rss_set_consequence.hxx"

#include "http.hxx"
#include "file.hxx"
#include "textfilter.hxx"

namespace news {
class rss_file_manager_feed_headline {
  private:
    std::string _file_location;

    const std::string _tab_char = "\t";
    const std::string _comment_char = "#";
    const std::string _newline_char = "\n";
    const std::string _feedname_start_char = "/";
    const std::string _headline_start_char = "_";
  public:
    void init(const std::string& file_location);
    bool get_can_feed_refresh(const rss_data_feed_name_spec& feed_name);
    rss_set_feed_headline get_set(const rss_data_feed_name_spec& feed_name);
    rss_set_feed_headline pull_set(const rss_data_feed_name_spec& feed_name);
    rss_set_consequence save_set(const rss_data_feed_name_spec& feed_name, rss_set_feed_headline& rss_set);
};
}
#endif

