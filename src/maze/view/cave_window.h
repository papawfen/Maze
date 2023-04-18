#ifndef CAVE_WINDOW_H
#define CAVE_WINDOW_H

#include <unistd.h>

#include <QFileDialog>
#include <QPainter>
#include <QWidget>

#include "../controller/controller.h"
#include "../model/cave.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class CaveWindow;
}
QT_END_NAMESPACE

class CaveWindow : public QWidget {
  Q_OBJECT

 public:
  CaveWindow(Controller *control, QWidget *parent);
  void paintEvent(QPaintEvent *);
  ~CaveWindow();

 private slots:
  void GenerateCaveClicked();

  void LoadFileClicked();

  void NextStepClicked();

  void AutoClicked();

  void GenerateMazeClicked();

 private:
  Ui::CaveWindow *ui_;
  QWidget *parent_;
  Controller *controller_;
  bool draw_flag_ = false;
  Matrix cave_;
  void Delay();
};

#endif  // CAVE_WINDOW_H
