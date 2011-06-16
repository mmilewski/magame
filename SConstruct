env = Environment()

env.Append(CPPPATH = ['/data/include/boost/', '/usr/include/', ])
env.Append(LIBPATH = [])

env.Append(CPPDEFINES = {'DEBUG_BUILD' : '1'})

env.Append(CPPFLAGS = Split('-W -Wall -Wextra -Woverloaded-virtual -pedantic'))
env.Append(CPPFLAGS = Split('-std=c++0x -g -ggdb'))

env.Append(LIBS = Split('SDL SDL_mixer SDL_image GL GLU'))

env.Program("game", Glob("*.cpp") + Glob("editor/*.cpp") + Glob("gui/*.cpp"))
