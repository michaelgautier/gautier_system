#include <fstream>
#include <iostream>

#include <Poco/String.h>
//#include <Poco/URI.h>
//#include <Poco/Net/HTTPClientSession.h>
//#include <Poco/Net/HTTPSClientSession.h>
//#include <Poco/Net/HTTPRequest.h>
//#include <Poco/Net/HTTPResponse.h>

/*
        C language API for libcurl
*/
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "http.hxx"
#include "file.hxx"

using namespace std;
using namespace Poco;
//using namespace Poco::Net;

using cls = rss_techconstruct::http;
using cls_fs = rss_techconstruct::file;

bool cls::check_url_is_http(string url) {
    bool result = false;

    int found_string_comparison_http = icompare(url, 0, 4, "http");

    result = (found_string_comparison_http == 0);

    if(!result) {
        int found_string_comparison_https = icompare(url, 0, 5, "https");

        result = (found_string_comparison_https == 0);
    }

    return result;
}

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

void cls::get_stream(string url, string& output) {
    curl_global_init(CURL_GLOBAL_ALL);

    auto curl_client = curl_easy_init();

    if(curl_client) {
        struct MemoryStruct chunk;
        chunk.memory = (char*)malloc(1);
        chunk.size = 0;
        /* will be grown as needed by the realloc above */
        /* no data at this point */

        auto curl_option_verbose /*debug only option*/ = curl_easy_setopt(curl_client, CURLOPT_VERBOSE, 1L);
        auto curl_option_method = curl_easy_setopt(curl_client, CURLOPT_HTTPGET, 1L);
        auto curl_option_url = curl_easy_setopt(curl_client, CURLOPT_URL, string(url).data());

        /* send all data to this function*/
        auto curl_option_writefunction = curl_easy_setopt(curl_client, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */
        auto curl_option_writedata = curl_easy_setopt(curl_client, CURLOPT_WRITEDATA, (void *)&chunk);

        /* some servers don't like requests that are made without a user-agent field, so we provide one */
        curl_easy_setopt(curl_client, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64; rv:10.0) Gecko/20100101 Firefox/10.0");
        curl_easy_setopt(curl_client, CURLOPT_TCP_KEEPALIVE, 1L);

        /*Other http header values*/
        struct curl_slist *curl_http_header = NULL;
        curl_http_header = curl_slist_append(curl_http_header, "Accept: application/xhtml+xml, application/xml");
        curl_http_header = curl_slist_append(curl_http_header, "Cache-Control: max-age=0");
        curl_http_header = curl_slist_append(curl_http_header, "Robots: off");

        curl_easy_setopt(curl_client, CURLOPT_HTTPHEADER, curl_http_header);

        auto curl_response = curl_easy_perform(curl_client);

        if(curl_response == CURLE_OK) {
            string response_data(chunk.memory);

            output = response_data;
        }

        curl_slist_free_all(curl_http_header);
        curl_easy_cleanup(curl_client);

        free(chunk.memory);
    }

    curl_global_cleanup();

    return;
}

//POCO C++ library does not work as well as libcurl

//POCO C++ library does not handle HTTPS  port 443   consistently well across system environments
//void cls::get_stream(string url, string& output) {
//    URI request_uri(url);

//    HTTPClientSession* http_session = new HTTPClientSession(request_uri.getHost());

//    HTTPRequest http_request("GET", url);

//    http_request.set("User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:10.0) Gecko/20100101 Firefox/10.0");
//    http_request.set("Accept", "application/xhtml+xml, application/xml");
//    http_request.set("Connection", "keep-alive");
//    http_request.set("Cache-Control", "max-age=0");
//    http_request.set("Accept-Encoding", "br");
//    http_request.set("Upgrade-Insecure-Requests", "0");
//    http_request.set("Robots", "off");

//    const Context::Ptr context = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_RELAXED, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");

//    const int found_string_comparison_https = icompare(url, 0, 5, "https");

//    if(found_string_comparison_https == 0) {
//        /*
//        Verify ssl on the operating system platform using OpenSSL

//        Phoronix.com is inaccessible through POCO C++ libraries on 7/10/2018. The response is still encoded without corresponding cipher decode to plain-text.
//        Cannot use standard http with phoronix.com as the server issues a 301 http status code indicating the url is permanently set to https.

//        Tested https://www.phoronix.com using command-line open ssl s_client -tls1 -status parameters and the results showed invalid.
//        Of course Mozilla Firefox has mitigations built-in but the POCO C++ library cannot handle the scenario.

//        Google's lets encrypt everywhere initiative does not work across the board.
//        Anyway, the SSL code is here in case things change. Pickup where I left off.

//        https://stackoverflow.com/questions/17354101/trouble-getting-poco-httpsclientsession-to-send-a-request-certificate-verify-f

//        */
//        Poco::Net::initializeSSL();

//        http_session = new HTTPSClientSession(request_uri.getHost(), request_uri.getPort(), context);

//        http_request.setMethod("POST");
//        http_request.setContentLength(0);
//        http_request.setContentType("application/x-www-form-urlencoded\r\n");
//        http_request.setChunkedTransferEncoding(false);
//    }

//    try {
//        http_session->sendRequest(http_request);

//        HTTPResponse http_response;

//        istream& temp_http_response_stream = http_session->receiveResponse(http_response);

//        auto http_status_code = http_response.getStatus();

//        if(http_status_code != 200) {
//            cout << url << " http response " << http_status_code << " " << http_response.getReason() << " " << http_response.getReasonForStatus(http_status_code) << "\n";
//        }

//        cls_fs filehandler;

//        filehandler.read_istream_into_string(temp_http_response_stream, output);
//    } catch(const exception e) {
//        cout << e.what() << " " << __FILE__ << " " << __func__ << " " << __LINE__ << "\n";
//    }

//    if(http_session) {
//        delete http_session;
//    }

//    return;
//}

