#include "outfiledialog.h"
#include "ui_outfiledialog.h"

outFileDialog::outFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::outFileDialog)
{
    ui->setupUi(this);
}

outFileDialog::~outFileDialog()
{
    delete ui;
}


void outFileDialog::on_okPushButton_clicked()
{
    QString uName,ofName;
    uName=ui->userNameLineEdit->text();
    ofName=ui->outFileNameLineEdit->text()+".sql";
    emit getBothName(uName,ofName);
    close();
}
