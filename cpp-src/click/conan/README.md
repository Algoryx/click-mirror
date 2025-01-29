# Conan usage

## Create click in local conan 2 cache on Macos with shared libraries

```bash
conan create . --channel "stable" --user algoryx -pr:b=default -o "*:shared=True" -s build_type=Release -s os.version=11.0
```

## Upload click to conan

```bash
conan upload click/<version>/algoryx/stable
```
