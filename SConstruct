EnsurePythonVersion(2, 6)
EnsureSConsVersion(2, 0)

env = Environment()

env.Append(CPPPATH = ['./', '/opt/boost/include/', '/usr/include/', ])
env.Append(LIBPATH = [])

env.Append(CPPDEFINES = {'DEBUG_BUILD' : '1'})

env.Append(CPPFLAGS = Split('-W -Wall -Wextra -Woverloaded-virtual -pedantic'))
env.Append(CPPFLAGS = Split('-std=c++0x -g -ggdb'))

env.Append(LIBS = Split('SDL SDL_mixer SDL_image GL GLU'))

files = Glob("*.cpp")
for directory in ['editor', 'gui', 'entity']:
    files = files + Glob(directory + "/*.cpp")
env.Program("game", files)
