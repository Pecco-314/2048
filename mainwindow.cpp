#include "mainwindow.h"
#include <QApplication>

void MainWindow::newGame(bool load){
    if(map!=nullptr) delete(map);
    map = new Map(this);
    connect(map, &Map::judge_wol, this, &MainWindow::wol);
    if(load){
        if(!quickLoad()){
            map->init();
        }
    }
    else{
        map->init();
        if(autosave->isChecked()) quickSave();
    }
    initPoints();
}

void MainWindow::edit(){
    QFile edits("edits.dat");
    edits.open(QIODevice::WriteOnly);
    QDataStream out(&edits);
    out << autosave->isChecked();
    edits.close();
}

bool MainWindow::readEdits(){
    bool e;
    QFile edits("edits.dat");
    if(!edits.exists())return true;
    edits.open(QIODevice::ReadOnly);
    QDataStream in(&edits);
    in >> e;
    edits.close();
    return e;
}

void MainWindow::openAbouts(){
    QMessageBox::about(this, tr("关于2048"),tr("<html><strong>2048 Desktop 1.1.3</strong><br/>原作者：Gabriele Cirulli<br/>开发者：Pecco<br/>电子邮件：pecco@qq.com</html>"));
}

void MainWindow::openHTP(){
    QMessageBox::about(this, tr("如何游戏"),tr("    2048是意大利人Gabriele Cirulli于2014年3月发明的小游戏。"
                                           "\n\n    游戏开始时，棋盘上会随机生成2个所带数字为2或4的方块。"
                                           "通过键盘控制，你可以让所有方块同时往某个方向移动。"
                                           "如果两个相同的方块在移动过程中碰撞，则会结合成一个方块，且所带的数字变成两者之和。"
                                           "每次移动后，会在空白处再随机生成一个数字为2或4的方块。"
                                           "\n\n    若再无可移动的方块，则游戏失败。"
                                           "若得到数字为2048的方块，则游戏胜利。"
                                           "游戏胜利后，仍然可以继续游玩，直到再无可移动的方块为止。"));
}

bool MainWindow::quickLoad(){
    return map->loadmap();
}

void MainWindow::load(){
    QString filename = QFileDialog::getOpenFileName(this, tr("加载..."), "", tr("数据文件 (*.dat)"));
    map->loadmap(filename);
    initPoints();
}

void MainWindow::quickSave(){
    map->savemap();
    initPoints();
}

void MainWindow::save(){
    QString filename = QFileDialog::getSaveFileName(this, tr("保存到..."), "", tr("数据文件 (*.dat)"));
    map->savemap(filename);
}

void MainWindow::initMenu(){
    menubar = menuBar();
    menubar->setGeometry(0, 0, this->width(), 30);

    game = menubar->addMenu(tr("游戏(&G)"));
    connect(game, &QMenu::aboutToShow, game, &QMenu::grabKeyboard);
    connect(game, &QMenu::aboutToHide, this, &QMainWindow::grabKeyboard);

    newgame = game->addAction(tr("新游戏(&N)"));
    newgame->setShortcut(QKeySequence(QString("F5")));
    connect(newgame, &QAction::triggered, this, &MainWindow::newGame);

    loadgame = game->addAction(tr("加载(&L)"));
    loadgame->setShortcut(QKeySequence(QString("Ctrl+L")));
    connect(loadgame, &QAction::triggered, this, &MainWindow::load);

    quickload = game->addAction(tr("快速加载"));
    quickload->setShortcut(QKeySequence(QString("Ctrl+Shift+L")));
    connect(quickload, &QAction::triggered, this, &MainWindow::quickLoad);

    game->addSeparator();

    savegame = game->addAction(tr("保存(&S)"));
    savegame->setShortcut(QKeySequence::Save);
    connect(savegame, &QAction::triggered, this, &MainWindow::save);

    quicksave = game->addAction(tr("快速保存"));
    quicksave->setShortcut(QKeySequence(QString("Ctrl+Shift+S")));
    connect(quicksave, &QAction::triggered, this, &MainWindow::quickSave);

    autosave = game->addAction(tr("自动保存"));
    autosave->setCheckable(true);
    autosave->setChecked(readEdits());
    connect(autosave, &QAction::changed, this, &MainWindow::edit);

    game->addSeparator();

    game->addAction(tr("退出(&Q)"), QApplication::quit, QKeySequence(QString("Ctrl+Q")));

    help = menubar->addMenu(tr("帮助(&H)"));
    connect(help, &QMenu::aboutToShow, help, &QMenu::grabKeyboard);
    connect(help, &QMenu::aboutToHide, this, &QMainWindow::grabKeyboard);

    howtoplay = help->addAction(tr("如何游戏(&H)"));
    connect(howtoplay, &QAction::triggered, this, &MainWindow::openHTP);

    about = help->addAction(tr("关于(&A)..."));
    connect(about, &QAction::triggered, this, &MainWindow::openAbouts);
}

void MainWindow::initPoints(){
    map->loadbest();
    ptlabel->setText(QString::number(map->pt));
    ptlabel->setGeometry(360, 645, 500, 325);
    ptlabel->setFont(QFont("方正粗圆_GBK", 30));
    ptlabel->setAlignment(Qt::AlignLeft);

    bestlabel->setText(QString::number(map->best));
    bestlabel->setGeometry(360, 740, 500, 325);
    bestlabel->setFont(QFont("方正粗圆_GBK", 30));
    bestlabel->setAlignment(Qt::AlignLeft);

    QPalette pa;
    pa.setColor(QPalette::WindowText, QColor("#776E65"));
    ptlabel->setPalette(pa);
    bestlabel->setPalette(pa);
}

void MainWindow::initFonts(){
    fontlist.clear();
    QFontDatabase::addApplicationFont(":/fonts/FangZhengCuYuan.ttf");
    QFontDatabase::addApplicationFont(":/fonts/ClearSans-Medium.ttf");
}

void MainWindow::paintEvent(QPaintEvent*){
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush("#DDD1C4"));
    painter.drawRect(0, 0, this->width(), this->height());
    painter.setBrush(QBrush("#BBADA0"));
    painter.drawRoundedRect(25, 55, 560, 560, 20, 20);
    painter.setBrush(QBrush("#CCC0B3"));
    for(int i=45;i<=450;i+=135)
        for(int j=75;j<=480;j+=135)
            painter.drawRoundedRect(i, j, 115, 115, 5, 5);
    painter.setPen(QColor("#776E65"));
    painter.setFont(QFont("方正粗圆_GBK", 30));
    painter.drawText(170, 690, tr("分数"));
    painter.drawText(170, 785, tr("最高分"));
}

void MainWindow::wol(){
    if(map->win){
        map->win = false;
        QMessageBox::StandardButton reply = QMessageBox::information(this, tr("你赢了！"), tr("要继续游戏吗？"), QMessageBox::Yes|QMessageBox::No);
        if(reply==QMessageBox::No)newGame();
    }
    if(map->cntpos==0&&!map->hasdialog){
        if(map->trymove()==false){
            map->lose = true;
            map->hasdialog = true;
            QMessageBox::StandardButton reply = QMessageBox::information(this, tr("你输了！"), tr("要重新开始吗？"), QMessageBox::Yes|QMessageBox::No);
            if(reply==QMessageBox::Yes)newGame();
            else map->hasdialog = false;
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    int key = event->key();
    if(key==Qt::Key_Control||key==Qt::Key_Alt||key==Qt::Key_Shift)
        return;
    map->move(key);
    ptlabel->setText(QString::number(map->pt));
    bestlabel->setText(QString::number(map->best));
    map->savebest();
    if(autosave->isChecked()) quickSave();
    map->judge();
}

void MainWindow::keyReleaseEvent(QKeyEvent*){}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setMinimumSize(610,840);
    setMaximumSize(610,840);
    setWindowTitle(tr("2048"));
    setWindowIcon(QIcon(":/icons/2048.ico"));
    initFonts();

    ptlabel = new QLabel(this);
    bestlabel = new QLabel(this);
    newGame(true);
    initMenu();

    this->grabKeyboard();

}

MainWindow::~MainWindow()
{
}

