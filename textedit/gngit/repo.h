#pragma once

#include <string>
#include "git2.h"

namespace gitnote {
    class Repo {
        public:
            Repo();
            ~Repo();
            int init(std::string path);
            int open(std::string path);

        private: 
            git_repository *repo = nullptr;
            std::string cwd;
    };
};