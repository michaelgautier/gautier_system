#include <fstream>
#include <iostream>

#include <Poco/String.h>
#include <Poco/URI.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

#include "http.hxx"
#include "file.hxx"

using namespace std;
using namespace Poco;
using namespace Poco::Net;

using cls = rss_techconstruct::http;
using cls_fs = rss_techconstruct::file;

bool cls::check_url_is_http(string url) {
	bool result = false;

	int found_string_comparison_http = icompare(url, 0, 4, "http");
	
	result = (found_string_comparison_http == 0);

	if(!result) {
		int found_string_comparison_https = icompare(url, 0, 5, "https");

		result = (found_string_comparison_https == 0);
	}

	return result;
}

void cls::get_stream(string url, string& output) {
	string request_method = "GET";

	URI request_uri(url);
	
	HTTPClientSession http_session(request_uri.getHost(), request_uri.getPort());

	HTTPRequest http_request(request_method, url);

	try {
		http_session.sendRequest(http_request);

		HTTPResponse http_response;

		istream& temp_http_response_stream = http_session.receiveResponse(http_response);

                cls_fs filehandler;
                
		filehandler.read_istream_into_string(temp_http_response_stream, output);
	}
	catch(const exception e) {
		if(e.what() == "Host not found") {
			cout << e.what() << " " << "reverting to offline copy if available.\n";
		}
	}

	return;
}

