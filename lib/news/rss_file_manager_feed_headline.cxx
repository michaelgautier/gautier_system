/*
Copyright 2018 Michael Gautier

This file is part of Gautier RSS System by Michael Gautier.

Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.

Portions of the POCO C++ Libraries utilize the following copyrighted material, the use of which is hereby acknowledged.
POCO C++ Libraries released under the Boost Software License; Copyright 2018, Applied Informatics Software Engineering GmbH and Contributors;
C++ Standard Library; Copyright 2018 Standard C++ Foundation.
*/
#include <Poco/DOM/Document.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/Node.h>
#include <Poco/DOM/NodeList.h>

#include <Poco/String.h>
#include <Poco/UnicodeConverter.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/SAX/SAXException.h>
#include <Poco/SAX/InputSource.h>

#include "rss_file_manager_feed_headline.hxx"

#include "techconstruct/http.hxx"
#include "techconstruct/file.hxx"

using namespace std;
using namespace Poco::XML;
using namespace Poco;

using cls = news::rss_file_manager_feed_headline;
using http = rss_techconstruct::http;

void process_node(Node* node, vector<news::rss_data_feed_headline_spec>& v);
vector<news::rss_data_feed_headline_spec> get_rss_feed(const news::rss_data_feed_name_spec& feed_name, string newsdocument);

void cls::init(const string& file_location) {
    _file_location = file_location;

    return;
}

news::rss_set_feed_headline cls::get_set(const news::rss_data_feed_name_spec& feed_name) {
    news::rss_set_feed_headline fh_set;

    //Read the file, get the feed headline, update the set.

    bool feed_match = false;

    auto call_rss_line = [=,&fh_set,&feed_match](string& data) {
        if(data.size() < 1) {
            return;
        }

        //At least 1 tab expected most lines
        auto tab_pos = data.find_first_of(_tab_char, 0);

        string first_char = data.substr(0, 1);

        if(first_char == _comment_char) {
            return;
        } else if (first_char == _feedname_start_char) {
            string name = data.substr(1, tab_pos-1);
            string last_checked_date_time = data.substr(tab_pos+1);

            feed_match = (feed_name.name == name);

            if(feed_match) {
                fh_set.last_checked_date_time_string = last_checked_date_time;
            }
        } else if (feed_match && first_char == _headline_start_char) {
            string headline = data.substr(1, tab_pos-1);

            auto tab_pos_next = data.find_first_of(_tab_char, tab_pos+1);

            string url = data.substr(tab_pos+1, tab_pos_next-1);
            string description = data.substr(tab_pos_next+1);

            news::rss_data_feed_headline_spec spec;

            spec.headline = headline;
            spec.url = url;
            spec.description = description;

            fh_set.add(spec);
        }
    };

    rss_techconstruct::file rssfile;
    rssfile.read_file_into_string(_file_location, call_rss_line);

    return fh_set;
}

/*Primary logic for rss feed retrieval.*/
news::rss_set_feed_headline cls::pull_set(const news::rss_data_feed_name_spec& feed_name) {
    news::rss_set_feed_headline fh_set_old = get_set(feed_name);
    news::rss_set_feed_headline fh_set;

    vector<news::rss_data_feed_headline_spec> headlines_old = fh_set_old.get_specs();
    vector<news::rss_data_feed_headline_spec> headlines_new;

    int headlines_old_size = headlines_old.size();

    http http_handler;

    if(http_handler.check_url_is_http(feed_name.url)) {
        string rss_feed_document_data;

        /*Expect an XML document representing the latest news feed.*/
        http_handler.get_stream(feed_name.url, rss_feed_document_data);

        if(!rss_feed_document_data.empty()) {
            headlines_new = get_rss_feed(feed_name, rss_feed_document_data);
        }
    }

    for (news::rss_data_feed_headline_spec headline_new : headlines_new) {
        string headline_n = headline_new.headline;

        bool headline_old_found = false;

        for (news::rss_data_feed_headline_spec headline_old : headlines_old) {
            string headline_o = headline_old.headline;

            headline_old_found =  (headline_n == headline_o);

            if (headline_old_found) {
                break;
            }
        }

        if (!headline_old_found) {
            fh_set.add(headline_new);
            fh_set_old.add(headline_new);
        }
    }

    headlines_old = fh_set_old.get_specs();

    if(!headlines_new.empty() && headlines_old_size != headlines_old.size()) {
        /*If this works right, the data will be added.*/
        save_set(feed_name, fh_set);
    }

    return fh_set_old;
}

news::rss_set_consequence cls::save_set(const news::rss_data_feed_name_spec& feed_name, news::rss_set_feed_headline& rss_set) {
    news::rss_set_consequence cs;

    //Read the feed headlines and create/replace the file.
    vector<news::rss_data_feed_headline_spec> v = rss_set.get_specs();

    auto call_rss_line = [=,&v](ofstream& data) {
        data << _feedname_start_char << feed_name.name << _tab_char << rss_set.last_checked_date_time_string << _newline_char;

        for(news::rss_data_feed_headline_spec& spec : v) {
            data << _headline_start_char << spec.headline << _tab_char << spec.url << _tab_char << spec.description << _newline_char;
        }
    };

    rss_techconstruct::file rssfile;
    rssfile.persist_stream(_file_location, call_rss_line);

    return cs;
}

vector<news::rss_data_feed_headline_spec> get_rss_feed(const news::rss_data_feed_name_spec& feed_name, string newsdocument) {
    vector<news::rss_data_feed_headline_spec> v;

    try {
        DOMParser reader;
        reader.setEncoding("utf-8");

        Document* xdoc = reader.parseString(newsdocument);

        Element* root = xdoc->documentElement();

        Node* currentnode = root->firstChild();

        /*
                Process:        CONVERT RSS XML TO SEQUENTIAL STRUCTURE
                Overview:       Translate multi-level Xml hierarchy into a 1-dimensional array
                Strategies:
                                Algorithmic approach - Branch and Bound (see Wladston, Ferreira, Filho 2018)
                                Replace structure processing by recursion with structure processing by iteration
                                Visits tree nodes through a bounded general graph search algorithm represented as a breadth first search
                History
                                9/30/2018 - Implemented as a conventional recursive algorithm. Termination criteria based on
                                visiting the last node at the first level. The linear array was populated based on all field
                                values set on a structured passed across recursive function calls.
                                11/3/2018 - Replaced the recursive process with an iteration modeled on processing a doublely linked list.
                Notes           Approach was chosen for the following reasons
                                - Avoid large function call stack accumulations in the event a larger document is input
                                - Fill output data structures based on the proximity of input elements rather than just their tag values
                                - Node proximity better determined while eliminating the need to forward a level indicator
        */
        while(currentnode != nullptr) {
            auto type = currentnode->nodeType();
            string name = Poco::toLower(currentnode->localName());

            if(name == "item" && type == Node::ELEMENT_NODE) {
                v.emplace_back(news::rss_data_feed_headline_spec());

                news::rss_data_feed_headline_spec* news = &v.back();
                news->feed_name = feed_name;
            }

            if(currentnode->hasChildNodes()) {
                process_node(currentnode, v);

                Node* nextnode = currentnode->firstChild();

                currentnode = nextnode;

                continue;
            }

            Node* nextnode = currentnode->nextSibling();

            if(nextnode != nullptr) {
                currentnode = nextnode;
            } else {
                currentnode = currentnode->parentNode();

                if(currentnode != nullptr) {
                    currentnode = currentnode->nextSibling();
                } else {
                    currentnode = nullptr;
                }
            }
        }
    } catch(SAXParseException e) {
        cout << e.name() << " File " << __FILE__ << " Line " << __LINE__ << " in function " << __func__ << "\n";
        cout << "\t" << e.what() << "\n";
        cout << "\t" << "Exception: Preceding line " << e.getLineNumber() << "\n";
        cout << "\t" << e.message() << "\n";
        cout << "\t" << e.displayText() << "\n";
    }

    return v;
}

void process_node(Node* node, vector<news::rss_data_feed_headline_spec>& v) {
    auto type = node->nodeType();

    if(!v.empty() && type == Node::ELEMENT_NODE) {
        news::rss_data_feed_headline_spec* news = &v.back();

        string name = Poco::toLower(node->localName());
        string text = node->innerText();

        if(name == "title") {
            news->headline = text;
        } else if(name == "link") {
            news->url = text;
        } else if(name == "description") {
            news->description = text;
        } else if(name == "pub_date" || name == "pubdate") {
            news->article_date = text;
        }/* else if(name == "encoded") { description should suffice
            news->content = text;
        }*/
    }

    return;
}
