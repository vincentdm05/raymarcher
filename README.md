## Welcome to my Raymarcher project

#### This is a spinoff of my [raytracer project](https://github.com/vincentdm05/raytracer)

### Compile

with `g++ -Wall -std=c++11 -o raymarch raymarch.cpp`


### Run

with `./raymarch`

Example usage (for mac):

* If `bin/` doesn't exist yet: `mkdir bin`
* `g++ -Wall -std=c++11 -o bin/raymarch raymarch.cpp && ./bin/raymarch > img.ppm && open img.ppm`

On Windows, I use a cool image viewer called nomacs, and MSYS2 for the bash shell:

`g++ -Wall -std=c++11 -o bin/raymarch raymarch.cpp && ./bin/raymarch > img.ppm && nomacs img.ppm`

For some easy profiling, I use `time`:

`time { ./bin/raymarch > img.ppm ; }`
