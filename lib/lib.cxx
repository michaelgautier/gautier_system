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

#include <codecvt>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <streambuf>
#include <utility>

#include <Poco/DOM/Document.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/Node.h>
#include <Poco/DOM/NodeList.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/UnicodeConverter.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/SAX/SAXException.h>
#include <Poco/SAX/InputSource.h>

#include "gautier_rss_article.hxx"
#include "gautier_rss_requestor.hxx"

using namespace std;
using namespace Poco::XML;
using namespace Poco::Net;
using namespace Poco;

using cls = gautier::system::rss::gautier_rss_requestor;
using cls_article = gautier::system::rss::gautier_rss_article;

void get_file_stream(string& output, string& location);
void get_http_response_stream(string& output, string& request_url);
void read_file_into_string(string& location, string& output);
void read_istream_into_string(istream& input, string& output);
bool request_url_is_http(string& request_url);
void collect_feed_impl(string feed_name, map<string, vector<cls_article*> >& feed_articles, NodeList* rss_xml_nodes, bool& on_item_node, cls_article* article_item);

void cls::request_feeds(  vector<gautier_rss_request>& feed_parameters
                        , map<string, vector<gautier_rss_article*>>& feed_articles) {
	for(auto rss_location : feed_parameters) {
		string feed_document_text;

		string feed_name = rss_location.feed_name;
		string feed_url = rss_location.feed_url;
		string feed_offline_location(feed_name + ".xml");
		
		feed_articles[feed_name] = vector<gautier_rss_article*>();

		if(request_url_is_http(feed_url)) {
			string feed_response_data;

			get_http_response_stream(feed_response_data, feed_url);

			if(!feed_response_data.empty()) {
				string feed_file_name(feed_offline_location.data(), ios_base::out | ios_base::trunc);
				
				ofstream feed_offline_file(feed_file_name.data());
				
				feed_offline_file << feed_response_data;
				
				feed_offline_file.flush();
				
				feed_offline_file.close();
			}
		}

		get_file_stream(feed_document_text, feed_offline_location);

		collect_feed(feed_name, feed_articles, feed_document_text);
	}

	return;
}

void cls::collect_feed(string feed_name, map<string, vector<gautier_rss_article*> >& feed_articles, string& feed_document_stream) {
	try {
		DOMParser rss_xml_reader;
		rss_xml_reader.setEncoding("utf-8");
	
		Document* rss_xml_document = rss_xml_reader.parseString(feed_document_stream);
	
		Element* rss_xml_root = rss_xml_document->documentElement();
	
		NodeList* rss_xml_nodes = rss_xml_root->childNodes();

		bool on_item_node = false;
		cls_article* article_item = new cls_article;

		collect_feed_impl(feed_name, feed_articles, rss_xml_nodes, on_item_node, article_item);
	} catch(SAXParseException e) {
		cout << "Exception handler on line " << __LINE__ << " func " << __func__ << "\n";
		cout << "\t" << "Exception generated on line " << e.getLineNumber() << "\n";
		cout << "\t" << e.message() << "\n";
		cout << "\t" << e.displayText() << "\n";
		cout << "\t" << e.name() << "\n";
		cout << "\t" << e.what() << "\n";
	}

	return;
}

void collect_feed_impl(string feed_name, map<string, vector<cls_article*> >& feed_articles, NodeList* rss_xml_nodes, bool& on_item_node, cls_article* article_item) {
	int node_count = rss_xml_nodes->length();
	
	bool is_item_node = false;
	
	for(int node_index = 0; node_index < node_count; node_index++) {
		auto xml_node = rss_xml_nodes->item(node_index);
		
		auto xml_node_type = xml_node->nodeType();

		if(xml_node_type == Node::ELEMENT_NODE) {
			string xml_node_name = toLower(xml_node->localName());

			if(xml_node_name == "item") {
				on_item_node = true;
			}
			else if(xml_node_name == "title") {
				article_item->headline = xml_node->innerText();
			}
			else if(xml_node_name == "link") {
				article_item->url = xml_node->innerText();
			}
			else if(xml_node_name == "description") {
				article_item->description = xml_node->innerText();
			}
			else if(xml_node_name == "pub_date" || xml_node_name == "pubdate") {
				article_item->article_date = xml_node->innerText();
			}

			bool required_values_available = !(article_item->headline.empty() && article_item->url.empty() && article_item->description.empty() && article_item->article_date.empty());

			if(required_values_available && on_item_node) {
				on_item_node = false;
				
				vector<cls_article*>* articles = &feed_articles[feed_name];

				articles->push_back(article_item);
				
				article_item = new cls_article;
			}
		}

		if(xml_node->hasChildNodes()) {
			auto xml_child_nodes = xml_node->childNodes();

			collect_feed_impl(feed_name, feed_articles, xml_child_nodes, on_item_node, article_item);
		}

	}

	return;
}

void read_file_into_string(string& location, string& output) {
	ifstream rss_file(location.data());

	while(!rss_file.eof()) {
		string line;
		
		getline(rss_file, line);

		output.append(line);
	}
	
	rss_file.close();

	return;
}

void read_istream_into_string(istream& input, string& output) {
	char data;

	while(!input.eof()) {
		input.get(data);

		if(input.rdstate() == ios_base::goodbit) {
			output.push_back(data);
		}
	}
	
	return;
}

bool request_url_is_http(string& request_url) {
	bool result = false;

	int found_string_comparison_http = icompare(request_url, 0, 4, "http");
	
	result = (found_string_comparison_http == 0);

	if(!result) {
		int found_string_comparison_https = icompare(request_url, 0, 5, "https");

		result = (found_string_comparison_https == 0);
	}
	
	return result;
}

void get_http_response_stream(string& output, string& request_url) {
	string request_method = "GET";

	URI request_uri(request_url);
	
	HTTPClientSession http_session(request_uri.getHost(), request_uri.getPort());

	HTTPRequest http_request(request_method, request_url);

	try {
		http_session.sendRequest(http_request);

		HTTPResponse http_response;

		istream& temp_http_response_stream = http_session.receiveResponse(http_response);

		read_istream_into_string(temp_http_response_stream, output);
	}
	catch(const exception& e) {
		if(e.what() == "Host not found") {
			cout << e.what() << " " << "reverting to offline copy if available.\n";
		}
	}

	return;
}

void get_file_stream(string& output, string& location) {
	read_file_into_string(location, output);

	return;
}
