/*
Copyright 2019 Michael Gautier

This file is part of Gautier RSS System by Michael Gautier.

Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.

C++ Standard Library; Copyright 2018 Standard C++ Foundation.
*/
#include <iostream>
#include "rss_cycle_feed_name.hxx"
#include "rss_cycle_feed_headline.hxx"

int main() {
    std::string feed_names_location = "feeds.txt";

    news::rss_cycle_feed_name file_c_feeds;

    file_c_feeds.init(feed_names_location);

    news::rss_set_feed_name rss_feeds_set = file_c_feeds.get_feed_names();

    std::vector<news::rss_data_feed_name_spec> rss_feeds = rss_feeds_set.get_specs();

    news::rss_cycle_feed_headline file_c_headlines;

    for(news::rss_data_feed_name_spec feed_name : rss_feeds) {
        std::cout << "******** feed: \t " << feed_name.name << "\n\n\n";

        file_c_headlines.init(feed_name.name + ".txt");

        news::rss_set_feed_headline rss_headlines_set = file_c_headlines.get_feed_headlines(feed_name);

        std::vector<news::rss_data_feed_headline_spec> feed_headlines = rss_headlines_set.get_specs();

        for(news::rss_data_feed_headline_spec feed_headline : feed_headlines) {
            std::string headline = feed_headline.headline;
            std::string url = feed_headline.url;
            std::string description = feed_headline.description;
            std::string article_date = feed_headline.article_date;

            std::cout << "\t headline: " << headline << "\n";
            std::cout << "\t date: " << article_date << "\n";
            std::cout << "\t url: " << url << "\n";
            std::cout << "\t description: " << description << "\n\n";
        }

        std::cout << "\n\n\n";
    }

    return 0;
}
