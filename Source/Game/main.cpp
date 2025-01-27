#include <engine/platform/project_system.h>
#include <engine/platform/file_system.h>
#include <engine/platform/window_system.h>
#include <iostream>

int main(int argc, const char *argv[]) {
    engine::project_system().set_project_info("My awesome game", {0, 0, 1});
    auto &wnd = engine::window_system();
    wnd.set_size(1280, 720);
    if (engine::succeeded(wnd.init())) {
        while (wnd.is_alive()) {
            wnd.update();
        }
    }
    else {
        std::cerr << "Failed to create window" << std::endl;
    }
    wnd.destroy();
    return 0;
}
