#ifndef OUTFILEDIALOG_H
#define OUTFILEDIALOG_H

#include <QDialog>

namespace Ui {
class outFileDialog;
}

class outFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit outFileDialog(QWidget *parent = 0);
    ~outFileDialog();


public slots:
    void on_okPushButton_clicked();

private:
    Ui::outFileDialog *ui;

signals:
    void getBothName(QString,QString);
};

#endif // OUTFILEDIALOG_H
