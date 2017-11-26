#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dlg=new outFileDialog;
    connect(dlg,SIGNAL(getBothName(QString,QString)),this,SLOT(outFile(QString,QString)));
    dbList.clear();
    //初始化显示可用数据库
    QSqlQuery query;
    query.exec("show databases;");
    while(query.next())
    {
        dbList<<query.value(0).toString();
    }
    ui->availibleDatabaseComboBox->addItems(dbList);

    //初始化显示可用表单
    query.exec("use "+ui->availibleDatabaseComboBox->currentText()+";");
    query.exec("show tables;");
    QList<QString> tableList;
    while(query.next())
    {
        tableList<<query.value(0).toString();
    }
    ui->availibleTableComboBox->addItems(tableList);

    //未勾选时，增删表单不可用
    ui->addTablePushButton->setEnabled(false);
    ui->delTablePushButton->setEnabled(false);
    //未勾选时，增删数据库不可用
    ui->addDatabasePushButton->setEnabled(false);
    ui->delDatabasePushButton->setEnabled(false);
    //未勾选时，命令行键入不可用
    ui->cmdCommandPushButton->setEnabled(false);
    ui->cmdTextEdit->setEnabled(false);
    ui->redoTextEditPushButton->setEnabled(false);
    ui->undoTextEditPushButton->setEnabled(false);


    connect(ui->showTablePushButton,SIGNAL(clicked(bool)),this,SLOT(updataDisplyDatabase()));
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
    connect(ui->allowChangeTableCheckBox,SIGNAL(stateChanged(int)),this,SLOT(checkChangeTable()));
    connect(ui->addTablePushButton,SIGNAL(clicked(bool)),this,SLOT(addTableReqeust()));
    connect(ui->cmdCommandPushButton,SIGNAL(clicked(bool)),this,SLOT(exeCmdCommand()));
    connect(ui->cmdCheckBox,SIGNAL(stateChanged(int)),this,SLOT(showCmdCommand()));
    connect(ui->delTablePushButton,SIGNAL(clicked(bool)),this,SLOT(delTableReqeust()));
    connect(ui->allowChangeDatabaseCheckBox,SIGNAL(stateChanged(int)),this,SLOT(checkChangeDatabase()));

    //初始化模型
     myTModel=new QSqlTableModel();

    //调用函数在view视图中显示详细信息
     //showTable();隐式调用show
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
    QList<QString> updataBaseList,uptableList;
    query.exec("show databases;");
    while(query.next())
    {
        updataBaseList<<query.value(0).toString();
    }
    while (!updataBaseList.isEmpty())
    {
        QString temp;
        temp=updataBaseList.takeLast();
        if(!dbList.contains(temp))
        {
            dbList.append(temp);
            ui->availibleDatabaseComboBox->addItem(temp);
        }
    }
    query.exec("use "+ui->availibleDatabaseComboBox->currentText()+";");
    query.exec("show tables;");
    while(query.next())
    {
        uptableList<<query.value(0).toString();
    }
    ui->availibleTableComboBox->clear();
    ui->availibleTableComboBox->addItems(uptableList);
//    while (!uptableList.isEmpty())
//    {
//        QString temp;
//        temp=uptableList.takeLast();
//        if(!tableList.contains(temp))
//        {
//            tableList.append(temp);
//            ui->availibleTableComboBox->addItem(temp);
//        }
//    }
//    showTable();
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

void MainWindow::checkChangeTable()
{
    if(ui->allowChangeTableCheckBox->isChecked())
    {
        ui->addTablePushButton->setEnabled(true);
        ui->delTablePushButton->setEnabled(true);
    }
    else
    {
        ui->addTablePushButton->setEnabled(false);
        ui->delTablePushButton->setEnabled(false);
    }
}

void MainWindow::addTableReqeust()
{
    QSqlQuery query;
    QString currentTable=ui->availibleTableComboBox->currentText();
    if(query.exec("drop table "+currentTable+";"))
    {
        QMessageBox::information(this,ui->availibleDatabaseComboBox->currentText(),
                                 tr("%1表单删除成功！").arg(currentTable));
    }
    else
    {
        QMessageBox::information(this,tr("表单"),
                                 tr("错误！错误信息：%1").arg( query.lastError().text()),
                                 QMessageBox::Ok);
    }


}

void MainWindow::delTableReqeust()
{
    if(QMessageBox::warning(this,"提示","是否要删除表单？",
                            QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
    {
        QSqlQuery query;
        QString currentTable=ui->availibleTableComboBox->currentText();
        if(query.exec("drop table "+currentTable+";"))
        {
            QMessageBox::information(this,ui->availibleDatabaseComboBox->currentText(),
                                     tr("%1表单删除成功！").arg(currentTable));
        }
        else
        {
            QMessageBox::information(this,tr("表单"),
                                     tr("错误！错误信息：%1").arg( query.lastError().text()),
                                     QMessageBox::Ok);
        }
    }

}

void MainWindow::showCmdCommand()
{
    if(ui->cmdCheckBox->isChecked())
    {
        ui->cmdCommandPushButton->setEnabled(true);
        ui->cmdTextEdit->setEnabled(true);
        ui->redoTextEditPushButton->setEnabled(true);
        ui->undoTextEditPushButton->setEnabled(true);
    }
    else
    {
        ui->cmdCommandPushButton->setEnabled(false);
        ui->cmdTextEdit->setEnabled(false);
        ui->redoTextEditPushButton->setEnabled(false);
        ui->undoTextEditPushButton->setEnabled(false);
    }
}

void MainWindow::exeCmdCommand()
{
    QString cmdStr=ui->cmdTextEdit->toPlainText();
    QSqlQuery query;
    if(query.exec(cmdStr))
    {
        QMessageBox::information(this,tr("表单"),
                                 tr("command执行成功！"));
    }
    else
    {
        QMessageBox::information(this,tr("表单"),
                                 tr("错误！错误信息：%1").arg( query.lastError().text()),
                                 QMessageBox::Ok);
    }
}

void MainWindow::on_redoTextEditPushButton_clicked()
{
    ui->cmdTextEdit->redo();
}

void MainWindow::on_undoTextEditPushButton_clicked()
{
    ui->cmdTextEdit->undo();
}

void MainWindow::checkChangeDatabase()
{
    if(ui->allowChangeTableCheckBox->isChecked())
    {
        ui->addDatabasePushButton->setEnabled(false);
        ui->delDatabasePushButton->setEnabled(false);
    }
    else
    {

        ui->addDatabasePushButton->setEnabled(true);
        ui->delDatabasePushButton->setEnabled(true);

    }
}

void MainWindow::on_addListPushButton_clicked()
{
    QString addName,addType,cmdQuery;
    addName=ui->addColNameLineEdit->text();
    addType=ui->addColTypelineEdit->text();
    cmdQuery="alter table "+ui->availibleTableComboBox->currentText()+" add "+addName+" "+addType+";";
    QSqlQuery  query;
    if(query.exec(cmdQuery))
    {
        QMessageBox::information(this,tr("表单"),
                                 tr("command执行成功！"));
    }
    else
    {
        QMessageBox::information(this,tr("表单"),
                                 tr("错误！错误信息：%1").arg( query.lastError().text()),
                                 QMessageBox::Ok);
    }

}

//mysqldump -u user_name -p database_name table_name > outfile_name.sql

void MainWindow::on_pushButton_2_clicked()
{
    dlg->show();
}

void MainWindow::outFile(QString userName,QString outName)
{
    QString tableName,dbName,outFileName,user,cmdQuery;
    tableName=ui->availibleTableComboBox->currentText();
    dbName=ui->availibleDatabaseComboBox->currentText();
    user=userName;
    outFileName=outName;
    cmdQuery="mysqldump -u "+user+" -p "+dbName+" "+tableName+" > "+outFileName+";";
    QSqlQuery query;
        if(query.exec(cmdQuery))
        {
            QMessageBox::information(this,tr("表单"),
                                     tr("输出执行成功！"));
        }
        else
        {
            QMessageBox::information(this,tr("表单"),
                                     tr("错误！错误信息：%1").arg( query.lastError().text()),
                                     QMessageBox::Ok);
        }
}
