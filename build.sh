#! /bin/bash

#  -o game.html    // Output file, the .html extension determines the files that need to be generated: `.wasm`, `.js` (glue code) and `.html` (optional: `.data`). All files are already configured to just work.
#  game.c          // The input files for compilation, in this case just one but it could be multiple code files: `game.c screen_logo.c screen_title.c screen_gameplay.c`
#  -Os -Wall       // Some config parameters for the compiler, optimize code for small size and show all warnings generated
#  ./path-to/libraylib.a   // This is the libraylib.a generated, it's recommended to provide it directly, with the path to it: i.e. `./raylib/src/libraylib.a`
#  -Ipath          // Include path to look for additional #include .h files (if required)
#  -Lpath          // Library path to look for additional library .a files (if required)
#  -s USE_GLFW=3   // We tell the linker that the game/library uses GLFW3 library internally, it must be linked automatically (emscripten provides the implementation)
#  -s ASYNCIFY     // Add this flag ONLY in case we are using ASYNCIFY code
#  --shell-file path-to/shell.html  // All webs need a "shell" structure to load and run the game, by default emscripten has a `shell.html` but we can provide our own
#
#  --- additional flags ---
#
#  --preload-file resources      // Specify a resources directory for data compilation (it will generate a .data file)
#  -s TOTAL_MEMORY=67108864      // Specify a heap memory size in bytes (default = 16MB) (67108864 = 64MB)
#  -s ALLOW_MEMORY_GROWTH=1      // Allow automatic heap memory resizing -> NOT RECOMMENDED!
#  -s FORCE_FILESYSTEM=1         // Force filesystem creation to load/save files data (for example if you need to support save-game or drag&drop files)
#  -s ASSERTIONS=1               // Enable runtime checks for common memory allocation errors (-O1 and above turn it off)
#  --profiling                   // Include information for code profiling

mkdir -p ./Build/HTML5
rm -fr ./Build/HTML5/*
emcc -o ./Build/HTML5/Outwit.html ./src/*.c -Os -Wall ./dependencies/raylib-06-04-2024/raylib/src/libraylib.a -I. -Idependencies/raylib-06-04-2024/raylib/src -L. -Ldependencies/raylib-06-04-2024/raylib/src -s USE_GLFW=3 --shell-file ./shell.html --preload-file ./resources -DPLATFORM_WEB -s TOTAL_MEMORY=1073741824 -DGAME_TITLE='"Outwit"' -s EXPORTED_RUNTIME_METHODS=ccall
mv ./Build/HTML5/Outwit.html ./Build/HTML5/index.html
cd ./Build/HTML5
zip -r Outwit.zip ./*
cd ../../
ls -al ./Build/HTML5/
