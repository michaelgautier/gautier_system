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
#./build_news_lib.sh

echo "Building visual function library"
./build_news_reader_lib.sh
./build_librssui_mainscreengenerator.sh

cp ../styles.css .
glib-compile-resources --target=../styles.cxx --generate-source ../styles.xml
g++ -c ../styles.cxx -o obj/gautier_rss_styles.o `pkg-config gtkmm-3.0 --cflags --libs`
rm styles.css

echo "Building rss reader"
g++ -c -g -I../lib/news/ -I../lib/news/techconstruct/ -I../lib/visualfunc/formulation/ -I../librss_ui_impl/ -I/usr/include/ `pkg-config gtkmm-3.0 --cflags --libs` -o obj/newsreader.o ../news_reader.cxx
g++ -lPocoFoundation -lPocoUtil -lPocoNet -lPocoXML -L./bin -lnews_parser -lnews_http -lnews_file -lnews_collector -lnews_feedscycle `pkg-config gtkmm-3.0 --cflags --libs` -Wl,-rpath,. -o bin/newsreader obj/newsreader.o obj/gautier_rss_styles.o obj/librssui_mainscreengenerator.o
ls -gGh obj/*.*o
ls -gGh bin/newsreader*
