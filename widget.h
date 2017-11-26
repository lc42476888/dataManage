#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QList>
#include <QStringListModel>
#include "mainwindow.h"
//#include <QFile>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void creatConnection();
    void cancelConnection();

private:
    Ui::Widget *ui;
//    QFile *qssFile;
};

#endif // WIDGET_H
