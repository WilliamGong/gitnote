#include "dialogauthssl.h"
#include "ui_dialogauthssl.h"

DialogAuthSSL::DialogAuthSSL(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAuthSSL)
{
    ui->setupUi(this);
}

DialogAuthSSL::~DialogAuthSSL()
{
    delete ui;
}

void DialogAuthSSL::setUername(QString username) {
    ui->lineEditUsername->setText(username);
}

void DialogAuthSSL::setPassword(QString password) {
    ui->lineEditPassword->setText(password);
}

QString DialogAuthSSL::getUsername() {
    return ui->lineEditUsername->text();
}

QString DialogAuthSSL::getPassword() {
    return ui->lineEditPassword->text();
}