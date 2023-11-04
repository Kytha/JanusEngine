CMAKE_MINIMUM_REQUIRED(VERSION 3.25)
PROJECT (JanusEngine LANGUAGES CXX C)
SET(CMAKE_CXX_STANDARD 17)

########################### SOURCE CODE ########################################

SET ( JANUS_ENGINE_SOURCE
  src/Core/Logging/Log.cpp
  src/Core/EntryPoint.cpp
  src/Core/Memory/Allocators.cpp
  src/Core/Memory/Memory.cpp
  src/Core/Memory/TrackingPolicy.cpp
  src/Core/Logging/LogSink.cpp
)


SET ( JANUS_ENGINE_HEADERS
  include/JanusEngine/Core/Core.h
  include/JanusEngine/Core/EntryPoint.h
  include/JanusEngine/Core/Logging/Log.h
  include/JanusEngine/Core/Memory/Memory.h
  include/JanusEngine/Core/Memory/MemoryArena.h
  include/JanusEngine/Core/Memory/TrackingPolicy.h
  include/JanusEngine/Core/Memory/Allocators.h
  include/JanusEngine/Core/Memory/TaggingPolicy.h
  include/JanusEngine/Core/Memory/ThreadPolicy.h
  include/JanusEngine/Core/Memory/BoundsCheckingPolicy.h
  include/JanusEngine/Core/Memory/Utils.h
  include/JanusEngine/Core/Memory/MemoryArea.h
  include/JanusEngine/Core/Logging/LogSink.h
)

############################# DEPENDENCIES ###########################################

ADD_SUBDIRECTORY(vendors/spdlog)
ADD_SUBDIRECTORY(vendors/ftl)
ADD_SUBDIRECTORY(vendors/glfw)

ADD_LIBRARY ( JanusEngine STATIC
  ${JANUS_ENGINE_SOURCE}
  ${JANUS_ENGINE_HEADERS}
  )

TARGET_INCLUDE_DIRECTORIES( JanusEngine PUBLIC include/JanusEngine)
TARGET_INCLUDE_DIRECTORIES( JanusEngine PUBLIC vendors/spdlog/include)
TARGET_INCLUDE_DIRECTORIES( JanusEngine PUBLIC vendors/glfw/include)


TARGET_LINK_LIBRARIES( JanusEngine PUBLIC spdlog PUBLIC glfw)
TARGET_INCLUDE_DIRECTORIES( JanusEngine PUBLIC vendors/ftl/include)


TARGET_LINK_LIBRARIES( JanusEngine PUBLIC spdlog)
TARGET_LINK_LIBRARIES( JanusEngine PUBLIC ftl)

############################# INSTALLATION ###########################################

# TO DO: Reintroduce precompiled header once vscode C++ tools supports proper intelisense 
TARGET_PRECOMPILE_HEADERS( JanusEngine PUBLIC "src/jnpch.h")
    
INSTALL ( TARGETS JanusEngine DESTINATION lib )


############################# TESTING ###########################################

SET( JANUS_ENGINE_TEST_SOURCE
  tst/Core/Memory/LinearAllocatorTest.cc
)

SET(DOWNLOAD_EXTRACT_TIMESTAMP true)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

ENABLE_TESTING()

ADD_EXECUTABLE(
  JanusEngineTest ${JANUS_ENGINE_TEST_SOURCE}
)

TARGET_INCLUDE_DIRECTORIES( JanusEngineTest PUBLIC include/JanusEngine)

TARGET_LINK_LIBRARIES(
  JanusEngineTest
  JanusEngine
  GTest::gtest_main
)

INCLUDE(GoogleTest)
gtest_discover_tests(JanusEngineTest)