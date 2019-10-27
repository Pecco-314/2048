#ifndef BLOCK_H
#define BLOCK_H

#include <QLabel>
#include <QPainter>
#include <QDebug>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QEvent>

class Map;

class Block : public QWidget
{
    Q_OBJECT

public:
    QPropertyAnimation *ani, *ani2, *ani_exp, *ani2_exp, *ani_move;
    Block(qint32 x=-1, qint32 y=-1, qint32 value=0, QWidget* parent = nullptr, bool combinable = true);
    ~Block();
    Block* getNeighbor(Map* map, int dir);
    qint32 getValue();
    qint32 getx();
    qint32 gety();
    bool getCombinable();
    void setCombinable(bool con);
    void setValue(qint32 v);
    void unhide();
    void dlt();
    void createAni(Block* to, Map* map, bool combined);
    void combine_ani();
private:
    bool combinable;
    qint32 x, y, value;
    QLabel *piclabel;
protected:
    void paintEvent(QPaintEvent*);
};

#endif // BLOCK_H
