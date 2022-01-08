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

void Repo::getStatus(git_status_list **stat) {
    git_status_options opts;
    git_status_options_init(&opts, GIT_STATUS_OPTIONS_VERSION);

    // scan index and workdir
    opts.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
    // flags
    opts.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED |
                    GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS |
                    GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX | 
                    GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;
    
    git_status_list_new(stat, this->repo, &opts);
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

std::vector<std::string> Repo::getRemotesList() {
    std::vector<std::string> ret;
    git_strarray remotes = {0};

    git_remote_list(&remotes, this->repo);

    for(int i = 0; i < (int) remotes.count; i++) {
        std::string name = remotes.strings[i];
        ret.push_back(name);
    }

    git_strarray_dispose(&remotes);
    return ret;
}

int Repo::remoteAdd(std::string name, std::string url) {
    int err;
    git_remote *remote = {0};

    err = git_remote_create(&remote, this->repo, name.c_str(), url.c_str());

    return err;
}

int Repo::remoteRemove(std::string name) {
    int err = git_remote_delete(this->repo, name.c_str());
    return err;
}

remoteInfo Repo::getRemoteInfo(std::string name) {
    remoteInfo ret;
    ret.name = name;
    git_remote *remote;

    git_remote_lookup(&remote, this->repo, name.c_str());

    ret.fetchUrl = git_remote_url(remote);
    const char * tmp = git_remote_pushurl(remote);
    if(tmp != nullptr) {
        ret.pushUrl = tmp;
    }

    git_remote_free(remote);
    return ret;
}

int Repo::rename(std::vector<char *> &problems, std::string oldName, std::string newName) {
    int err;
    git_strarray originalProblems = {0};

    err = git_remote_rename(&originalProblems, this->repo, oldName.c_str(), newName.c_str());

    if(!err) {
        return 0;
    }

    for(int i = 0; i < (int) originalProblems.count; i++) {
        problems.push_back(originalProblems.strings[i]);
    }

    git_strarray_dispose(&originalProblems);
    return err;
}

int Repo::setFetchUrl(std::string name, std::string url) {
    int err;

    err = git_remote_set_url(this->repo, name.c_str(), url.c_str());

    return err;
}

int Repo::setPushUrl(std::string name, std::string url) {
    int err;

    err = git_remote_set_pushurl(this->repo, name.c_str(), url.c_str());

    return err;
}

int Repo::push(authSSLInfo auth) {
    /**
     * exit code:
     * 0 - normal exit
     * 1 - remote lookup failed 
     * 2 - push options init failed
     * 3 - push failed
     */
    // could only push to origin
    int err;
    git_push_options options;
    git_remote *remote = nullptr;
    char *refspec = "refs/heads/master";
    const git_strarray refspecs = {
		&refspec,
		1
	};
    bool isSetUpstream = false;

    const char *tmp1 = auth.username.c_str();
    const char *tmp2 = auth.password.c_str();
    strcpy(user_name, tmp1);
    strcpy(passwd, tmp2);

    err = git_remote_lookup(&remote, this->repo, "origin");
    if(err) {
        return 1;
    }
    int cnt = git_remote_refspec_count(remote);
    if(cnt) {
        isSetUpstream = true;
    }

    err = git_push_options_init(&options, GIT_PUSH_OPTIONS_VERSION);
    if(err) {
        return 2;
    }
    options.callbacks.credentials = credsSSL;

    err = git_remote_push(remote, &refspecs, &options);
    if((err == -1) && !isSetUpstream) {
        int errSet = git_remote_add_push(this->repo, "origin", refspec);
        if(!errSet) {
            isSetUpstream = true;
            err = git_remote_push(remote, &refspecs, &options);
        }
        return 3;
    }else if(err) {
        return 3;
    }

    return 0;
}