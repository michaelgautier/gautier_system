#Copyright 2017 Michael Gautier
#This file is part of Gautier RSS System by Michael Gautier.
#Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
#Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.
#Portions of the POCO C++ Libraries utilize the following copyrighted material, the use of which is hereby acknowledged.
#POCO C++ Libraries released under the Boost Software License; Copyright 2017, Applied Informatics Software Engineering GmbH and Contributors; 
#C++ Standard Library; Copyright 2017 Standard C++ Foundation.

reset
echo "Building rss_requestor library"
./build_news_lib.sh

echo "	build"
ls -gGhU

rm obj/newsc.o
rm bin/newsc.e

echo "Building rss_requestor cli"
g++ -std=c++14 -c -g -I../lib/news -I/usr/include -o obj/newsc.o ../news_cli.cxx
g++ -lPocoFoundation -lPocoUtil -lPocoNet -lPocoXML -L./bin -lnews_parser -lnews_http -lnews_file -lnews_collector -Wl,-rpath,. -o bin/newsc obj/newsc.o 
cp --update ../config/feeds.txt bin
cp --update ../config/*.xml bin

echo "	bin"
ls -gGh bin/

