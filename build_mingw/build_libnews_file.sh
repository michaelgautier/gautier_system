#Copyright 2019 Michael Gautier
#This file is part of Gautier RSS System by Michael Gautier.
#Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
#Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.
#C++ Standard Library; Copyright 2018 Standard C++ Foundation.
rm obj/libnews_file.o

echo "build file lib"

x86_64-w64-mingw32-g++ -std=c++17 -O3 -c -fPIC -I../lib/news/techconstruct/ -o obj/libnews_file.o ../lib/news/techconstruct/file.cxx
x86_64-w64-mingw32-g++ -std=c++17 -O3 -c -fPIC -I../lib/news/techconstruct/ -o obj/libnews_textfilter.o ../lib/news/techconstruct/textfilter.cxx

ls -gGh obj/libnews_file*

