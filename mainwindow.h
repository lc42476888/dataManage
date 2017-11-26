#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "widget.h"
#include <QMainWindow>
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
    void showTable();
    void selectFromTable();
    void updataDisplyDatabase();
    void changeDataFromTable();
    void cancelChangeFromTable();
    void addLineFromTable();
    void deleteLineFromTable();
    void ascendOrderFromTable();
    void descendOrderFromTable();
    void checkLockTable();


private:
    Ui::MainWindow *ui;
    QString dbName;
    QSqlTableModel *myTModel;

};

#endif // MAINWINDOW_H
