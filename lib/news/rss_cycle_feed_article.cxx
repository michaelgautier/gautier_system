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
#include "rss_cycle_feed_article.hxx"

using namespace std;
using cls = news::rss_cycle_feed_article;

void cls::init(string file_location) {
    _file_location = file_location;

    _set = new news::rss_data_feed_article_set;

    return;
}

news::rss_data_feed_article_spec cls::get_single_feed_article(const news::rss_data_feed_name_spec& feed_name, const unsigned short int headline_index) {
    news::rss_data_feed_article_spec a;

    if(_set) {
        //Find the spec in the set and assign to n.
    }

    return a;
}

news::rss_data_feed_article_set cls::get_feed_articles(const news::rss_data_feed_name_spec& feed_name) {
    news::rss_file_manager_feed_article fm;
    fm.init(_file_location);

    news::rss_data_feed_article_set fa;

    fa = fm.get_set(feed_name);

    _set = &fa;

    return fa;
}

news::rss_consequence_set cls::set_single_feed_article(const news::rss_data_feed_article_spec& feed_article) {
    news::rss_file_manager_feed_article fm;
    fm.init(_file_location);

    //Add the spec to the set.

    news::rss_consequence_set cs = fm.save_set(feed_article.feed_headline.feed_name, *_set);

    return cs;
}

news::rss_consequence_set cls::set_multiple_feed_articles(const news::rss_data_feed_name_spec& feed_name, const news::rss_data_feed_article_set& rss_set) {
    news::rss_file_manager_feed_article fm;
    fm.init(_file_location);

    //Add specs to the set.

    news::rss_consequence_set cs = fm.save_set(feed_name, *_set);

    return cs;
}

news::rss_consequence_set cls::remove_multiple_feed_articles(const news::rss_data_feed_name_spec& feed_name, const unsigned short int headline_index_first, const unsigned short int headline_index_last) {
    news::rss_file_manager_feed_article fm;
    fm.init(_file_location);

    //Remove the specs from the set.

    news::rss_consequence_set cs = fm.save_set(feed_name, *_set);

    return cs;
}

