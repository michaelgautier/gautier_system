#Copyright 2019 Michael Gautier
#This file is part of Gautier RSS System by Michael Gautier.
#Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
#Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.
#C++ Standard Library; Copyright 2018 Standard C++ Foundation.

cp ../styles.css .
glib-compile-resources --target=../styles.cxx --generate-source ../styles.xml
g++ -std=c++17 -O3 -c -fPIC ../styles.cxx -o obj/gautier_rss_styles.o `pkg-config gtkmm-3.0 --cflags --libs`

rm styles.css
rm obj/librssui_mainscreengenerator.o

echo "build main screen generator"

g++ -std=c++17 -O3 -c -fPIC -I../lib/news/ -I../lib/news/techconstruct/ -I../librss_ui_impl/ `pkg-config gtkmm-3.0 --cflags --libs` -o obj/librssui_mainscreengenerator.o ../librss_ui_impl/mainscreengenerator.cxx

ls -gGh obj/librssui_mainscreengenerator*

