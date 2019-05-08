#Copyright 2019 Michael Gautier
#This file is part of Gautier RSS System by Michael Gautier.
#Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
#Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.
#C++ Standard Library; Copyright 2018 Standard C++ Foundation.

reset

export CC=x86_64-w64-mingw32-gcc
export CXX=x86_64-w64-mingw32-c++
export LD=x86_64-w64-mingw32-ld
export AR=x86_64-w64-mingw32-ar
export AS=x86_64-w64-mingw32-as
export NM=x86_64-w64-mingw32-nm
export STRIP=x86_64-w64-mingw32-strip
export RANLIB=x86_64-w64-mingw32-ranlib
export DLLTOOL=x86_64-w64-mingw32-dlltool
export OBJDUMP=x86_64-w64-mingw32-objdump
export RESCOMP=x86_64-w64-mingw32-windres
export WINDRES=x86_64-w64-mingw32-windres

./build_libnews_rss.sh

echo "Building newsc"

x86_64-w64-mingw32-g++ -std=c++17 -O3 -c -fPIC -I../lib/news -I../lib/news/techconstruct/ `xml2-config --cflags` -o obj/newsc.o ../news_cli.cxx
x86_64-w64-mingw32-g++ -std=c++17 -O3 -lxml2 -lcurl -L./bin -Wl,-rpath,. -o bin/newsc obj/libnews_file.o obj/libnews_http.o obj/libnews_textfilter.o obj/newsc.o obj/libnews_rss_cycle_feed_article.o obj/libnews_rss_cycle_feed_headline.o obj/libnews_rss_cycle_feed_name.o obj/libnews_rss_data_consequence_spec.o obj/libnews_rss_data_feed_article_spec.o obj/libnews_rss_data_feed_headline_spec.o obj/libnews_rss_data_feed_name_spec.o obj/libnews_rss_file_manager_feed_article.o obj/libnews_rss_file_manager_feed_headline.o obj/libnews_rss_file_manager_feed_name.o obj/libnews_rss_set_consequence.o obj/libnews_rss_set_feed_headline.o obj/libnews_rss_set_feed_name.o `x86_64-w64-mingw32-pkg-config --static --libs libcurl libxml-2.0`

#cp --update ../config/feeds.txt bin
#cp --update ../config/*.xml bin

ls -gGh bin/newsc*

