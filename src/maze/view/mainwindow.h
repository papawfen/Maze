#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QGraphicsScene>
#include <QMainWindow>
#include <QPainter>

#include "../controller/controller.h"
#include "cave_window.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(Controller *controller_, QWidget *parent = nullptr);
  ~MainWindow();

  void SetController(Controller *control) { controller_ = control; }
  void SetMatrixes();

 protected:
  void paintEvent(QPaintEvent *);

 private slots:
  void OnFilePath();

  void OnGenerateMaze();

  void OnGeneratePath();

  void OnCaveWindow();

 private:
  void SetMaximums();
  void ShowPathGeneration();

 private:
  Ui::MainWindow *ui_;
  Controller *controller_;
  CaveWindow *cave_;
  Matrix lower_walls_;
  Matrix right_walls_;
  QString path_ = NULL;
  bool draw_flag_{false};
  int rows_ = 10;
  int cols_ = 10;
  bool paint_path_{false};
};

#endif  // MAINWINDOW_H
