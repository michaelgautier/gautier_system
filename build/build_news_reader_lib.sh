#Copyright 2017 Michael Gautier
#This file is part of Gautier RSS System by Michael Gautier.
#Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
#Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.
#Portions of the POCO C++ Libraries utilize the following copyrighted material, the use of which is hereby acknowledged.
#POCO C++ Libraries released under the Boost Software License; Copyright 2017, Applied Informatics Software Engineering GmbH and Contributors; 
#C++ Standard Library; Copyright 2017 Standard C++ Foundation.

rm obj/libnews_visual*.o
rm bin/libnews_visual*.so

echo "	build"
ls -gGhU

echo "build visual callable"
g++ -std=c++14 -c -fPIC -g -I../lib/visualfunc/formulation/ -o obj/libnews_visualcallable.o ../lib/visualfunc/formulation/visualcallable.cxx
g++ -g -shared -I../lib/visualfunc/formulation/ -Wl,-rpath,. -o bin/libnews_visualcallable.so obj/libnews_visualcallable.o

echo "build main screen blueprint"
g++ -std=c++14 -c -fPIC -g -I../lib/visualfunc/formulation/ -I../librss_ui_impl/ -o obj/librssui_mainscreenblueprint.o ../librss_ui_impl/mainscreenblueprint.cxx
g++ -g -shared -Wl,-rpath,. -o bin/librssui_mainscreenblueprint.so obj/librssui_mainscreenblueprint.o

echo "build main screen generator"
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -I../lib/news/techconstruct/ -I../lib/visualfunc/formulation/ -I../librss_ui_impl/ -o obj/librssui_mainscreengenerator.o ../librss_ui_impl/mainscreengenerator.cxx
g++ -g -shared -L./bin -I../librss_ui_impl/ -Wl,-rpath,. -o bin/librssui_mainscreengenerator.so obj/librssui_mainscreengenerator.o

echo "	obj"
ls -gGh obj/

echo "	bin"
ls -gGh bin/*.so
