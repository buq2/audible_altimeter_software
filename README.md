# Audible altimeter project software /firmware

![Audible altimeter](https://github.com/buq2/audible_altimeter_software/blob/master/images/small_case13.jpg)

Schematics are in repo https://github.com/buq2/audible_altimeter

More information at http://buq2.com/

# Build

```
cmake -S . -B build
cmake --build build --parallel 12 --config Release
```

# Docker build

```
docker build . -t audible_altimeter_software
```
