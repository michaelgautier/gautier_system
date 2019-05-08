#Copyright 2019 Michael Gautier
#This file is part of Gautier RSS System by Michael Gautier.
#Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
#Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.
#C++ Standard Library; Copyright 2018 Standard C++ Foundation.

echo "Building rss_requestor library"

./build_libnews_file.sh
./build_libnews_http.sh

echo "build rss lib"

rm obj/libnews_rss*.o

#Application Bridge
x86_64-w64-mingw32-g++ -std=c++17 -O3 -c -fPIC -I../lib/news/ -o obj/libnews_rss_data_consequence_spec.o ../lib/news/rss_data_consequence_spec.cxx
x86_64-w64-mingw32-g++ -std=c++17 -O3 -c -fPIC -I../lib/news/ -o obj/libnews_rss_set_consequence.o ../lib/news/rss_set_consequence.cxx

#Feed Data
x86_64-w64-mingw32-g++ -std=c++17 -O3 -c -fPIC -I../lib/news/ -o obj/libnews_rss_data_feed_name_spec.o ../lib/news/rss_data_feed_name_spec.cxx
x86_64-w64-mingw32-g++ -std=c++17 -O3 -c -fPIC -I../lib/news/ -o obj/libnews_rss_data_feed_headline_spec.o ../lib/news/rss_data_feed_headline_spec.cxx
x86_64-w64-mingw32-g++ -std=c++17 -O3 -c -fPIC -I../lib/news/ -o obj/libnews_rss_data_feed_article_spec.o ../lib/news/rss_data_feed_article_spec.cxx

x86_64-w64-mingw32-g++ -std=c++17 -O3 -c -fPIC -I../lib/news/ -o obj/libnews_rss_set_feed_name.o ../lib/news/rss_set_feed_name.cxx
x86_64-w64-mingw32-g++ -std=c++17 -O3 -c -fPIC -I../lib/news/ `xml2-config --cflags` -o obj/libnews_rss_set_feed_headline.o ../lib/news/rss_set_feed_headline.cxx

#File Managers
x86_64-w64-mingw32-g++ -std=c++17 -O3 -c -fPIC -I../lib/news/ -I../lib/news/techconstruct/ -o obj/libnews_rss_file_manager_feed_name.o ../lib/news/rss_file_manager_feed_name.cxx

x86_64-w64-mingw32-g++ -std=c++17 -O3 -c -fPIC -I../lib/news/ -I../lib/news/techconstruct/ `xml2-config --cflags` -o obj/libnews_rss_file_manager_feed_headline.o ../lib/news/rss_file_manager_feed_headline.cxx
x86_64-w64-mingw32-g++ -std=c++17 -O3 -c -fPIC -I../lib/news/ -I../lib/news/techconstruct/ -o obj/libnews_rss_file_manager_feed_article.o ../lib/news/rss_file_manager_feed_article.cxx

#Feed Cycles
x86_64-w64-mingw32-g++ -std=c++17 -O3 -c -fPIC -I../lib/news/ -I../lib/news/techconstruct/ -o obj/libnews_rss_cycle_feed_name.o ../lib/news/rss_cycle_feed_name.cxx
x86_64-w64-mingw32-g++ -std=c++17 -O3 -c -fPIC -I../lib/news/ -I../lib/news/techconstruct/ `xml2-config --cflags` -o obj/libnews_rss_cycle_feed_headline.o ../lib/news/rss_cycle_feed_headline.cxx
x86_64-w64-mingw32-g++ -std=c++17 -O3 -c -fPIC -I../lib/news/ -I../lib/news/techconstruct/ -o obj/libnews_rss_cycle_feed_article.o ../lib/news/rss_cycle_feed_article.cxx

ls -gGh obj/libnews_rss*

