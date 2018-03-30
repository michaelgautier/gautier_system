#Copyright 2018 Michael Gautier
#This file is part of Gautier RSS System by Michael Gautier.
#Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
#Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.
#Portions of the POCO C++ Libraries utilize the following copyrighted material, the use of which is hereby acknowledged.
#POCO C++ Libraries released under the Boost Software License; Copyright 2018, Applied Informatics Software Engineering GmbH and Contributors; 
#C++ Standard Library; Copyright 2018 Standard C++ Foundation.
rm obj/libui_interactionstate.o
rm bin/libui_interactionstate.so
echo "build interaction state"
clang++ -std=c++14 -c -fPIC -g -I../lib/visualfunc/formulation/ -o obj/libui_interactionstate.o ../lib/visualfunc/formulation/interactionstate.cxx
clang++ -g -shared -L./bin -I../lib/visualfunc/formulation/ -Wl,-rpath,. -o bin/libui_interactionstate.so obj/libui_interactionstate.o
ls -gGh bin/libui_interactionstate*
