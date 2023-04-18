#include <QApplication>

#include "view/cave_window.h"
#include "view/mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  Model model;
  Controller control(&model);
  MainWindow w(&control);
  w.show();
  return a.exec();
}
