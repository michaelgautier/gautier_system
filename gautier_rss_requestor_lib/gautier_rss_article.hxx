/*
	Licensed under the Gnu Public License version 3
*/
#ifndef __gautier_system_rss_gautier_rss_article__
#define __gautier_system_rss_gautier_rss_article__
#include <string>
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
#endif
