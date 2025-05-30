list=$(ldd bin/petrichor_engine | awk '{print $3}' | grep -v '^$')
libs="./bin/libs"

# Whitelist of libs to vendor (just filenames, not paths)
whitelist=(
  "libSDL3.so.0"
  "libfreetyped.so.6"
  "libfreetype.so.6"
  "libharfbuzz.so.0"
  "libbrotlidec.so.1"
  "libglib-2.0.so.0"
  "libgraphite2.so.3"
  "libbrotlicommon.so.1"
  "libpcre2-8.so.0"
)

echo "Copying required libraries..."

for r in $list; do
    libname=$(basename "$r")
    for w in "${whitelist[@]}"; do
        if [ "$libname" == "$w" ]; then
            cp "$r" "$libs" && echo "Copied: $r"
            break
        fi
    done
done

echo "Patching libraries to use the correct path..." 

for lib in bin/libs/*.so*; do
    [ -f "$lib" ] && patchelf --set-rpath '$ORIGIN' "$lib" && echo "Patched: $lib"
done

echo "Done!"
