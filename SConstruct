env = Environment()
env.MergeFlags("-lSDL -lGL -lGLU -lSDL_mixer -lSDL_image")
env.MergeFlags("-g -ggdb")
env.MergeFlags("-W -Wall -Wextra -Woverloaded-virtual -pedantic")
env.Program("game", Glob("*.cpp") + Glob("editor/*.cpp") + Glob("gui/*.cpp"))

# mozna takze skompilowac z linii polecen
#  g++ *.cpp editor/*.cpp gui/*.cpp -lSDL -lSDL_image -lSDL_mixer -lGL -lGLU -o game
