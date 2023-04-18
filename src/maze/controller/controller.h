#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../model/model.h"

namespace s21 {
class Controller {
 public:
  explicit Controller(Model* mod) : model_(mod){};
  ~Controller(){};

  bool ReadFromFile(std::string path) { return model_->ReadFile(path); };

  void GenerateMaze(int rows, int cols) { model_->GenerateMaze(rows, cols); };

  S21Matrix& GetLMatrix() { return model_->GetLMatrix(); }

  S21Matrix& GetRMatrix() { return model_->GetRMatrix(); }

  std::vector<std::pair<int, int>> GetMazeSolution(int startX, int startY,
                                                   int endX, int endY) {
    return model_->GetMazeSolution(startX, startY, endX, endY);
  }

  bool InitGeneratedCave(int rows, int cols, int chance) {
    return model_->GenerateCave(rows, cols, chance);
  }

  bool InitCaveFromFile(std::string path) { return model_->LoadCave(path); }

  void UpdateCave(int birth, int death) { model_->UpdateCave(birth, death); }

  S21Matrix& GetCave() { return model_->GetCave(); }

 private:
  Model* model_;
};
}  // namespace s21

#endif  // CONTROLLER_H
