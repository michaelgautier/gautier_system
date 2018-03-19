#Copyright 2018 Michael Gautier
#This file is part of Gautier RSS System by Michael Gautier.
#Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
#Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.
#Portions of the POCO C++ Libraries utilize the following copyrighted material, the use of which is hereby acknowledged.
#POCO C++ Libraries released under the Boost Software License; Copyright 2018, Applied Informatics Software Engineering GmbH and Contributors; 
#C++ Standard Library; Copyright 2018 Standard C++ Foundation.

reset
echo "Building rss_requestor library"
#./build_news_lib.sh

echo "Building visual function library"
./build_news_reader_lib.sh

echo "Building rss reader"
#shared libraries - system
#g++ -std=c++14 -c -g  -I../lib/news/ -I../lib/news/techconstruct/ -I../lib/visualfunc/formulation/ -I../librss_ui_impl/ -I/usr/include/ -o obj/newsreader.o ../news_reader.cxx
#g++ -lfltk -lPocoFoundation -lPocoUtil -lPocoNet -lPocoXML -L./bin -lnews_parser -lnews_http -lnews_file -lnews_collector -lnews_feedscycle -lrssui_mainscreengenerator -lnews_visualcallable -Wl,-rpath,. -o bin/newsreader obj/newsreader.o 

#shared libraries - manual
g++ -std=c++14 -c -g -I../lib/news/ -I../lib/news/techconstruct/ -I../lib/visualfunc/formulation/ -I../librss_ui_impl/ -I/usr/include/ -o obj/newsreader.o ../news_reader.cxx
g++ -lPocoFoundation -lPocoUtil -lPocoNet -lPocoXML -ldlib -lallegro -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_dialog -L./bin -lnews_parser -lnews_http -lnews_file -lnews_collector -lnews_feedscycle -lrssui_mainscreengenerator -lrssui_interactionstate -lrssui_textbuffer -lnews_visualcallable -Wl,-rpath,. -o bin/newsreader obj/newsreader.o

#static compile FLTK
#g++ -std=c++14 -c -g -I$HOME/Code/lib_gcc/fltk/include/ -I../lib/news/ -I../lib/news/techconstruct/ -I../lib/visualfunc/formulation/ -I../librss_ui_impl/ -I/usr/include/ -o obj/newsreader.o ../news_reader.cxx

#g++ -g `$HOME/Code/lib_gcc/fltk/bin/fltk-config --ldstaticflags --use-gl` -lPocoFoundation -lPocoUtil -lPocoNet -lPocoXML -L./bin -lnews_parser -lnews_http -lnews_file -lnews_collector -lnews_feedscycle -lrssui_mainscreengenerator -lnews_visualcallable -Wl,-rpath,. -o bin/newsreader obj/newsreader.o $HOME/Code/lib_gcc/fltk/lib/libfltk_gl.a
echo "	bin"
ls -gGh bin/

