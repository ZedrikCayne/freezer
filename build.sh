pushd crankshaft
if [ ! -d obj ]; then mkdir obj; fi
make lib
popd
pushd build
if [ -f freezer ]; then rm freezer; fi
if [ ! -f CMakeCache.txt ]; then cmake ../CMakeLists.txt; fi
cmake --build .
popd
