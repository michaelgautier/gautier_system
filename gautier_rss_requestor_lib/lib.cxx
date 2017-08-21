#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/DOM/DOMParser.h>
#include <gautier_rss_requestor.hxx>

void gautier::system::rss::gautier_rss_requestor::request_feeds(
				std::vector<gautier_rss_request>& 				feed_parameters, 
				std::map<std::string, std::vector<gautier_rss_article>>& 	feed_articles
			) {
				std::string request_method = "GET";

				Poco::Net::HTTPClientSession http_session;

				for(auto rss_location : feed_parameters) {

					Poco::Net::HTTPRequest http_request(request_method, rss_location.feed_url);
					
					http_session.sendRequest(http_request);

					Poco::Net::HTTPResponse http_response;
					
					std::istream& http_response_stream = http_session.receiveResponse(http_response);
					
					std::string http_response_text;
					
					http_response_stream >> http_response_text;
					
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
