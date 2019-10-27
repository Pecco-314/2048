#include "map.h"
#include <cstdlib>
#include <ctime>

Map::Map(QWidget* parent):QWidget(parent){
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            this->blocks[i][j] = new Block(i, j, 0, parent);
            this->blocks[i][j]->unhide();
        }
    }
}

Map::~Map(){
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            delete(this->blocks[i][j]);
        }
    }
}

Block* Map::generate(){
    Block *b = nullptr;
    srand(static_cast<unsigned>(clock()*(unsigned)time(0)));
    while(cntpos){
        b = blocks[rand()%4][rand()%4];
        if(b->getValue()==0)
            break;
    }
    b->setValue((rand()%10)?2:4);
    b->ani->start();
    b->ani2->start();
    connect(b->ani2, &QPropertyAnimation::finished, this, &Map::judge);
    --cntpos;
    return b;
}

void Map::hideall(){
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            blocks[i][j]->hide();
        }
    }
}

void Map::unhideall(){
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            blocks[i][j]->unhide();
        }
    }
}

void Map::init(){
    generate();
    generate();
}

void Map::savemap(QString filename){
    QFile save(filename);
    save.open(QIODevice::WriteOnly);
    QDataStream out(&save);
    out << pt;
    for(int i=0;i<4;++i)
        for(int j=0;j<4;++j)
            out << blocks[i][j]->getValue();
    save.close();
}

bool Map::loadmap(QString filename){
    qint32 v;
    QFile save(filename);
    if(save.exists()){
        save.open(QIODevice::ReadOnly);
        QDataStream in(&save);
        in >> pt;
        cntpos = 16;
        for(int i=0;i<4;++i){
            for(int j=0;j<4;++j){
                in >> v;
                blocks[i][j]->setValue(v);
                blocks[i][j]->ani->start();
                blocks[i][j]->ani2->start();
                if(v) cntpos--;
            }
        }
        save.close();
    }
    else return false;
    return true;
}

void Map::savebest(){
    QFile bst("best.dat");
    bst.open(QIODevice::WriteOnly);
    QDataStream out(&bst);
    out << best;
}

void Map::loadbest(){
    QFile bst("best.dat");
    if(bst.exists()){
        bst.open(QIODevice::ReadOnly);
        QDataStream in(&bst);
        in >> best;
        bst.close();
    }
}

void Map::setpts(qint32 x){
    pt = x;
    best = (x>best)?x:best;
}

bool Map::moveto(qint32 x, qint32 y, int dir){
    Block *b = blocks[x][y] , *fr = blocks[x][y];
    int value = fr->getValue();
    if(!value)return false;
    while(b->getNeighbor(this, dir)->getValue()==0){
        b = b->getNeighbor(this, dir);
    }
    if(b->getNeighbor(this, dir)->getValue()==value&&b->getCombinable()&&b->getNeighbor(this, dir)->getCombinable()){
        b = b->getNeighbor(this, dir);
        fr->createAni(b, this, true);

        b->setValue(value*2);
        fr->setValue(0);
        b->setCombinable(false);
        ++cntpos;
        setpts(pt+value*2);
        if(value==1024) win = true;
    }
    else{
        fr->createAni(b, this, false);
        if(b!=fr){
            fr->setValue(0);
            b->setValue(value);
        }
        else{
            return false;
        }
    }
    return true;
}

void Map::judge(){
    emit judge_wol();
}

bool Map::tryto(qint32 x, qint32 y, int dir){
    Block* b = blocks[x][y];
    int value = blocks[x][y]->getValue();
    if(!value)return false;
    while(b->getNeighbor(this, dir)->getValue()==0){
        b = b->getNeighbor(this, dir);
    }
    if(b->getNeighbor(this, dir)->getValue()==value){
        return true;
    }
    else{
        if(b==blocks[x][y]){
            return false;
        }
    }
    return true;
}

bool Map::move(int dir){
    bool moved = false;
    hideall();
    for(int i=0;i<4;++i)
        for(int j=0;j<4;++j)
            blocks[i][j]->setCombinable(true);
    switch(dir){
    case Qt::Key_Left:
    case Qt::Key_A:
        for(int i=0;i<4;++i){
            for(int j=0;j<4;++j){
                if(moveto(i, j, dir))moved = true;
            }
        }
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        for(int i=3;i>=0;--i){
            for(int j=0;j<4;++j){
                if(moveto(i, j, dir))moved = true;
            }
        }
        break;
    case Qt::Key_Up:
    case Qt::Key_W:
        for(int i=0;i<4;++i){
            for(int j=0;j<4;++j){
                if(moveto(j, i, dir))moved = true;
            }
        }
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        for(int i=3;i>=0;--i){
            for(int j=0;j<4;++j){
                if(moveto(j, i, dir))moved = true;
            }
        }
        break;
    default:
        unhideall();
        return false;
    }
    if(moved)generate();
    return moved;
}

bool Map::trymove(){
    bool moved = false;
    for(int i=3;i>=0;--i){
        for(int j=0;j<4;++j){
            if(tryto(i, j, Qt::Key_Up))moved = true;
            if(tryto(i, j, Qt::Key_Down))moved = true;
            if(tryto(i, j, Qt::Key_Left))moved = true;
            if(tryto(i, j, Qt::Key_Right))moved = true;
        }
    }
    return moved;
}
