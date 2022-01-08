#include <QAction>
#include <QtGui/QKeySequence>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include "textedit.h"

Textedit::Textedit(QWidget *parent) : QMainWindow(parent) {
  gitStatus = new DialogGitStatus(this);
  gitRemote = new DialogGitRemote(this);
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
  connect(ui.actionGitRemote, &QAction::triggered, this, &Textedit::startDialogGitRemote);
  connect(ui.actionGitPush, &QAction::triggered, this, &Textedit::gitPush);
  connect(ui.actionGitFetch, &QAction::triggered, this, &Textedit::gitFetch);
  connect(ui.actionGitPull, &QAction::triggered, this, &Textedit::gitPull);
  connect(ui.actionAbout, &QAction::triggered, this, &Textedit::about);

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
  delete gitStatus;
  delete gitRemote;
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
    QMessageBox::critical(this, 
                         tr("Git Status"),
                         tr("No Git repository has opened. "));
  } else {
    gitStatus->setRepo(&this->repo);
    gitStatus->update();
    gitStatus->show();
  }
 }

 void Textedit::startDialogGitRemote() {
   if(!isGitOpened) {
     QMessageBox::critical(this, 
                            tr("Git Remote"), 
                            tr("No Git repository has opened. "));
   }else {
     gitRemote->setRepo(&this->repo);
     gitRemote->update();
     gitRemote->show();
   }
 }

 void Textedit::gitPush() {
   QString pushTitle = tr("Push");
   if(!isGitOpened) {
    QMessageBox::critical(this, 
                            pushTitle, 
                            tr("Git repository has not been opened. "));
    return;
   }
  int err;
  DialogAuthSSL *dlg = new DialogAuthSSL(this);
  gitnote::authSSLInfo auth;

  int exit = dlg->exec();
  if(exit == QDialog::Accepted) {
    auth.username = dlg->getUsername().toStdString();
    auth.password = dlg->getPassword().toStdString();
  }

   err = this->repo.push(auth);

   switch(err) {
    case 0:
      QMessageBox::information(this, 
                                pushTitle, 
                                tr("Push succeed. "));
      break;
    case 1:
      QMessageBox::critical(this, 
                            pushTitle, 
                            tr("Finding remote repository failed. \nPlease check remote settings. "));
      break;
    case 2:
      QMessageBox::critical(this, 
                            pushTitle, 
                            tr("Push options init failed. "));
      break;
    case 3:
      QMessageBox::critical(this, 
                            pushTitle, 
                            tr("Push failed. "));
      break;
   }
 }

 void Textedit::gitFetch() {
   int err;
   QString fetchTitle = tr("Fetch");
   if(!isGitOpened) {
    QMessageBox::critical(this, 
                            fetchTitle, 
                            tr("Git repository has not been opened. "));
    return;
   }
   err = this->repo.fetch();
   switch(err) {
    case 0:
      QMessageBox::information(this, 
                                fetchTitle, 
                                tr("Fetch succeed. "));
      break;
    case 1: 
      QMessageBox::warning(this, 
                            fetchTitle, 
                            tr("Remote look up failed. \nCheck remote settings. "));
      break;
    case 2: 
      QMessageBox::critical(this, 
                            fetchTitle, 
                            tr("Fetch failed. "));
      break;
   }
 }

 void Textedit::gitPull() {
   int err;
   QString pullTitle = tr("Pull");

   if(!isGitOpened) {
    QMessageBox::critical(this, 
                            pullTitle, 
                            tr("Git repository has not been opened. "));
    return;
   }

   err = this->repo.fetch();
   if(err) {
    QMessageBox::critical(this, 
                            pullTitle, 
                            tr("Fetch failed. "));
    return;
   }

   err = this->repo.merge("origin/master");
   switch(err) {
    case 0:
      QMessageBox::information(this, 
                              pullTitle, 
                              tr("Pull completed. "));
    break;
    case 1: 
      QMessageBox::critical(this, 
                                pullTitle, 
                                tr("Unexpected state. "));
      break;
    case 2: 
      QMessageBox::critical(this, 
                                pullTitle, 
                                tr("Resolving heads failed. "));
      break;
    case 3: 
      QMessageBox::critical(this, 
                                pullTitle, 
                                tr("Merge analysis failed. "));
      break;
    case 4: 
      QMessageBox::critical(this, 
                                pullTitle, 
                                tr("Fastforward failed. "));
      break;
    case 5: 
      QMessageBox::critical(this, 
                                pullTitle, 
                                tr("Fast-forward is preferred, but only a merge is possible. "));
      break;
    case 6: 
      QMessageBox::critical(this, 
                                pullTitle, 
                                tr("Merge failed. "));
      break;
    case 100: 
      QMessageBox::information(this, 
                                pullTitle, 
                                tr("Already up-to-date"));
      break;
   }
 }

 void Textedit::about() {
   QMessageBox::about(this, 
                        tr("About"), 
                        tr("GitNote \nVersion: "));
 }