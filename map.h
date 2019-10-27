#ifndef MAP_H
#define MAP_H

#include "block.h"
#include <QFile>
#include <QKeyEvent>
#include <QMessageBox>

class Map : public QWidget
{
    Q_OBJECT
public:
    Map(QWidget* parent = nullptr);
    ~Map();
    Block *blocks[4][4];
    qint32 pt=0, best=0, cntpos=16;
    bool win=false, lose=false, hasdialog=false;
    void init();
    Block* generate();
    bool moveto(qint32 x, qint32 y, int dir);
    bool tryto(qint32 x, qint32 y, int dir);
    bool move(int dir);
    bool trymove();
    void savebest();
    void loadbest();
    void savemap(QString filename="map.dat");
    bool loadmap(QString filename="map.dat");
    void setpts(qint32 x);
    void unhideall();
    void hideall();
    void judge();
private:
    QWidget* parent;
signals:
    void judge_wol();
};

#endif // MAP_H
