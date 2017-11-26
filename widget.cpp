#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->loginPushButton,SIGNAL(clicked(bool)),this,SLOT(creatConnection()));
    connect(ui->cancelPushButton,SIGNAL(clicked(bool)),this,SLOT(cancelConnection()));
    QList<QString> availibleDatabase = QSqlDatabase::drivers();
    ui->databaseDriversComboBox->addItems(availibleDatabase);
//    qssFile = new QFile(":/qss/my.qss",this);
//    qssFile->open(QFile::ReadOnly);
//    QString style=tr(qssFile->readAll());
//    qApp->setStyleSheet(style);
//    qssFile->close();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::creatConnection()
{

    QString users,pw,dbDriver;
    users=ui->userLineEdit->text();
    pw=ui->pwLineEdit->text();
    dbDriver=ui->databaseDriversComboBox->currentText();
    QSqlDatabase db =QSqlDatabase::addDatabase(dbDriver);
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword(pw);
    if(!db.open())
    {
        QMessageBox::warning(this,tr("登录失败"),
                             tr("密码错误或该数据库不存在"),QMessageBox::Ok);
    }
    else
    {
        MainWindow *w2= new MainWindow();
        w2->show();
        close();
    }

}

void Widget::cancelConnection()
{

    close();
}
