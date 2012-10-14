#include "StdAfx.h"

#include "App.h"
#include "Engine.h"
#include <boost/version.hpp>

std::string asString(SDL_version& v) {
    std::ostringstream oss;
    oss << (int)v.major << "." << (int)v.minor << "." << (int)v.patch;
    return oss.str();
}

std::ostream& PrintVersionsOfUsedLibraries(std::ostream& os) {
    os << "Built with \n";
    const std::string indent = "   ";
    os << indent << "Boost: " << BOOST_LIB_VERSION << "\n";
    SDL_version sdlver; SDL_VERSION(&sdlver);
    os << indent << "SDL: " << asString(sdlver) << "\n";
    SDL_version mixerver; SDL_MIXER_VERSION(&mixerver);
    os << indent << "SDL_mixer: " << asString(mixerver) << "\n";
    SDL_version imgver; SDL_IMAGE_VERSION(&imgver);
    os << indent << "SDL_image: " << asString(imgver) << "\n";
    os << indent << "OpenGL vendor: " << (char*)glGetString(GL_VENDOR) << "\n";
    os << indent << "OpenGL renderer: " << (char*)glGetString(GL_RENDERER) << "\n";
    os << indent << "OpenGL version: " << (char*)glGetString(GL_VERSION) << "\n";
    os << indent << "OpenGL shaders version: " << (char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    return os;
}

std::ostream& PrintKeyUsage(std::ostream& os) {
    os << "      strzałki lewo/prawo  -  poruszanie się postacią\n"
       << "         strzałka do góry  -  skok\n"
       << " przytrzymanie klawisza d  -  bieganie\n"
       << "   naciśnięcie klawisza s  -  strzelanie\n"
       << "\n"
       << "       --------- edytor poziomów ---------\n"
       << "                 klawisz 1 - pokaż/ukryj GUI\n"
       << "                 klawisz 0 - zmień tryb edytuj <-> graj\n"
       << "       lewy przycisk myszy - wybieranie i dodawanie elementów\n";
    return os;
}

int main(/* int argc, char *argv[] */) {
    Engine::Get().Load();
    size_t width = 640;
    size_t height = 420;
    Engine::Get().GetWindow()->SetSize(width, height);
    Engine::Get().GetWindow()->SetFullscreen(false);

    std::ostream& output = std::cout;
    App app(false/*fullscreen?*/);
    app.Init();
    PrintVersionsOfUsedLibraries(output) << "\n\n";
    PrintKeyUsage(output) << "\n\n";
    output.flush();
    app.MainLoop();

    return 0;
}
