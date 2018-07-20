#include <fstream>
#include <iostream>

#include <Poco/String.h>

/*
        C language API for libcurl
*/
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "http.hxx"
#include "file.hxx"

using namespace std;

using cls = rss_techconstruct::http;
using cls_fs = rss_techconstruct::file;

/*
   7/12/2018 1:45AM - Now using cURL.
   POCO C++ libraries - HTTPSClientSession did not handle ssl well out of the box.
   The following code was taken 99% verbatim from the cURL Everything pdf published on the curl website http://haxx.se.

   It is primarily C language code rather than C++ but it works perfectly.
*/
struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;

    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);

    if(mem->memory == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");

        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void cls::get_stream(const string& url, string& output) {
    /*
       7/12/2018 1:45AM - Based on the actual libcurl example C code shown on https://ec.haxx.se/ on this date.
       At this time the relevant pages are:

       https://ec.haxx.se/libcurlexamples.html
       https://ec.haxx.se/libcurl-http-requests.html
    */

    /*Startup*/
    curl_global_init(CURL_GLOBAL_ALL);

    /*HTTP Request*/
    auto curl_client = curl_easy_init();

    if(curl_client) {
        struct MemoryStruct chunk;
        chunk.memory = (char*)malloc(1);
        chunk.size = 0;
        /* will be grown as needed by the realloc above */
        /* no data at this point */

        /*debug only option*/
        curl_easy_setopt(curl_client, CURLOPT_VERBOSE, 1L);

        curl_easy_setopt(curl_client, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl_client, CURLOPT_URL, string(url).data());

        /* send all data to this function*/
        curl_easy_setopt(curl_client, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl_client, CURLOPT_WRITEDATA, (void *)&chunk);

        /* some servers don't like requests that are made without a user-agent field, so we provide one */
        curl_easy_setopt(curl_client, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64; rv:10.0) Gecko/20100101 Firefox/10.0");
        curl_easy_setopt(curl_client, CURLOPT_TCP_KEEPALIVE, 1L);

        /*Other http header values*/
        struct curl_slist *curl_http_header = NULL;
        curl_http_header = curl_slist_append(curl_http_header, "Accept: application/xhtml+xml, application/xml");
        curl_http_header = curl_slist_append(curl_http_header, "Cache-Control: max-age=0");
        curl_http_header = curl_slist_append(curl_http_header, "Robots: off");

        curl_easy_setopt(curl_client, CURLOPT_HTTPHEADER, curl_http_header);

        /*Access the web page*/
        auto curl_response = curl_easy_perform(curl_client);

        /*Check the http response*/
        if(curl_response == CURLE_OK) {
            string response_data(chunk.memory);

            output = response_data;
        }

        /*Clean up*/
        curl_slist_free_all(curl_http_header);
        curl_easy_cleanup(curl_client);

        free(chunk.memory);
    }

    /*Clean up following startup*/
    curl_global_cleanup();

    return;
}
