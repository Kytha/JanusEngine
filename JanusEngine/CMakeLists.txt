PROJECT (JanusEngine LANGUAGES CXX C)

########################### SOURCE CODE ########################################

SET ( JANUS_ENGINE_SOURCE
  src/Core/Log.cpp
  src/Core/EntryPoint.cpp
)


SET ( JANUS_ENGINE_HEADERS
  include/JanusEngine/Core/Core.h
  include/JanusEngine/Core/EntryPoint.h
  include/JanusEngine/Core/Log.h
)

############################# DEPENDENCIES ###########################################

ADD_LIBRARY ( JanusEngine STATIC
  ${JANUS_ENGINE_SOURCE}
  ${JANUS_ENGINE_HEADERS}
  )

TARGET_INCLUDE_DIRECTORIES( JanusEngine PUBLIC include/JanusEngine)
TARGET_PRECOMPILE_HEADERS( JanusEngine PUBLIC "src/jnpch.h")
    
INSTALL ( TARGETS JanusEngine DESTINATION lib )