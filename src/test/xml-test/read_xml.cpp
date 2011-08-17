#include <iostream>
#include <string>
#include <stdlib.h>

#include "xercesc/util/PlatformUtils.hpp"
#include "xercesc/dom/DOMDocument.hpp"
#include "xercesc/dom/DOMNode.hpp"
#include "xercesc/dom/DOMNodeList.hpp"
#include "xercesc/parsers/XercesDOMParser.hpp"
#include "xercesc/util/XMLString.hpp"

using std::cout;
using std::endl;
using std::string;
using namespace xercesc;

int main(int argc, char ** argv) {
    try {
        XMLPlatformUtils::Initialize();
    } catch(const XMLException& ex) {
        cout << "error during initialization" << endl;
        return 0;
    }
    XercesDOMParser* parser = new XercesDOMParser();
    const char* file_path = "test.xml";
    parser->parse(file_path);
    DOMDocument* pdocument = parser->getDocument();
    XMLCh tmp[50];
    XMLString::transcode("framework", tmp, 49);
    DOMNodeList* node_list = pdocument->getElementsByTagName(tmp);
    for (unsigned int i = 0; i < node_list->getLength(); ++i) {
        DOMNode* node = node_list->item(i);
        DOMNodeList* child_nodes = node->getChildNodes();
        for (unsigned int j = 0; j < child_nodes->getLength(); ++j) {
            DOMNode* fnode = child_nodes->item(j);
            XMLCh str1[10], str2[10];
            XMLString::transcode("name", str1, 9);
            XMLString::transcode("quota", str2, 9);
            if(XMLString::compareString(fnode->getNodeName(), str1) == 0) {
                char* value = XMLString::transcode(fnode->getTextContent());
                string ds = value;
                cout << ds << endl;
                XMLString::release(&value);
            } else if (XMLString::compareString(fnode->getNodeName(), str2) == 0) {
                char* value = XMLString::transcode(fnode->getTextContent());
                string ss = value;
                cout << ss << endl;
                XMLString::release(&value);
            }
        }
    }
    delete parser;
    XMLPlatformUtils::Terminate();
    return 0;
}
