#include <iostream>

#include "App.h"
#include "Engine.h"


int main(int argc, char *argv[]) {
    std::cout << "      strzałki lewo/prawo  -  poruszanie się postacią\n"
              << "         strzałka do góry  -  skok\n"
              << " przytrzymanie klawisza d  -  bieganie"
              << std::endl;

    Engine::Get().Load();

    size_t width = 600;
    size_t height = 400;
    Engine::Get().Window()->SetSize(width, height);
    App app(false);

    app.Run();
    return 0;
}
