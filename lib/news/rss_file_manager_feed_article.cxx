/*
Copyright 2019 Michael Gautier

This file is part of Gautier RSS System by Michael Gautier.

Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.

C++ Standard Library; Copyright 2018 Standard C++ Foundation.
*/
#include "rss_file_manager_feed_article.hxx"

using cls = news::rss_file_manager_feed_article;
using http = rss_techconstruct::http;

void cls::init(const std::string& file_location) {
    _file_location = file_location;

    return;
}

news::rss_data_feed_article_spec cls::pull_spec(const news::rss_data_feed_headline_spec& feed_headline) {
    news::rss_data_feed_article_spec spec;
    spec.feed_headline = feed_headline;

    http http_handler;

    if(!feed_headline.url.empty()) {
        std::string rss_feed_document_data;

        /*Expect an HTML document representing the latest news article.*/
        http_handler.get_stream(feed_headline.url, rss_feed_document_data);

        if(!rss_feed_document_data.empty()) {
            spec.content = rss_feed_document_data;

            save_spec(spec);
        }
    }

    return spec;
}

news::rss_data_feed_article_spec cls::get_spec(const news::rss_data_feed_headline_spec& feed_headline) {
    news::rss_data_feed_article_spec spec;
    spec.feed_headline = feed_headline;

    //Read the file, get the feed article, update the set.
    bool feed_match = false;
    bool headline_match = false;

    auto call_rss_line = [=,&spec,&feed_match,&headline_match](std::string& data) {
        if(data.size() < 1) {
            return;
        }

        //At least 1 tab expected most lines
        const auto tab_pos = data.find_first_of(_tab_char, 0);

        const std::string first_char = data.substr(0, 1);

        if(first_char == _comment_char) {
            return;
        } else if (first_char == _feedname_start_char) {
            const std::string name = data.substr(1, tab_pos-1);
            const std::string last_checked_date_time = data.substr(tab_pos+1);

            feed_match = (feed_headline.feed_name.name == name);
        } else if (feed_match && first_char == _headline_start_char) {
            const std::string headline = data.substr(1, tab_pos-1);
            const std::string url = data.substr(tab_pos+1);

            if (headline == feed_headline.headline) {
                headline_match = true;
            }
        } else if (headline_match) {
            spec.content = spec.content + data;
        }
    };

    rss_techconstruct::file rssfile;
    rssfile.read_file_into_string(_file_location, call_rss_line);

    return spec;
}

news::rss_set_consequence cls::save_spec(const news::rss_data_feed_article_spec& feed_article) {
    news::rss_set_consequence cs;

    //Read the feed articles and create/replace the file.
    auto call_rss_line = [=,&feed_article](std::ofstream& data) {
        data << _feedname_start_char << feed_article.feed_headline.feed_name.name << _tab_char << feed_article.feed_headline.article_date << _newline_char;

        data << _headline_start_char << feed_article.feed_headline.headline << _tab_char << feed_article.feed_headline.url << _newline_char;

        data << feed_article.content << _newline_char;
    };

    rss_techconstruct::file rssfile;
    rssfile.persist_stream(_file_location, call_rss_line);

    news::rss_data_consequence_spec consq;
    consq.set_catalog_number(1);
    consq.set_statement("set saved operation finished");

    cs.add(consq);

    return cs;
}

