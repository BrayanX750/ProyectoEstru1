#include "FileSystem.h"
#include <iostream>

FileSystem::FileSystem() {
    // root is mandatory: "/"
    root = new DirectoryNode("/", 0);
}

FileSystem::~FileSystem() {
    delete root;
    root = 0;
}

DirectoryNode* FileSystem::getRoot() const {
    return root;
}

bool FileSystem::isValidName(const std::string& name, std::string& error) const {
    if (name.size() == 0) {
        error = "No se permite un nombre vacío.";
        return false;
    }

    if (name.find('/') != std::string::npos) {
        error = "El nombre no puede contener '/'.";
        return false;
    }

    if (name == "." || name == "..") {
        error = "Los nombres '.' y '..' no están permitidos.";
        return false;
    }

    return true;
}

std::string FileSystem::normalizePath(const std::string& path) const {
    if (path.size() == 0) return "/";

    std::string p = path;

    if (p[0] != '/') {
        p = "/" + p;
    }

    if (p.size() > 1) {
        if (p[p.size() - 1] == '/') {
            p.erase(p.size() - 1, 1);
        }
    }

    return p;
}

std::vector<std::string> FileSystem::splitPath(const std::string& path) const {
    std::vector<std::string> parts;
    std::string current;
    int i;

    for (i = 0; i < (int)path.size(); i++) {
        char c = path[i];

        if (c == '/') {
            if (current.size() > 0) {
                parts.push_back(current);
                current = "";
            }
        } else {
            current.push_back(c);
        }
    }

    if (current.size() > 0) {
        parts.push_back(current);
    }

    return parts;
}

DirectoryNode* FileSystem::findDirectory(const std::string& path, std::string& error) const {
    std::string p = normalizePath(path);

    if (p == "/") {
        return root;
    }

    std::vector<std::string> parts = splitPath(p);

    DirectoryNode* currentDir = root;
    int i;

    for (i = 0; i < (int)parts.size(); i++) {
        std::string partName = parts[i];

        Node* child = currentDir->findChildByName(partName);
        if (child == 0) {
            error = "La ruta no existe: falta '" + partName + "'";
            return 0;
        }

        if (child->getType() != Node::DIRECTORY_NODE) {
            error = "'" + partName + "' no es un directorio.";
            return 0;
        }

        currentDir = (DirectoryNode*)child;
    }

    return currentDir;
}

bool FileSystem::createDirectory(const std::string& parentPath, const std::string& name, std::string& error) {
    error = "";

    std::string nameError;
    if (!isValidName(name, nameError)) {
        error = nameError;
        return false;
    }

    DirectoryNode* parentDir = findDirectory(parentPath, error);
    if (parentDir == 0) {
        return false;
    }

    if (parentDir->existsNameHere(name)) {
        error = "Ya existe '" + name + "' dentro de " + normalizePath(parentPath);
        return false;
    }

    DirectoryNode* newDir = new DirectoryNode(name, parentDir);
    parentDir->addChild(newDir);
    return true;
}

bool FileSystem::createFile(const std::string& parentPath, const std::string& name, std::string& error) {
    error = "";

    std::string nameError;
    if (!isValidName(name, nameError)) {
        error = nameError;
        return false;
    }

    DirectoryNode* parentDir = findDirectory(parentPath, error);
    if (parentDir == 0) {
        return false;
    }

    if (parentDir->existsNameHere(name)) {
        error = "Ya existe '" + name + "' dentro de " + normalizePath(parentPath);
        return false;
    }

    FileNode* newFile = new FileNode(name, parentDir);
    parentDir->addChild(newFile);
    return true;
}

static void printRecursive(const DirectoryNode* dir, int level) {
    int i;

    for (i = 0; i < dir->getChildrenCount(); i++) {
        Node* n = dir->getChildAt(i);

        int j;
        for (j = 0; j < level; j++) {
            std::cout << "  ";
        }

        if (n->getType() == Node::DIRECTORY_NODE) {
            std::cout << "[D] " << n->getName() << "\n";
            printRecursive((DirectoryNode*)n, level + 1);
        } else {
            std::cout << "[A] " << n->getName() << "\n";
        }
    }
}

void FileSystem::printTree() const {
    std::cout << "/\n";
    printRecursive(root, 1);
}
