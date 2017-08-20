rm bin/gautier_rss_requestor_cli

reset

g++ -std=c++14 -c -g -I../gautier_rss_requestor_lib/ -o bin/gautier_rss_requestor_cli ../gautier_rss_requestor_lib/cli.cxx

echo "	build"
ls -gGhU

echo "	bin"
ls -gGh bin/

echo "	obj"
ls -gGh obj/


