#include "dialoggitremoteedit.h"
#include "ui_dialoggitremoteedit.h"

DialogGitRemoteEdit::DialogGitRemoteEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGitRemoteEdit)
{
    ui->setupUi(this);
}

DialogGitRemoteEdit::~DialogGitRemoteEdit()
{
    delete ui;
}

void DialogGitRemoteEdit::setName(QString name) {
    ui->lineEditName->setText(name);
}

void DialogGitRemoteEdit::setFetchUrl(QString url) {
    ui->lineEditFetch->setText(url);
}

void DialogGitRemoteEdit::setPushUrl(QString url) {
    ui->lineEditPush->setText(url);
}

QString DialogGitRemoteEdit::getName() {
    return ui->lineEditName->text();
}

QString DialogGitRemoteEdit::getFetchUrl() {
    return ui->lineEditFetch->text();
}

QString DialogGitRemoteEdit::getPushUrl() {
    return ui->lineEditPush->text();
}