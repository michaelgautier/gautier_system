#Copyright 2018 Michael Gautier
#This file is part of Gautier RSS System by Michael Gautier.
#Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
#Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.
#Portions of the POCO C++ Libraries utilize the following copyrighted material, the use of which is hereby acknowledged.
#POCO C++ Libraries released under the Boost Software License; Copyright 2018, Applied Informatics Software Engineering GmbH and Contributors; 
#C++ Standard Library; Copyright 2018 Standard C++ Foundation.
reset
#echo "Building rss_requestor library"
#uncomment the following line to build the rss parser library for the user interface.
./build_libnews_rss.sh

cp ../styles.css .
glib-compile-resources --target=../styles.cxx --generate-source ../styles.xml
g++ -c ../styles.cxx -o obj/gautier_rss_styles.o `pkg-config gtkmm-3.0 --cflags --libs`

rm styles.css
rm obj/librssui_mainscreengenerator.o

echo "build main screen generator"

g++ -c -g -I../lib/news/ -I../lib/news/techconstruct/ -I../librss_ui_impl/ `pkg-config gtkmm-3.0 --cflags --libs` `pkg-config webkit2gtk-4.0 --cflags --libs` -o obj/librssui_mainscreengenerator.o ../librss_ui_impl/mainscreengenerator.cxx

ls -gGh obj/librssui_mainscreengenerator*

echo "Building newsreader"

g++ -c -g -I../lib/news/ -I../lib/news/techconstruct/ -I../librss_ui_impl/ -I/usr/include/ `pkg-config gtkmm-3.0 --cflags --libs` `pkg-config webkit2gtk-4.0 --cflags --libs` -o obj/newsreader.o ../news_reader.cxx
g++ -lPocoFoundation -lPocoUtil -lPocoNet -lPocoXML -L./bin `pkg-config gtkmm-3.0 --cflags --libs` -Wl,-rpath,. -o bin/newsreader obj/newsreader.o obj/gautier_rss_styles.o obj/librssui_mainscreengenerator.o obj/libnews_file.o obj/libnews_http.o obj/libnews_rss_cycle_feed_article.o obj/libnews_rss_cycle_feed_headline.o obj/libnews_rss_cycle_feed_name.o obj/libnews_rss_data_consequence_spec.o obj/libnews_rss_data_feed_article_spec.o obj/libnews_rss_data_feed_headline_spec.o obj/libnews_rss_data_feed_name_spec.o obj/libnews_rss_file_manager_feed_article.o obj/libnews_rss_file_manager_feed_headline.o obj/libnews_rss_file_manager_feed_name.o obj/libnews_rss_set_consequence.o obj/libnews_rss_set_feed_headline.o obj/libnews_rss_set_feed_name.o

ls -gGh obj/newsreader*
ls -gGh bin/newsreader*

