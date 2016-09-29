#include "cell.h"
#include "constdefine.h"
#include <QPainter>
#include <QTime>

QMap<int , QColor> Cell::_colorMap;

Cell::Cell(QWidget *parent, int num)
    : QWidget(parent)
    , _num(num)
{
    setFixedSize(CELLSIZE, CELLSIZE);
}

 void Cell::paintEvent(QPaintEvent *)
 {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setBrush(getColor(_num));
    p.setPen(QColor(255,255,255,0));
    p.drawRoundedRect(QRect(2,2, width()-4, height()-4), 5,5);

    if (_num == 0)return;
    QFont font;
    font.setBold(true);
    font.setPixelSize(23);
    p.setFont(font);
    QColor color = ((_num%3)%2 == 0) ? QColor(255,255,255) : QColor(117, 109,100);
    p.setPen(color);
    p.drawText(rect(), QString::number(_num), QTextOption(Qt::AlignCenter));
 }

QColor Cell::getColor(int nIndex)
{
    if (_num == 0) return QColor(204,192,178);
    if (_colorMap.contains(nIndex))return _colorMap[nIndex];
    QTime time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    int x = qrand()%255;
    int y = qrand()%255;
    int z = qrand()%255;
    _colorMap[nIndex] = QColor(x,y,z);
    return _colorMap[nIndex];
}

void Cell::setNum(int num)
{
    _num = num;
    update();
}

int Cell::getNum()
{
    return _num;
}
