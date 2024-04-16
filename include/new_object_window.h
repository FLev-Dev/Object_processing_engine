#ifndef NEW_OBJECT_WINDOW_H
#define NEW_OBJECT_WINDOW_H
#include "include/object_processing_engine.h"
//Qt libs
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QStringListModel>


class New_object_window : public QDialog {
    Q_OBJECT

private:
    Object_processing_engine* engine;

    QDialog* add_new_type_dialog;
    QDialog* error_new_type;
    QDialog* sucess_new_type;
    QDialog* error_new_obj;
    QLabel* new_obj_message_error;
    QDialog* sucess_new_obj;

    QLineEdit* input_object_name;
    QLineEdit* input_object_x;
    QLineEdit* input_object_y;
    QLineEdit* input_new_type;
    QStringListModel* cb_model;
    QComboBox* choose_object_type;

    void add_new_type_ui(QValidator*& text_validator);

public:
    New_object_window(Object_processing_engine* _engine);
    ~New_object_window(){delete engine;}

public slots:
    void update_types();
    void add_new_type();
    void new_object_create();

signals:
    void include_buttons();
    void change_label(bool);
};

#endif // NEW_OBJECT_WINDOW_H
