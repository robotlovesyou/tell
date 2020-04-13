configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CharCursorTestFile.txt ${CMAKE_CURRENT_BINARY_DIR}/CharCursorTestFile.txt)
add_executable(Tests_run TestMain.cpp FieldDescriptorTests.cpp MessageDescriptorTests.cpp SourceDescriptorTests.cpp TokenTests.cpp CharCursorTests.cpp)
target_link_libraries(Tests_run tell)