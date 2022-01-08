#ifndef DIALOGGITREMOTEADD_H
#define DIALOGGITREMOTEADD_H

#include <QDialog>

namespace Ui {
class DialogGitRemoteAdd;
}

class DialogGitRemoteAdd : public QDialog {
    Q_OBJECT

public:
    explicit DialogGitRemoteAdd(QWidget *parent = nullptr);
    ~DialogGitRemoteAdd();
    void setName(QString name);
    QString getName();
    QString getUrl();

private:
    Ui::DialogGitRemoteAdd *ui;
};

#endif // DIALOGGITREMOTEADD_H
