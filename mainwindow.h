#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QFontDatabase>
#include "map.h"

class Pt : public QLabel{
    public:
        Pt(QWidget* parent = nullptr);
        ~Pt();
};

class Best : public QLabel{
    public:
        Best(QWidget* parent = nullptr);
        ~Best();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void openAbouts();
    void openHTP();
    void newGame(bool load=false);
    void load();
    void quickLoad();
    void save();
    void quickSave();
    void initMenu();
    void initPoints();
    void edit();
    void initFonts();
    bool readEdits();
    void wol();

private:
    Map* map = nullptr;
    QLabel *ptlabel, *bestlabel;
    bool autosaving = true;
    QStringList fontlist;
    QMenuBar* menubar;
    QMenu* game;
    QAction* newgame;
    QAction* loadgame;
    QAction* quickload;
    QAction* savegame;
    QAction* quicksave;
    QAction* autosave;
    QAction* quit;
    QMenu* help;
    QAction* about;
    QAction* howtoplay;

protected:
    void paintEvent(QPaintEvent*);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent*);
};
#endif // MAINWINDOW_H
