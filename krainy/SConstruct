env = Environment();
env.MergeFlags("-lSDL -lGL -lGLU -lSDL_mixer");
env.MergeFlags("-g -ggdb");
env.MergeFlags("-W -Wall -pedantic");
env.Program("game", Glob("*.cpp"));

# mozna takze skompilowac z linii polecen
#  g++ *.cpp -lSDL -lSDL_mixer -lGL -lGLU -o game
