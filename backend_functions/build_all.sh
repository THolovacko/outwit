#! /bin/bash

for dir in */ ; do
    if [ -f "$dir/build.sh" ]; then
        echo "Running build.sh in $dir"
        (cd "$dir" && ./build.sh)
    else
        echo "No build.sh found in $dir"
    fi
done
