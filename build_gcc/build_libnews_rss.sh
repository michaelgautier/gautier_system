#Copyright 2018 Michael Gautier
#This file is part of Gautier RSS System by Michael Gautier.
#Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
#Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.
#Portions of the POCO C++ Libraries utilize the following copyrighted material, the use of which is hereby acknowledged.
#POCO C++ Libraries released under the Boost Software License; Copyright 2018, Applied Informatics Software Engineering GmbH and Contributors; 
#C++ Standard Library; Copyright 2018 Standard C++ Foundation.

echo "Building rss_requestor library"

./build_libnews_file.sh
./build_libnews_http.sh

echo "build rss lib"

rm obj/libnews_rss*.o

#Application Bridge
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_consequence_spec.o ../lib/news/rss_data_consequence_spec.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_set_consequence.o ../lib/news/rss_set_consequence.cxx

#Feed Data
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_name_spec.o ../lib/news/rss_data_feed_name_spec.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_headline_spec.o ../lib/news/rss_data_feed_headline_spec.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_article_spec.o ../lib/news/rss_data_feed_article_spec.cxx

g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_set_feed_name.o ../lib/news/rss_set_feed_name.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_set_feed_headline.o ../lib/news/rss_set_feed_headline.cxx

#File Managers
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_file_manager_feed_name.o ../lib/news/rss_file_manager_feed_name.cxx

g++ -std=c++14 -c -fPIC -g -I../lib/news/ -I../lib/news/techconstruct/ `xml2-config --cflags` -o obj/libnews_rss_file_manager_feed_headline.o ../lib/news/rss_file_manager_feed_headline.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -I../lib/news/techconstruct/ -o obj/libnews_rss_file_manager_feed_article.o ../lib/news/rss_file_manager_feed_article.cxx

#Feed Cycles
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_cycle_feed_name.o ../lib/news/rss_cycle_feed_name.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_cycle_feed_headline.o ../lib/news/rss_cycle_feed_headline.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_cycle_feed_article.o ../lib/news/rss_cycle_feed_article.cxx

ls -gGh obj/libnews_rss*

