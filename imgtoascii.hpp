#pragma once

#include <glib.h>

extern char character_list[11];

unsigned char *read_image(const char *image_path, int *width, int *height, int *channels);
