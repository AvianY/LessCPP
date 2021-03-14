add_library(sfml-widgets src/button.cc src/text_input.cc src/framed_text.cc src/list_view.cc)

target_link_libraries(sfml-widgets sfml-graphics)

target_include_directories(sfml-widgets PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
)
