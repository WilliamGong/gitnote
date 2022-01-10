# gitNote
一个基于 git 进行数据同步的云笔记
## 依赖
+ Qt 5.12.11
## 构建
1. 建立 `build` 目录：`mkdir build && cd build`
1. CMake 配置：`cmake .. -DCMAKE_BUILD-TYPE=Release`
1. 编译：`cmake --build .`

> 这里的`CMAKE_BUILD_TYPE`可根据需要选择`Release`和`Debug`。
## 安装

    cmake --install . --config=Release
> 因为 cmake 不指定配置就无法正确安装，所以需要手动指定。

# 功能
+ 基础文本编辑
+ Git
    + 仓库打开和初始化
    + git status
    + git add 和 git commit
    + fetch, pull, push


## 注意
由于历史残留原因，`textedit`目录下还有 qmake 用的项目文件，不建议使用。   
**由于功能实现不完整以及异常处理不完全，不要再生产环境下使用该项目的所有关于 git 的功能。**