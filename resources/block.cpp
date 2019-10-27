#include "map.h"

Block::Block(qint32 x, qint32 y, qint32 value, QWidget* parent, bool combinable):QWidget(parent), x(x), y(y), value(value), combinable(combinable){
    setGeometry(45+x*135, 75+y*135, 115, 115);
    piclabel = new QLabel(this);
    piclabel->setAlignment(Qt::AlignCenter);
    piclabel->setGeometry(0, 0, 115, 115);

    ani = new QPropertyAnimation(this, "geometry");
    ani->setDuration(100);
    ani->setStartValue(QRect(102+x*135, 132+y*135, 0, 0));
    ani->setEndValue(QRect(45+x*135, 75+y*135, 115, 115));

    ani2 = new QPropertyAnimation(piclabel, "geometry");
    ani2->setDuration(100);
    ani2->setStartValue(QRect(0, 0, 0, 0));
    ani2->setEndValue(QRect(0, 0, 115, 115));

    ani_exp = new QPropertyAnimation(this, "geometry");
    ani_exp->setDuration(100);
    ani_exp->setStartValue(QRect(45+x*135, 75+y*135, 115, 115));
    ani_exp->setKeyValueAt(0.5, QRect(35+x*135, 65+y*135, 135, 135));
    ani_exp->setEndValue(QRect(45+x*135, 75+y*135, 115, 115));
    ani_exp->setEasingCurve(QEasingCurve::InOutSine);

    ani2_exp = new QPropertyAnimation(piclabel, "geometry");
    ani2_exp->setDuration(100);
    ani2_exp->setStartValue(QRect(0, 0, 115, 115));
    ani2_exp->setKeyValueAt(0.5, QRect(10, 10, 115, 115));
    ani2_exp->setEndValue(QRect(0, 0, 115, 115));
    ani2_exp->setEasingCurve(QEasingCurve::InOutSine);
}

Block::~Block(){
    delete(piclabel);
}

void Block::dlt(){
    delete(this);
}

void Block::createAni(Block* to, Map* map, bool combined){
    Block *newb = new Block(x, y, value, parentWidget());
    newb->unhide();
    newb->ani_move = new QPropertyAnimation(newb, "geometry");
    newb->ani_move->setDuration(100);
    newb->ani_move->setStartValue(QRect(x*135+45, y*135+75, 115, 115));
    newb->ani_move->setEndValue(QRect(to->getx()*135+45, to->gety()*135+75, 115, 115));
    connect(newb->ani_move, &QPropertyAnimation::finished, map, &Map::unhideall);
    if(combined){
        Block *newbt = new Block(to->getx(), to->gety(), to->getValue(), parentWidget());
        connect(newb->ani_move, &QPropertyAnimation::finished, newbt, &Block::combine_ani);
        connect(newbt->ani_exp, &QPropertyAnimation::finished, newb, &Block::dlt);
        connect(newbt->ani_exp, &QPropertyAnimation::finished, newbt, &Block::dlt);
    }
    else{
        connect(newb->ani_move, &QPropertyAnimation::finished, newb, &Block::dlt);
    }
    newb->ani_move->start();
}

void Block::combine_ani(){
    value *= 2;
    unhide();
    ani_exp->start();
    ani2_exp->start();
}

Block* Block::getNeighbor(Map* map, int dir){
    Block* b = new Block(-1, -1, -1);
    switch(dir){
    case Qt::Key_Left:
    case Qt::Key_A:
        if(x)
            b = map->blocks[x-1][y];
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        if(x!=3)
            b = map->blocks[x+1][y];
        break;
    case Qt::Key_Up:
    case Qt::Key_W:
        if(y)
            b = map->blocks[x][y-1];
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        if(y!=3)
            b = map->blocks[x][y+1];
        break;
    }
    return b;
}

qint32 Block::getValue(){
    return value;
}

qint32 Block::getx(){
    return x;
}

qint32 Block::gety(){
    return y;
}

bool Block::getCombinable(){
    return combinable;
}

void Block::setValue(qint32 v){
    value = v;
    update();
}

void Block::setCombinable(bool con){
    combinable = con;
}

void Block::unhide(){
    show();
    piclabel->show();
}

void Block::paintEvent(QPaintEvent*){
    if(x==-1)return;
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    switch(value){
    case 2:
        painter.setBrush(QBrush("#EEE4DA"));
        piclabel->setFont(QFont("Clear Sans Medium", 25));
        piclabel->setText("<h2><font color=#776E65>2</font></h2>");
        break;
    case 4:
        painter.setBrush(QBrush("#EDE0C8"));
        piclabel->setFont(QFont("Clear Sans Medium", 25));
        piclabel->setText("<h2><font color=#776E65>4</font></h2>");
        break;
    case 8:
        painter.setBrush(QBrush("#F2BE78"));
        piclabel->setFont(QFont("Clear Sans Medium", 25));
        piclabel->setText("<h2><font color=#F9F6F2>8</font></h2>");
        break;
    case 16:
        painter.setBrush(QBrush("#F59563"));
        piclabel->setFont(QFont("Clear Sans Medium", 23));
        piclabel->setText("<h2><font color=#F9F6F2>16</font></h2>");
        break;
    case 32:
        painter.setBrush(QBrush("#F67C5F"));
        piclabel->setFont(QFont("Clear Sans Medium", 23));
        piclabel->setText("<h2><font color=#F9F6F2>32</font></h2>");
        break;
    case 64:
        painter.setBrush(QBrush("#F65E3B"));
        piclabel->setFont(QFont("Clear Sans Medium", 23));
        piclabel->setText("<h2><font color=#F9F6F2>64</font></h2>");
        break;
    case 128:
        painter.setBrush(QBrush("#EDCF72"));
        piclabel->setFont(QFont("Clear Sans Medium", 20));
        piclabel->setText("<h2><font color=#F9F6F2>128</font></h2>");
        break;
    case 256:
        painter.setBrush(QBrush("#EDCC61"));
        piclabel->setFont(QFont("Clear Sans Medium", 20));
        piclabel->setText("<h2><font color=#F9F6F2>256</font></h2>");
        break;
    case 512:
        painter.setBrush(QBrush("#EDC850"));
        piclabel->setFont(QFont("Clear Sans Medium", 20));
        piclabel->setText("<h2><font color=#F9F6F2>512</font></h2>");
        break;
    case 1024:
        painter.setBrush(QBrush("#EDC53F"));
        piclabel->setFont(QFont("Clear Sans Medium", 16));
        piclabel->setText("<h2><font color=#F9F6F2>1024</font></h2>");
        break;
    case 2048:
        painter.setBrush(QBrush("#EEC22E"));
        piclabel->setFont(QFont("Clear Sans Medium", 16));
        piclabel->setText("<h2><font color=#F9F6F2>2048</font></h2>");
        break;
    case 4096:
        painter.setBrush(QBrush("#3D3A33"));
        piclabel->setFont(QFont("Clear Sans Medium", 16));
        piclabel->setText("<h2><font color=#F9F6F2>4096</font></h2>");
        break;
    case 8192:
        painter.setBrush(QBrush("#3D3A33"));
        piclabel->setFont(QFont("Clear Sans Medium", 16));
        piclabel->setText("<h2><font color=#F9F6F2>8192</font></h2>");
        break;
    case 16384:
        painter.setBrush(QBrush("#3D3A33"));
        piclabel->setFont(QFont("Clear Sans Medium", 13));
        piclabel->setText("<h2><font color=#F9F6F2>16384</font></h2>");
        break;
    case 32768:
        painter.setBrush(QBrush("#3D3A33"));
        piclabel->setFont(QFont("Clear Sans Medium", 13));
        piclabel->setText("<h2><font color=#F9F6F2>32786</font></h2>");
        break;
    case 65536:
        painter.setBrush(QBrush("#3D3A33"));
        piclabel->setFont(QFont("Clear Sans Medium", 13));
        piclabel->setText("<h2><font color=#F9F6F2>65536</font></h2>");
        break;
    case 131072:
        painter.setBrush(QBrush("#3D3A33"));
        piclabel->setFont(QFont("Clear Sans Medium", 11));
        piclabel->setText("<h2><font color=#F9F6F2>131072</font></h2>");
        break;
    default:
        painter.setBrush(Qt::NoBrush);
        piclabel->setText("");
        break;
    }
    painter.drawRoundedRect(0, 0, width(), width(), 5, 5);
}
