@echo off
set CC=.\bin\tcc\tcc
@REM set CC="/"

echo "Compiling..."
%CC% -vv -o out\TicTac.exe -I./lib/raylib/include TicTac.c -L./lib/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
echo "Compile Completed"