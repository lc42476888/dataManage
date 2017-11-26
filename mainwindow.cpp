#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //初始化显示可用数据库
    QSqlQuery query;
    QList<QString> dbList,tableList;
    query.exec("show databases;");
    while(query.next())
    {
        dbList<<query.value(0).toString();
    }
    ui->availibleDatabaseComboBox->addItems(dbList);

    //初始化显示可用表单
    query.exec("use "+ui->availibleDatabaseComboBox->currentText()+";");
    query.exec("show tables;");
    while(query.next())
    {
        tableList<<query.value(0).toString();
    }
    ui->availibleTableComboBox->addItems(tableList);


    connect(ui->showTablePushButton,SIGNAL(clicked(bool)),this,SLOT(showTable()));
    connect(ui->availibleDatabaseComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updataDisplyDatabase()));
    connect(ui->availibleTableComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(showTable()));
    connect(ui->confirmChangePushButton,SIGNAL(clicked(bool)),this,SLOT(changeDataFromTable()));
    connect(ui->cancelChangePushButton,SIGNAL(clicked(bool)),this,SLOT(cancelChangeFromTable()));
    connect(ui->addLinePushButton,SIGNAL(clicked(bool)),this,SLOT(addLineFromTable()));
    connect(ui->deleteLinePushButton,SIGNAL(clicked(bool)),this,SLOT(deleteLineFromTable()));
    connect(ui->ascendOrderPushButton,SIGNAL(clicked(bool)),this,SLOT(ascendOrderFromTable()));
    connect(ui->descendOrderPushButton,SIGNAL(clicked(bool)),this,SLOT(descendOrderFromTable()));
    connect(ui->selectFromTablePushButton,SIGNAL(clicked(bool)),this,SLOT(selectFromTable()));
    connect(ui->lockTableCheckBox,SIGNAL(stateChanged(int)),this,SLOT(checkLockTable()));

    //初始化模型
     myTModel=new QSqlTableModel();

     //在view视图中显示详细信息
     showTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showTable()
{
    myTModel->setTable(ui->availibleTableComboBox->currentText());
    //相当于执行("select * from student")结果存在一个结果集里面
    myTModel->select();
    //设置编辑策略
    //先缓存直到调用revertAll()或者submitAll()
    myTModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //为视图设置模型
    ui->tableView->setModel(myTModel);
}

void MainWindow::checkLockTable()
{
    if(ui->lockTableCheckBox->isChecked())
    {
        ui->availibleDatabaseComboBox->setEnabled(false);
        ui->availibleTableComboBox->setEnabled(false);
    }
    else
    {
        ui->availibleDatabaseComboBox->setEnabled(true);
        ui->availibleTableComboBox->setEnabled(true);
    }
}

void MainWindow::updataDisplyDatabase()
{

    //更新可用表单
    QSqlQuery query;
    QList<QString> tableList;
    query.exec("use "+ui->availibleDatabaseComboBox->currentText()+";");
    query.exec("show tables;");
    while(query.next())
    {
        tableList<<query.value(0).toString();
    }
    ui->availibleTableComboBox->clear();
    ui->availibleTableComboBox->addItems(tableList);

    showTable();
}

void MainWindow::changeDataFromTable()
{
    //开始事务操作
    myTModel->database().transaction();

    if(myTModel->submitAll())
    {
        if(myTModel->database().commit())
        {
            QMessageBox::information(this,tr("表单"),
                                     tr("数据修改成功！"));
        }
    }
    else
    {
        //未提交成功则回滚
        myTModel->database().rollback();
        QMessageBox::information(this,tr("表单"),
                                 tr("数据库错误：%1").arg(myTModel->lastError().text()),
                                 QMessageBox::Ok);
    }
}


void MainWindow::cancelChangeFromTable()
{
    myTModel->revertAll();
}

void MainWindow::addLineFromTable()
{
    //先获取行数
    int rowNum = myTModel->rowCount();
    int id = 123456;
    myTModel->insertRow(rowNum);
    myTModel->setData(myTModel->index(rowNum,0),id);

    //需要submitAll()才有效
}


void MainWindow::deleteLineFromTable()
{

    //获取选中的行
    int rowNum =ui->tableView->currentIndex().row();
    //删除
    myTModel->removeRow(rowNum);
}

void MainWindow::ascendOrderFromTable()
{
    int column=ui->ascendOrderLineEdit->text().toInt();
    myTModel->setSort(column,Qt::AscendingOrder);
    //排序过后需要再次执行此命令，获得结果集
    myTModel->select();

}

void MainWindow::descendOrderFromTable()
{
    int column=ui->descendOrderLineEdit->text().toInt();
    myTModel->setSort(column,Qt::DescendingOrder);
    //排序过后需要再次执行此命令，获得结果集
    myTModel->select();

}

void MainWindow::selectFromTable()
{
    QString fields=ui->selectFieldsLineEdit->text();
    QString columnName=ui->selectColumnLineEdit->text();
    QString selectQuery=columnName+" = "+fields;
    myTModel->setFilter(QString (selectQuery));
    myTModel->select();
//    QString table=ui->availibleTableComboBox->currentText();
//    QString selectQuery="SELECT * from "+table+" where "+columnName+" like '%,"+
//            fields+"' or "+columnName+" like '%,"+fields+",%' or "+columnName+" like '"+
//            fields+",%' or "+columnName+" ="+fields;
//    QString selectQ="select * from user where password like '%,123%' or password like '%123%' or password like '%123,%' or password = '%,123,%';";
//    myTModel->setQuery(selectQ);

}
