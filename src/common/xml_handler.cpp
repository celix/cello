#include "common/xml_handler.h"
#include "common/string_utility.h"

void XmlHandler::RemoveEmptyTextNode(DOMNode* node) {
    DOMNode* fnode = node->getFirstChild();
    while (fnode) {
        DOMNode* next = fnode->getNextSibling();
        if (fnode->getNodeType() == DOMNode::TEXT_NODE) {
            string ss = XMLString::transcode(fnode->getNodeValue());
            ss = StringUtility::Trim(ss);
            // remove empty text node
            if (ss.empty())
                node->removeChild(fnode);
        } else if (fnode->getNodeType() == DOMNode::ELEMENT_NODE) {
            RemoveEmptyTextNode(fnode);
        }
        fnode = next;
    }
}
