add_executable(roblox-clone
    main.cpp
    core/Application.cpp
    core/Logger.cpp
    core/Config.cpp
    renderer/Renderer.cpp
    renderer/Window.cpp
    renderer/Shader.cpp
    renderer/Mesh.cpp
    renderer/Texture.cpp
    renderer/Material.cpp
    scene/Scene.cpp
    scene/Entity.cpp
    scripting/ScriptEngine.cpp
    scripting/ScriptBindings.cpp
    network/NetworkManager.cpp
    network/Server.cpp
    network/Client.cpp
)

if(ROBLOX_CLONE_BUILD_EDITOR)
    target_sources(roblox-clone PRIVATE
        editor/Editor.cpp
        editor/Viewport.cpp
    )
    target_compile_definitions(roblox-clone PRIVATE ROBLOX_CLONE_BUILD_EDITOR)
endif()

target_include_directories(roblox-clone PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(roblox-clone PRIVATE
    spdlog::spdlog
    SDL2::SDL2
    SDL2::SDL2main
    GLEW::GLEW
    OpenGL::GL
    glm::glm
    EnTT::EnTT
    sol2::sol2
    Lua::Lua
    enet::enet
    imgui::imgui
    nlohmann_json::nlohmann_json
)

if(WIN32)
    target_link_libraries(roblox-clone PRIVATE
        opengl32
        glu32
    )
endif()

if(ROBLOX_CLONE_BUILD_EDITOR)
    target_link_libraries(roblox-clone PRIVATE imgui::imgui)
endif()

target_compile_definitions(roblox-clone PRIVATE
    $<$<CONFIG:DEBUG>:DEBUG>
    $<$<CONFIG:RELEASE>:NDEBUG>
)
