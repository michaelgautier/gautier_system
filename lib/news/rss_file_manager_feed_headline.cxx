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
#include <ctime>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <Poco/String.h>

#include "rss_file_manager_feed_headline.hxx"

#include "techconstruct/http.hxx"
#include "techconstruct/file.hxx"

using namespace std;

using cls = news::rss_file_manager_feed_headline;
using http = rss_techconstruct::http;

const string _headline_node_name_rss = "item";
const string _headline_node_name_atom = "entry";

void process_node(const news::rss_data_feed_name_spec& feed_name, xmlNode* node, vector<news::rss_data_feed_headline_spec>& v);
vector<news::rss_data_feed_headline_spec> get_rss_feed(const news::rss_data_feed_name_spec& feed_name, const string& newsdocument);

void cls::init(const string& file_location) {
    _file_location = file_location;

    return;
}

bool cls::get_can_feed_refresh(const news::rss_data_feed_name_spec& feed_name) {
    bool can_refresh = true;

    bool feed_match = false;
    string last_checked_date_time_string;

    auto call_rss_line = [=,&feed_match,&last_checked_date_time_string](string& data,bool& ended) {
        if(data.size() < 1) {
            return;
        }

        //At least 1 tab expected most lines
        const auto tab_pos = data.find_first_of(_tab_char, 0);

        const string first_char = data.substr(0, 1);

        if(first_char == _comment_char) {
            return;
        } else if (first_char == _feedname_start_char) {
            const string name = data.substr(1, tab_pos-1);
            const string last_checked_date_time = data.substr(tab_pos+1);

            feed_match = (feed_name.name == name);

            if(feed_match) {
                last_checked_date_time_string = last_checked_date_time;

                ended = true;
            }
        }
    };

    rss_techconstruct::file rssfile;
    rssfile.read_file_into_string(_file_location, call_rss_line);

    if(!last_checked_date_time_string.empty()) {
        const auto last_checked_date_time_seconds_l = stoll(last_checked_date_time_string);

        const auto last_checked_date_time_seconds_t = (time_t)last_checked_date_time_seconds_l;

        const string last_checked_date_time_str = asctime(localtime(&last_checked_date_time_seconds_t));

        const auto last_checked_date_time = ctime(&last_checked_date_time_seconds_t);

        const auto current_time_t = time(NULL);

        const string current_time_str = asctime(localtime(&current_time_t));

        const auto current_time = ctime(&current_time_t);

        const auto elapsed_seconds = difftime(current_time_t, last_checked_date_time_seconds_t);

        cout
                << feed_name.name << "\n"
                << "\tLast checked: " << last_checked_date_time_str << " "
                << "\tCurrent time: " << current_time_str << " "
                << "\tSeconds since last update: " << elapsed_seconds << " \n";

        //Only refresh rss feed no earlier than once an hour = 3,600 seconds.
        can_refresh = elapsed_seconds > 3599;
    } else {
        can_refresh = true;
    }

    return can_refresh;
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
            const string headline = data.substr(1, tab_pos-1);

            const auto tab_pos_next = data.find_first_of(_tab_char, tab_pos+1);

            string url = data.substr(tab_pos+1, tab_pos_next-tab_pos);
            string description = data.substr(tab_pos_next+1);

            news::rss_data_feed_headline_spec spec;

            spec.headline = headline;
            spec.url = url;
            spec.description = description;
            spec.article_date = fh_set.last_checked_date_time_string;

            fh_set.add(spec);
        }
    };

    rss_techconstruct::file rssfile;
    rssfile.read_file_into_string(_file_location, call_rss_line);

    return fh_set;
}

/*Primary logic for rss feed retrieval.*/
news::rss_set_feed_headline cls::pull_set(const news::rss_data_feed_name_spec& feed_name) {
    news::rss_set_feed_headline fh_set = get_set(feed_name);

    if(get_can_feed_refresh(feed_name)) {
        vector<news::rss_data_feed_headline_spec> headlines_new;

        http http_handler;

        if(!feed_name.url.empty()) {
            string rss_feed_document_data;

            /*Expect an XML document representing the latest news feed.*/
            http_handler.get_stream(feed_name.url, rss_feed_document_data);

            if(!rss_feed_document_data.empty()) {
                const string rss_data_location = feed_name.name + ".xml";

                rss_techconstruct::file rssfile;
                rssfile.erase_stream(string(rss_data_location));
                rssfile.persist_stream(string(rss_data_location), rss_feed_document_data);

                headlines_new = get_rss_feed(feed_name, string(rss_data_location));
            }
        }

        int added_spec_size = 0;

        for(const news::rss_data_feed_headline_spec headline_new : headlines_new) {
            const bool spec_added = fh_set.add(headline_new);

            if(spec_added) {
                added_spec_size++;
            }
        }

        if(added_spec_size > 0) {
            save_set(feed_name, fh_set);
        }
    }

    return fh_set;
}

news::rss_set_consequence cls::save_set(const news::rss_data_feed_name_spec& feed_name, news::rss_set_feed_headline& rss_set) {
    news::rss_set_consequence cs;

    //Read the feed headlines and create/replace the file.
    vector<news::rss_data_feed_headline_spec> v = rss_set.get_specs();

    auto call_rss_line = [=,&v](ofstream& data) {
        data << _feedname_start_char << feed_name.name << _tab_char << time(NULL) << _newline_char;

        for(news::rss_data_feed_headline_spec& spec : v) {
            data << _headline_start_char << spec.headline << _tab_char << spec.url << _tab_char << spec.description << _newline_char;
        }
    };

    rss_techconstruct::file rssfile;
    rssfile.persist_stream(_file_location, call_rss_line);

    return cs;
}

vector<news::rss_data_feed_headline_spec> get_rss_feed(const news::rss_data_feed_name_spec& feed_name, const string& newsdocument) {
    vector<news::rss_data_feed_headline_spec> v;

    LIBXML_TEST_VERSION

    auto xml_parse_options = (XML_PARSE_RECOVER | XML_PARSE_NOERROR | XML_PARSE_NOWARNING | XML_PARSE_NOBLANKS | XML_PARSE_NOCDATA);

    xmlDoc* doc = NULL;

    doc = xmlParseFile(newsdocument.data());

    if(doc) {
        xmlNode* root_element = NULL;

        /*Get the root element node */
        root_element = xmlDocGetRootElement(doc);

        process_node(feed_name, root_element, v);
    }

    xmlFreeDoc(doc);

    /*
     * Cleanup function for the XML library.
     */
    xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();

    return v;
}

void process_node(const news::rss_data_feed_name_spec& feed_name, xmlNode* parentnode, vector<news::rss_data_feed_headline_spec>& v) {
    const string parentnode_name = (parentnode ? Poco::toLower(string(reinterpret_cast<const char*>(parentnode->name))) : "");

    const bool parentnode_is_rss_item = (parentnode_name == _headline_node_name_rss);
    const bool parentnode_is_atom_entry = (parentnode_name == _headline_node_name_atom);

    const bool parentnode_is_item = (parentnode && (parentnode_is_rss_item || parentnode_is_atom_entry));

    if(parentnode->children) {
        xmlNode* childnode = NULL;

        for(childnode = parentnode->children; childnode; childnode = childnode->next) {
            const auto childnode_type = childnode->type;

            if(childnode_type != XML_ELEMENT_NODE) {
                continue;
            }

            const string name = Poco::toLower(string(reinterpret_cast<const char*>(childnode->name)));

            //cout << "node name: " << name << "\n";

            if(name == _headline_node_name_rss || name == _headline_node_name_atom) {
                v.emplace_back(news::rss_data_feed_headline_spec());

                news::rss_data_feed_headline_spec* news = &v.back();
                news->feed_name = feed_name;
            } else if(parentnode_is_item && !v.empty()) {
                news::rss_data_feed_headline_spec* news = &v.back();

                const string text = string(reinterpret_cast<const char*>(xmlNodeGetContent(childnode)));

                //cout << "  text: " << text << "\n";

                if(parentnode_is_rss_item) {
                    if(name == "title") {
                        news->headline = text;
                    } else if(name == "link") {
                        news->url = text;
                    } else if(name == "description") {
                        news->description = text;
                    } else if(name == "pub_date" || name == "pubdate") {
                        news->article_date = text;
                    }
                } else if (parentnode_is_atom_entry) {
                    if(name == "title") {
                        news->headline = text;
                    } else if(name == "link") {
                        news->url = text;
                    } else if(name == "summary") {
                        news->description = text;
                    } else if(name == "published" || name == "updated") {
                        news->article_date = text;
                    }
                }
            }

            if(childnode->children) {
                process_node(feed_name, childnode, v);
            }
        }
    }

    return;
}
