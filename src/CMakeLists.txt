set(PROJECT_SOURCES
  main.cpp
  Scanner.h
  Scanner.cpp
  Parser.cpp
  Parser.h
  ASTNodes.cpp
  ASTNodes.h
        SemanticAnalyzer.cpp SemanticAnalyzer.h SymbolTable.cpp SymbolTable.h)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/data/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/data/
  )

