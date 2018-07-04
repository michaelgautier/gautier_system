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
#include <iostream>
#include "rss_file_manager_feed_name.hxx"
#include "rss_file_manager_feed_headline.hxx"

using namespace std;

int main() {
    string feed_names_location = "feeds.txt";

    news::rss_file_manager_feed_name file_m_feeds;

    file_m_feeds.init(feed_names_location);

    news::rss_set_feed_name rss_feeds_set = file_m_feeds.get_set();

    vector<news::rss_data_feed_name_spec> rss_feeds = rss_feeds_set.get_specs();

    news::rss_file_manager_feed_headline file_m_headlines;

    for(news::rss_data_feed_name_spec feed_name : rss_feeds) {
        cout << "******** feed: \t " << feed_name.name << "\n\n\n";

        file_m_headlines.init(feed_name.name + ".txt");

        news::rss_set_feed_headline rss_headlines_set = file_m_headlines.get_set(feed_name);

        vector<news::rss_data_feed_headline_spec> feed_headlines = rss_headlines_set.get_specs();

        if(feed_headlines.empty()) {
            rss_headlines_set = file_m_headlines.pull_set(feed_name);

            feed_headlines = rss_headlines_set.get_specs();
        }

        for(news::rss_data_feed_headline_spec feed_headline : feed_headlines) {
            string headline = feed_headline.headline;
            string url = feed_headline.url;
            string description = feed_headline.description;
            string article_date = feed_headline.article_date;

            cout << "\t headline: " << headline << "\n";
            cout << "\t date: " << article_date << "\n";
            cout << "\t url: " << url << "\n";
            cout << "\t description: " << description << "\n\n";
        }

        cout << "\n\n\n";
    }

    return 0;
}
