namespace gautier::system::rss {
	class gautier_rss_article {
		public:
			int id{0};

			std::string headline{};
			std::string url{};
			std::string description{};
			std::string article_date{};
	};
}
