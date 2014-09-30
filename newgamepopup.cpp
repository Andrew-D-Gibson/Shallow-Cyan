#include "newgamepopup.h"
#include "ui_newgamepopup.h"

newGamePopUp::newGamePopUp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newGamePopUp)
{
    ui->setupUi(this);
    ui->label->setText("<b>Choose your side:</b>");
    setWindowTitle("");
}

newGamePopUp::~newGamePopUp()
{
    delete ui;
}

void newGamePopUp::on_WhiteButton_clicked()
{
    whiteSelected = 1;
}

void newGamePopUp::on_BlackButton_clicked()
{
    whiteSelected = 0;
}

void newGamePopUp::on_buttonBox_accepted()
{
    emit colorChosen(whiteSelected);
}

void newGamePopUp::on_buttonBox_rejected()
{
    emit colorChosen(-1);
}
