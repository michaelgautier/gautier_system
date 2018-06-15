rm obj/libnews_rss_activity_timer.o
rm obj/libnews_rss_consequence_spec.o
rm obj/libnews_rss_consequence_set.o
rm obj/libnews_rss_data_feed_article_set.o
rm obj/libnews_rss_data_feed_article_spec.o
rm obj/libnews_rss_data_feed_headline_set.o
rm obj/libnews_rss_data_feed_headline_spec.o
rm obj/libnews_rss_data_feed_name_set.o
rm obj/libnews_rss_data_feed_name_spec.o
rm obj/libnews_rss_cycle_feed_article.o
rm obj/libnews_rss_cycle_feed_headline.o
rm obj/libnews_rss_cycle_feed_name.o
rm obj/libnews_rss_file_manager_feed_article.o
rm obj/libnews_rss_file_manager_feed_headline.o
rm obj/libnews_rss_file_manager_feed_name.o

echo "build rss lib"

g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_activity_timer.o ../lib/news/rss_activity_timer.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_consequence_spec.o ../lib/news/rss_consequence_spec.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_consequence_set.o ../lib/news/rss_consequence_set.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_article_set.o ../lib/news/rss_data_feed_article_set.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_article_spec.o ../lib/news/rss_data_feed_article_spec.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_headline_set.o ../lib/news/rss_data_feed_headline_set.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_headline_spec.o ../lib/news/rss_data_feed_headline_spec.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_name_set.o ../lib/news/rss_data_feed_name_set.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_name_spec.o ../lib/news/rss_data_feed_name_spec.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_cycle_feed_article.o ../lib/news/rss_cycle_feed_article.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_cycle_feed_headline.o ../lib/news/rss_cycle_feed_headline.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_cycle_feed_name.o ../lib/news/rss_cycle_feed_name.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_file_manager_feed_article.o ../lib/news/rss_file_manager_feed_article.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_file_manager_feed_headline.o ../lib/news/rss_file_manager_feed_headline.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_file_manager_feed_name.o ../lib/news/rss_file_manager_feed_name.cxx

