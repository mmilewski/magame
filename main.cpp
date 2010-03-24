#include "StdAfx.h"

#include "App.h"
#include "Engine.h"


int main(int argc, char *argv[]) {
    std::cout << "      strzałki lewo/prawo  -  poruszanie się postacią\n"
              << "         strzałka do góry  -  skok\n"
              << " przytrzymanie klawisza d  -  bieganie"
              << std::endl;

    Engine::Get().Load();

    size_t width = 800;
    size_t height = 600;
    Engine::Get().GetWindow()->SetSize(width, height);
    Engine::Get().GetWindow()->SetFullscreen(false);
    App app(false);

    app.Run();
    return 0;
}
