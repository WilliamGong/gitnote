#ifndef DIALOGGITREMOTEEDIT_H
#define DIALOGGITREMOTEEDIT_H

#include <QDialog>

namespace Ui {
class DialogGitRemoteEdit;
}

class DialogGitRemoteEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGitRemoteEdit(QWidget *parent = nullptr);
    ~DialogGitRemoteEdit();

    void setName(QString name);
    void setFetchUrl(QString url);
    void setPushUrl(QString url);
    QString getName();
    QString getFetchUrl();
    QString getPushUrl();

private:
    Ui::DialogGitRemoteEdit *ui;
};

#endif // DIALOGGITREMOTEEDIT_H
