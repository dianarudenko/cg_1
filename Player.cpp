#include "Player.h"


bool Player::Moved() const
{
  if(coords.x == old_coords.x && coords.y == old_coords.y)
    return false;
  else
    return true;
}

void Player::ProcessInput(MovementDir dir)
{
  int move_dist = move_speed * 1;
  switch(dir)
  {
    case MovementDir::UP:
      old_coords.y = coords.y;
      for (int i = 0; i < move_dist; i++) {
        if (map.GetTileType(coords.x - map_pos.x + tileSize / 2,
            coords.y - map_pos.y + tileSize / 2) == EXIT) {
          state = WIN;
          break;
        }
        if (map.GetTileType(coords.x - map_pos.x + tileSize / 2,
            coords.y - map_pos.y + tileSize / 2) == TRAP) {
          state = DIED;
          break;
        }
        if (map.GetTileType(coords.x - map_pos.x, coords.y - map_pos.y + tileSize) == WALL) break;
        if (map.GetTileType(coords.x - map_pos.x + tileSize - 1, coords.y - map_pos.y + tileSize) == WALL) break;
        if (WINDOW_HEIGHT - coords.y < DELTA) {
            map_pos.y--;
        } else {
          coords.y++;
        }
      }
      break;
    case MovementDir::DOWN:
      old_coords.y = coords.y;
      for (int i = 0; i < move_dist; i++) {
        if (map.GetTileType(coords.x - map_pos.x + tileSize / 2,
            coords.y - map_pos.y + tileSize / 2) == EXIT) {
          state = WIN;
          break;
        }
        if (map.GetTileType(coords.x - map_pos.x + tileSize / 2,
            coords.y - map_pos.y + tileSize / 2) == TRAP) {
          state = DIED;
          break;
        }
        if (map.GetTileType(coords.x - map_pos.x, coords.y - map_pos.y - 1) == WALL) break;
        if (map.GetTileType(coords.x - map_pos.x + tileSize - 1, coords.y - map_pos.y - 1) == WALL) break;
        if (coords.y < DELTA) {
            map_pos.y++;
        } else {
          coords.y--;
        }
      }
      break;
    case MovementDir::LEFT:
      old_coords.x = coords.x;
      for (int i = 0; i < move_dist; i++) {
        if (map.GetTileType(coords.x - map_pos.x + tileSize / 2,
            coords.y - map_pos.y + tileSize / 2) == EXIT) {
          state = WIN;
          break;
        }
        if (map.GetTileType(coords.x - map_pos.x + tileSize / 2,
            coords.y - map_pos.y + tileSize / 2) == TRAP) {
          state = DIED;
          break;
        }
        if (map.GetTileType(coords.x - map_pos.x - 1, coords.y - map_pos.y) == WALL) break;
        if (map.GetTileType(coords.x - map_pos.x - 1, coords.y - map_pos.y + tileSize - 1) == WALL) break;
        if (coords.x < DELTA) {
            map_pos.x++;
        } else {
          coords.x--;
        }
      }
      break;
    case MovementDir::RIGHT:
      old_coords.x = coords.x;
      for (int i = 0; i < move_dist; i++) {
        if (map.GetTileType(coords.x - map_pos.x + tileSize / 2,
            coords.y - map_pos.y + tileSize / 2) == EXIT) {
          state = WIN;
          break;
        }
        if (map.GetTileType(coords.x - map_pos.x + tileSize / 2,
            coords.y - map_pos.y + tileSize / 2) == TRAP) {
          state = DIED;
          break;
        }
        if (map.GetTileType(coords.x - map_pos.x + tileSize, coords.y - map_pos.y) == WALL) break;
        if (map.GetTileType(coords.x - map_pos.x + tileSize, coords.y - map_pos.y + tileSize - 1) == WALL) break;
        if (WINDOW_WIDTH - coords.x < DELTA) {
            map_pos.x--;
        } else {
          coords.x++;
        }
      }
      break;
    default:
      break;
  }
}

void Player::Draw(Image &screen)
{
  // if (coords.x < 0) {
  //   coords.x = screen.Width() + coords.x;
  // } else {
  //   coords.x = coords.x % screen.Width();
  // }
  // if (coords.y < 0) {
  //   coords.y = screen.Height() + coords.y;
  // } else {
  //   coords.y = coords.y % screen.Height();
  // }
  if(Moved())
  {
    for(int y = old_coords.y; y < old_coords.y + tileSize; ++y)
    {
      for(int x = old_coords.x; x < old_coords.x + tileSize; ++x)
      {
        screen.PutPixel(x, y, screen.GetPixel(x, y));
      }
    }
    old_coords = coords;
  }

  for(int y = coords.y; y < coords.y + tileSize; ++y)
  {
    for(int x = coords.x; x < coords.x + tileSize; ++x)
    {
      screen.MixPixels(x, y, tile->GetPixel(x - coords.x, tileSize - y + coords.y - 1));
    }
  }
}