project(Test)

file(GLOB_RECURSE _src *.h *.cpp)

add_executable(${PROJECT_NAME} ${_src} Resources/lite_res.qrc)

find_package(language-manager CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
        lyric-tab::lyric-tab
        language-manager::language-manager
)