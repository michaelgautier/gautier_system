rm obj/libnews_rss_*.o

echo "build rss lib"

#Feed Data
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_name_spec.o ../lib/news/rss_data_feed_name_spec.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_headline_spec.o ../lib/news/rss_data_feed_headline_spec.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_article_spec.o ../lib/news/rss_data_feed_article_spec.cxx

g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_set_feed_name.o ../lib/news/rss_set_feed_name.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_set_feed_headline.o ../lib/news/rss_set_feed_headline.cxx

#File Managers
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_file_manager_feed_name.o ../lib/news/rss_file_manager_feed_name.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -I../lib/news/techconstruct/ -o obj/libnews_rss_file_manager_feed_headline.o ../lib/news/rss_file_manager_feed_headline.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -I../lib/news/techconstruct/ -o obj/libnews_rss_file_manager_feed_article.o ../lib/news/rss_file_manager_feed_article.cxx

#Feed Cycles
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_cycle_feed_name.o ../lib/news/rss_cycle_feed_name.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_cycle_feed_headline.o ../lib/news/rss_cycle_feed_headline.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_cycle_feed_article.o ../lib/news/rss_cycle_feed_article.cxx

#Application Bridge
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_consequence_spec.o ../lib/news/rss_data_consequence_spec.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_set_consequence.o ../lib/news/rss_set_consequence.cxx

g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_activity_timer.o ../lib/news/rss_activity_timer.cxx

