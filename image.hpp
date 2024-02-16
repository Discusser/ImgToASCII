#include <memory>
#include <string>

class Image {
 public:
  explicit Image(const std::string &path, int desired_num_channels);
  void resize(int new_width, int new_height);

  const unsigned char *get_data();
  const std::string &get_path();
  int get_width();
  int get_height();
  int get_num_channels();

 private:
  std::shared_ptr<unsigned char> data;
  std::string path;
  int width;
  int height;
  int num_channels;
};
