#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent, QString title, QString content);

    ~Dialog();
    Ui::Dialog *ui;

private slots:

    void on_yesBtn_clicked();

    void on_noBtn_clicked();

private:
};

#endif
