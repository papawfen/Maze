#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(Controller *controller, QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow), controller_(controller) {
  ui_->setupUi(this);
  ui_->lineEdit_startY->hide();
  ui_->lineEdit_startX->hide();
  ui_->lineEdit_endY->hide();
  ui_->lineEdit_endX->hide();
  ui_->label_3->hide();
  ui_->label_4->hide();
  ui_->label_5->hide();
  ui_->label_6->hide();
  ui_->GeneratePath->hide();
  cave_ = new CaveWindow(controller_, this);
  connect(ui_->FilePath, SIGNAL(clicked()), this, SLOT(OnFilePath()));
  connect(ui_->GeneratePath, SIGNAL(clicked()), this, SLOT(OnGeneratePath()));
  connect(ui_->GenerateMaze, SIGNAL(clicked()), this, SLOT(OnGenerateMaze()));
  connect(ui_->CaveWindow, SIGNAL(clicked()), this, SLOT(OnCaveWindow()));
}

MainWindow::~MainWindow() {
  delete ui_;
  delete cave_;
}

void MainWindow::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  int offset_x = 50;
  int offset_y = 50;
  painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap));
  if (draw_flag_) {
    int x = 500 / cols_;
    int y = 500 / rows_;
    painter.drawLine(offset_x, offset_y, offset_x + x * (cols_), offset_y);
    painter.drawLine(offset_x, offset_y, offset_x, offset_y + y * (rows_));
    painter.drawLine(offset_x + x * (cols_), offset_y, offset_x + x * (cols_),
                     offset_y + y * (rows_));
    painter.drawLine(offset_x + x * (cols_), offset_y + y * (rows_), offset_x,
                     offset_y + y * (rows_));

    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        if (lower_walls_(i, j)) {
          painter.drawLine(offset_x + x * (j), offset_y + y * (i + 1),
                           offset_x + x * (j + 1), offset_y + y * (i + 1));
        }
        if (right_walls_(i, j)) {
          painter.drawLine(offset_x + x * (j + 1), offset_y + y * (i),
                           offset_x + x * (j + 1), offset_y + y * (i + 1));
        }
      }
    }
  }
  if (paint_path_) {
    painter.setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::FlatCap));
    int endX = ui_->lineEdit_endX->text().toInt();
    int endY = ui_->lineEdit_endY->text().toInt();
    int startX = ui_->lineEdit_startX->text().toInt();
    int startY = ui_->lineEdit_startY->text().toInt();
    int x = 500 / cols_;
    int y = 500 / rows_;
    std::vector<std::pair<int, int>> solution =
        controller_->GetMazeSolution(startX, startY, endX, endY);
    for (std::size_t i = 0; i < solution.size() - 1; ++i) {
      painter.drawLine(x * solution[i].first + x / 2 + offset_x,
                       y * solution[i].second + y / 2 + offset_y,
                       x * solution[i + 1].first + x / 2 + offset_x,
                       y * solution[i + 1].second + y / 2 + offset_y);
    }
  }
}

void MainWindow::OnFilePath() {
  path_ = QFileDialog::getOpenFileName(this, NULL, "/~", "TXT (*.txt)");
  if (!path_.isNull()) {
    if (!controller_->ReadFromFile(path_.toStdString())) {
      ui_->ErrorLine->setText("wrong input file, choose another one!");
    } else {
      ui_->ErrorLine->setText("");
      SetMatrixes();
      ShowPathGeneration();
      SetMaximums();
      draw_flag_ = true;
      repaint();
    }
  }
}

void MainWindow::ShowPathGeneration() {
  ui_->lineEdit_startY->show();
  ui_->lineEdit_startX->show();
  ui_->lineEdit_endY->show();
  ui_->lineEdit_endX->show();
  ui_->label_3->show();
  ui_->label_4->show();
  ui_->label_5->show();
  ui_->label_6->show();
  ui_->GeneratePath->show();
}

void MainWindow::SetMaximums() {
  ui_->lineEdit_endX->setMaximum(cols_ - 1);
  ui_->lineEdit_startX->setMaximum(cols_ - 1);
  ui_->lineEdit_endY->setMaximum(rows_ - 1);
  ui_->lineEdit_startY->setMaximum(rows_ - 1);
}

void MainWindow::OnGenerateMaze() {
  if (!ui_->GenRows->text().isEmpty()) rows_ = ui_->GenRows->text().toInt();
  if (!ui_->GenCols->text().isEmpty()) cols_ = ui_->GenCols->text().toInt();
  controller_->GenerateMaze(rows_, cols_);
  SetMatrixes();
  draw_flag_ = true;
  ShowPathGeneration();
  SetMaximums();
  repaint();
}

void MainWindow::OnGeneratePath() {
  paint_path_ = true;
  draw_flag_ = true;
  repaint();
}

void MainWindow::SetMatrixes() {
  lower_walls_ = controller_->GetLMatrix();
  right_walls_ = controller_->GetRMatrix();
  rows_ = right_walls_.GetRows();
  cols_ = right_walls_.GetCols();
}

void MainWindow::OnCaveWindow() {
  this->hide();
  cave_->show();
}
