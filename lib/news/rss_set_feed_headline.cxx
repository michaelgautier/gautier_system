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
#include "rss_set_feed_headline.hxx"

using namespace std;
using cls = news::rss_set_feed_headline;

bool cls::add(const news::rss_data_feed_headline_spec spec) {
    const string headline_n = spec.headline;

    bool spec_found = false;
    bool spec_added = false;

    for (news::rss_data_feed_headline_spec spec_current : _set) {
        const string headline_o = spec_current.headline;

        spec_found = (headline_n == headline_o);

        if (spec_found) {
            break;
        }
    }

    if (!spec_found) {
        _set.push_back(spec);

        spec_added = true;
    }

    return spec_added;
}

vector<news::rss_data_feed_headline_spec> cls::get_specs() {
    return _set;
}
