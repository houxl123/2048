#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class Cell;
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyReleaseEvent(QKeyEvent *);
    void paintEvent(QPaintEvent *);

private:
    bool _bRelease;
    QList<QList<Cell *> > _cellList;
};

#endif // MAINWINDOW_H
