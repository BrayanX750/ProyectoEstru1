#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "DirectoryNode.h"
#include "Node.h"

#include <QListWidgetItem>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentPath("/"),
    historyIndex(-1) {

    ui->setupUi(this);

    connect(ui->btnBack, &QPushButton::clicked, this, &MainWindow::onBackClicked);
    connect(ui->btnForward, &QPushButton::clicked, this, &MainWindow::onForwardClicked);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::onItemDoubleClicked);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::onPathEnterPressed);

    buildDemoData();

    history.push_back("/");
    historyIndex = 0;

    loadDirectory("/", false);
    refreshButtons();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::buildDemoData() {
    std::string err;

    fs.createDirectory("/", "Docs", err);
    fs.createDirectory("/", "Images", err);
    fs.createDirectory("/Docs", "Projects", err);

    fs.createFile("/Docs", "readme.txt", err);
    fs.createFile("/Docs/Projects", "plan.pdf", err);
}

std::string MainWindow::normalizePathForGui(const std::string& p) const {
    if (p.empty()) return "/";

    std::string out = p;

    if (out[0] != '/') out = "/" + out;

    if (out.size() > 1 && out.back() == '/') {
        out.pop_back();
    }

    return out;
}

void MainWindow::refreshButtons() {
    bool canBack = (historyIndex > 0);
    bool canForward = (historyIndex >= 0 && historyIndex < (int)history.size() - 1);

    ui->btnBack->setEnabled(canBack);
    ui->btnForward->setEnabled(canForward);
}

std::string MainWindow::buildPathFromNode(const Node* n) const {
    if (n == nullptr) return "/";

    std::string result = "";
    const Node* current = n;

    while (current != nullptr) {
        std::string part = current->getName();

        if (part == "/") {
            if (result.empty()) result = "/";
            break;
        }

        if (result.empty()) result = "/" + part;
        else result = "/" + part + result;

        current = current->getParent();
    }

    return result;
}

void MainWindow::loadDirectory(const std::string& path, bool pushToHistory) {
    std::string err;
    std::string p = normalizePathForGui(path);

    DirectoryNode* dir = fs.findDirectory(p, err);
    if (dir == nullptr) {
        QMessageBox::warning(this, "Ruta inválida", QString::fromStdString(err));
        ui->statusbar->showMessage("No se pudo abrir la ruta.", 2500);
        return;
    }

    currentPath = p;
    ui->lineEdit->setText(QString::fromStdString(currentPath));

    if (pushToHistory) {
        if (historyIndex < (int)history.size() - 1) {
            history.erase(history.begin() + historyIndex + 1, history.end());
        }
        history.push_back(currentPath);
        historyIndex = (int)history.size() - 1;
    }

    ui->listWidget->clear();

    for (int i = 0; i < dir->getChildrenCount(); i++) {
        Node* child = dir->getChildAt(i);
        if (!child) continue;

        bool isDir = (child->getType() == Node::DIRECTORY_NODE);

        std::string label;
        if (isDir) label = "[D] " + child->getName();
        else label = "[A] " + child->getName();

        QListWidgetItem* item =
            new QListWidgetItem(QString::fromStdString(label));

        item->setData(Qt::UserRole,
                      QString::fromStdString(child->getName()));
        item->setData(Qt::UserRole + 1,
                      isDir ? 1 : 0);

        ui->listWidget->addItem(item);
    }

    ui->statusbar->showMessage(
        "Directorio actual: " + QString::fromStdString(currentPath), 2000);

    refreshButtons();
}

void MainWindow::onItemDoubleClicked(QListWidgetItem* item) {
    if (!item) return;

    QString nameQ = item->data(Qt::UserRole).toString();
    int isDir = item->data(Qt::UserRole + 1).toInt();

    std::string name = nameQ.toStdString();

    std::string nextPath;
    if (currentPath == "/") nextPath = "/" + name;
    else nextPath = currentPath + "/" + name;

    if (isDir == 0) {
        ui->lineEdit->setText(QString::fromStdString(nextPath));
        ui->statusbar->showMessage("Archivo seleccionado", 2000);
        return;
    }

    loadDirectory(nextPath, true);
}

void MainWindow::onBackClicked() {
    if (historyIndex <= 0) return;

    historyIndex--;
    loadDirectory(history[historyIndex], false);
    ui->statusbar->showMessage("Atrás", 1200);
    refreshButtons();
}

void MainWindow::onForwardClicked() {
    if (historyIndex >= (int)history.size() - 1) return;

    historyIndex++;
    loadDirectory(history[historyIndex], false);
    ui->statusbar->showMessage("Adelante", 1200);
    refreshButtons();
}

void MainWindow::onPathEnterPressed() {
    QString text = ui->lineEdit->text().trimmed();
    if (text.isEmpty()) {
        ui->lineEdit->setText(QString::fromStdString(currentPath));
        return;
    }

    loadDirectory(text.toStdString(), true);
}
