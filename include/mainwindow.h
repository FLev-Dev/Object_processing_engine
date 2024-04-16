#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "include/data_view.h"
#include "include/new_object_window.h"
#include "include/object_processing_engine.h"
#include "include/file_manager.h"
#include "include/sort_window.h"
//Qt libs
#include <QMainWindow>
#include <QStackedWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
    //ui
    Ui::MainWindow *ui;

    QStackedWidget* window;
    Data_view* data_view;
    New_object_window* new_object_window;
    Sort_window* sort_window;

    //
    Object_processing_engine engine;
    File_manager* file_manager;

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void set_label(bool not_empty, std::string file_name);
    void set_label(bool not_empty, QString file_name);

public slots:
    void enable_buttons();

};
#endif // MAINWINDOW_H
