# LUT grading

This project demonstates how to use 3D color lookup cubes to add hardware accelerated color grading to OpenGL projects.

Image magick can generate perfectly laid out identity color tables.

To generate an identity lookup table using imagemagick:

	convert   hald:8   hald_8.png

# Artist's workflow

Open a screengrab of your current render output, create adjustment layers in photoshop or other image manipulation software and ajust the parameters until you're happy with the look.

Apply these exact adjustment layers to the identity color table (```hald_8.png```) in our case. Double-check if hald_8.png is using 8 bits per channel. Save the color-graded lookup table png file with a telling name (e.g. ```night-for-day.png```).

If you use this lookup table png in the test project, your images should be properly color graded.

# More information about 3D LUT Color Grading

* [wiki](https://en.wikipedia.org/wiki/3D_lookup_table)
* [image magick](http://www.imagemagick.org/Usage/color_mods/#hald-clut)
* [hald-clut](http://www.quelsolaar.com/technology/clut.html)

## Screenshot

![img](screenshot.png)