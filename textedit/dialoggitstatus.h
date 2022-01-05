#ifndef DIALOGGITSTATUS_H
#define DIALOGGITSTATUS_H

#include <QDialog>
#include <QStandardItemModel>

#include "repo.h"

namespace Ui {
class DialogGitStatus;
}

class DialogGitStatus : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGitStatus(QWidget *parent = nullptr);
    ~DialogGitStatus();
    inline void setRepo(gitnote::Repo *repo) {
        this->repo = repo;
    }
    void update();

private:
    Ui::DialogGitStatus *ui;
    gitnote::Repo *repo = nullptr;
    QStandardItemModel *table;

    void setGitStatus(git_status_list *stat);
};

#endif // DIALOGGITSTATUS_H
