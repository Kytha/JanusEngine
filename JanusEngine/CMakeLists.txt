PROJECT (JanusEngine LANGUAGES CXX C)

########################### SOURCE CODE ########################################

SET ( JANUS_ENGINE_SOURCE
  src/Core/Log.cpp
  src/Core/EntryPoint.cpp
  src/Core/Memory/Allocators.cpp
  src/Core/Memory/Memory.cpp
  src/Core/Memory/TrackingPolicy.cpp
)


SET ( JANUS_ENGINE_HEADERS
  include/JanusEngine/Core/Core.h
  include/JanusEngine/Core/EntryPoint.h
  include/JanusEngine/Core/Log.h
  include/JanusEngine/jnpch.h
  include/JanusEngine/Core/Memory/Memory.h
  include/JanusEngine/Core/Memory/MemoryArena.h
  include/JanusEngine/Core/Memory/TrackingPolicy.h
  include/JanusEngine/Core/Memory/Allocators.h
  include/JanusEngine/Core/Memory/TaggingPolicy.h
  include/JanusEngine/Core/Memory/ThreadPolicy.h
  include/JanusEngine/Core/Memory/BoundsCheckingPolicy.h
  include/JanusEngine/Core/Memory/Utils.h
  include/JanusEngine/Core/Memory/MemoryArea.h
)

############################# DEPENDENCIES ###########################################

ADD_LIBRARY ( JanusEngine STATIC
  ${JANUS_ENGINE_SOURCE}
  ${JANUS_ENGINE_HEADERS}
  )

TARGET_INCLUDE_DIRECTORIES( JanusEngine PUBLIC include/JanusEngine)

# TO DO: Reintroduce precompiled header once vscode C++ tools supports proper intelisense 
#TARGET_PRECOMPILE_HEADERS( JanusEngine PUBLIC "src/jnpch.h")
    
INSTALL ( TARGETS JanusEngine DESTINATION lib )