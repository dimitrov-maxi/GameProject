cmake_minimum_required(VERSION 3.28.1)
project(GameProject)

set(CMAKE_CXX_STANDARD 26)

add_executable(GameProject main.cpp
        Utility/engine.cpp
        Utility/engine.h
        Entities/Entity.cpp
        Entities/Entity.h
        Entities/Player.cpp
        Entities/Player.h
        Entities/Monster.cpp
        Entities/Monster.h
        Entities/AllEntities.h
        Utility/Action.cpp
        Utility/Action.h
        Utility/CustomFunctions.cpp
        Utility/CustomFunctions.h
        Entities/EntityBehaviour.cpp
        Entities/EntityBehaviour.h
        Utility/campaign.cpp
        Utility/campaign.h
)
if(WIN32)
    target_link_libraries(GameProject PRIVATE iphlpapi ws2_32)
endif()
