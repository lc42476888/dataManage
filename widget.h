#ifndef WIDGET_H
#define WIDGET_H
#include <QDebug>
#include <QWidget>
#include <QRegExp>
#include "mainwindow.h"

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
    void setConnectName();

private:
    Ui::Widget *ui;
//    QFile *qssFile;
};

#endif // WIDGET_H
