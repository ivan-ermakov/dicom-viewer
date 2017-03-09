#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool loadFile(const QString &fileName);

private slots:
    void open();

private:
    Ui::MainWindow *ui;

    QImage image;
    QLabel* imgLabel;

    QMenu *fileMenu;

    QAction *openAct;
};

#endif // MAINWINDOW_H
