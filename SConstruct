EnsurePythonVersion(2, 6)
EnsureSConsVersion(2, 0)

env = Environment()

env.Append(CPPPATH = ['./', '/opt/boost/include/', '/usr/include/', ])
env.Append(LIBPATH = [])

env.Append(CPPDEFINES = {'DEBUG_BUILD' : '1'})

env.Append(CPPFLAGS = Split('-std=c++0x -g -ggdb'))
env.Append(CPPFLAGS = Split('-W -Wall -Wextra -pedantic -Woverloaded-virtual -Wnon-virtual-dtor -Wpointer-arith'))
env.Append(CPPFLAGS = Split('-Wno-long-long -Wundef -Wcast-align -Wcomment -Winit-self -Wchar-subscripts'))

env.Append(LIBS = Split('SDL SDL_mixer SDL_image GL GLU'))

files = Glob("*.cpp")
for directory in ['editor', 'entity', 'gui', 'math', 'video']:
    files = files + Glob(directory + "/*.cpp")
env.Program("game", files)
