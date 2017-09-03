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

void get_file_stream(std::string& output, std::string& location);
void get_http_response_stream(std::string& output, std::string& request_url);
void read_file_into_string(std::string& location, std::string& output);
void read_istream_into_string(std::istream& input, std::string& output);
bool request_url_is_http(std::string& request_url);
void collect_feed_impl(std::string feed_name, std::map<std::string, std::vector<gautier::system::rss::gautier_rss_article*> >& feed_articles, Poco::XML::NodeList* rss_xml_nodes, bool& on_item_node, gautier::system::rss::gautier_rss_article* article_item);

void gautier::system::rss::gautier_rss_requestor::request_feeds(
				std::vector<gautier_rss_request>& 				feed_parameters, 
				std::map<std::string, std::vector<gautier_rss_article*> >& 	feed_articles
			) {
        for(auto rss_location : feed_parameters) {
	        std::string feed_document_text;

                std::string feed_name = rss_location.feed_name;
                std::string feed_url = rss_location.feed_url;
                std::string feed_offline_location(feed_name + ".xml");
                
                feed_articles[feed_name] = std::vector<gautier_rss_article*>();

                if(request_url_is_http(feed_url)) {
                        std::string feed_response_data;

                        get_http_response_stream(feed_response_data, feed_url);

                        if(!feed_response_data.empty()) {
                                std::string feed_file_name(feed_offline_location.data(), std::ios_base::out | std::ios_base::trunc);
                                
                                std::ofstream feed_offline_file(feed_file_name.data());
                                
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

void gautier::system::rss::gautier_rss_requestor::collect_feed(std::string feed_name, std::map<std::string, std::vector<gautier_rss_article*> >& feed_articles, std::string& feed_document_stream) {
        try {
                Poco::XML::DOMParser rss_xml_reader;
                rss_xml_reader.setEncoding("utf-8");
	
	        Poco::XML::Document* rss_xml_document = rss_xml_reader.parseString(feed_document_stream);
	
	        Poco::XML::Element* rss_xml_root = rss_xml_document->documentElement();
	
	        Poco::XML::NodeList* rss_xml_nodes = rss_xml_root->childNodes();

                bool on_item_node = false;
                gautier::system::rss::gautier_rss_article* article_item = new gautier::system::rss::gautier_rss_article;

                collect_feed_impl(feed_name, feed_articles, rss_xml_nodes, on_item_node, article_item);
        } catch(Poco::XML::SAXParseException e) {
                std::cout << "Exception handler on line " << __LINE__ << " func " << __func__ << "\n";
                std::cout << "\t" << "Exception generated on line " << e.getLineNumber() << "\n";
                std::cout << "\t" << e.message() << "\n";
                std::cout << "\t" << e.displayText() << "\n";
                std::cout << "\t" << e.name() << "\n";
                std::cout << "\t" << e.what() << "\n";
        }

        return;
}

void collect_feed_impl(std::string feed_name, std::map<std::string, std::vector<gautier::system::rss::gautier_rss_article*> >& feed_articles, Poco::XML::NodeList* rss_xml_nodes, bool& on_item_node, gautier::system::rss::gautier_rss_article* article_item) {
        int node_count = rss_xml_nodes->length();
        
        bool is_item_node = false;
        
	for(int node_index = 0; node_index < node_count; node_index++) {
		auto xml_node = rss_xml_nodes->item(node_index);
                
                auto xml_node_type = xml_node->nodeType();

                if(xml_node_type == Poco::XML::Node::ELEMENT_NODE) {
                        std::string xml_node_name = Poco::toLower(xml_node->localName());

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
                                
                                std::vector<gautier::system::rss::gautier_rss_article*>* articles = &feed_articles[feed_name];

                                articles->push_back(article_item);
                                
                                article_item = new gautier::system::rss::gautier_rss_article;
                        }
                }

                if(xml_node->hasChildNodes()) {
                        auto xml_child_nodes = xml_node->childNodes();

                        collect_feed_impl(feed_name, feed_articles, xml_child_nodes, on_item_node, article_item);
                }

        }

        return;
}

void read_file_into_string(std::string& location, std::string& output) {
        std::ifstream rss_file(location.data());

        while(!rss_file.eof()) {
                std::string line;
                
                std::getline(rss_file, line);

                output.append(line);
        }
        
        rss_file.close();

        return;
}

void read_istream_into_string(std::istream& input, std::string& output) {
        char data;

        while(!input.eof()) {
                input.get(data);

                if(input.rdstate() == std::ios_base::goodbit) {
                        output.push_back(data);
                }
        }
        
        return;
}

bool request_url_is_http(std::string& request_url) {
        bool result = false;

        int found_string_comparison_http = Poco::icompare(request_url, 0, 4, "http");
        
        result = (found_string_comparison_http == 0);

        if(!result) {
                int found_string_comparison_https = Poco::icompare(request_url, 0, 5, "https");

                result = (found_string_comparison_https == 0);
        }
        
        return result;
}

void get_http_response_stream(std::string& output, std::string& request_url) {
	std::string request_method = "GET";

        Poco::URI request_uri(request_url);
        
        Poco::Net::HTTPClientSession http_session(request_uri.getHost(), request_uri.getPort());

        Poco::Net::HTTPRequest http_request(request_method, request_url);

        try {
                http_session.sendRequest(http_request);

                Poco::Net::HTTPResponse http_response;

                std::istream& temp_http_response_stream = http_session.receiveResponse(http_response);

                read_istream_into_string(temp_http_response_stream, output);
        }
        catch(const std::exception& e) {
                if(e.what() == "Host not found") {
                        std::cout << e.what() << " " << "reverting to offline copy if available.\n";
                }
        }

        return;
}

void get_file_stream(std::string& output, std::string& location) {
        read_file_into_string(location, output);

        return;
}
