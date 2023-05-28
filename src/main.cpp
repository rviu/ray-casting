#include <cmath>
#include <string>
#include <vector>
#include <iostream>

#include "quickcg.h"

int main() {
  const int map_width = 10;
  const int map_height = 10;
  const int screen_width = 640;
  const int screen_height = 480;

  const std::vector<std::vector<int>> world_map {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
  };

  double pos_x = 5;
  double pos_y = 5;
  double dir_x = -1;
  double dir_y = 0;
  double plane_x = 0;
  double plane_y = 0.66;

  double cur_time = 0;
  double prev_time = 0;

  QuickCG::screen(screen_width, screen_height, 0, "ray-casting");

  while (!QuickCG::done()) {
  }
}
