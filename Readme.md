# Build and test instructions

## Build and test c++

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_GENERATOR=Ninja ../cpp-src
ninja
```

## Reset c++ builds

```bash
cd build
rm -rf * .github.conan.cmake
```

## Build and test python

```bash
protoc -I=protobuf-src --python_out=python-src/src messaging.proto
pytest
```
