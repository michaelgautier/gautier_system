rm bin/gautier_rss_requestor_cli
rm obj/gautier_rss_requestor_cli.o

reset

g++ -std=c++14 -c -g -I../gautier_rss_requestor_lib/ -I/usr/include -lgautier_rss_requestor -o obj/gautier_rss_requestor_cli.o ../gautier_rss_requestor_lib/cli.cxx
g++ -lPocoFoundation -lPocoUtil -lPocoNet -lPocoXML -o bin/gautier_rss_requestor_cli obj/gautier_rss_requestor_lib.o obj/gautier_rss_requestor_cli.o

echo "	build"
ls -gGhU

echo "	bin"
ls -gGh bin/

echo "	obj"
ls -gGh obj/


