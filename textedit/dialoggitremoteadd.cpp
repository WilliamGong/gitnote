#include "dialoggitremoteadd.h"
#include "ui_dialoggitremoteadd.h"

DialogGitRemoteAdd::DialogGitRemoteAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGitRemoteAdd)  {

    ui->setupUi(this);
}

DialogGitRemoteAdd::~DialogGitRemoteAdd() {
    delete ui;
}

void DialogGitRemoteAdd::setName(QString name) {
    ui->lineEditName->setText(name);
}

QString DialogGitRemoteAdd::getName() {
    return ui->lineEditName->text();
}

QString DialogGitRemoteAdd::getUrl() {
    return ui->lineEditUrl->text();
}