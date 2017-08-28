#Copyright 2017 Michael Gautier
#This file is part of Gautier RSS System by Michael Gautier.
#Gautier RSS System by Michael Gautier is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
#Gautier RSS System by Michael Gautier is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#You should have received a copy of the GNU General Public License along with Gautier RSS System by Michael Gautier.  If not, see <http://www.gnu.org/licenses/>.
#Portions of the POCO C++ Libraries utilize the following copyrighted material, the use of which is hereby acknowledged.
#POCO C++ Libraries released under the Boost Software License; Copyright 2017, Applied Informatics Software Engineering GmbH and Contributors; 
#C++ Standard Library; Copyright 2017 Standard C++ Foundation.

rm bin/gautier_rss_requestor_cli
rm obj/gautier_rss_requestor_cli.o

reset

g++ -std=c++14 -c -g -I../gautier_rss_requestor_lib/ -I/usr/include -lgautier_rss_requestor -o obj/gautier_rss_requestor_cli.o ../gautier_rss_requestor_cli.cxx
g++ -lPocoFoundation -lPocoUtil -lPocoNet -lPocoXML -o bin/gautier_rss_requestor_cli obj/gautier_rss_requestor_lib.o obj/gautier_rss_requestor_cli.o
cp ../config/feeds.txt bin

echo "	build"
ls -gGhU

echo "	bin"
ls -gGh bin/

echo "	obj"
ls -gGh obj/


