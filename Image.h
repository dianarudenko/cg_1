#ifndef MAIN_IMAGE_H
#define MAIN_IMAGE_H

#include <string>

constexpr int WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 1024;

#define FLOOR_PATH "./resources/floor.png"
#define WALLS_PATH "./resources/wall.png"
#define PLAYER_PATH "./resources/player1.png"
#define EXIT_PATH "./resources/exit.png"
#define EXIT_MSG_PATH "./resources/exit_msg.png"

#define DELTA 160 // 5 * 32

constexpr int tileSize = 32;

struct Pixel
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

struct Point
{
  int x;
  int y;
};

enum tileType {UNKNOWN, FLOOR, WALL, EXIT};

constexpr Pixel backgroundColor{0, 0, 0, 0};

struct Image
{
  explicit Image(const std::string &a_path);
  Image(int a_width, int a_height, int a_channels);
  Image() {}

  int Save(const std::string &a_path);

  int Width()    const { return width; }
  int Height()   const { return height; }
  int Channels() const { return channels; }
  size_t Size()  const { return size; }
  Pixel* Data()        { return data; }

  Pixel GetPixel(int x, int y) { return data[width * y + x];}
  void  PutPixel(int x, int y, const Pixel &pix) { data[width * y + x] = pix; }
  void  PutTile (int x, int y, Image &img, tileType type);
  tileType GetTileType(int x, int y) { return tileMap[width * y + x]; }
  void Clean();
  void MixPixels (int x, int y, const Pixel &pix);

  ~Image();

private:
  int width = -1;
  int height = -1;
  int channels = 3;
  size_t size = 0;
  Pixel *data = nullptr;
  tileType *tileMap = nullptr;
  bool self_allocated = false;
};

extern Image map;
extern Point map_pos;
extern bool reach_exit;

#endif //MAIN_IMAGE_H
