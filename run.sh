echo "Building project..."
cmake -S . -B build
cmake --build ./build/
echo "Running executable..."
build/C_http
