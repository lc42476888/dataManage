#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "outfiledialog.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QList>
#include <QFile>
#include <QStringListModel>
#include <QSqlTableModel>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    void selectFromTable();
    void updataDisplyDatabase();
    void changeDataFromTable();
    void cancelChangeFromTable();
    void addLineFromTable();
    void deleteLineFromTable();
    void ascendOrderFromTable();
    void descendOrderFromTable();
    void checkLockTable();
    void checkChangeTable();
    void addTableReqeust();
    void delTableReqeust();
    void showCmdCommand();
    void exeCmdCommand();
    void showTable();
    void checkChangeDatabase();
    void outFile(QString userName,QString outName);

private slots:
    void on_redoTextEditPushButton_clicked();

    void on_undoTextEditPushButton_clicked();

    void on_addListPushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QString dbName;
    QSqlTableModel *myTModel;
    QList<QString> dbList;
    outFileDialog *dlg;

};

#endif // MAINWINDOW_H
