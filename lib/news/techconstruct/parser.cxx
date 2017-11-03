#include <iostream>
#include <queue>

#include <Poco/DOM/Document.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/Node.h>
#include <Poco/DOM/NodeList.h>

#include <Poco/String.h>
#include <Poco/UnicodeConverter.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/SAX/SAXException.h>
#include <Poco/SAX/InputSource.h>

#include "parser.hxx"

using namespace std;
using namespace rss;
using namespace Poco::XML;
using namespace Poco;

using cls = rss_techconstruct::parser;

void process_node(Node* node, vector<material>& v);

vector<material> cls::read(string newsdocument) {
        vector<material> v;

	try {
		DOMParser reader;
		reader.setEncoding("utf-8");
	
		Document* xdoc = reader.parseString(newsdocument);
	
		Element* root = xdoc->documentElement();
	
                Node* currentnode = root->firstChild();

                /*
                        Process:        CONVERT RSS XML TO SEQUENTIAL STRUCTURE
                        Overview:       Translate multi-level Xml hierarchy into a 1-dimensional array
                        Strategies:
                                        Algorithmic approach - Branch and Bound (see Wladston, Ferreira, Filho 2017)
                                        Replace structure processing by recursion with structure processing by iteration
                                        Visits tree nodes through a bounded general graph search algorithm represented as a breadth first search
                        History
                                        9/30/2017 - Implemented as a conventional recursive algorithm. Termination criteria based on
                                        visiting the last node at the first level. The linear array was populated based on all field
                                        values set on a structured passed across recursive function calls.
                                        11/3/2017 - Replaced the recursive process with an iteration modeled on processing a doublely linked list.
                        Notes           Approach was chosen for the following reasons
                                        - Avoid large function call stack accumulations in the event a larger document is input
                                        - Fill output data structures based on the proximity of input elements rather than just their tag values
                                        - Node proximity better determined while eliminating the need to forward a level indicator
                */                
                while(currentnode != nullptr) {
                        auto type = currentnode->nodeType();
                        string name = Poco::toLower(currentnode->localName());

                        if(name == "item" && type == Node::ELEMENT_NODE) {
                                v.emplace_back(material());
                        }

                        if(currentnode->hasChildNodes()) {
                                process_node(currentnode, v);

                                Node* nextnode = currentnode->firstChild();
                                
                                currentnode = nextnode;
                                
                                continue;
                        }

                        Node* nextnode = currentnode->nextSibling();

                        if(nextnode != nullptr) {
                                currentnode = nextnode;
                        }
                        else {
                                currentnode = currentnode->parentNode();

                                if(currentnode != nullptr) {
                                        currentnode = currentnode->nextSibling();
                                }
                                else {
                                        currentnode = nullptr;
                                }
                        }
                }		
	} catch(SAXParseException e) {
		cout << e.name() << " File " << __FILE__ << " Line " << __LINE__ << " in function " << __func__ << "\n";
		cout << "\t" << e.what() << "\n";
		cout << "\t" << "Exception: Preceding line " << e.getLineNumber() << "\n";
		cout << "\t" << e.message() << "\n";
		cout << "\t" << e.displayText() << "\n";
	}

	return v;
}

void process_node(Node* node, vector<material>& v) {
        auto type = node->nodeType();

        if(!v.empty() && type == Node::ELEMENT_NODE) {
                material* news = &v.back();

                string name = Poco::toLower(node->localName());
                string text = node->innerText();

                if(name == "title") {
                        news->headline = text;
                }
                else if(name == "link") {
                        news->url = text;
                }
                else if(name == "description") {
                        news->description = text;
                }
                else if(name == "pub_date" || name == "pubdate") {
                        news->article_date = text;
                }
        }
        
        return;
}
