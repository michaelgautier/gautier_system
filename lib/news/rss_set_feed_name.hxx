/*
Copyright 2019 Michael Gautier

This file is part of Gautier RSS System by Michael Gautier.

Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.

C++ Standard Library; Copyright 2018 Standard C++ Foundation.
*/
#ifndef __news_rss_set_feed_name__
#define __news_rss_set_feed_name__
#include <vector>
#include "rss_data_feed_name_spec.hxx"

namespace news {
class rss_set_feed_name {
  private:
    std::vector<rss_data_feed_name_spec> _set;
  public:
    void add(const rss_data_feed_name_spec spec);
    std::vector<rss_data_feed_name_spec> get_specs();
};
}
#endif

