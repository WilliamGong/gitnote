#include <QAction>
#include <QtGui/QKeySequence>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include "textedit.h"

Textedit::Textedit(QWidget *parent) : QMainWindow(parent) {
  gitStatus = new DialogGitStatus(this);
  ui.setupUi(this);

  // connect signal and slot

  // File
  connect(ui.actionOpenFile, &QAction::triggered, this,
          static_cast<void (Textedit::*)()>(&Textedit::openFile));
  connect(ui.actionSaveFile, &QAction::triggered, this, &Textedit::saveFile);
  connect(ui.actionNewFile, &QAction::triggered, this, &Textedit::newFile);
  connect(ui.actionOpenDir, &QAction::triggered, this, &Textedit::openDir);
  connect(ui.treeViewDir, &QTreeView::doubleClicked, this,
          static_cast<void (Textedit::*)(const QModelIndex &)>(&Textedit::openFile));

  // git
  connect(ui.actionGitInit, &QAction::triggered, this, &Textedit::gitInit);
  connect(ui.actionGitStatus, &QAction::triggered, this, &Textedit::startDialogGitStatus);

  // var init
  this->path = "Untitled.txt";
  this->info = QFileInfo(path);
  this->isFileDefault = true;
  this->cwd = QDir::homePath();
  // Dir tree
  modelDir.setRootPath(this->cwd);
  ui.treeViewDir->setModel(&modelDir);
  ui.treeViewDir->setRootIndex(modelDir.index(this->cwd));
  ui.treeViewDir->setColumnHidden(1, true);
  ui.treeViewDir->setColumnHidden(2, true);
  ui.treeViewDir->setColumnHidden(3, true);
  

  // set window
  this->setWindowTitle(this->info.fileName());
 }

Textedit::~Textedit() {
  delete[] gitStatus;
}

void Textedit::openFile() { 
	QString path =
      QFileDialog::getOpenFileName(this, tr("Open file"), ".", tr("Text files(*.txt)"));

    this->openFile(path);
}

void Textedit::openFile(const QModelIndex &index) {
  if (!index.isValid()) {
    QMessageBox::warning(this, tr("Open file"), tr("Error: invalid path. "));
    return;
  }
  if (modelDir.fileInfo(index).isFile()) {
    this->openFile(modelDir.fileInfo(index).absoluteFilePath());
  }
}

void Textedit::openFile(QString path) {
  if (!path.isEmpty()) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      // Warning: cannot open file
      QMessageBox::warning(this, tr("Read file"),
                           tr("Cannot open file: \n%1").arg(path));
      return;
    }

    this->path = path;
    this->info = QFileInfo(path);
    this->setWindowTitle(this->info.fileName());
    // Dir tree
    modelDir.setRootPath(info.dir().absolutePath());
    ui.treeViewDir->setRootIndex(modelDir.index(info.dir().absolutePath()));
    this->cwd = info.dir().absolutePath();

    if (!isGitOpened) {
      int err = this->repo.open(info.dir().absolutePath().toStdString());
      if (!err) {
        QMessageBox::information(this, tr("Git"),
                               tr("Git repository open sucessfully. "));
        isGitOpened = true;
      } else {
        isGitOpened = false;
      }
    }

    this->isFileDefault = false;
    QTextStream in(&file);
    ui.textEdit->setText(in.readAll());
    file.close();
  } else {
    QMessageBox::warning(this, tr("Path"), tr("You didn't select any file. "));
  }
}

void Textedit::saveFile() {
  if (this->isFileDefault) {
    QString path = QFileDialog::getSaveFileName(this, tr("Select file"), ".",
                                                tr("Text files(*.txt)"));

    if (!path.isEmpty()) {
      QFile file(path);
      file.open(QIODevice::WriteOnly | QIODevice::Text);
      QTextStream out(&file);
      out << ui.textEdit->toPlainText();
      file.close();
    } else {
      QMessageBox::warning(this, tr("Path"),
                           tr("You didn't select any file. "));
    }
  } else {
    QFile file(this->path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << ui.textEdit->toPlainText();
    file.close();
  }
}
  

void Textedit::newFile() {
  QString path = QFileDialog::getSaveFileName(this, tr("Create new file"), ".",
                                              tr("Text files(*.txt)"));
  if (!path.isEmpty()) {
    this->path = path;
    this->info = QFileInfo(path);
    this->setWindowTitle(this->info.fileName());
    this->isFileDefault = false;

    // empty textEdit
    ui.textEdit->setText(NULL);
  }
}

void Textedit::openDir() {
  QString dir =
      QFileDialog::getExistingDirectory(this, tr("Open Directory"), ".");

  if (!dir.isEmpty()) {
    this->cwd = dir;
    modelDir.setRootPath(dir);
    ui.treeViewDir->setRootIndex(modelDir.index(dir));

    isGitOpened = false;
    if(!isGitOpened) {
      int err = this->repo.open(dir.toStdString());
      if(!err) {
        QMessageBox::information(this, 
                              tr("Git"), 
                              tr("Git repository open sucessfully. "));
        isGitOpened = true;
      } else {
        isGitOpened = false;
      } 
    }
  } else {
    QMessageBox::warning(this, 
                        tr("Open directory"),
                        tr("Can not open this directory: \n%1").arg(dir));
  }
}

void Textedit::gitInit() {
  if(!isGitOpened) {
    int err = this->repo.init(this->cwd.toStdString());
    if(!err) {
      QMessageBox::information(this, 
                              tr("Init succeed. "), 
                              tr("Git repository initialization succeed. "));
      isGitOpened = true;
    }
  }else {
    QMessageBox::critical(this, 
                          tr("Git Init"), 
                          tr("A Git repository has been opened, do not re init. "));
  }
}

void Textedit::startDialogGitStatus() {
  if (!isGitOpened) {
    QMessageBox::warning(this, 
                         tr("Git Status"),
                         tr("No Git repository has opened. "));
  } else {
    gitStatus->setRepo(&this->repo);
    gitStatus->update();
    gitStatus->show();
  }
 }