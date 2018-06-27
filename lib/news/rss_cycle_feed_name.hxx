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

#ifndef __news_rss_cycle_feed_name__
#define __news_rss_cycle_feed_name__
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "rss_set_feed_name.hxx"
#include "rss_data_feed_name_spec.hxx"
#include "rss_file_manager_feed_name.hxx"
#include "rss_set_consequence.hxx"

/*
        Core mechanism for RSS.
                You need RSS feeds first.
                The feed is the location on the Internet where the feed articles reside.
                In this process, this "feed name cycle" + "feed name file manager" is the starting point for the entire process.

                Use get_feed_names to start the process.

                Use get_single_feed_name as a means to refresh feed information after a process has started.
*/
namespace news {
using namespace std;
class rss_cycle_feed_name {
  private:
    rss_set_feed_name* _set = nullptr;
    string _file_location;
  public:
    void init(string file_location);
    rss_data_feed_name_spec get_single_feed_name(const unsigned short int feed_index);
    rss_set_feed_name get_feed_names();
    rss_set_consequence set_single_feed_name(const rss_data_feed_name_spec& feed_name);
    rss_set_consequence remove_single_feed_name(const rss_data_feed_name_spec& feed_name);
};
}
#endif

