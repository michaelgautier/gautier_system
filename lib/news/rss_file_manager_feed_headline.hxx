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

#ifndef __news_rss_file_manager_feed_headline__
#define __news_rss_file_manager_feed_headline__
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "rss_data_feed_name_set.hxx"
#include "rss_check_feed_name.hxx"

namespace news {
using namespace std;
class rss_file_manager_feed_headline {
  private:
  public:
    void init(const string& file_location);
    rss_data_feed_name_set get_set();
    rss_check_feed_name save_set(rss_data_feed_name_set& rss_set);
};
}
#endif

