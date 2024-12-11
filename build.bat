@echo off
set CC=.\bin\tcc\tcc
@REM set CC=zig cc
echo "Compiling..."
set INPUT=TicTac.c src/game.c  src/board.c src/control.c src/menu.c src/leaderboard.c
set FLAGS= -Wall -Wextra -g 
@REM set FLAGS=
set I_PATH=-I./src -I./lib/raylib/include
set L_PATH=-L./lib/raylib/lib
set LIBS=-lraylib -lopengl32 -lgdi32 -lwinmm
%CC%  %FLAGS% -o out\TicTac.exe %I_PATH% %INPUT% %L_PATH% %LIBS%
echo "Compile Completed"

if %ERRORLEVEL% neq 0 (
    echo "Compilation failed. Exiting."
    exit /b %ERRORLEVEL%
)

echo "Compile completed successfully."
echo "Running the binary..."
out\TicTac.exe