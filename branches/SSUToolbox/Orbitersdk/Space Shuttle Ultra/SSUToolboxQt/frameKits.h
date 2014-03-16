#ifndef FRAMEKITS_H
#define FRAMEKITS_H

#include <QFrame>

#include <QPushButton>
#include <QVBoxLayout>

#include <vector>

using namespace std;

class Kit : public QObject
{
    Q_OBJECT

public:

    Kit(QString name, QObject *frame, QVBoxLayout *vblMods, QVBoxLayout *vblItems)
    {
        mod = false;
        item = false;
        this->name = name;
        buttonMod = new QPushButton();
        buttonMod->setText(name);
        buttonMod->setCheckable(true);
        vblMods->addWidget(buttonMod);
        connect(buttonMod, SIGNAL(clicked()), frame, SLOT(buttonModClicked()));
        buttonItem = new QPushButton();
        buttonItem->setText(name);
        buttonItem->setCheckable(true);
        vblItems->addWidget(buttonItem);
        connect(buttonItem, SIGNAL(clicked()), frame, SLOT(buttonItemClicked()));
    }

    QString name;
    bool mod;   //orbiter modification kit
    bool item;  //actual item
    QPushButton *buttonMod;
    QPushButton *buttonItem;
};

namespace Ui {
class FrameKits;
}

class FrameKits : public QFrame
{
    Q_OBJECT

public:
    explicit FrameKits(QWidget *parent = 0);
    ~FrameKits();

    void updateParameters();

    vector<Kit*> kits;

private:
    void writeToButtonStates();
    void readFromButtonsStates();

public slots:
    void buttonModClicked();
    void buttonItemClicked();

private slots:
    void on_buttonCheckConfig_clicked();

private:
    Ui::FrameKits *ui;
};

#endif // FRAMEKITS_H
