ADD_EXECUTABLE(tests tests.cc)
TARGET_LINK_LIBRARIES(tests miniocpp ${requiredlibs})


ADD_EXECUTABLE(upload-big-object upload-big-object.cc)
TARGET_LINK_LIBRARIES(upload-big-object miniocpp ${requiredlibs})
