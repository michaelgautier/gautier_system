#include <codecvt>
#include <cstddef>
#include <fstream>
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

void gautier::system::rss::gautier_rss_requestor::request_feeds(
				std::vector<gautier_rss_request>& 				feed_parameters, 
				std::map<std::string, std::vector<gautier_rss_article> >& 	feed_articles
			) {
        for(auto rss_location : feed_parameters) {
                    std::istringstream s1("");
                    std::istream feed_document_stream(s1.rdbuf());
                
                std::string feed_name = rss_location.feed_name;
                std::string feed_url = rss_location.feed_url;
                
                feed_articles[feed_name] = std::vector<gautier_rss_article>();
                                     
                std::cout << "checking feed " << feed_name << " \n";

                if(request_url_is_http(feed_url)) {
                        get_http_response_stream(feed_document_stream, feed_url);
                } 
                else {
                        get_file_stream(feed_document_stream, feed_url);
                }

	        std::wstring feed_document_text;

                std::cout << "diagnostic output \n";
                
                
                
        //					while(!feed_document_stream.eof()) {
        //                                                std::wstring output;
        //                                                
        //						feed_document_stream >> output;

        //                                                feed_document_text += output;
        //					}
                
                std::cout << "feed collection \n";
                
                collect_feed(feed_name, feed_articles, feed_document_stream);

	        //std::wcout << feed_document_text;
        }

        return;
}
/*
        Planned refactoring
        Eventually, I am going to excise this method to another class.
*/
bool gautier::system::rss::gautier_rss_requestor::request_url_is_http(std::string& request_url) {
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
void gautier::system::rss::gautier_rss_requestor::get_http_response_stream(std::istream& http_response_stream, std::string& request_url) {
	std::string request_method = "GET";

        Poco::URI request_uri(request_url);
        
        Poco::Net::HTTPClientSession http_session(request_uri.getHost(), request_uri.getPort());

        Poco::Net::HTTPRequest http_request(request_method, request_url);

        http_session.sendRequest(http_request);

        Poco::Net::HTTPResponse http_response;

        std::istream& temp_http_response_stream = http_session.receiveResponse(http_response);

//        while(!temp_http_response_stream.eof()){
//                std::string output;

//                temp_http_response_stream >> output;
//                
//                std::wstring output_w =  Poco::UnicodeConverter::to<std::wstring>(output);

//                http_response_stream << output_w;
//        }
        
        
        temp_http_response_stream.swap(http_response_stream);
        
        return;
}

void gautier::system::rss::gautier_rss_requestor::get_file_stream(std::istream& file_stream, std::string& location) {
        std::ifstream rss_file;

        rss_file.open(location.data(), std::ios::in);

        file_stream.rdbuf(rss_file.rdbuf());
//        while(!rss_file.eof()) {
//                std::string output;

//                rss_file >> output;

//                std::wstring output_w =  Poco::UnicodeConverter::to<std::wstring>(output);
//                
//                file_stream << output_w;
//        }
        
        rss_file.close();
        
        return;
}

/*
        *************************************  Not yet tested.
                                                ********************************** Not yet tested.
*/
void collect_feed_impl(std::string feed_name, std::map<std::string, std::vector<gautier::system::rss::gautier_rss_article> >& feed_articles, Poco::XML::NodeList* rss_xml_nodes) {
        int node_count = rss_xml_nodes->length();
        
        int id = 0;

        std::string headline = "";
        std::string url = "";
        std::string description = "";
        std::string article_date = "";

        bool on_item_node = false;
        bool is_item_node = false;
        
	for(int node_index = 0; node_index < node_count; node_index++) {
		auto xml_node = rss_xml_nodes->item(node_index);
                
                auto xml_node_type = xml_node->nodeType();

                if(xml_node_type == Poco::XML::Node::ELEMENT_NODE) {
                        std::string xml_node_name = xml_node->localName();

                        if(xml_node_name == "item") {
                                on_item_node = true;
                                is_item_node = true;
                                std::cout << "found item \n";
                        }
                        else if(xml_node_name == "title") {
                                headline = xml_node->innerText();
                                std::cout << "found " << headline << " \n";
                        }
                        else if(xml_node_name == "link") {
                                url = xml_node->innerText();
                        }
                        else if(xml_node_name == "description") {
                                description = xml_node->innerText();
                        }
                        else if(xml_node_name == "pub_date") {
                                article_date = xml_node->innerText();
                        }

                        if(!is_item_node && on_item_node && xml_node->lastChild()) {
                                on_item_node = false;

                                bool required_values_available = (!headline.empty() && !url.empty());
                                
                                if(required_values_available) {
                                        gautier::system::rss::gautier_rss_article article_item;

		                        article_item.headline = headline;
		                        article_item.url = url;
		                        article_item.description = description;
		                        article_item.article_date = article_date;

                                        std::vector<gautier::system::rss::gautier_rss_article> articles = feed_articles[feed_name];
                                        
                                        articles.push_back(article_item);

                                        std::cout << "finished " << headline << " \n";

                                        headline = "";
		                        url = "";
		                        description = "";
		                        article_date = "";
                                }
                        }
                        
                        is_item_node = false;                        
                }

                if(xml_node->hasChildNodes()) {
                        auto xml_child_nodes = xml_node->childNodes();

                        std::cout << "child nodes \n";

                        collect_feed_impl(feed_name, feed_articles, xml_child_nodes);
                }

        }

        return;
}
/*
        *************************************  Not yet tested.
                                                ********************************** Not yet tested.
*/
void gautier::system::rss::gautier_rss_requestor::collect_feed(std::string feed_name, std::map<std::string, std::vector<gautier_rss_article> >& feed_articles, std::istream& feed_document_stream) {
        try {
        Poco::XML::DOMParser rss_xml_reader;
        rss_xml_reader.setEncoding("utf-8");
	
        //std::string feed_document_str;
        //Poco::UnicodeConverter::convert(feed_document_stream, feed_document_str);

        Poco::XML::InputSource* source = new Poco::XML::InputSource(feed_document_stream);

	Poco::XML::Document* rss_xml_document = rss_xml_reader.parse(source);
	
	Poco::XML::Element* rss_xml_root = rss_xml_document->documentElement();
	
	Poco::XML::NodeList* rss_xml_nodes = rss_xml_root->childNodes();

        std::cout << "collecting nodes \n";

        collect_feed_impl(feed_name, feed_articles, rss_xml_nodes);
        } catch(Poco::XML::SAXParseException e) {
                std::cout << e.message() << "\n";
                std::cout << e.displayText() << "\n";
                std::cout << e.name() << "\n";
                std::cout << e.what() << "\n";
                
                std::cout << "line " << e.getLineNumber() << "\n";
        }
        return;
}

