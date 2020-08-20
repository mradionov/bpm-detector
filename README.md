### Build

Prepare directory:

```bash
mkdir build
cd build

```

Build and run main executable:

```bash
cmake ..
make
./bpm
```

Build with tests:

```bash
cmake -DBUILD_TESTS=ON ..
make
./unit_tests
```

### Reference

- http://soundfile.sapp.org/doc/WaveFormat/
- http://www.geosci.usyd.edu.au/users/jboyden/vad/

### TODO

WAV read
- RIFF - little-endian, RIFX - big-endian
- if not PCM - extra params in the end of "fmt"
- extra chunks

Types:
- calculate average without sum overflow
- how to write chars to file
- proper rendering of values in the image

### Extra

View file in hex:

```bash
xxd -g 1 test.wav | less
```
