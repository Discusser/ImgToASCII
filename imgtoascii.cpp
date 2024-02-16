#include <algorithm>
#include <glib.h>

#include "./imgtoascii.hpp"

std::vector<char> character_list = {'@', 'J', 'D', '%', '*', 'P', '+', 'Y', '$', ',', '.'};

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
  parse_command_line_arguments(&argc, &argv);

  Image image = read_image(img_path);
  print_image(image);

  return 0;
}

Image read_image(std::string image_path) {
  Image image(image_path, 0);

  if (!image.get_data()) {
    g_error("Could not load image '%s'\n", image_path.data());
  }

  if (image.get_width() > max_width || image.get_height() > max_height) {
    double ratio =
        std::min(max_width, max_height) / static_cast<double>(std::max(image.get_width(), image.get_height()));
    int new_width = static_cast<double>(image.get_width()) * ratio;
    int new_height = static_cast<double>(image.get_height()) * ratio;
    image.resize(new_width, new_height);

    if (!image.get_data()) {
      g_error("Could not resize image '%s'\n", image_path.data());
    }
  }

  return image;
}

void parse_command_line_arguments(int *argc, char **argv[]) {
  GError *error = nullptr;
  GOptionContext *context = g_option_context_new(nullptr);
  g_option_context_add_main_entries(context, command_line_arguments, nullptr);
  g_option_context_set_help_enabled(context, true);

  if (!g_option_context_parse(context, argc, argv, &error)) {
    g_error("An error occured while parsing command line arguments: %s\n", error->message);
    g_error_free(error);
  }

  g_option_context_free(context);

  if (!img_path) {
    g_print("Missing image path, specify it with -f or --file\n");
  }
}

void print_image(Image image) {
  for (int j = 0; j < image.get_height(); j++) {
    for (int i = 0; i < image.get_width(); i++) {
      const unsigned char *pixel_offset = image.get_data() + (i + image.get_width() * j) * image.get_num_channels();

      unsigned char grayscale = 0;
      unsigned char rgb[] = {0, 0, 0};
      for (int k = 0; k < image.get_num_channels(); k++) {
        grayscale += pixel_offset[k];
        if (k < static_cast<int>(sizeof(rgb))) {
          rgb[k] = pixel_offset[k];
        }
      }
      grayscale /= image.get_num_channels();

      char character = character_list[static_cast<int>(grayscale / 255.0 * character_list.size())];
      set_terminal_background(rgb[0], rgb[1], rgb[2]);
      g_print("%c%c", character, character);
      clear_terminal_background();
    }
    g_print("\n");
  }
}

void set_terminal_background(int r, int g, int b) {
  g_print("\x1b[48;2;%i;%i;%im", r, g, b);
}

void clear_terminal_background() {
  g_print("\x1b[0m");
}
