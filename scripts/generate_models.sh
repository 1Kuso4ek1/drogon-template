# Generates src/Models/Test.h and src/Models/Test.cc
build/drogon_ctl create model src/Models/

# Move and rename src/Models/*.h to include/Models/*.hpp
# Rename src/Models/*.cc to src/Models/*.cpp
find src/Models -name "*.h" -exec sh -c 'mv "$1" "include/Models/${1##src/Models/}"' sh {} \;
find include/Models -name "*.h" -exec sh -c 'mv "$1" "${1%.h}.hpp"' sh {} \;
find src/Models -name "*.cc" -exec sh -c 'mv "$1" "${1%.cc}.cpp"' sh {} \;

# Rename only directives of "quotes" to also contain "Models/"
find src/Models -name "*.cpp" -type f -exec sed -i 's/\"\(.*\)\.h/\"Models\/\1\.hpp/g' {} \;
