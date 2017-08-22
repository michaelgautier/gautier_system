/*
	Licensed under the Gnu Public License version 3
*/
#ifndef __gautier_system_rss_gautier_rss_requestor__
#define __gautier_system_rss_gautier_rss_requestor__

#include <gautier_rss_request.hxx>
#include <gautier_rss_article.hxx>
#include <vector>
#include <map>
namespace gautier::system::rss {
	class gautier_rss_requestor {
		private:
		        bool request_url_is_http(std::string& request_url);
		        void get_http_response_stream(std::stringstream& http_response_stream, std::string& request_url);
		        void get_file_stream(std::stringstream& file_stream, std::string& location);
		public:
			void request_feeds(
				std::vector<gautier::system::rss::gautier_rss_request>& 				feed_parameters, 
				std::map<std::string, std::vector<gautier::system::rss::gautier_rss_article>>& 		feed_articles
			);
	};
}
#endif
