#ifndef DATA_VIEW_H
#define DATA_VIEW_H
#include "include/object_processing_engine.h"
//Qt libs
#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLayout>

class Data_view : public QWidget
{
    Q_OBJECT

    Object_processing_engine* engine;
    QWidget* table_parent;
    QGridLayout* main_layout;
    QGridLayout* table_layout;

    void create_table(QTableWidget* table, const std::vector<Object>& objects);

public:
    QPushButton* back_button;

    explicit Data_view(Object_processing_engine* _engine, QWidget *parent = nullptr);
    virtual ~Data_view(){delete engine;}

public slots:
    void set_tabs();
    void update_sort();
    void show_data_view();
};

#endif // DATA_VIEW_H
