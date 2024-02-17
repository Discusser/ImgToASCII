#pragma once

#include <glib.h>

#include <string>
#include <vector>

#include "./image.hpp"

extern std::vector<char> character_list;

void clear_terminal_background();
Image read_image(std::string image_path);
std::vector<Image> read_images(std::vector<std::string> image_paths);
void set_terminal_background(int r, int g, int b, int a);
void parse_command_line_arguments(int *argc, char **argv[]);
void print_images(std::vector<Image> images, int spacing);
void print_image_row(Image image, int row);
