# Build

```bash
mkdir build
cd build
#cmake -DCMAKE_BUILD_TYPE=Release ../cpp-src
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_GENERATOR=Ninja ../cpp-src
# Generate protobuf files and build:
ninja
```

## Reset

```bash
rm -rf * .github.conan.cmake
```
