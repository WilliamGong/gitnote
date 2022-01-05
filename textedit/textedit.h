#pragma once

#include <QtWidgets/QMainWindow>
#include <QFileInfo>
#include <QFileSystemModel>

#include "ui_textedit.h"
#include "repo.h"
#include "dialoggitstatus.h"

// forward declaration
class QAction;

class Textedit : public QMainWindow {
  Q_OBJECT

 public:
  Textedit(QWidget *parent = Q_NULLPTR);
  ~Textedit();

  // File operations
  void openFile();
  void openFile(const QModelIndex &index);
  void openFile(QString path);
  void openDir();
  void saveFile();
  void newFile();

  // git 
  void gitInit();
  void startDialogGitStatus();

 private:
  Ui::TexteditClass ui;

  // File System
  QString path;
  QString cwd;
  QFileInfo info;
  QFileSystemModel modelDir;

  //git
  gitnote::Repo repo;
  DialogGitStatus *gitStatus;

  // check if file path is default
  bool isFileDefault;
};
