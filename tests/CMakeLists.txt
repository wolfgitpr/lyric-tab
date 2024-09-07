project(Test)

file(GLOB_RECURSE _src *.h *.cpp)

add_executable(${PROJECT_NAME} ${_src} Resources/lite_res.qrc)

if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif ()

find_package(language-manager CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
        lyric-tab::lyric-tab
        language-manager::language-manager
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# copy cpp-pinyin dict

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/cpp-pinyin/dict
        ${CMAKE_BINARY_DIR}/bin/dict
)

install(DIRECTORY ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/cpp-pinyin/dict
        DESTINATION bin
)