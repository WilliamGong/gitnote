#include "dialoggitstatus.h"
#include "ui_dialoggitstatus.h"

DialogGitStatus::DialogGitStatus(QWidget *parent) 
                                : QDialog(parent),
                                ui(new Ui::DialogGitStatus) {

    ui->setupUi(this);

   table = new QStandardItemModel(this);

   table->setColumnCount(2);
   table->setHeaderData(0, Qt::Horizontal, tr("File"));
   table->setHeaderData(1, Qt::Horizontal, tr("Status"));

   //signal and slots
   connect(ui->btnRefresh, &QPushButton::clicked, this, &DialogGitStatus::update);
   connect(ui->btnAdd, &QPushButton::clicked, this, &DialogGitStatus::add);
   connect(ui->btnCommit, &QPushButton::clicked, this,
           &DialogGitStatus::commit);
}

DialogGitStatus::~DialogGitStatus() {
    delete table;
    delete ui;
}

void DialogGitStatus::update() {
    git_status_list *stat;
    this->repo->getStatus(&stat);
    this->setGitStatus(stat);
    ui->tableViewGitStatus->setModel(table);
}

void DialogGitStatus::setGitStatus(git_status_list *stat) {
    if(stat == nullptr) {
        return;
    }
    const git_status_entry *s;
    int cnt = git_status_list_entrycount(stat);
    /**
     * istatus: status of index to HEAD.  
     * wstatus: status of work dir to index. 
     * 
     */
    QString path, fstatus, istatus, wstatus;

    delete table;
    table = new QStandardItemModel(this);
    table->setColumnCount(2);
    table->setHeaderData(0, Qt::Horizontal, tr("File"));
    table->setHeaderData(1, Qt::Horizontal, tr("Status"));

    for(int i = 0; i < cnt; i++) {
        s = git_status_byindex(stat, i);

        if(s->status == GIT_STATUS_CURRENT) {
            continue;
        }

        // git file status 
        if (s->status & GIT_STATUS_INDEX_NEW)
			istatus = 'A';
		if (s->status & GIT_STATUS_INDEX_MODIFIED)
			istatus = 'M';
		if (s->status & GIT_STATUS_INDEX_DELETED)
			istatus = 'D';
		if (s->status & GIT_STATUS_INDEX_RENAMED)
			istatus = 'R';
		if (s->status & GIT_STATUS_INDEX_TYPECHANGE)
			istatus = 'T';

		if (s->status & GIT_STATUS_WT_NEW) {
			if (istatus == ' ')
				istatus = '?';
			wstatus = '?';
		}
		if (s->status & GIT_STATUS_WT_MODIFIED)
			wstatus = 'M';
		if (s->status & GIT_STATUS_WT_DELETED)
			wstatus = 'D';
		if (s->status & GIT_STATUS_WT_RENAMED)
			wstatus = 'R';
		if (s->status & GIT_STATUS_WT_TYPECHANGE)
			wstatus = 'T';

		if (s->status & GIT_STATUS_IGNORED) {
			istatus = '!';
			wstatus = '!';
		}

		if (istatus == '?' && wstatus == '?')
			continue;
        
        fstatus = istatus + wstatus;

        if(s->head_to_index) {
            path = s->head_to_index->new_file.path;
        }
        if(s->index_to_workdir) {
            path = s->index_to_workdir->new_file.path;
        }

        table->setItem(i, 0, new QStandardItem(path));
        table->setItem(i, 1, new QStandardItem(fstatus));
    }
}

void DialogGitStatus::add() {
    this->repo->addAll();
    this->update();
}

void DialogGitStatus::commit() {
    int err;

    this->repo->addAll();
    err = this->repo->commit();

    if(err == 0) {
        QMessageBox::information(this, 
                                    tr("Git commit"), 
                                    tr("Commit succeed. "));
    }

    this->update();
}