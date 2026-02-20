
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QListWidgetItem;   // forward declaration
class Node;              // forward declaration
class DirectoryNode;     // forward declaration

#include "FileSystem.h"  // sí lo usás directo (fs)

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::MainWindow *ui;

    FileSystem fs;

    std::string currentPath;
    std::vector<std::string> history;
    int historyIndex;

private:
    void buildDemoData();
    void loadDirectory(const std::string& path, bool pushToHistory);
    void refreshButtons();

    std::string normalizePathForGui(const std::string& p) const;
    std::string buildPathFromNode(const Node* n) const;

private slots:
    void onBackClicked();
    void onForwardClicked();
    void onItemDoubleClicked(QListWidgetItem* item);
    void onPathEnterPressed();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif
