#ifndef ABOUTFORM_H
#define ABOUTFORM_H

#include <QtGui/QWidget>

namespace Ui {
class AboutForm;
}

class AboutForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit AboutForm(QWidget *parent = 0);
    ~AboutForm();
    
protected:
    void changeEvent(QEvent *e);
    
private:
    Ui::AboutForm *ui;
};

#endif // ABOUTFORM_H
