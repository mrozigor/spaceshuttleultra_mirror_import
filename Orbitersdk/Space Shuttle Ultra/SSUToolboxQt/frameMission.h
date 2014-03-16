#ifndef FRAMEMISSION_H
#define FRAMEMISSION_H

#include <QFrame>

namespace Ui {
class FrameMission;
}

class FrameMission : public QFrame
{
    Q_OBJECT

public:

    enum Orbiter{
        NotSSUOrbiter=0, Atlantis, Challenger, Columbia, Discovery, Enterprise, Endeavour
    };

    static const QString orbiterNames[7];

    explicit FrameMission(QWidget *parent = 0);
    ~FrameMission();

private:
    Ui::FrameMission *ui;

signals:
    void generateFiles();
private slots:
    void on_buttonMakeFiles_clicked();

public:
    void updateParameters();

    QString missionName;
    Orbiter orbiter;
};

#endif // FRAMEMISSION_H
