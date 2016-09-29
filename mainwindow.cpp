#include "mainwindow.h"
#include "constdefine.h"
#include <QKeyEvent>
#include <QDebug>
#include "cell.h"
#include <QTime>
#include <QPropertyAnimation>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
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
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    QList<QList<Cell *> > cellTemp;
    if (e->key() == Qt::Key_Up)
    {
        for(int i=0; i<CELLNUM; i++)
        {
            cellTemp.append(QList<Cell*>());
            for (int j=0; j<CELLNUM; j++)
            {
                cellTemp[i].append(_cellList[j][i]);
            }
        }
    }
    else if(e->key() == Qt::Key_Down)
    {
        for(int i=0; i<CELLNUM; i++)
        {
            cellTemp.append(QList<Cell*>());
            for (int j=CELLNUM-1; j>=0; j--)
            {
                cellTemp[i].append(_cellList[j][i]);
            }
        }
    }
    else if(e->key() == Qt::Key_Left)
    {
        for(int i=0; i<CELLNUM; i++)
        {
            cellTemp.append(QList<Cell*>());
            for (int j=0; j<CELLNUM; j++)
            {
                cellTemp[i].append(_cellList[i][j]);
            }
        }
    }
    else if(e->key() == Qt::Key_Right)
    {
        for(int i=0; i<CELLNUM; i++)
        {
            cellTemp.append(QList<Cell*>());
            for (int j=CELLNUM-1; j>=0; j--)
            {
                cellTemp[i].append(_cellList[i][j]);
            }
        }
    }

    if (cellTemp.count() >0 )moveOper(cellTemp);
//    if (bMove)
//    {
//        QList<QPair<int,int> > pairList1;
//        QList<QPair<int,int> > pairList2;
//        QList<QPair<int,int> > pairList3;
//        for(int i=CELLNUM-1; i>=0; i--)
//        {
//            for(int j=CELLNUM-1; j>=0; j--)
//            {
//                if(_cellList[j][i]->getNum() == 0)
//                {
//                    if((i==0 && j==0) || (i==CELLNUM-1 && j==0) || (i==0 && j==CELLNUM-1) || (i==CELLNUM-1 && j==CELLNUM-1))
//                    {
//                        pairList1.append(QPair<int,int>(j,i));
//                    }
//                    else if (i==0 || i==CELLNUM-1 || j==0 || j==CELLNUM-1)
//                    {
//                        pairList2.append(QPair<int,int>(j,i));
//                    }
//                    else
//                    {
//                        pairList3.append(QPair<int,int>(j,i));
//                    }
//                }
//            }
//        }

//        QTime time= QTime::currentTime();
//        qsrand(time.msec()+time.second()*1000);
//        if(pairList1.count() == 0 && pairList2.count() == 0 && pairList3.count() == 0)
//        {

//        }
//        else if(pairList1.count() != 0)
//        {
//            int nTemp = qrand()%pairList1.count();
//            _cellList[pairList1[nTemp].first][pairList1[nTemp].second]->setNum(2);
//        }
//        else if(pairList2.count() != 0)
//        {
//            int nTemp = qrand()%pairList2.count();
//            _cellList[pairList2[nTemp].first][pairList2[nTemp].second]->setNum(2);
//        }
//        else if(pairList3.count() != 0)
//        {
//            int nTemp = qrand()%pairList3.count();
//            _cellList[pairList3[nTemp].first][pairList3[nTemp].second]->setNum(2);
//        }
//    }
}

void MainWindow::movieOper(const moveSut &sut)
{
    Cell *pTemp = new Cell(this, sut.value1);
    pTemp->show();

    QPropertyAnimation *pAnimat = new QPropertyAnimation(pTemp, "pos", pTemp);
    pAnimat->setDuration(200);
    pAnimat->setStartValue(QPoint(sut.x1, sut.y1));
    pAnimat->setEndValue(QPoint(sut.x2, sut.y2));
    _cellList[sut.y1/CELLSIZE][sut.x1/CELLSIZE]->setNum(0);
    _cellList[sut.y2/CELLSIZE][sut.x2/CELLSIZE]->setNum(sut.value2);
    connect(pAnimat, &QPropertyAnimation::finished, this, [=]()
    {
        _cellList[sut.y2/CELLSIZE][sut.x2/CELLSIZE]->setNum(sut.value2+sut.value1);
        pTemp->deleteLater();
    });

    pAnimat->start();
}

void MainWindow::moveOper(QList<QList<Cell *> > cellList)
{
    int num[CELLNUM][CELLNUM];
    QList<QList<int> > numList;
    for (int i=0; i<CELLNUM; i++)
    {
        numList<<QList<int>();
        for (int j =0; j<CELLNUM; j++)
        {
            num[i][j] = cellList[i][j]->getNum();
            numList[i]<<0;
        }
    }

    bool bMove = false;
    for (int i=0; i<CELLNUM; i++)
    {
        for(int j=1; j<CELLNUM; j++)
        {
            for (int k=j-1; k>=0; k--)
            {
                if(num[i][j] == 0)break;
                if(num[i][k] == 0)
                {
                    if(k == 0 || (k>=1 && num[i][k-1] != 0 && num[i][k-1] != num[i][j]))
                    {
                        bMove = true;
                        num[i][k] = num[i][j];
                        num[i][j] = 0;
                        movieOper(moveSut(cellList[i][j]->x(),cellList[i][j]->y(),cellList[i][k]->x(),cellList[i][k]->y(),num[i][k],0));
                        break;
                    }
                    else continue;
                }
                if (num[i][j] != num[i][k])break;
                bMove = true;
                num[i][j] = 0;
                num[i][k] = 2*num[i][k];
                movieOper(moveSut(cellList[i][j]->x(),cellList[i][j]->y(),cellList[i][k]->x(),cellList[i][k]->y(),num[i][k]/2,num[i][k]/2));
                break;
            }
        }
    }

    for (int i=0; i<CELLNUM; i++)
    {
        for (int j=0; j<CELLNUM; j++)
        {
            numList[cellList[i][j]->y()/CELLSIZE][cellList[i][j]->x()/CELLSIZE] = num[i][j];
        }
    }

    if(bMove) nextPoint(numList);
}

void MainWindow::nextPoint(QList<QList<int> > numList)
{
    {
//        QList<QPair<int,int> > pairList1;
//        QList<QPair<int,int> > pairList2;
        QList<QPair<int,int> > pairList3;
        for(int i=CELLNUM-1; i>=0; i--)
        {
            for(int j=CELLNUM-1; j>=0; j--)
            {
                if(numList[i][j] == 0)
                {
//                    if((i==0 && j==0) || (i==CELLNUM-1 && j==0) || (i==0 && j==CELLNUM-1) || (i==CELLNUM-1 && j==CELLNUM-1))
//                    {
//                        pairList1.append(QPair<int,int>(i,j));
//                    }
//                    else if (i==0 || i==CELLNUM-1 || j==0 || j==CELLNUM-1)
//                    {
//                        pairList2.append(QPair<int,int>(i,j));
//                    }
//                    else
                    {
                        pairList3.append(QPair<int,int>(i,j));
                    }
                }
            }
        }

        QTime time= QTime::currentTime();
        qsrand(time.msec()+time.second()*1000);
        if(/*pairList1.count() == 0 && pairList2.count() == 0 && */pairList3.count() == 0)
        {

        }
//        else if(pairList1.count() != 0)
//        {
//            int nTemp = qrand()%pairList1.count();
//            _cellList[pairList1[nTemp].first][pairList1[nTemp].second]->setNum(2);
//        }
//        else if(pairList2.count() != 0)
//        {
//            int nTemp = qrand()%pairList2.count();
//            _cellList[pairList2[nTemp].first][pairList2[nTemp].second]->setNum(2);
//        }
        else if(pairList3.count() != 0)
        {
            int nTemp = qrand()%pairList3.count();
            _cellList[pairList3[nTemp].first][pairList3[nTemp].second]->setNum(2);
        }
    }

}
