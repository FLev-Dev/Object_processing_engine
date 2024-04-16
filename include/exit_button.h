#ifndef EXIT_BUTTON_H
#define EXIT_BUTTON_H
//Qt libs
#include <QPushButton>
#include <QObject>
#include <QKeyEvent>

class Exit_button: public QPushButton {
    Q_OBJECT

private:
    QDialog* dialog;
    QPushButton* background;

protected:
    void show_dialog();
    void delete_dialog();

public:
    explicit Exit_button(const QString &text, QWidget *parent = nullptr);

};

#endif // EXIT_BUTTON_H
