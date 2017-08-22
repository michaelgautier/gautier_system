#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/URI.h>
#include <Poco/Path.h>
#include <gautier_rss_requestor.hxx>
#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>
void gautier::system::rss::gautier_rss_requestor::request_feeds(
				std::vector<gautier_rss_request>& 				feed_parameters, 
				std::map<std::string, std::vector<gautier_rss_article> >& 	feed_articles
			) {
				for(auto rss_location : feed_parameters) {
                                        std::stringstream feed_document_stream;
                                        
                                        std::string feed_url = rss_location.feed_url;
                                                                                
                                        if(request_url_is_http(feed_url)) {
                                                get_http_response_stream(feed_document_stream, feed_url);
                                        } 
                                        else {
                                                //open a file and get the data into the stream
                                                get_file_stream(feed_document_stream, feed_url);
                                        }

					std::string feed_document_text;

					while(!feed_document_stream.eof()) {
						feed_document_stream >> feed_document_text;

						std::cout << feed_document_text;
					}

					/*
					Poco::DOMParser rss_xml_reader;
					
					Poco::Document* rss_xml_document = rss_xml_reader.parse(http_response_stream);
					
					Poco::Element* rss_xml_root = rss_xml_document->documentElement();
					
					Poco::NodeList* rss_xml_nodes = rss_xml_root->childNodes();
					
					for(auto rss_node : rss_xml_nodes) {
						gautier_rss_article article_item;

						article_item.headline = "";
						article_item.url = "";
						article_item.description = "";
						article_item.article_date = "";

						feed_articles.add(article_item);

						continue;
					}*/
				}

				return;
}

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

void gautier::system::rss::gautier_rss_requestor::get_http_response_stream(std::stringstream& http_response_stream, std::string& request_url) {
	std::string request_method = "GET";

        Poco::URI request_uri(request_url);
        
        Poco::Net::HTTPClientSession http_session(request_uri.getHost(), request_uri.getPort());

        Poco::Net::HTTPRequest http_request(request_method, request_url);

        http_session.sendRequest(http_request);

        Poco::Net::HTTPResponse http_response;

        std::istream& temp_http_response_stream = http_session.receiveResponse(http_response);

        while(!temp_http_response_stream.eof()){
                std::string output;
                
                temp_http_response_stream >> output;
                
                http_response_stream << output;
        }
        
        return;
}

void gautier::system::rss::gautier_rss_requestor::get_file_stream(std::stringstream& file_stream, std::string& location) {
        std::ifstream rss_file;

        rss_file.open(location.data(), std::ios::in);

        while(!rss_file.eof()) {
                std::string output;

                rss_file >> output;

                file_stream << output;
        }
        
        rss_file.close();
        
        return;
}

