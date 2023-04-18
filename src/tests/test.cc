#include <gtest/gtest.h>

#include "../maze/model/cave.h"

TEST(cave, cave_init_and_update) {
  s21::Cave cave;
  double start_pos[10][10] = {1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0,
                              0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1,
                              1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0,
                              1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1,
                              0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0,
                              1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0};
  std::string path = "tests/test.txt";
  cave.setPath(path);
  cave.ReadFile();
  S21Matrix cave_(cave.GetCave());
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      ASSERT_TRUE(start_pos[i][j] == cave_(i, j));
    }
  }

  S21Matrix prev_step(10, 10);
  cave.setBirth_lim(4);
  cave.setDeath_lim(3);
  while (1) {
    cave.UpdateMatrix();
    cave_ = cave.GetCave();
    if (prev_step == cave_) break;
    prev_step = cave_;
  }
  double end_pos[10][10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1,
                            1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
                            0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
                            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      ASSERT_TRUE(end_pos[i][j] == cave_(i, j));
    }
  }
}
