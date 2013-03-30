EnsurePythonVersion(2, 6)
EnsureSConsVersion(2, 0)

def pch(target, source, env, for_signature):
    includes = ''
    for path in env['CPPPATH']:
       includes += ' -I' + path
    return 'g++ -o '+source[0].name+'.gch -c -std=c++0x -g -ggdb -DDEBUG_BUILD=1 '+includes+' '+source[0].name

bld = Builder(generator = pch)
env = Environment(BUILDERS = {'PrecompileHeader' : bld})

env.Append(CPPPATH = ['./', '/opt/boost/include/', '/usr/include/', ])
env.Append(LIBPATH = [])

env.Append(CPPDEFINES = {'DEBUG_BUILD':1,
                        })

env.Append(CPPFLAGS = Split('-std=c++0x'))
env.Append(CPPFLAGS = Split('-g -ggdb'))
#env.Append(CPPFLAGS = Split('-O2'))
env.Append(CPPFLAGS = Split('-W -Wall -Wextra -pedantic -Woverloaded-virtual -Wnon-virtual-dtor -Wpointer-arith'))
env.Append(CPPFLAGS = Split('-Wno-long-long -Wcast-align -Wcomment -Winit-self -Wchar-subscripts'))

env.Append(LIBS = Split('SDL SDL_mixer SDL_image GL GLU'))

files = Glob("*.cpp")
for directory in ['appstates', 'common', 'editor', 'game', 'game/entity', 'gui', 'math', 'video']:
    files = files + Glob(directory + "/*.cpp")

import hashlib, pickle
pch_source_file = 'StdAfx.h'
pch_digest_file = pch_source_file + '.digest'
def sha1_file(fname): return hashlib.sha1(open(fname,'rb').read()).hexdigest()
def make_pch() : env.PrecompileHeader('A-make-gch-first', pch_source_file)
try:
  if (pickle.load(open(pch_digest_file, 'r')) != sha1_file(pch_source_file)):
    make_pch()
except:
  make_pch()
finally:
   pickle.dump(sha1_file(pch_source_file), open(pch_digest_file,'w'))

env.Program("run", files)
