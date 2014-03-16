#include "frameKits.h"
#include "ui_frameKits.h"

FrameKits::FrameKits(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameKits)
{
    ui->setupUi(this);

    kits.push_back(new Kit("RMS", this, ui->layoutMods, ui->layoutItems));
        kits.back()->mod = true;
        kits.back()->item = true;
    kits.push_back(new Kit("ODS", this, ui->layoutMods, ui->layoutItems));
        kits.back()->mod = true;
        kits.back()->item = true;
    kits.push_back(new Kit("OMS", this, ui->layoutMods, ui->layoutItems));
        kits.back()->mod = true;
        kits.back()->item = true;
    kits.push_back(new Kit("Centaur", this, ui->layoutMods, ui->layoutItems));
    kits.push_back(new Kit("EDO", this, ui->layoutMods, ui->layoutItems));
    kits.push_back(new Kit("KUBand", this, ui->layoutMods, ui->layoutItems));
        kits.back()->mod = true;
        kits.back()->item = true;

    writeToButtonStates();
}

FrameKits::~FrameKits()
{
    delete ui;
}

void FrameKits::buttonModClicked()
{
    for(unsigned int i=0;i<kits.size();i++)
        if(!kits[i]->buttonMod->isChecked())
            kits[i]->buttonItem->setChecked(false);
}

void FrameKits::buttonItemClicked()
{
    for(unsigned int i=0;i<kits.size();i++)
    {
        if(kits[i]->buttonItem->isChecked())
            kits[i]->buttonMod->setChecked(true);
        else kits[i]->buttonMod->setChecked(false);
    }
}

void FrameKits::writeToButtonStates()
{
    for(unsigned int i=0;i<kits.size();i++)
    {
        kits[i]->buttonMod->setChecked(kits[i]->mod);
        kits[i]->buttonItem->setChecked(kits[i]->item);
    }
}

void FrameKits::readFromButtonsStates()
{
    for(unsigned int i=0;i<kits.size();i++)
    {
        kits[i]->mod = kits[i]->buttonMod->isChecked();
        kits[i]->item = kits[i]->buttonItem->isChecked();
    }
}

void FrameKits::on_buttonCheckConfig_clicked()
{
    readFromButtonsStates();

    for(unsigned int i=0;i<kits.size();i++)
    {
        //check incompatibilities
    }
}

void FrameKits::updateParameters()
{
    on_buttonCheckConfig_clicked();
}
