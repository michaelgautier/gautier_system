/*
Copyright 2019 Michael Gautier

This file is part of Gautier RSS System by Michael Gautier.

Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.

C++ Standard Library; Copyright 2018 Standard C++ Foundation.
*/
#include "file.hxx"
using cls = rss_techconstruct::file;

void cls::read_file_into_string(std::string location, std::string& output) {
    std::ifstream rss_file(location.data());

    while(!rss_file.eof()) {
        std::string line;

        std::getline(rss_file, line);

        output.append(line);
    }

    rss_file.close();

    //cout << "File" << __FILE__ << " Line " << __LINE__ << " location " << location << "\n";
    //cout << "File" << __FILE__ << " Line " << __LINE__ << " output.empty " << output.empty() << "\n";

    return;
}

void cls::read_file_into_string(std::string location, std::function<void(std::string&)> file_callable) {
    std::ifstream rss_file(location.data());

    if(rss_file.rdstate() == std::ios_base::goodbit) {
        while(!rss_file.eof()) {
            std::string line;

            std::getline(rss_file, line);

            file_callable(line);
        }
    }

    rss_file.close();

    //cout << "File" << __FILE__ << " Line " << __LINE__ << " location " << location << "\n";
    //cout << "File" << __FILE__ << " Line " << __LINE__ << " output.empty " << output.empty() << "\n";

    return;
}

void cls::read_file_into_string(std::string location, std::function<void(std::string&,bool&)> file_callable) {
    std::ifstream rss_file(location.data());

    if(rss_file.rdstate() == std::ios_base::goodbit) {
        bool ended = false;

        while(!rss_file.eof()) {
            std::string line;

            std::getline(rss_file, line);

            file_callable(line, ended);

            if(ended) {
                break;
            }
        }
    }

    rss_file.close();

    return;
}

void cls::get_stream(std::string location, std::string& output) {
    read_file_into_string(location, output);

    return;
}

void cls::read_istream_into_string(std::istream& input, std::string& output) {
    char data;

    while(!input.eof()) {
        input.get(data);

        if(input.rdstate() == std::ios_base::goodbit) {
            output.push_back(data);
        }
    }

    return;
}

void cls::erase_stream(std::string location) {
    std::string feed_file_name(location.data(), std::ios_base::out | std::ios_base::trunc);

    std::ofstream feed_offline_file(feed_file_name.data());

    feed_offline_file << "";

    feed_offline_file.flush();

    feed_offline_file.close();

    return;
}

void cls::persist_stream(std::string location, std::string data_to_persist) {
    if(!data_to_persist.empty()) {
        std::string feed_file_name(location.data(), std::ios_base::out | std::ios_base::app);

        std::ofstream feed_offline_file(feed_file_name.data());

        feed_offline_file << data_to_persist;

        feed_offline_file.flush();

        feed_offline_file.close();
    }

    return;
}

void cls::persist_stream(std::string location, std::function<void(std::ofstream&)> file_callable) {
    std::string feed_file_name(location.data(), std::ios_base::out | std::ios_base::app);

    std::ofstream feed_offline_file(feed_file_name.data());

    file_callable(feed_offline_file);

    feed_offline_file.flush();

    feed_offline_file.close();

    return;
}

