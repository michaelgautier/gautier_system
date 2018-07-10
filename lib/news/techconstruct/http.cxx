#include <fstream>
#include <iostream>

#include <Poco/String.h>
#include <Poco/URI.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

#include "http.hxx"
#include "file.hxx"

using namespace std;
using namespace Poco;
using namespace Poco::Net;

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

void cls::get_stream(string url, string& output) {
    URI request_uri(url);

    HTTPClientSession* http_session = new HTTPClientSession(request_uri.getHost());

    HTTPRequest http_request("GET", url);

    http_request.set("User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:10.0) Gecko/20100101 Firefox/10.0");
    http_request.set("Accept", "application/xhtml+xml, application/xml");
    http_request.set("Connection", "keep-alive");
    http_request.set("Cache-Control", "max-age=0");
    http_request.set("Accept-Encoding", "br");
    http_request.set("Upgrade-Insecure-Requests", "0");
    http_request.set("Robots", "off");

    const Context::Ptr context = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_RELAXED, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");

    const int found_string_comparison_https = icompare(url, 0, 5, "https");

    if(found_string_comparison_https == 0) {
        /*
        Verify ssl on the operating system platform using OpenSSL

        Phoronix.com is inaccessible through POCO C++ libraries on 7/10/2018. The response is still encoded without corresponding cipher decode to plain-text.
        Cannot use standard http with phoronix.com as the server issues a 301 http status code indicating the url is permanently set to https.

        Tested https://www.phoronix.com using command-line open ssl s_client -tls1 -status parameters and the results showed invalid.
        Of course Mozilla Firefox has mitigations built-in but the POCO C++ library cannot handle the scenario.

        Google's lets encrypt everywhere initiative does not work across the board.
        Anyway, the SSL code is here in case things change. Pickup where I left off.

        https://stackoverflow.com/questions/17354101/trouble-getting-poco-httpsclientsession-to-send-a-request-certificate-verify-f

        */
        Poco::Net::initializeSSL();

        http_session = new HTTPSClientSession(request_uri.getHost(), request_uri.getPort(), context);

        http_request.setMethod("POST");
        http_request.setContentLength(0);
        http_request.setContentType("application/x-www-form-urlencoded\r\n");
        http_request.setChunkedTransferEncoding(false);
    }

    try {
        http_session->sendRequest(http_request);

        HTTPResponse http_response;

        istream& temp_http_response_stream = http_session->receiveResponse(http_response);

        auto http_status_code = http_response.getStatus();

        if(http_status_code != 200) {
            cout << url << " http response " << http_status_code << " " << http_response.getReason() << " " << http_response.getReasonForStatus(http_status_code) << "\n";
        }

        cls_fs filehandler;

        filehandler.read_istream_into_string(temp_http_response_stream, output);
    } catch(const exception e) {
        cout << e.what() << " " << __FILE__ << " " << __func__ << " " << __LINE__ << "\n";
    }

    if(http_session) {
        delete http_session;
    }

    return;
}

