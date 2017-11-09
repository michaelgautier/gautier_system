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
#include <ios>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/names.h>
#include <FL/Fl_Double_Window.H>

#include "material.hxx"
#include "request.hxx"
#include "collector.hxx"

using material = rss::material;
using request = rss::request;
using collector = rss::collector;

#include "visualcycle.hxx"
#include "visualcallable.hxx"
#include "visualsheet.hxx"

using namespace std;
namespace ns_vf = visualfunc::formulation;

/*
        Mix of the functional style with objects passed through functions.
        The functional nature is driven by the way FLTK is designed.

        Plan to have icons at the bottom for navigation.
        A mobile app friendly design that can work equally on desktop and mobile
        in that the program structure is simple, not too complex.
        
        If abstracted properly, all the high-level program branches can be 
        placed here. The key is to properly abstract the code that would 
        otherwise be in the function bodies.
        
        The way that will work is to write all the code in the function bodies 
        first and then systematically abstract the function bodies. That will 
        allow for greater, up-front, productivity while planning for a cleaner 
        program representation in this module.
*/

//void get_feed_namedaddresses(string& location, vector<request>& feed_parameters);
//void get_feed_parameter_lines_from_config(string& location, vector<string>& feed_parameter_lines);

//void render_reader(Fl_Double_Window& winhost, Fl_Widget& winframe, bool is_inited);

int main() {
        auto cf = ns_vf::get_visual_sheet_config();

        ns_vf::visualsheet vs = ns_vf::make_visual_sheet(cf);

        vector<ns_vf::visualcallable> vce = ns_vf::make_visual_callables(ns_vf::get_visual_callable_prototypes());

        vs(vce);

        bool success = vs();

	return (int)success;
}

//void render_reader(Fl_Double_Window& winhost, Fl_Widget& winframe, bool is_inited) {
//        if(!is_inited) {
//                //construct the reader

//                //add data to the reader
//	        vector<request> feed_parameters;

//	        string feed_names_location = "feeds.txt";

//	        get_feed_namedaddresses(feed_names_location, feed_parameters);

//	        collector rss_requestor;

//	        for(auto feedsource : feed_parameters) {
//	                cout << "******** feed: \t " << feedsource.feedname << "\n\n\n";

//	                vector<material> feed_articles = rss_requestor.pull(feedsource);

//	                for(auto feed_article_entry : feed_articles) {
//		                string headline = feed_article_entry.headline;
//		                string url = feed_article_entry.url;
//		                string description = feed_article_entry.description;
//		                string article_date = feed_article_entry.article_date;

//		                cout << "\t headline: " << headline << "\n";
//		                cout << "\t date: " << article_date << "\n";
//		                cout << "\t url: " << url << "\n";
//		                cout << "\t description: " << description << "\n";
//	                }

//	                cout << "\n\n\n";
//	        }
//        }
//        return;
//}

//void get_feed_namedaddresses(string& location, vector<request>& feed_parameters) {
//	vector<string> feed_parameter_lines;

//	get_feed_parameter_lines_from_config(location, feed_parameter_lines);

//	for(auto feed_line : feed_parameter_lines) {
//		//define feed url
//		if(feed_line.size() > 1 && feed_line[0] == '#') {
//			continue;
//		}

//		auto separator_pos = feed_line.find_first_of("\t");

//		if(separator_pos == string::npos) {
//			continue;
//		}
// 
//		string feed_name = feed_line.substr(0, separator_pos);
//		string feed_url = feed_line.substr(separator_pos+1);
//		
//		request feed_request;

//		feed_request.feedname = feed_name;
//		feed_request.webaddress = feed_url;

//		feed_parameters.push_back(feed_request);
//	}
//	
//	return;
//}

//void get_feed_parameter_lines_from_config(string& location, vector<string>& feed_parameter_lines) {
//	ifstream feeds_file(location.data());
//		
//	while(!feeds_file.eof()) {
//		string feeds_data;

//		getline(feeds_file, feeds_data);

//		feed_parameter_lines.push_back(feeds_data);
//	}
//	
//	feeds_file.close();

//	return;
//}

