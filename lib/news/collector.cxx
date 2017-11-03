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
#include "techconstruct/file.hxx"
#include "techconstruct/http.hxx"
#include "techconstruct/parser.hxx"

#include "collector.hxx"

using cls = rss::collector;
using file = rss_techconstruct::file;
using http = rss_techconstruct::http;
using parser = rss_techconstruct::parser;

using namespace std;
using namespace rss;

vector<material> cls::pull(request request) {
	string feedname = request.feedname;
	string webaddress = request.webaddress;
	string filesystem_location = (feedname + ".xml");

        string stream_result;

        http http_handler;
        file file_handler;
        parser parse_handler;

	if(http_handler.check_url_is_http(webaddress)) {
		http_handler.get_stream(webaddress, stream_result);

                if(!stream_result.empty()) {
                        file_handler.persist_stream(filesystem_location, stream_result);
                }
                else {
                        file_handler.get_stream(filesystem_location, stream_result);                
                }
	} 
	else {
	        file_handler.get_stream(filesystem_location, stream_result);
	}

        vector<material> value = parse_handler.read(stream_result);

	return value;
}
