#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>


Image::Image(const std::string &a_path)
{
  if((data = (Pixel*)stbi_load(a_path.c_str(), &width, &height, &channels, 4)) != nullptr)
  {
    size = width * height * channels;
  } else {
    std::cout << "Error while creating an Image\n";
  }
}

Image::Image(int a_width, int a_height, int a_channels)
{
  data = new Pixel[a_width * a_height ]{};
  tileMap = new tileType[a_width * a_height]{UNKNOWN};

  if(data != nullptr)
  {
    width = a_width;
    height = a_height;
    size = a_width * a_height * a_channels;
    channels = a_channels;
    self_allocated = true;
  }
}

void Image::Clean() {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      data[width * y + x] = backgroundColor;
    }
  }
}

int Image::Save(const std::string &a_path)
{
  auto extPos = a_path.find_last_of('.');
  if(a_path.substr(extPos, std::string::npos) == ".png" || a_path.substr(extPos, std::string::npos) == ".PNG")
  {
    stbi_write_png(a_path.c_str(), width, height, channels, data, width * channels);
  }
  else if(a_path.substr(extPos, std::string::npos) == ".jpg" || a_path.substr(extPos, std::string::npos) == ".JPG" ||
          a_path.substr(extPos, std::string::npos) == ".jpeg" || a_path.substr(extPos, std::string::npos) == ".JPEG")
  {
    stbi_write_jpg(a_path.c_str(), width, height, channels, data, 100);
  }
  else
  {
    std::cerr << "Unknown file extension: " << a_path.substr(extPos, std::string::npos) << "in file name" << a_path << "\n";
    return 1;
  }
  tileMap = new tileType[width * height]{UNKNOWN};
  return 0;
}

void Image::PutTile(int x, int y, Image & img, tileType type) {
  if ((x > width) || (y > height)) {
    return;
  }
  for (int i = 0; i < tileSize; i++) {
    if (i + y > height) {
      return;
    }
    if (i + y < 0) {
      continue;
    }
    for (int j = 0; j < tileSize; j++) {
      if (j + x > width) {
        return;
      }
      if (j + x < 0) {
        continue;
      }
      PutPixel(j + x, i + y, img.GetPixel(j, tileSize - i - 1));
  //std::cout << "1\n";
      tileMap[(i + y) * width + j + x] = type;
  //std::cout << j << " ";
    }
    //std::cout << i << "\n";
  }
}


  void Image::MixPixels (int x, int y, const Pixel &pix) {
    data[width * y + x].r = pix.a / 255.0 * (pix.r - data[width * y + x].r) + data[width * y + x].r;
    data[width * y + x].g = pix.a / 255.0 * (pix.g - data[width * y + x].g) + data[width * y + x].g;
    data[width * y + x].b = pix.a / 255.0 * (pix.b - data[width * y + x].b) + data[width * y + x].b;
    data[width * y + x].a = pix.a * (1 - data[width * y + x].a / 255.0) + data[width * y + x].a;
  }

Image::~Image()
{
  if(self_allocated)
    delete [] data;
  else
  {
    stbi_image_free(data);
  }
}