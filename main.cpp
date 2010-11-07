#include "StdAfx.h"

#include "App.h"
#include "Engine.h"


int main(/* int argc, char *argv[] */) {
    std::cout << "      strzałki lewo/prawo  -  poruszanie się postacią\n"
              << "         strzałka do góry  -  skok\n"
              << " przytrzymanie klawisza d  -  bieganie\n"
              << "   naciśnięcie klawisza s  -  strzelanie\n"
              << "\n"
              << "       --------- edytor poziomów ---------\n"
              << "                 klawisz 1 - pokaż/ukryj GUI\n"
              << "                 klawisz 0 - zmień tryb edytuj <-> graj\n"
              << "       lewy przycisk myszy - wybieranie i dodawanie elementów\n"
              << "\n"
              << std::endl;

    Engine::Get().Load();

    size_t width = 640;
    size_t height = 420;
    Engine::Get().GetWindow()->SetSize(width, height);
    Engine::Get().GetWindow()->SetFullscreen(false);
    App app(false);

    app.Run();
    return 0;
}
