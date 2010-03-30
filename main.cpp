#include <iostream>

#include "App.h"
#include "Engine.h"

int main(int argc, char *argv[]) {
    std::cout << "      strzałki lewo/prawo  -  poruszanie się postacią\n"
              << "         strzałka do góry  -  skok\n"
              << " przytrzymanie klawisza d  -  bieganie\n"
              << "   naciśnięcie klawisza s  -  strzelanie\n"
              << std::endl;

    Engine::Get().Load();

    size_t width = 640;
    size_t height = 420;
    Engine::Get().GetWindow()->SetSize(width, height);
    App app(false);

    app.Run();
    return 0;
}
