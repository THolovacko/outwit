## Tom's Tech Stack
* MacBook Pro; 14-inch, Nov 2023; Apple M3 Max; 36 GB; macOS Sonoma 14.5

## Getting started
1. Setup [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html) (version 3.1.61)
2. Setup AWS CLI (version aws-cli/2.16.10 Python/3.11.9 Darwin/23.5.0 source/arm64)
3. Setup GO (version go1.22.4 darwin/arm64)
4. Confirm you can use ```emcc``` and ```emrun``` from the command line
5. Confirm you can run ```zip``` from the command line
6. Build game with ```./build.sh``` from the root directory
7. Run game with ```./run.sh``` from the root directory

\* *You might need to make the scripts build.sh and run.sh executable with something like chmod +x (also build_all.sh in backend directory and build.sh for each subdirectory)*
