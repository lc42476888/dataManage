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
    ui->ipLabel->setVisible(false);
    ui->ipLineEdit->setVisible(false);
    ui->portLabel->setVisible(false);
    ui->portLineEdit->setVisible(false);
    connect(ui->netConnectPushButton,SIGNAL(toggled(bool)),this,SLOT(setConnectName()));
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

    QString users,pw,dbDriver,port;
    users=ui->userLineEdit->text();
    pw=ui->pwLineEdit->text();
    dbDriver=ui->databaseDriversComboBox->currentText();
    QSqlDatabase db =QSqlDatabase::addDatabase(dbDriver);

    if(ui->netConnectPushButton->isChecked())
    {
        QRegExp reg("\\b((?:(?:25[0-5]|2[0-4]\\d|[01]?\\d?\\d)\\.)"
                    "{3}(?:25[0-5]|2[0-4]\\d|[01]?\\d?\\d))\\b");
        if(!reg.exactMatch(ui->ipLineEdit->text()))
        {
            QMessageBox::warning(this,"jinggao","请输入正确的ip地址",QMessageBox::Yes);
            qDebug()<<reg.matchedLength();
        }
        else
        {
             db.setPort(ui->portLineEdit->text().toInt());
             db.setHostName(ui->ipLineEdit->text());
             db.setUserName(ui->userLineEdit->text());
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
    }
    else
    {
        db.setHostName("localhost");
        db.setUserName(ui->userLineEdit->text());
        db.setPassword(pw);
        if(!db.open())
        {
            QMessageBox::warning(this,tr("登录失败"),
                                 tr("密码错误或无法连接至数据库"),QMessageBox::Ok);
        }
        else
        {
            MainWindow *w2= new MainWindow();
            w2->show();
            close();
        }
    }


}

void Widget::setConnectName()
{
    if (ui->netConnectPushButton->isChecked())
    {
        ui->ipLabel->setVisible(true);
        ui->ipLineEdit->setVisible(true);
        ui->portLabel->setVisible(true);
        ui->portLineEdit->setVisible(true);
        ui->netConnectPushButton->setText("切换至本地登录");
    }
    else
    {
        ui->ipLabel->setVisible(false);
        ui->ipLineEdit->setVisible(false);
        ui->portLabel->setVisible(false);
        ui->portLineEdit->setVisible(false);
        ui->netConnectPushButton->setText("切换至远程连接");

    }


}

void Widget::cancelConnection()
{

    close();
}

