#ifndef CELL_H
#define CELL_H

#include <QWidget>
#include <QMap>

class Cell : public QWidget
{
    Q_OBJECT
public:
    Cell(QWidget *parent =0 ,int num = 0);

    void setNum(int num);
    int getNum();
protected:
    void paintEvent(QPaintEvent *);
    QColor getColor(int nIndex);

private:
    int _num;
    static QMap<int ,QColor> _colorMap;
};

#endif // CELL_H
