#include <algorithm>
#include <cstdlib>
#include <glib.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wunused-variable"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>
#pragma GCC diagnostic pop

#include "./imgtoascii.hpp"

char character_list[11] = {'@', 'J', 'D', '%', '*', 'P', '+', 'Y', '$', ',', '.'};
gchar *img_path;
int max_width = 384;
int max_height = 384;

GOptionEntry command_line_arguments[] = {
    {"file", 'f', G_OPTION_FLAG_NONE, G_OPTION_ARG_FILENAME, &img_path,
     "The path to the image to be converted to ASCII", ""},
    {"max_height", 'h', G_OPTION_FLAG_NONE, G_OPTION_ARG_INT, &max_height, "Maximum height of the image in px"},
    {"max_width", 'w', G_OPTION_FLAG_NONE, G_OPTION_ARG_INT, &max_width, "Maximum width of the image in px"},
    {nullptr},
};

int main(int argc, char *argv[]) {
  GError *error = nullptr;
  GOptionContext *context = g_option_context_new(nullptr);
  g_option_context_add_main_entries(context, command_line_arguments, nullptr);
  g_option_context_set_help_enabled(context, true);

  if (!g_option_context_parse(context, &argc, &argv, &error)) {
    g_error("An error occured while parsing command line arguments: %s\n", error->message);
    g_error_free(error);
  }

  g_option_context_free(context);

  if (!img_path) {
    g_print("Missing image path, specify it with -f or --file\n");
    return -1;
  }

  int width, height, channels;
  unsigned char *image = read_image(img_path, &width, &height, &channels);
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      unsigned char *pixel_offset = image + (i + width * j) * channels;

      unsigned char grayscale = 0;
      g_print("\x1b[48;2;");
      for (int k = 0; k < channels; k++) {
        grayscale += pixel_offset[k];
        if (k <= 3) {
          g_print("%i", pixel_offset[k]);
          if (k < 3)
            g_print(";");
        }
      }
      g_print("m");
      grayscale /= channels;

      char character =
          character_list[static_cast<int>(static_cast<double>(grayscale) / 255.0 * sizeof(character_list))];
      g_print("%c%c", character, character);
    }
    g_print("\n");
  }

  return 0;
}

unsigned char *read_image(const char *image_path, int *width, int *height, int *channels) {
  unsigned char *image = stbi_load(image_path, width, height, channels, 0);

  if (!image) {
    g_error("Error loading image with path %s\n", image_path);
    return nullptr;
  }

  if (*width > max_width || *height > max_height) {
    double ratio = std::min(max_width, max_height) / static_cast<double>(std::max(*width, *height));
    int new_width = static_cast<double>(*width) * ratio;
    int new_height = static_cast<double>(*height) * ratio;
    unsigned char *resized_image = static_cast<unsigned char *>(std::malloc(new_width * new_height * *channels));
    stbir_resize_uint8(image, *width, *height, 0, resized_image, new_width, new_height, 0, *channels);

    if (!resized_image) {
      g_error("Error resizing image\n");
    } else {
      stbi_image_free(image);
      image = resized_image;
      *width = new_width;
      *height = new_height;
    }
  }

  return image;
}
