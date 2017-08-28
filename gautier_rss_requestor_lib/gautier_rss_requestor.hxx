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

#ifndef __gautier_system_rss_gautier_rss_requestor__
#define __gautier_system_rss_gautier_rss_requestor__

#include <gautier_rss_request.hxx>
#include <gautier_rss_article.hxx>
#include <vector>
#include <map>
namespace gautier::system::rss {
	class gautier_rss_requestor {
		private:
		        void collect_feed(std::string feed_name, std::map<std::string, std::vector<gautier_rss_article*> >& feed_articles, std::string& feed_document_stream);
		public:
			void request_feeds(
				std::vector<gautier::system::rss::gautier_rss_request>& 				feed_parameters, 
				std::map<std::string, std::vector<gautier::system::rss::gautier_rss_article*> >& 		feed_articles
			);
	};
}
#endif
