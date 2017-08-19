namespace gautier::system::rss {
	class gautier_rss_requestor {
		private:
		public:
			void request_feeds(
				std::vector<gautier_rss_request>& 				feed_parameters, 
				std::map<std::string, std::vector<gautier_rss_article>>& 	feed_articles
			);
	};
}
