#include <algorithm>
#include <glib.h>

#include "./imgtoascii.hpp"

std::vector<char> character_list = {'@', 'J', 'D', '%', '*', 'P', '+', 'Y', '$', ',', '.'};

char **img_paths;
int max_width = 384;
int max_height = 384;
bool transparent_background = false;
int image_spacing = 8;
bool no_colors = false;

GOptionEntry command_line_arguments[] = {
    {"file", 'f', G_OPTION_FLAG_NONE, G_OPTION_ARG_FILENAME_ARRAY, &img_paths,
     "The path to the image to be converted to ASCII. To set multiple files, repeat the argument", ""},
    {"max_height", 'h', G_OPTION_FLAG_NONE, G_OPTION_ARG_INT, &max_height, "Maximum height of the image in px"},
    {"max_width", 'w', G_OPTION_FLAG_NONE, G_OPTION_ARG_INT, &max_width, "Maximum width of the image in px"},
    {"transparent_background", 't', G_OPTION_FLAG_NONE, G_OPTION_ARG_NONE, &transparent_background,
     "Whether or not the background should be transparent on images with alpha channels"},
    {"image_spacing", 's', G_OPTION_FLAG_NONE, G_OPTION_ARG_INT, &image_spacing,
     "The spacing between images when printing multiple images at the same time"},
    {"no_colors", 0, G_OPTION_FLAG_NONE, G_OPTION_ARG_NONE, &no_colors, "Disables colors"},
    {nullptr},
};

int main(int argc, char *argv[]) {
  parse_command_line_arguments(&argc, &argv);

  std::vector<std::string> paths;
  int i = 0;
  while (img_paths[i]) {
    paths.push_back(img_paths[i]);
    i++;
  }

  std::vector<Image> images = read_images(paths);
  print_images(images, image_spacing);

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

std::vector<Image> read_images(std::vector<std::string> image_paths) {
  std::vector<Image> images;
  images.reserve(image_paths.size());

  for (std::string path : image_paths) {
    images.push_back(read_image(path));
  }

  return images;
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

  if (!img_paths) {
    g_print("Missing image path, specify it with -f or --file\n");
  }
}

void print_images(std::vector<Image> images, int spacing) {
  std::string string_spacing(spacing, ' ');
  int max_height = 0;
  for (Image image : images) {
    max_height = std::max(image.get_height(), max_height);
  }
  for (int j = 0; j < max_height; j++) {
    for (Image image : images) {
      if (j < image.get_height()) {
        print_image_row(image, j);
        g_print("%s", string_spacing.data());
      }
    }
    g_print("\n");
  }
}

void print_image_row(Image image, int row) {
  for (int i = 0; i < image.get_width(); i++) {
    const unsigned char *pixel_offset = image.get_data() + (i + image.get_width() * row) * image.get_num_channels();

    unsigned char grayscale = 0;
    unsigned char rgb[] = {0, 0, 0, 1};
    for (int k = 0; k < image.get_num_channels(); k++) {
      if (k < 3)
        grayscale += pixel_offset[k];
      if (k < static_cast<int>(sizeof(rgb))) {
        rgb[k] = pixel_offset[k];
      }
    }
    grayscale /= image.get_num_channels();

    char character = character_list[static_cast<int>(grayscale / 255.0 * character_list.size())];
    if (!no_colors)
      set_terminal_background(rgb[0], rgb[1], rgb[2], rgb[3]);
    if (rgb[3] == 0 && transparent_background)
      character = ' ';
    g_print("%c%c", character, character);
    if (!no_colors)
      clear_terminal_background();
  }
}

void set_terminal_background(int r, int g, int b, int a) {
  if (a == 0)
    clear_terminal_background();
  else
    g_print("\x1b[48;2;%i;%i;%im", r, g, b);
}

void clear_terminal_background() {
  g_print("\x1b[0m");
}
