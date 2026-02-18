add_executable(roblox-clone-tests
    main.cpp
)

target_link_libraries(roblox-clone-tests PRIVATE
    spdlog::spdlog
)

enable_testing()

add_test(NAME roblox-clone-tests COMMAND roblox-clone-tests)
