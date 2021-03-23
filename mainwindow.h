#include <fstream>
#include <QMainWindow>
#include "QLineEdit"
#include "ui_mainwindow.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void process();

private:
    Ui::MainWindow *ui;
    clock_t start_time;
    clock_t end_time;

};
