#ifndef SRC_COMMON_XML_HANDLER_H
#define SRC_COMMON_XML_HANDLER_H

#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/util/XMLString.hpp>

using xercesc::DOMNode;
using xercesc::XMLString;

class XmlHandler {
public:
    static void RemoveEmptyTextNode(DOMNode* node);
};

#endif
