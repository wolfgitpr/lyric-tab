project(Test)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE _src *.h *.cpp)

add_executable(${PROJECT_NAME} ${_src})

#if (WIN32)
#    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
#endif ()

find_package(LangCore CONFIG REQUIRED)
find_package(LangPlugins CONFIG REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets
        lyric-tab::lyric-tab
        LangCore::LangCore
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        # copy LangMgr plugins
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "$<TARGET_FILE_DIR:LangPlugins::LangPlugins>/../lib/plugins/LangPlugins"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/LangPlugins"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${LangCore_DIR}/G2pPackages"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/G2pPackages"
)