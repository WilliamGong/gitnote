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