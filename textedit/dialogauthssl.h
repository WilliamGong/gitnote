#ifndef DIALOGAUTHSSL_H
#define DIALOGAUTHSSL_H

#include <QDialog>

namespace Ui {
class DialogAuthSSL;
}

class DialogAuthSSL : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAuthSSL(QWidget *parent = nullptr);
    ~DialogAuthSSL();
    void setUername(QString username);
    void setPassword(QString password);
    QString getUsername();
    QString getPassword();

private:
    Ui::DialogAuthSSL *ui;
};

#endif // DIALOGAUTHSSL_H
