#pragma once

#include <string>
#include "git2.h"

namespace gitnote {

enum index_mode {
	INDEX_NONE,
	INDEX_ADD
};

    struct index_options {
	    int dry_run;
	    int verbose;
	    git_repository *repo;
	    enum index_mode mode;
	    int add_update;
    };
    class Repo {
        public:
            Repo();
            ~Repo();
            int init(std::string path);
            int open(std::string path);

            // status
            void statusUpdate();
            inline git_status_list* getStatus() {
                return this->stat;
            }

            // add and commit
            void addAll();
            int commit();

        private: 
            git_repository *repo = nullptr;

            // git status
            git_status_list *stat = nullptr;
            // index
            git_index *index = nullptr;

            std::string cwd;
    };
};