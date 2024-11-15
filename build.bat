@echo off
set CC=.\bin\tcc\tcc
@REM set CC="/"

echo "Compiling..."
set INPUT=TicTac.c src/screen.c  src/board.c
%CC% -vv -Wall -Wextra -g -o out\TicTac.exe -I./src -I./lib/raylib/include %INPUT% -L./lib/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
echo "Compile Completed"