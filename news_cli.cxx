/*
Copyright 2017 Michael Gautier

This file is part of Gautier RSS System by Michael Gautier.

Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.

Portions of the POCO C++ Libraries utilize the following copyrighted material, the use of which is hereby acknowledged.
POCO C++ Libraries released under the Boost Software License; Copyright 2017, Applied Informatics Software Engineering GmbH and Contributors; 
C++ Standard Library; Copyright 2017 Standard C++ Foundation.
*/
#include <iostream>

#include "collector.hxx"
#include "request.hxx"
#include "material.hxx"
#include "feedscycle.hxx"

using collector = rss::collector;
using material = rss::material;
using request = rss::request;
using feedscycle = rss::feedscycle;

using namespace std;

int main() {
	vector<request> feed_parameters;

	string feed_names_location = "feeds.txt";
	
	collector rss_requestor;

        feedscycle feeds_group;
        
	feeds_group.get_feed_names_and_addresses(feed_names_location, feed_parameters);
	
	for(auto feedsource : feed_parameters) {
	        cout << "******** feed: \t " << feedsource.feedname << "\n\n\n";

	        vector<material> feed_articles = rss_requestor.pull(feedsource);
	
	        for(auto feed_article_entry : feed_articles) {
		        string headline = feed_article_entry.headline;
		        string url = feed_article_entry.url;
		        string description = feed_article_entry.description;
		        string article_date = feed_article_entry.article_date;

		        cout << "\t headline: " << headline << "\n";
		        cout << "\t date: " << article_date << "\n";
		        cout << "\t url: " << url << "\n";
		        cout << "\t description: " << description << "\n";
	        }

	        cout << "\n\n\n";
	}
		
	return 0;
}
