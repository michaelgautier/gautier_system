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
                
                feed_articles[feed_name] = std::vector<gautier_rss_article*>();

                if(request_url_is_http(feed_url)) {
                        get_http_response_stream(feed_document_text, feed_url);
                } 
                else {
                        get_file_stream(feed_document_text, feed_url);
                }
                
                collect_feed(feed_name, feed_articles, feed_document_text);
        }

        return;
}

/*
        *************************************  Not yet tested.
                                                ********************************** Not yet tested.
*/
void gautier::system::rss::gautier_rss_requestor::collect_feed(std::string feed_name, std::map<std::string, std::vector<gautier_rss_article*> >& feed_articles, std::string& feed_document_stream) {
        try {
                std::cout << "feed: " << feed_name << "\n";

                Poco::XML::DOMParser rss_xml_reader;
                rss_xml_reader.setEncoding("utf-8");
	
//                std::wstring feed_document_str;
//                Poco::UnicodeConverter::convert(feed_document_stream, feed_document_str);

//                Poco::XML::InputSource* source = new Poco::XML::InputSource(feed_document_stream);

	        Poco::XML::Document* rss_xml_document = rss_xml_reader.parseString(feed_document_stream);
	
	        Poco::XML::Element* rss_xml_root = rss_xml_document->documentElement();
	
	        Poco::XML::NodeList* rss_xml_nodes = rss_xml_root->childNodes();

                bool on_item_node = false;
                gautier::system::rss::gautier_rss_article* article_item = new gautier::system::rss::gautier_rss_article;

                collect_feed_impl(feed_name, feed_articles, rss_xml_nodes, on_item_node, article_item);
        } catch(Poco::XML::SAXParseException e) {
                std::cout << e.message() << "\n";
                std::cout << e.displayText() << "\n";
                std::cout << e.name() << "\n";
                std::cout << e.what() << "\n";
                
                std::cout << "line " << e.getLineNumber() << "\n";
        }
        return;
}
/*
        *************************************  Not yet tested.
                                                ********************************** Not yet tested.
*/
void collect_feed_impl(std::string feed_name, std::map<std::string, std::vector<gautier::system::rss::gautier_rss_article*> >& feed_articles, Poco::XML::NodeList* rss_xml_nodes, bool& on_item_node, gautier::system::rss::gautier_rss_article* article_item) {
        int node_count = rss_xml_nodes->length();
        
        bool is_item_node = false;
        
	for(int node_index = 0; node_index < node_count; node_index++) {
		auto xml_node = rss_xml_nodes->item(node_index);
                
                auto xml_node_type = xml_node->nodeType();

                if(xml_node_type == Poco::XML::Node::ELEMENT_NODE) {
                        std::string xml_node_name = xml_node->localName();

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
                        else if(xml_node_name == "pub_date") {
                                article_item->article_date = xml_node->innerText();
                        }

                        bool required_values_available = !(article_item->headline.empty() && article_item->url.empty() && article_item->description.empty() && article_item->article_date.empty());

                        if(required_values_available && on_item_node) {
                                on_item_node = false;

                                std::cout << "\t" << article_item->headline << " " << article_item->article_date << " " << article_item->url << " " << article_item->description << "\n";
                                
                                std::vector<gautier::system::rss::gautier_rss_article*> articles = feed_articles[feed_name];

                                articles.push_back(article_item);
                                
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
        std::ifstream rss_file;

        rss_file.open(location.data(), std::ios::in);

        char data;

        while(!rss_file.eof()) {
                rss_file.get(data);

                if(rss_file.rdstate() == std::ios_base::goodbit) {
                        output.push_back(data);
                }
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

/*
        Planned refactoring
        Eventually, I am going to excise this method to another class.
*/
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

/*
        Planned refactoring
        Eventually, I am going to excise this method to another class.
*/
void get_http_response_stream(std::string& output, std::string& request_url) {
	std::string request_method = "GET";

        Poco::URI request_uri(request_url);
        
        Poco::Net::HTTPClientSession http_session(request_uri.getHost(), request_uri.getPort());

        Poco::Net::HTTPRequest http_request(request_method, request_url);

        http_session.sendRequest(http_request);

        Poco::Net::HTTPResponse http_response;

        std::istream& temp_http_response_stream = http_session.receiveResponse(http_response);

        read_istream_into_string(temp_http_response_stream, output);

        return;
}

void get_file_stream(std::string& output, std::string& location) {
        read_file_into_string(location, output);

        return;
}
