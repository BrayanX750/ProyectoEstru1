#ifndef FILENODE_H
#define FILENODE_H

#include "Node.h"

class FileNode : public Node {
public:
    FileNode(const std::string& nameValue, DirectoryNode* parentValue)
        : Node(nameValue, Node::FILE_NODE, parentValue) {}
};

#endif
