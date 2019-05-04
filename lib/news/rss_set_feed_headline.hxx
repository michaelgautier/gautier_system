/*
Copyright 2019 Michael Gautier

This file is part of Gautier RSS System by Michael Gautier.

Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.

C++ Standard Library; Copyright 2018 Standard C++ Foundation.
*/
#ifndef __news_rss_set_data_feed_headline__
#define __news_rss_set_data_feed_headline__
#include <string>
#include <vector>
#include "rss_data_feed_headline_spec.hxx"

namespace news {
class rss_set_feed_headline {
  private:
    std::vector<rss_data_feed_headline_spec> _set;
  public:
    std::string last_checked_date_time_string;
    bool add(const rss_data_feed_headline_spec spec);
    std::vector<rss_data_feed_headline_spec> get_specs();
};
}
#endif

