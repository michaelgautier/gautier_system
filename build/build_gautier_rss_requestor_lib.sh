rm obj/*.o

reset

g++ -std=c++14 -c -g -I../gautier_rss_requestor_lib/ -o obj/gautier_rss_requestor_lib.o ../gautier_rss_requestor_lib/lib.cxx

echo "	build"
ls -gGhU

echo "	bin"
ls -gGh bin/

echo "	obj"
ls -gGh obj/


