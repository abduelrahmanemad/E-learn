#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent, QString title, QString content) :
    QDialog(parent),
    ui(new Ui::Dialog) {
    ui->setupUi(this);
    ui->title->setText("<html><head/><body><p><span style=\"font-size:14pt; font-weight:700;\">" + title + "</span></p></body></html>");
    ui->content->setText("<html><head/><body><p><span style=\"font-size:12pt;\">" + content + "</span></p></body></html>");
}

Dialog::~Dialog() {
    delete ui;
}

void Dialog::on_yesBtn_clicked() {
    done(1);
}


void Dialog::on_noBtn_clicked() {
    done(0);
}

