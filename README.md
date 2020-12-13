## Welcome to my Raymarcher project

#### This is a spinoff of my [raytracer project](https://github.com/vincentdm05/raytracer)

### Compile & run

Assuming a file called `raymarch.cpp` containing scene description similar to the sample programs, use
`g++ -Wall -std=c++11 -o raymarch raymarch.cpp`

All sample programs will take as argument the name of a file to write to, if no argument is given the program name is used.
`./raymarch output_file`

Then simply open the generated `output_file.ppm` with your image viewer of choice.


### Samples

There are examples and tests that can be compiled and ran automatically.
Tests are executed using the bash script found in the `tests` folders.
Examples are built and ran using the `raymarch.sh` script found in the `tools` folder.
Note that on Windows you'll have to have `nomacs` installed and an alias set for the example script to be able to show the result. Otherwise you can simply open the resulting file manually.

All example programs can produce a PPM image, written to a file for which the name can be given as parameter. If no parameter is given, the name of the executable is used to generate the output file.
