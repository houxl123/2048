#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
//#include <QList>
class Cell;
class MainWindow : public QWidget
{
    Q_OBJECT

    struct moveSut
    {
        moveSut(int _x1=0,int _y1=0,int _x2=0, int _y2=0, int _value1=0,int _value2=0)
            : x1(_x1), y1(_y1), x2(_x2), y2(_y2), value1(_value1),value2(_value2)
        {
        }
        int x1,y1,x2,y2,value1,value2;
    };

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyReleaseEvent(QKeyEvent *);
    void paintEvent(QPaintEvent *);

    void moveOper(QList<QList<Cell *> > cellList);
    void movieOper(const moveSut &sut);
    void nextPoint(QList<QList<int> > numList);

private:
    QList<QList<Cell *> > _cellList;
};

#endif // MAINWINDOW_H
