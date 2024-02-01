project(example)

add_executable(example
    main.cpp
)
set_property(TARGET example PROPERTY CXX_STANDARD 23)

target_link_libraries(example PRIVATE appgl)
