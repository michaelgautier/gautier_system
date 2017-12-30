#include <iostream>
#include <string>
#include <fstream>

#include "material.hxx"
#include "request.hxx"

#include "feedscycle.hxx"

using namespace std;
using cls = rss::feedscycle;

using material = rss::material;
using request = rss::request;

void cls::get_feed_names_and_addresses(string& location, vector<request>& feed_parameters) {
	vector<string> feed_parameter_lines;

	get_feed_parameter_lines_from_config(location, feed_parameter_lines);

	for(auto feed_line : feed_parameter_lines) {
		//define feed url
		if(feed_line.size() > 1 && feed_line[0] == '#') {
			continue;
		}

		auto separator_pos = feed_line.find_first_of("\t");

		if(separator_pos == string::npos) {
			continue;
		}
 
		string feed_name = feed_line.substr(0, separator_pos);
		string feed_url = feed_line.substr(separator_pos+1);
		
		request feed_request;

		feed_request.feedname = feed_name;
		feed_request.webaddress = feed_url;

		feed_parameters.push_back(feed_request);
	}
	
	return;
}

bool cls::set_feed_names_and_addresses(string& location, string& feed_name, string& feed_url) {
        return false;
}

void cls::get_feed_parameter_lines_from_config(string& location, vector<string>& feed_parameter_lines) {
	ifstream feeds_file(location.data());
		
	while(!feeds_file.eof()) {
		string feeds_data;

		getline(feeds_file, feeds_data);

		feed_parameter_lines.push_back(feeds_data);
	}
	
	feeds_file.close();

	return;
}
