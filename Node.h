#ifndef NODE_H
#define NODE_H

#include <string>

class DirectoryNode;

class Node {
public:
    enum Type {
        FILE_NODE,
        DIRECTORY_NODE
    };

protected:
    std::string name;
    Type type;
    DirectoryNode* parent;

public:
    Node(const std::string& nameValue, Type typeValue, DirectoryNode* parentValue)
        : name(nameValue), type(typeValue), parent(parentValue) {}

    virtual ~Node() {}

    std::string getName() const {
        return name;
    }

    Type getType() const {
        return type;
    }

    DirectoryNode* getParent() const {
        return parent;
    }

    void setName(const std::string& newName) {
        name = newName;
    }
};

#endif
