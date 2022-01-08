#include "dialoggitremote.h"
#include "ui_dialoggitremote.h"

DialogGitRemote::DialogGitRemote(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGitRemote) {

    ui->setupUi(this);

    this->modelRemotesList = new QStringListModel(this);

    ui->listViewRemote->setModel(this->modelRemotesList);

    // connect signal and slots
    connect(ui->btnRefresh, &QPushButton::clicked, this, &DialogGitRemote::update);
    connect(ui->btnRemoteAdd, &QPushButton::clicked, this, &DialogGitRemote::add);
    connect(ui->btnRemove, &QPushButton::clicked, this, &DialogGitRemote::remove);
    connect(ui->btnEdit, &QPushButton::clicked, this, &DialogGitRemote::edit);
}

DialogGitRemote::~DialogGitRemote() {
    delete modelRemotesList;
    delete remotesList;
    delete ui;
}

void DialogGitRemote::update() {
    std::vector<std::string> remotesList = this->repo->getRemotesList();

    delete this->remotesList;
    this->remotesList = new QStringList();

    for(std::vector<std::string>::iterator i = remotesList.begin(); 
        i != remotesList.end(); 
        i++) {
            std::string stlStr = *i;
            QString tmp =  stlStr.c_str();
            *this->remotesList << tmp;
        }

    this->modelRemotesList->setStringList(*this->remotesList);
}

void DialogGitRemote::add() {
    QString addTitle = tr("Add");
    DialogGitRemoteAdd *dlg = new DialogGitRemoteAdd(this);
    QString name = "origin"; // default name
    QString url;

    dlg->setName(name);

    int exit = dlg->exec();
    if(exit == QDialog::Accepted) {
        name = dlg->getName().simplified();
        url = dlg->getUrl().simplified();
    }else if(exit == QDialog::Rejected) {
        delete dlg;
        return;
    }

    if(name.isEmpty()) {
        QMessageBox::critical(this, 
                                addTitle, 
                                tr("Name is empty. "));

        delete dlg;
        return;
    }
    if(url.isEmpty()) {
        QMessageBox::critical(this, 
                                addTitle, 
                                tr("URL is empty. "));
        
        delete dlg;
        return;
    }

    int err = this->repo->remoteAdd(name.toStdString(), url.toStdString());

    if(err != 0) {
        QMessageBox::critical(this, 
                                addTitle,
                                tr("Remote add failed. "));

    }

    this->update();
    delete dlg;
    return;
}

void DialogGitRemote::remove() {
    QModelIndex index;
    index = ui->listViewRemote->currentIndex();
    QString current = modelRemotesList->data(index).toString();

    if(!index.isValid()) {
        return;
    }

    int err = repo->remoteRemove(current.toStdString());
    if(err) {
        QMessageBox::critical(this, 
                                tr("Remove"), 
                                tr("Remove failed. "));
    }

    this->update();
}

void DialogGitRemote::edit() {
    QModelIndex index;
    index = ui->listViewRemote->currentIndex();
    QString current = modelRemotesList->data(index).toString();

    if(!index.isValid()) {
        return;
    }

    DialogGitRemoteEdit *dlg = new DialogGitRemoteEdit(this);
    QString editTitle = tr("Edit");

    gitnote::remoteInfo info = this->repo->getRemoteInfo(current.toStdString());
    QString name = info.name.c_str();
    QString fetchUrl = info.fetchUrl.c_str();
    QString pushUrl;
    if(info.pushUrl.empty()) {
        pushUrl = info.fetchUrl.c_str();
    }else {
        pushUrl = info.pushUrl.c_str();
    }

    dlg->setName(name);
    dlg->setFetchUrl(fetchUrl);
    dlg->setPushUrl(pushUrl);

    int exit = dlg->exec();
    if(exit == QDialog::Accepted) {
        name = dlg->getName().simplified();
        fetchUrl = dlg->getFetchUrl().simplified();
        pushUrl = dlg->getPushUrl().simplified();
    }else if(exit == QDialog::Rejected) {
        delete dlg;
        return;
    }

    int err;
    std::vector<char *> problems;
    bool isNameChanged = false, isFetchUrlChanged = false;

    if(info.name != name.toStdString()) {
        if(name.isEmpty()) {
            QMessageBox::critical(this, 
                                    editTitle, 
                                    tr("Name is empty. \nName will not change. "));
        }else {
            err = this->repo->rename(problems, info.name, name.toStdString());
            if(err) {
                QMessageBox::critical(this, 
                                        editTitle, 
                                        tr("Rename failed with new name: \n%1\nName will not change. ").arg(problems[0]));

            }else {
                isNameChanged = true;
            }
        }
    }else {
        if(info.fetchUrl != fetchUrl.toStdString()) {
            if(fetchUrl.isEmpty()) {
                QMessageBox::critical(this, 
                                        editTitle, 
                                        tr("Fetch URL is empty. \nFetch URL will not change. "));
            }else {
                if(isNameChanged) {
                    err = this->repo->setFetchUrl(name.toStdString(), fetchUrl.toStdString());
                }else {
                    err = this->repo->setFetchUrl(info.name, fetchUrl.toStdString());
                }
                if(err) {
                    QMessageBox::critical(this, 
                                            editTitle, 
                                            tr("Fetch URL change failed. \nFetch URL will not change. "));
                }else {
                    isFetchUrlChanged = true;
                }
            }
        }
        if(info.pushUrl != pushUrl.toStdString()) {
            if(isNameChanged) {
                if(isFetchUrlChanged) {
                    if(pushUrl.isEmpty()) {
                        err = this->repo->setPushUrl(name.toStdString(), fetchUrl.toStdString());
                    }else {
                        err = this->repo->setPushUrl(name.toStdString(), pushUrl.toStdString());
                    }
                }else {
                    if(pushUrl.isEmpty()) {
                        err = this->repo->setPushUrl(name.toStdString(), info.fetchUrl);
                    }else {
                        err = this->repo->setPushUrl(name.toStdString(), pushUrl.toStdString());
                    }
                }
            }else {
                if(isFetchUrlChanged) {
                    if(pushUrl.isEmpty()) {
                        err = this->repo->setPushUrl(info.name, fetchUrl.toStdString());
                    }else {
                        err = this->repo->setPushUrl(info.name, pushUrl.toStdString());
                    }
                }else {
                    if(pushUrl.isEmpty()) {
                        err = this->repo->setPushUrl(info.name, info.fetchUrl);
                    }else {
                        err = this->repo->setPushUrl(info.name, pushUrl.toStdString());
                    }
                }
            }
            if(err) {
                QMessageBox::critical(this, 
                                        editTitle, 
                                        tr("Push URL is failed. \nPush URL will not change. "));
            }
        }
    }

    this->update();
    delete dlg;
    return;
}