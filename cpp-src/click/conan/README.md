# Conan usage

## Create click in local conan cache

```bash
conan create .
```

## Upload click to conan.io

```bash
conan upload click/<version>/algoryx/stable
```

## Install Find* for conan dependencies defined in conanfile.py

NOTE: This needs to be done before running cmake

```bash
cd oos
conan install ..
```
