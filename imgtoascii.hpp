#pragma once

#include <glib.h>

#include <string>
#include <vector>

#include "./image.hpp"

extern std::vector<char> character_list;

void clear_terminal_background();
Image read_image(std::string image_path);
void set_terminal_background(int r, int g, int b);
void parse_command_line_arguments(int *argc, char **argv[]);
void print_image(Image image);
