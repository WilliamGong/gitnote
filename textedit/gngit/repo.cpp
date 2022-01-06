#include <time.h>

#include "repo.h"

using namespace gitnote;

Repo::Repo() {
    int errLibgitInit = git_libgit2_init();
}

Repo::~Repo() {
    if(this->repo != nullptr) {
        git_repository_free(this->repo);
    }
    if (this->index != nullptr) {
      git_index_free(this->index);
    }
    git_libgit2_shutdown();
}

int Repo::init(std::string path) {
    int err = -1;
    if(this->repo == nullptr) {
        err = git_repository_init(&this->repo, path.c_str(), 0);
        this->cwd = path;
    }
    if (err == 0) {
      git_repository_index(&this->index, this->repo);
    }

    return err;
}

int Repo::open(std::string path) {
    int err = git_repository_open(&this->repo, path.c_str());
    if(err == 0) {
        this->cwd = path;
      git_repository_index(&this->index, this->repo);
    }

    return err;
}

void Repo::statusUpdate() {
    git_status_options opts;
    git_status_options_init(&opts, GIT_STATUS_OPTIONS_VERSION);

    // scan index and workdir
    opts.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
    // flags
    opts.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED |
                    GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS |
                    GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX | 
                    GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;
    
    git_status_list_new(&this->stat, this->repo, &opts);
}

void Repo::addAll() { 
    index_options options = {0, 0, this->repo, INDEX_ADD, 0};
    git_strarray path = {0};
    path.count = 1;
    path.strings = (char **) calloc(path.count, sizeof(char *));

    path.strings[0] = ".";
    git_index_add_all(this->index, &path, 0, NULL, &options);

    git_index_write(this->index);
}

int Repo::commit() {
    int err;

    std::string comment;

    git_oid commitOid, treeOid;
    git_tree *tree;
    git_object *parent = nullptr;
    git_reference *ref = nullptr;
    git_signature *signature;

    git_revparse_ext(&parent, &ref, this->repo, "HEAD");
    git_index_write_tree(&treeOid, this->index);
    git_index_write(this->index);
    git_tree_lookup(&tree, this->repo, &treeOid);
    git_signature_default(&signature, this->repo);

    time_t currentTime;
    time(&currentTime);
    comment = ctime(&currentTime);

    err = git_commit_create_v(&commitOid, 
                                this->repo, 
                                "HEAD", 
                                signature, 
                                signature, 
                                "utf-8", 
                                comment.c_str(), 
                                tree, 
                                parent ? 1 : 0, 
                                parent);

    git_index_write(this->index);
    git_signature_free(signature);
	git_tree_free(tree);

    return err;
}