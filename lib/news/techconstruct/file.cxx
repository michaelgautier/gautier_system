#include <iostream>
#include <fstream>

#include "file.hxx"

using namespace std;
using cls = rss_techconstruct::file;

void cls::read_file_into_string(string location, string& output) {
	ifstream rss_file(location.data());

	while(!rss_file.eof()) {
		string line;
		
		getline(rss_file, line);

		output.append(line);
	}
	
	rss_file.close();

        //cout << "File" << __FILE__ << " Line " << __LINE__ << " location " << location << "\n";
        //cout << "File" << __FILE__ << " Line " << __LINE__ << " output.empty " << output.empty() << "\n";

	return;
}

void cls::get_stream(string location, string& output) {
	read_file_into_string(location, output);

	return;
}

void cls::read_istream_into_string(istream& input, string& output) {
	char data;

	while(!input.eof()) {
		input.get(data);

		if(input.rdstate() == ios_base::goodbit) {
			output.push_back(data);
		}
	}
	
	return;
}

void cls::persist_stream(string filesystem_location, string data_to_persist) {
	if(!data_to_persist.empty()) {
		string feed_file_name(filesystem_location.data(), ios_base::out | ios_base::trunc);

		ofstream feed_offline_file(feed_file_name.data());

		feed_offline_file << data_to_persist;

		feed_offline_file.flush();

		feed_offline_file.close();
	}

        return;
}
