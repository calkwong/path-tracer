#include "app.h"

#include "tracy/Tracy.hpp"

int main() {
    std::this_thread::sleep_for(std::chrono::seconds(1)); // establish connection with tracy
    ZoneScoped;

    Application app{};

    app.init();

    app.render();

    return 0;
}