#Copyright 2017 Michael Gautier
#This file is part of Gautier RSS System by Michael Gautier.
#Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
#Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.
#Portions of the POCO C++ Libraries utilize the following copyrighted material, the use of which is hereby acknowledged.
#POCO C++ Libraries released under the Boost Software License; Copyright 2017, Applied Informatics Software Engineering GmbH and Contributors; 
#C++ Standard Library; Copyright 2017 Standard C++ Foundation.

rm obj/libnews_visualcallable.o
rm obj/libnews_visualcallableproto.o
rm obj/libnews_visualcycle.o
rm obj/libnews_visualsheet.o
rm obj/libnews_visualsheetconfigparam.o

rm bin/libnews_visualcallable.so
rm bin/libnews_visualcycle.so
rm bin/libnews_visualcycle.so
rm bin/libnews_visualsheet.so
rm bin/libnews_visualsheetconfigparam.so

reset

echo "	build"
ls -gGhU

echo "build visual callable"
g++ -std=c++14 -c -fPIC -g -I../lib/visualfunc/formulation/ -o obj/libnews_visualcallable.o ../lib/visualfunc/formulation/visualcallable.cxx
g++ -g -shared -I../lib/visualfunc/formulation/ -Wl,-rpath,. -o bin/libnews_visualcallable.so obj/libnews_visualcallable.o

echo "build visual callable proto"
g++ -std=c++14 -c -fPIC -g -I../lib/visualfunc/formulation/ -o obj/libnews_visualcallableproto.o ../lib/visualfunc/formulation/visualcallableproto.cxx
g++ -g -shared -I../lib/visualfunc/formulation/ -Wl,-rpath,. -o bin/libnews_visualcallableproto.so obj/libnews_visualcallableproto.o

#echo "build visual cycle"
#g++ -std=c++14 -c -fPIC -g -I../lib/visualfunc/formulation/ -o obj/libnews_visualcycle.o ../lib/visualfunc/formulation/visualcycle.cxx
#g++ -g -shared -I../lib/visualfunc/formulation/ -Wl,-rpath,. -o bin/libnews_visualcycle.so obj/libnews_visualcycle.o

#echo "build visual sheet"
#g++ -std=c++14 -c -fPIC -g -I../lib/visualfunc/formulation/ -o obj/libnews_visualsheet.o ../lib/visualfunc/formulation/visualsheet.cxx
#g++ -g -shared -I../lib/visualfunc/formulation/ -Wl,-rpath,. -o bin/libnews_visualsheet.so obj/libnews_visualsheet.o

#echo "build visual sheet config param"
#g++ -std=c++14 -c -fPIC -g -I../lib/visualfunc/formulation/ -o obj/libnews_visualsheetconfigparam.o ../lib/visualfunc/formulation/visualsheetconfigparam.cxx
#g++ -g -shared -I../lib/visualfunc/formulation/ -Wl,-rpath,. -o bin/libnews_visualsheetconfigparam.so obj/libnews_visualsheetconfigparam.o

echo "build main screen header bar"
g++ -std=c++14 -c -fPIC -g -I../librss_ui_impl/ -o obj/librssui_mainscreenheaderbar.o ../librss_ui_impl/mainscreenheaderbar.cxx
g++ -g -shared -I../librss_ui_impl/ -Wl,-rpath,. -o bin/librssui_mainscreenheaderbar.so obj/librssui_mainscreenheaderbar.o

#must be last
echo "build main screen generator"
g++ -std=c++14 -c -fPIC -g -I../lib/visualfunc/formulation/ -I../librss_ui_impl/ -o obj/librssui_mainscreengenerator.o ../librss_ui_impl/mainscreengenerator.cxx
g++ -g -shared -L./bin -I../librss_ui_impl/ -lrssui_mainscreenheaderbar -Wl,-rpath,. -o bin/librssui_mainscreengenerator.so obj/librssui_mainscreengenerator.o

echo "	obj"
ls -gGh obj/

echo "	bin"
ls -gGh bin/*.so
