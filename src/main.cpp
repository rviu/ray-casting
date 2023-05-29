#include <cmath>
#include <string>
#include <vector>
#include <iostream>

#include "quickcg.h"

const int map_width = 10;
const int map_height = 10;
const int screen_width = 640;
const int screen_height = 480;
const int tex_width = 64;
const int tex_height = 64;

const std::vector<std::vector<int>> world_map {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 2, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 2, 0, 3, 3, 0, 0, 0, 1},
  {1, 0, 2, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 4, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 4, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 4, 0, 1},
  {1, 0, 0, 0, 3, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

unsigned int buffer[screen_height][screen_width];

int main() {
  double pos_x = 5;
  double pos_y = 5;
  double dir_x = -1;
  double dir_y = 0;
  double plane_x = 0;
  double plane_y = 0.66;

  double cur_time = 0;
  double prev_time = 0;

  std::vector<unsigned int> texture[4];
  for (int i = 0; i < 4; i++) {
    texture[i].resize(tex_width * tex_height);
  }

  QuickCG::screen(screen_width, screen_height, 0, "ray-casting");

  /*
  // Generated textures
  for (int x = 0; x < tex_width; x++) {
    for (int y = 0; y < tex_height; y++) {
      int y_color = y * 256 / tex_height;
      int xy_color = y * 128 / tex_height + x * 128 / tex_width;
      int xor_color = (x * 256 / tex_width) ^ (y * 256 / tex_height);

      texture[0][tex_width * y + x] = 65536 * 254 * (x != y && x != tex_width - y);
      texture[1][tex_width * y + x] = 256 * xy_color + 65536 * xy_color;
      texture[2][tex_width * y + x] = 65536 * 192 * (x % 16 && y % 16);
      texture[3][tex_width * y + x] = 256 * xor_color;
    }
  }
  */

  // Textures from images
  unsigned long tw, th;
  QuickCG::loadImage(texture[0], tw, th, "textures/greystone.png");
  QuickCG::loadImage(texture[1], tw, th, "textures/redbrick.png");
  QuickCG::loadImage(texture[2], tw, th, "textures/purplestone.png");
  QuickCG::loadImage(texture[3], tw, th, "textures/colorstone.png");

  while (!QuickCG::done()) {
    for (int x = 0; x < QuickCG::w; x++) {
      double camera_x = 2 * x / static_cast<double>(QuickCG::w) - 1;

      double ray_dir_x = dir_x + plane_x * camera_x;
      double ray_dir_y = dir_y + plane_y * camera_x;

      int map_pos_x = static_cast<int>(pos_x);
      int map_pos_y = static_cast<int>(pos_y);

      double next_side_dist_x;
      double next_side_dist_y;

      double delta_dist_x = std::abs(1 / ray_dir_x);
      double delta_dist_y = std::abs(1 / ray_dir_y);

      double perp_wall_dist;

      int step_dir_x;
      int step_dir_y;

      bool hit = false;
      int hit_side;

      if (ray_dir_x < 0) {
        step_dir_x = -1;
        next_side_dist_x = (pos_x - map_pos_x) * delta_dist_x;
      } else {
        step_dir_x = 1;
        next_side_dist_x = (map_pos_x + 1.0 - pos_x) * delta_dist_x;
      }

      if (ray_dir_y < 0) {
        step_dir_y = -1;
        next_side_dist_y = (pos_y - map_pos_y) * delta_dist_y;
      } else {
        step_dir_y = 1;
        next_side_dist_y = (map_pos_y + 1.0 - pos_y) * delta_dist_y;
      }

      while (hit == false) {
        if (next_side_dist_x < next_side_dist_y) {
          next_side_dist_x += delta_dist_x;
          map_pos_x += step_dir_x;
          hit_side = 0;
        } else {
          next_side_dist_y += delta_dist_y;
          map_pos_y += step_dir_y;
          hit_side = 1;
        }

        if (world_map[map_pos_x][map_pos_y] > 0) {
          hit = true;
        }
      }

      if (hit_side == 0) {
        perp_wall_dist = next_side_dist_x - delta_dist_x;
      } else {
        perp_wall_dist = next_side_dist_y - delta_dist_y;
      }

      int line_height = static_cast<int>(QuickCG::h / perp_wall_dist);

      int pitch = 0;

      int draw_start = -line_height / 2 + QuickCG::h / 2;
      if (draw_start < 0) {
        draw_start = 0;
      }

      int draw_end = line_height / 2 + QuickCG::h / 2;
      if (draw_end >= QuickCG::h) {
        draw_end = QuickCG::h - 1;
      }

      int tex_num = world_map[map_pos_x][map_pos_y] -1;

      double wall_x;

      if (hit_side == 0) {
        wall_x = pos_y + perp_wall_dist * ray_dir_y;
      } else {
        wall_x = pos_x + perp_wall_dist * ray_dir_x;
      }

      wall_x -= floor((wall_x));

      int tex_x = static_cast<int>(wall_x * double(tex_width));
      if (hit_side == 0 && ray_dir_x > 0) tex_x = tex_width - tex_x - 1;
      if (hit_side == 1 && ray_dir_y < 0) tex_x = tex_width - tex_x - 1;

      double step = 1.0 * tex_height / line_height;
      double tex_pos = (draw_start - pitch - QuickCG::h / 2 + line_height / 2) * step;

      for (int y = draw_start; y < draw_end; y++) {
        int tex_y = static_cast<int>(tex_pos) & (tex_height - 1);
        tex_pos += step;
        unsigned int color = texture[tex_num][tex_height * tex_y + tex_x];

        if (hit_side == 1) {
          color = (color >> 1) & 8355711;
        }

        buffer[y][x] = color;
      }
    }

    QuickCG::drawBuffer(buffer[0]);
    for (int y = 0; y < QuickCG::h; y++) {
      for (int x = 0; x < QuickCG::w; x++) {
        buffer[y][x] = 0;
      }
    }

    prev_time = cur_time;
    cur_time = QuickCG::getTicks();

    double frame_time = (cur_time - prev_time) / 1000.0;
    QuickCG::print(1.0 / frame_time);

    QuickCG::redraw();

    double move_speed = frame_time * 3.0;
    double rot_speed = frame_time * 2.0;

    QuickCG::readKeys();

    if (QuickCG::keyDown(SDLK_w)) {
      if (world_map[static_cast<int>(pos_x + dir_x * move_speed)][static_cast<int>(pos_y)] == 0) {
        pos_x += dir_x * move_speed;
      }

      if (world_map[static_cast<int>(pos_x)][static_cast<int>(pos_y + dir_y * move_speed)] == 0) {
        pos_y += dir_y * move_speed;
      }
    }

    if (QuickCG::keyDown(SDLK_s)) {
      if (world_map[static_cast<int>(pos_x - dir_x * move_speed)][static_cast<int>(pos_y)] == 0) {
        pos_x -= dir_x * move_speed;
      }

      if (world_map[static_cast<int>(pos_x)][static_cast<int>(pos_y - dir_y * move_speed)] == 0) {
        pos_y -= dir_y * move_speed;
      }
    }

    if (QuickCG::keyDown(SDLK_a)) {
      double prev_dir_x = dir_x;
      dir_x = dir_x * std::cos(rot_speed) - dir_y * std::sin(rot_speed);
      dir_y = prev_dir_x * std::sin(rot_speed) + dir_y * std::cos(rot_speed);

      double prev_plane_x = plane_x;
      plane_x = plane_x * std::cos(rot_speed) - plane_y * std::sin(rot_speed);
      plane_y = prev_plane_x * std::sin(rot_speed) + plane_y * std::cos(rot_speed);
    }

    if (QuickCG::keyDown(SDLK_d)) {
      double prev_dir_x = dir_x;
      dir_x = dir_x * std::cos(-rot_speed) - dir_y * std::sin(-rot_speed);
      dir_y = prev_dir_x * std::sin(-rot_speed) + dir_y * std::cos(-rot_speed);

      double prev_plane_x = plane_x;
      plane_x = plane_x * std::cos(-rot_speed) - plane_y * std::sin(-rot_speed);
      plane_y = prev_plane_x * std::sin(-rot_speed) + plane_y * std::cos(-rot_speed);
    }
  }
}
