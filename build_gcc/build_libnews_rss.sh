rm obj/libnews_rss_activity_timer.o
rm obj/libnews_rss_data_check.o
rm obj/libnews_rss_check_feed_article.o
rm obj/libnews_rss_check_feed_headline.o
rm obj/libnews_rss_check_feed_name.o
rm obj/libnews_rss_data_feed_article_set.o
rm obj/libnews_rss_data_feed_article_spec.o
rm obj/libnews_rss_data_feed_error_set.o
rm obj/libnews_rss_data_feed_error_spec.o
rm obj/libnews_rss_data_feed_headline_set.o
rm obj/libnews_rss_data_feed_headline_spec.o
rm obj/libnews_rss_data_feed_name_set.o
rm obj/libnews_rss_data_feed_name_spec.o
rm obj/libnews_rss_data_feed_status_set.o
rm obj/libnews_rss_data_feed_status_spec.o
rm obj/libnews_rss_enactor_feed_article_cycle.o
rm obj/libnews_rss_enactor_feed_headline_cycle.o
rm obj/libnews_rss_enactor_feed_name_cycle.o
rm obj/libnews_rss_file_manager_feed_article.o
rm obj/libnews_rss_file_manager_feed_headline.o
rm obj/libnews_rss_file_manager_feed_name.o

echo "build rss lib"

g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_check.o ../lib/news/rss_data_check.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_activity_timer.o ../lib/news/rss_activity_timer.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_check_feed_article.o ../lib/news/rss_check_feed_article.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_check_feed_headline.o ../lib/news/rss_check_feed_headline.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_check_feed_name.o ../lib/news/rss_check_feed_name.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_article_set.o ../lib/news/rss_data_feed_article_set.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_article_spec.o ../lib/news/rss_data_feed_article_spec.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_error_set.o ../lib/news/rss_data_feed_error_set.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_error_spec.o ../lib/news/rss_data_feed_error_spec.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_headline_set.o ../lib/news/rss_data_feed_headline_set.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_headline_spec.o ../lib/news/rss_data_feed_headline_spec.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_name_set.o ../lib/news/rss_data_feed_name_set.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_name_spec.o ../lib/news/rss_data_feed_name_spec.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_status_set.o ../lib/news/rss_data_feed_status_set.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_data_feed_status_spec.o ../lib/news/rss_data_feed_status_spec.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_enactor_feed_article_cycle.o ../lib/news/rss_enactor_feed_article_cycle.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_enactor_feed_headline_cycle.o ../lib/news/rss_enactor_feed_headline_cycle.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_enactor_feed_name_cycle.o ../lib/news/rss_enactor_feed_name_cycle.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_file_manager_feed_article.o ../lib/news/rss_file_manager_feed_article.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_file_manager_feed_headline.o ../lib/news/rss_file_manager_feed_headline.cxx
g++ -std=c++14 -c -fPIC -g -I../lib/news/ -o obj/libnews_rss_file_manager_feed_name.o ../lib/news/rss_file_manager_feed_name.cxx

