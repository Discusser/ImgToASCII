# ImgToASCII
Simple program written in C++ to convert images to ASCII.
## Build instructions
This project uses meson for building
```
git clone https://github.com/Discusser/ImgToASCII.git
sudo apt install meson libglib2.0-dev
meson setup builddir && cd builddir/
meson compile
```
## Usage
From `./ImgToASCII --help`:
```
Usage:
  ImgToASCII [OPTION?]

Help Options:
  -?, --help                       Show help options

Application Options:
  -f, --file=                      The path to the image to be converted to ASCII. To set multiple files, repeat the argument
  -h, --max_height                 Maximum height of the image in px
  -w, --max_width                  Maximum width of the image in px
  -t, --transparent_background     Whether or not the background should be transparent on images with alpha channels
  -s, --image_spacing              The spacing between images when printing multiple images at the same time
  --no_colors                      Disables colors
```
