@echo off
set CC=.\bin\tcc\tcc
@REM set CC=zig cc

echo "Compiling..."
set INPUT=TicTac.c src/screen.c  src/board.c
set FLAGS= -Wall -Wextra -g 
@REM set FLAGS=
set I_PATH=-I./src -I./lib/raylib/include
set L_PATH=-L./lib/raylib/lib
set LIBS=-lraylib -lopengl32 -lgdi32 -lwinmm
%CC%  %FLAGS% -o out\TicTac.exe %I_PATH% %INPUT% %L_PATH% %LIBS%
echo "Compile Completed"