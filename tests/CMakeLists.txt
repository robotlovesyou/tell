configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CharCursorTestFile.txt ${CMAKE_CURRENT_BINARY_DIR}/CharCursorTestFile.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CharCursorTestEmptyFile.txt ${CMAKE_CURRENT_BINARY_DIR}/CharCursorTestEmptyFile.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CharCursorTestSingleCharFile.txt ${CMAKE_CURRENT_BINARY_DIR}/CharCursorTestSingleCharFile.txt COPYONLY)
add_executable(Tests_run
        TestMain.cpp
        TokenTests.cpp
        CharCursorTests.cpp
        ErrorReporterTests.cpp
        LexerTests.cpp
        DocCommentContext.cpp
        MessageDeclarationTests.cpp
        DirectiveDeclarationTests.cpp
        ParserTestHelpers.h
        ServiceDeclarationTests.cpp
        ParserTests.cpp
        ASTTests.cpp
        ScalarTypeDefTests.cpp
        FieldTests.cpp
        MessageTypeDefTests.cpp
        MapTypeDefTests.cpp
        ListTypeDefTests.cpp
        ArgumentTests.cpp
        CallTests.cpp
        SerializableTypeDefTests.cpp
        SerializableFieldTests.cpp
        SerializableMessageDeclarationTests.cpp
        SerializableASTTests.cpp
        SerializableTestHelpers.h
        SerializableDirectiveDeclarationTests.cpp
        SerializableCallTests.cpp
        SerializableServiceDeclarationTests.cpp
        ToSerializableTests.cpp)
target_include_directories(Tests_run PUBLIC ../til)
target_link_libraries(Tests_run til)