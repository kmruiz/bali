# bali

A lightweight, embeddable JavaScript runtime.

## Bootstrap for developers

### Configure the build directory (Meson + Ninja)

```sh
$ meson setup build
```

### Generate compile_commands.json for clangd

```sh
$ ninja -C build -t compdb > compile_commands.json
```

### Run all tests

```sh
$ meson test -C build --verbose
```

### Generate doxygen documentation

```sh
$ ninja -C build docs
```

Documentation will be generated in build/docs/. There is an html and a latex version. 
