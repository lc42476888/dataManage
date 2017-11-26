#ifndef ADDTABLEDIALOG_H
#define ADDTABLEDIALOG_H

#include <QDialog>

namespace Ui {
class addTableDialog;
}

class addTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addTableDialog(QWidget *parent = 0);
    ~addTableDialog();

private:
    Ui::addTableDialog *ui;
};

#endif // ADDTABLEDIALOG_H
