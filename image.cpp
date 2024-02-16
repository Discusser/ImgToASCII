#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wunused-variable"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>
#pragma GCC diagnostic pop

#include "./image.hpp"

Image::Image(const std::string &path, int desired_num_channels) {
  this->data =
      std::shared_ptr<unsigned char>(stbi_load(path.data(), &this->width, &this->height, &this->num_channels, 0));
}

const std::string &Image::get_path() {
  return this->path;
}

int Image::get_width() {
  return this->width;
}

int Image::get_height() {
  return this->height;
}

int Image::get_num_channels() {
  return this->num_channels;
}

const unsigned char *Image::get_data() {
  return this->data.get();
}

void Image::resize(int new_width, int new_height) {
  auto resized_image = std::shared_ptr<unsigned char>(
      static_cast<unsigned char *>(std::malloc(new_width * new_height * this->num_channels)));
  stbir_resize_uint8(this->data.get(), this->width, this->height, 0, resized_image.get(), new_width, new_height, 0,
                     this->num_channels);
  this->width = new_width;
  this->height = new_height;
  this->data = resized_image;
}
