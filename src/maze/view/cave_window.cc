#include "cave_window.h"

#include <QMainWindow>

#include "ui_cave.h"

CaveWindow::CaveWindow(Controller *control, QWidget *parent)
    : ui_(new Ui::CaveWindow), parent_(parent), controller_(control) {
  ui_->setupUi(this);
  ui_->Auto->hide();
  ui_->NextStep->hide();
  ui_->Time->hide();
  connect(ui_->LoadFile, SIGNAL(clicked()), this, SLOT(LoadFileClicked()));
  connect(ui_->GenerateCave, SIGNAL(clicked()), this,
          SLOT(GenerateCaveClicked()));
  connect(ui_->NextStep, SIGNAL(clicked()), this, SLOT(NextStepClicked()));
  connect(ui_->Auto, SIGNAL(clicked()), this, SLOT(AutoClicked()));
  connect(ui_->GenerateMaze, SIGNAL(clicked()), this,
          SLOT(GenerateMazeClicked()));
}

CaveWindow::~CaveWindow() { delete ui_; }

void CaveWindow::GenerateCaveClicked() {
  if (!controller_->InitGeneratedCave(ui_->Rows->text().toInt(),
                                      ui_->Cols->text().toInt(),
                                      ui_->Chance->text().toInt())) {
    ui_->ErrorLine->setText("cant save this cave");
  } else {
    ui_->ErrorLine->setText("");
    cave_ = controller_->GetCave();
    draw_flag_ = true;
    repaint();
    ui_->Auto->show();
    ui_->NextStep->show();
    ui_->Time->show();
  }
}

void CaveWindow::LoadFileClicked() {
  QString Qpath = QFileDialog::getOpenFileName(this, NULL, "/~", "TXT (*.txt)");
  if (!Qpath.isNull()) {
    if (!controller_->InitCaveFromFile(Qpath.toStdString())) {
      ui_->ErrorLine->setText("wrong input file, choose another one!");
    } else {
      ui_->ErrorLine->setText("");
      cave_ = controller_->GetCave();
      draw_flag_ = true;
      repaint();
      ui_->Auto->show();
      ui_->NextStep->show();
      ui_->Time->show();
    }
  }
}

void CaveWindow::paintEvent(QPaintEvent *) {
  int offset_x = 50;
  int offset_y = 50;
  if (draw_flag_) {
    QPainter painter(this);
    QPen pen;
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap));
    pen.setColor(Qt::black);
    pen.setWidth(2);
    int cols = cave_.GetCols();
    int rows = cave_.GetRows();
    int x = 500 / cols;
    int y = 500 / rows;
    painter.drawLine(offset_x, offset_y, offset_x + x * (cols), offset_y);
    painter.drawLine(offset_x, offset_y, offset_x, offset_y + y * (rows));
    painter.drawLine(offset_x + x * (cols), offset_y, offset_x + x * (cols),
                     offset_y + y * (rows));
    painter.drawLine(offset_x + x * (cols), offset_y + y * (rows), offset_x,
                     offset_y + y * (rows));
    painter.setBrush(Qt::SolidPattern);
    for (int i = 0; i < cave_.GetRows(); ++i) {
      for (int j = 0; j < cave_.GetCols(); ++j) {
        if (cave_(i, j)) {
          painter.drawRect(QRect(x * (j) + offset_x, y * (i) + offset_y, x, y));
        }
      }
    }
    draw_flag_ = false;
  }
}

void CaveWindow::NextStepClicked() {
  controller_->UpdateCave(ui_->Birth->text().toInt(),
                          ui_->Death->text().toInt());
  cave_ = controller_->GetCave();
  draw_flag_ = true;
  repaint();
}

void CaveWindow::AutoClicked() {
  Matrix prev(cave_.GetRows(), cave_.GetCols());
  while (1) {
    controller_->UpdateCave(ui_->Birth->text().toInt(),
                            ui_->Death->text().toInt());
    cave_ = controller_->GetCave();
    draw_flag_ = true;
    repaint();
    if (prev == cave_) break;
    prev = cave_;
    Delay();
  }
}

void CaveWindow::Delay() {
  QTime die_time = QTime::currentTime().addMSecs(ui_->Time->text().toInt());
  while (QTime::currentTime() < die_time)
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void CaveWindow::GenerateMazeClicked() {
  this->hide();
  parent_->show();
}
