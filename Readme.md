# Click

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

## Test frameworks

### C++ catch2 framework

We are using [Catch2](https://github.com/catchorg/Catch2/) testing framework with cmake integration

- [catch2 matchers](https://github.com/catchorg/Catch2/blob/devel/docs/matchers.md#top)

### Python pytest

Python tests are using pytest
