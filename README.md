### NOTE: This uses Chakra because V8 does not offer proper memory limiting and will OOM on out of memory for isolates. This was originally created because I needed encapsulation but it doesn't really need to exist because of memory hooks added in V8 recently.

![tranquil banner](https://raw.githubusercontent.com/ScytheApp/tranquil/master/assets/banner.png)

## tranquil
Tranquil if a free and open source Javascript runtime powered by the ChakraCore engine. Designed to be high performance and easy to use.

### Links
[License](https://github.com/ScytheApp/tranquil/blob/master/LICENSE)

### How does it work?
Tranquil at its core is just a modern C++ wrapper for the ChakraCore API aiming to make it more user friendly. It implements its own eventloop and some other systems that make development faster and easier.

### Compiling
This project has been developed on Ubuntu 20.04 and only tested on Ubuntu 20.04. It is not guaranteed to work on any other platform.
Note: It will only work on Ubuntu, forgot I used static linking
```shell script
git clone https://github.com/ScytheApp/tranquil.git
cd tranquil
mkdir build && cd build
cmake ..
cmake --build build --target tranquil
```
