cmake_minimum_required(VERSION 3.1.0)
project(gitnote VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Git)

execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_VARIABLE GIT_VERSION 
                OUTPUT_STRIP_TRAILING_WHITESPACE 
                )

configure_file(version.h.in include/version.h)

add_subdirectory(textedit)
add_subdirectory(libgit2)

install(TARGETS textedit git2 
        CONFIGURATIONS Debug 
        RUNTIME DESTINATION bin 
        LIBRARY DESTINATION lib 
        ARCHIVE DESTINATION lib
        )
install(FILES 
        textedit/textedit_zh.qm 
        textedit/qt_zh_CN.qm 
        CONFIGURATIONS Debug 
        DESTINATION bin 
        )

