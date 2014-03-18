#ifndef MYQTWINMANAGER_H
#define MYQTWINMANAGER_H

#include <QObject>

#include <windows.h>

class MyQtWinManager : public QObject
{
    Q_OBJECT
public:

    HWND hOrbiterWin;

    explicit MyQtWinManager(QObject *parent = 0);

    void sendUpdateWindow() { emit updateWin(); }
    void sendShowWin() { emit showWin(); }

signals:
    void updateWin();
    void showWin();

};

#endif // MYQTWINMANAGER_H
