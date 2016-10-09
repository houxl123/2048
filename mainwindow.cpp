#include "mainwindow.h"
#include "constdefine.h"
#include <QKeyEvent>
#include <QDebug>
#include "cell.h"
#include <QTime>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , _bRelease(false)
{
    setFixedSize(CELLNUM*CELLSIZE, CELLNUM*CELLSIZE);

    for(int i=0; i<CELLNUM; i++)
    {
        _cellList.append(QList<Cell*>());
        for(int j=0; j<CELLNUM; j++)
        {
            Cell *pTemp = new Cell(this);
            pTemp->move(j*CELLSIZE, i*CELLSIZE);
            _cellList[i].append(pTemp);;
        }
    }

    _cellList[CELLNUM-1][CELLNUM-1]->setNum(2);
}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(rect(), QColor(188,172,161));

    QPen pen;
    pen.setBrush(QColor(255,255,255,0));
    p.setPen(pen);
    p.setBrush(QColor(204,192,178));
    for (int i=0; i<CELLNUM; i++)
    {
        for (int j=0; j<CELLNUM; j++)
        {
            p.drawRoundedRect(QRect(2+i*CELLSIZE,2+j*CELLSIZE, CELLSIZE-4, CELLSIZE-4), 5,5);
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    if (_bRelease) return;

    QList<QList<int> > valueList;
    QList<QList<QPoint> >pointList;
    if (e->key() == Qt::Key_Up)
    {
        for(int i=0; i<CELLNUM; i++)
        {
            valueList.append(QList<int>());
            pointList.append(QList<QPoint>());
            for (int j=0; j<CELLNUM; j++)
            {
                valueList[i].append(_cellList[j][i]->getNum());
                pointList[i].append(_cellList[j][i]->pos());
            }
        }
    }
    else if(e->key() == Qt::Key_Down)
    {
        for(int i=0; i<CELLNUM; i++)
        {
            valueList.append(QList<int>());
            pointList.append(QList<QPoint>());
            for (int j=0; j<CELLNUM; j++)
            {
                valueList[i].append(_cellList[3-j][i]->getNum());
                pointList[i].append(_cellList[3-j][i]->pos());
            }
        }
    }
    else if(e->key() == Qt::Key_Left)
    {
        for(int i=0; i<CELLNUM; i++)
        {
            valueList.append(QList<int>());
            pointList.append(QList<QPoint>());
            for (int j=0; j<CELLNUM; j++)
            {
                valueList[i].append(_cellList[i][j]->getNum());
                pointList[i].append(_cellList[i][j]->pos());
            }
        }
    }
    else if(e->key() == Qt::Key_Right)
    {
        for(int i=0; i<CELLNUM; i++)
        {
            valueList.append(QList<int>());
            pointList.append(QList<QPoint>());
            for (int j=0; j<CELLNUM; j++)
            {
                valueList[i].append(_cellList[i][3-j]->getNum());
                pointList[i].append(_cellList[i][3-j]->pos());
            }
        }
    }
    else return;

    QParallelAnimationGroup *group = new QParallelAnimationGroup;
    for (int i=0; i<valueList.count(); i++)
    {
        for (int j=0; j<valueList[i].count()-1; )
        {
            if (valueList[i][j] == 0)
            {
                int tmp = -1;
                for (int k=j+1; k<valueList[i].count(); k++)
                {
                    if (valueList[i][k] != 0)
                    {
                        tmp = k;
                        break;
                    }
                }

                if (tmp > 0)
                {
                    valueList[i][j] = valueList[i][tmp];
                    valueList[i][tmp] = 0;
                    Cell *pTemp = _cellList[pointList[i][tmp].y()/CELLSIZE][pointList[i][tmp].x()/CELLSIZE];
                    pTemp->raise();
                    QPropertyAnimation *anim = new QPropertyAnimation(pTemp, "pos");
                    anim->setStartValue(pointList[i][tmp]);
                    anim->setEndValue(pointList[i][j]);
                    anim->setDuration(200);
                    group->addAnimation(anim);
                }
                else j++;
            }
            else
            {
                int tmp = -1;
                for (int k=j+1; k<valueList[i].count(); k++)
                {
                    if (valueList[i][k] == valueList[i][j])
                    {
                        tmp = k;
                        break;
                    }
                    else if (valueList[i][k] != 0)break;
                }

                if (tmp > 0)
                {
                    valueList[i][j] = 2*valueList[i][tmp];
                    valueList[i][tmp] = 0;

                    Cell *pTemp = _cellList[pointList[i][tmp].y()/CELLSIZE][pointList[i][tmp].x()/CELLSIZE];
                    pTemp->raise();
                    QPropertyAnimation *anim = new QPropertyAnimation(pTemp, "pos");
                    anim->setStartValue(pointList[i][tmp]);
                    anim->setEndValue(pointList[i][j]);
                    anim->setDuration(200);
                    group->addAnimation(anim);
                }
                j++;
            }
        }
    }

    if (group->animationCount() <= 0 )return;
    connect(group, &QAnimationGroup::finished, this, [=]()
    {
        for (int i=0; i<group->animationCount(); i++)
        {
            QAbstractAnimation *pAnim = group->animationAt(i);
            pAnim->deleteLater();
        }
        group->clear();
        group->deleteLater();

        for(int i=0; i<CELLNUM; i++)
        {
            for(int j=0; j<CELLNUM; j++)
            {
                _cellList[i][j]->move(j*CELLSIZE, i*CELLSIZE);
                _cellList[pointList[i][j].y()/CELLSIZE][pointList[i][j].x()/CELLSIZE]->setNum(valueList[i][j]);
            }
        }

        QList<QPair<int,int> > pairList;
        for(int i=0; i<_cellList.count(); i++)
        {
            for(int j=0; j<_cellList[i].count(); j++)
            {
                if (_cellList[i][j]->getNum() == 0)
                    pairList.append(QPair<int,int>(i,j));
            }
        }

        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        int num = qrand()%pairList.count();
        _cellList[pairList[num].first][pairList[num].second]->setNum(2);
        _bRelease = false;
    });
    _bRelease = true;
    group->start();
}
