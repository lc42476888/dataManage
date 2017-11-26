#include "addtabledialog.h"
#include "ui_addtabledialog.h"

addTableDialog::addTableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addTableDialog)
{
    ui->setupUi(this);
    //QList<QString> tableList<<"int"<<"QString";
    //ui->dataTypeCmboBox1->addItems(tableList);
}

addTableDialog::~addTableDialog()
{
    delete ui;
}
