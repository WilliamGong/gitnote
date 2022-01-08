#ifndef DIALOGGITREMOTE_H
#define DIALOGGITREMOTE_H

#include <QDialog>
#include <QStringListModel>
#include <QMessageBox>

#include "repo.h"
#include "dialoggitremoteadd.h"
#include "dialoggitremoteedit.h"

namespace Ui {
class DialogGitRemote;
}

class DialogGitRemote : public QDialog {
    Q_OBJECT

public:
    explicit DialogGitRemote(QWidget *parent = nullptr);
    ~DialogGitRemote();

    inline void setRepo(gitnote::Repo *repo) {
        this->repo = repo;
    }

    void update();

    // option dialogs
    void add();
    void remove();
    void edit();

private:
    Ui::DialogGitRemote *ui;

    // git repo
    gitnote::Repo *repo = nullptr;

    // models
    QStringListModel *modelRemotesList;

    // remote list
    QStringList *remotesList = nullptr;

};

#endif // DIALOGGITREMOTE_H
