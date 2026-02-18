#include "core/Application.hpp"
#include "core/Logger.hpp"

int main(int argc, char* argv[]) {
    roblox_clone::core::Logger::init();
    
    roblox_clone::core::Application app;
    if (!app.initialize(argc, argv)) {
        return 1;
    }
    
    return app.run();
}
