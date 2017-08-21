rm obj/*.o
rm bin/libgautier_rss_requestor*

reset

g++ -std=c++14 -c -fPIC -g -I../gautier_rss_requestor_lib/ -o obj/gautier_rss_requestor_lib.o ../gautier_rss_requestor_lib/lib.cxx
g++ -g -shared -I../gautier_rss_requestor_lib/ -lPocoFoundation -lPocoUtil -lPocoNet -o bin/libgautier_rss_requestor.so obj/gautier_rss_requestor_lib.o

echo "	build"
ls -gGhU

echo "	bin"
ls -gGh bin/

echo "	obj"
ls -gGh obj/


