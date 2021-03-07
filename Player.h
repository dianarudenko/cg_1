#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include <iostream>

enum class MovementDir
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

struct Player
{
  explicit Player(Point pos = {.x = 10, .y = 10}) :
                 coords(pos), old_coords(coords) {};

  bool Moved() const;
  void ProcessInput(MovementDir dir);
  void Draw(Image &screen);
  Point Coords() {return coords; }
  void SetTile (Image & tile) {this->tile = &tile; }

private:
  Point coords {.x = 10, .y = 10};
  Point old_coords {.x = 10, .y = 10};
  Pixel color {.r = 255, .g = 0, .b = 0, .a = 255};
  Image *tile = nullptr;
  int move_speed = 4;

};

#endif //MAIN_PLAYER_H
