#ifndef DIRECTORYNODE_H
#define DIRECTORYNODE_H

#include "Node.h"
#include <vector>

class DirectoryNode : public Node {
private:
    std::vector<Node*> children;

public:
    DirectoryNode(const std::string& nameValue, DirectoryNode* parentValue)
        : Node(nameValue, Node::DIRECTORY_NODE, parentValue) {}

    virtual ~DirectoryNode() {
        int i;
        for (i = 0; i < (int)children.size(); i++) {
            delete children[i];
            children[i] = 0;
        }
        children.clear();
    }

    int getChildrenCount() const {
        return (int)children.size();
    }

    Node* getChildAt(int index) const {
        if (index < 0 || index >= (int)children.size()) {
            return 0;
        }
        return children[index];
    }

    bool existsNameHere(const std::string& candidate) const {
        int i;
        for (i = 0; i < (int)children.size(); i++) {
            if (children[i]->getName() == candidate) {
                return true;
            }
        }
        return false;
    }

    Node* findChildByName(const std::string& candidate) const {
        int i;
        for (i = 0; i < (int)children.size(); i++) {
            if (children[i]->getName() == candidate) {
                return children[i];
            }
        }
        return 0;
    }

    void addChild(Node* newChild) {
        children.push_back(newChild);
    }
};

#endif
