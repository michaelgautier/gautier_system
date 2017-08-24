#include <gautier_rss_article.hxx>
#include <gautier_rss_request.hxx>
#include <gautier_rss_requestor.hxx>

int main() {
        
	gautier::system::rss::gautier_rss_requestor rss_requestor;
	
	gautier::system::rss::gautier_rss_request test_request;
	
	test_request.feed_name = "arstechnica";
	test_request.feed_url = "/home/standardaccount/Downloads/feeds/feed_arstechnica.xml";

	std::vector<gautier::system::rss::gautier_rss_request> 					feed_parameters;
	std::map<std::string, std::vector<gautier::system::rss::gautier_rss_article> >		feed_articles;

	feed_parameters.push_back(test_request);
	
	rss_requestor.request_feeds(feed_parameters, feed_articles);
	
	return 0;
}
