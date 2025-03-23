list=`ldd bin/petrichor_engine | awk '{print $3}' | grep -v '^$'`
libs="./bin/libs"

echo "Copying required libraries..."

for r in $list; do
    cp $r $libs && echo "Copied: $r"
done

echo "Patching libraries to use the correct path..." 

for lib in bin/libs/*.so*; do
    [ -f "$lib" ] && patchelf --set-rpath '$ORIGIN' "$lib" && echo "Patched: $lib"
done

echo "Done!"