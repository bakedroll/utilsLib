begin_project(utilsLibTest EXECUTABLE OPTIONAL)

require_library(GTest)
require_project(utilsLib)

add_source_directory(test)
