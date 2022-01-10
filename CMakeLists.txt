cmake_minimum_required(VERSION 3.1.0)
project(gitnote VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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

