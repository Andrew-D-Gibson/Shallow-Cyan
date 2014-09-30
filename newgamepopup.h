#ifndef NEWGAMEPOPUP_H
#define NEWGAMEPOPUP_H

#include <QDialog>

namespace Ui {
class newGamePopUp;
}

class newGamePopUp : public QDialog
{
    Q_OBJECT

public:
    explicit newGamePopUp(QWidget *parent = 0);
    ~newGamePopUp();


private slots:

    void on_WhiteButton_clicked();

    void on_BlackButton_clicked();

    void on_buttonBox_accepted();


    void on_buttonBox_rejected();

signals:

    void colorChosen(int white);


private:
    Ui::newGamePopUp *ui;

    bool whiteSelected = 1;
};

#endif // NEWGAMEPOPUP_H
