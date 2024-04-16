#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include "include/object_processing_engine.h"
//Std libs
#include <vector>
#include <iostream>
//Qt libs
#include <QWidget>
#include <QDialog>


class File_manager: public QObject {
    Q_OBJECT

    QDialog* error_dialog;
    QDialog* sucess_dialog;

    std::string default_file = "data.txt";

    Object_processing_engine* engine;

public:
    explicit File_manager(Object_processing_engine* _engine, QWidget *parent = nullptr);
    virtual ~File_manager(){delete engine;}

    bool read_from_file(std::string file_name = "");

    void set_default_file(std::string new_file);
    std::string get_default_file();

    void add_to_log(std::string log_record);
    void add_to_log(QString log_record);

public slots:
    void save_to_file();

signals:
    void include_buttons();
    void change_label(bool);

};

#endif // FILE_MANAGER_H
