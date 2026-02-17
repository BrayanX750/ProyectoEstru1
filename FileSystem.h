#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "DirectoryNode.h"
#include "FileNode.h"
#include <string>
#include <vector>

class FileSystem {
private:
    DirectoryNode* root;

    bool isValidName(const std::string& name, std::string& error) const;
    std::string normalizePath(const std::string& path) const;
    std::vector<std::string> splitPath(const std::string& path) const;

public:
    FileSystem();
    ~FileSystem();

    DirectoryNode* getRoot() const;

    DirectoryNode* findDirectory(const std::string& path, std::string& error) const;

    bool createDirectory(const std::string& parentPath, const std::string& name, std::string& error);
    bool createFile(const std::string& parentPath, const std::string& name, std::string& error);

    void printTree() const;
};

#endif
