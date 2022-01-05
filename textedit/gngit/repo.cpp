#include "repo.h"

using namespace gitnote;

Repo::Repo() {
    int errLibgitInit = git_libgit2_init();
}

Repo::~Repo() {
    if(this->repo != nullptr) {
        git_repository_free(this->repo);
    }
    git_libgit2_shutdown();
}

int Repo::init(std::string path) {
    int err;
    if(this->repo == nullptr) {
        err = git_repository_init(&this->repo, path.c_str(), 0);
        this->cwd = path;
    }

    return err;
}

int Repo::open(std::string path) {
    int err = git_repository_open(&this->repo, path.c_str());
    if(err == 0) {
        this->cwd = path;
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
