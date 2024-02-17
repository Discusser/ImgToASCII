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
  -f, --file=                      The path to the i
  -h, --max_height                 Maximum height of
  -w, --max_width                  Maximum width of 
  -t, --transparent_background     Whether or not th
  -s, --image_spacing              The spacing betwe
  --no_colors                      Disables colors
```
