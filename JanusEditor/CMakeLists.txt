PROJECT (JanusEditor LANGUAGES CXX C)

########################### SOURCE CODE ########################################

SET ( JANUS_EDITOR_SOURCE
  src/JanusEditor.cpp
)


SET ( JANUS_EDITOR_HEADERS
  include/JanusEditor.h
)

ADD_EXECUTABLE ( JanusEditor
  ${JANUS_EDITOR_SOURCE}
  ${JANUS_EDITOR_HEADERS}
  )

SET_TARGET_PROPERTIES ( JanusEditor PROPERTIES OUTPUT_NAME JanusEditor )
TARGET_LINK_LIBRARIES ( JanusEditor JanusEngine )

TARGET_INCLUDE_DIRECTORIES( JanusEditor PUBLIC ../JanusEngine/include PRIVATE include)

add_custom_target(copy_resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/resources
)

add_dependencies(JanusEditor copy_resources)

INSTALL ( TARGETS
  JanusEditor
  DESTINATION
  bin
  )
