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

#include <gautier_rss_article.hxx>
#include <gautier_rss_request.hxx>
#include <gautier_rss_requestor.hxx>

#include <ios>
#include <iostream>
#include <fstream>
#include <vector>

void get_feed_namedaddresses(std::string& location, std::vector<gautier::system::rss::gautier_rss_request>& feed_parameters);
void get_feed_parameter_lines_from_config(std::string& location, std::vector<std::string>& feed_parameter_lines);

int main() {
	std::vector<gautier::system::rss::gautier_rss_request> 					feed_parameters;
	std::map<std::string, std::vector<gautier::system::rss::gautier_rss_article*> >		feed_articles;

        std::string feed_names_location = "feeds.txt";
        
        get_feed_namedaddresses(feed_names_location, feed_parameters);
	gautier::system::rss::gautier_rss_requestor rss_requestor;
	
	rss_requestor.request_feeds(feed_parameters, feed_articles);
	
        for(auto feed_article_entry : feed_articles) {
                std::cout << feed_article_entry.first << "\n";

                std::vector<gautier::system::rss::gautier_rss_article*> feed_articles = feed_article_entry.second;

                for(gautier::system::rss::gautier_rss_article* feed_article_item : feed_articles) {
	                std::string headline = feed_article_item->headline;
	                std::string url = feed_article_item->url;
	                std::string description = feed_article_item->description;
	                std::string article_date = feed_article_item->article_date;
                        
                        std::cout << "headline: " << headline << "\n";
                        std::cout << "date: " << article_date << "\n";
                        std::cout << "url: " << url << "\n";
                        std::cout << "description: " << description << "\n";
                }
        }
        
	return 0;
}

void get_feed_namedaddresses(std::string& location, std::vector<gautier::system::rss::gautier_rss_request>& feed_parameters) {
        std::vector<std::string> feed_parameter_lines;

        get_feed_parameter_lines_from_config(location, feed_parameter_lines);

        for(auto feed_line : feed_parameter_lines) {
                //define feed url
                if(feed_line.size() > 1 && feed_line[0] == '#') {
                        continue;
                }

                auto separator_pos = feed_line.find_first_of("\t");

                if(separator_pos == std::string::npos) {
                        continue;
                }
 
                std::string feed_name = feed_line.substr(0, separator_pos);
                std::string feed_url = feed_line.substr(separator_pos+1);
                
                gautier::system::rss::gautier_rss_request feed_request;

	        feed_request.feed_name = feed_name;
	        feed_request.feed_url = feed_url;

                feed_parameters.push_back(feed_request);
        }
        
        return;
}

void get_feed_parameter_lines_from_config(std::string& location, std::vector<std::string>& feed_parameter_lines) {
        std::ifstream feeds_file(location.data());
                
        while(!feeds_file.eof()) {
                std::string feeds_data;

                std::getline(feeds_file, feeds_data);

                feed_parameter_lines.push_back(feeds_data);
        }
        
        feeds_file.close();

        return;
}
