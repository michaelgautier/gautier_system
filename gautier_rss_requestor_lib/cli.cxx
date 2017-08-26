#include <gautier_rss_article.hxx>
#include <gautier_rss_request.hxx>
#include <gautier_rss_requestor.hxx>

#include <iostream>
#include <fstream>

int main() {
        
	gautier::system::rss::gautier_rss_requestor rss_requestor;
	
	gautier::system::rss::gautier_rss_request test_request;
	
         
        std::ifstream feeds_file;
        feeds_file.open("feeds.txt");
        
        std::string feeds_lines;
        
        while(!feeds_file.eof()) {
                char feeds_data;
                
                feeds_data = feeds_file.get();
                
                feeds_lines.push_back(feeds_data);
        }
        
        //to do: read the feeds data.       
        
        feeds_file.close();
        
	test_request.feed_name = "arstechnica";
	test_request.feed_url = "http://feeds.arstechnica.com/arstechnica/index";

	std::vector<gautier::system::rss::gautier_rss_request> 					feed_parameters;
	std::map<std::string, std::vector<gautier::system::rss::gautier_rss_article*> >		feed_articles;

	feed_parameters.push_back(test_request);
	
	rss_requestor.request_feeds(feed_parameters, feed_articles);
	
	return 0;
}
