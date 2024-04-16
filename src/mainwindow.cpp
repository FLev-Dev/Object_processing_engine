#include "include/mainwindow.h"
#include "include/exit_button.h"
//Qt libs
#include "ui_mainwindow.h"
#include <QSpacerItem>
#include <QString>
#include <QFileDialog>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){


    ui->setupUi(this);
    this->setWindowTitle(" ");

    file_manager = new File_manager(&engine, this);
    sort_window = new Sort_window(&engine);

    auto exit_button = new Exit_button("Выход");
    ui->menu_layout->addWidget(exit_button);
    auto spacer = new QSpacerItem(40, 120);
    ui->menu_layout->addItem(spacer);
    ui->check_data_button->setProperty("widget.index", 1);

    new_object_window = new New_object_window(&engine);

    auto file_dialog = new QFileDialog;
    file_dialog->setWindowFlags(file_dialog->windowFlags()& ~Qt::WindowContextHelpButtonHint);
    file_dialog->setDirectory(QDir::currentPath());
    file_dialog->setWindowTitle("Выбор файла");

    window = new QStackedWidget(this);
    window->setGeometry(this->geometry());
    window->addWidget(ui->main_menu);
    data_view = new Data_view(&engine);
    data_view->setGeometry(this->geometry());
    window->addWidget(data_view);
    window->setCurrentIndex(0);
    window->show();


    connect(ui->load_button, &QPushButton::released, file_dialog, &QFileDialog::show);
    connect(file_dialog, &QFileDialog::fileSelected,
            this, [this](QString str){set_label(file_manager->read_from_file(str.toStdString()), str);});
    connect(ui->add_obj_button, &QPushButton::released,
            new_object_window, &New_object_window::show);
    connect(ui->add_obj_button, &QPushButton::released,
            new_object_window, &New_object_window::update_types);
    connect(ui->save_button, &QPushButton::released, file_manager,
            &File_manager::save_to_file);
    connect(new_object_window, &New_object_window::change_label,
            this, [this](){set_label(true, file_manager->get_default_file());});
    connect(new_object_window, &New_object_window::include_buttons,
            this, &MainWindow::enable_buttons);
    connect(file_manager, &File_manager::change_label,
            this, [this](){set_label(true, file_manager->get_default_file());});
    connect(file_manager, &File_manager::include_buttons,
            this, &MainWindow::enable_buttons);
    connect(ui->sort_button, &QPushButton::released, sort_window, &Sort_window::show);
    connect(data_view->back_button, &QPushButton::released, this, [this](){window->setCurrentWidget(ui->main_menu);});
    connect(ui->check_data_button, &QPushButton::released, this, [this](){data_view->set_tabs();window->setCurrentWidget(data_view);});

    if(file_manager->read_from_file())
        ui->log_label->setText("По умолчанию загружен файл "
                               + QString::fromStdString(file_manager->get_default_file()));
    else ui->log_label->setText("Файл по умолчанию ("
                               + QString::fromStdString(file_manager->get_default_file())
                               + ") пуст");
}

void MainWindow::enable_buttons(){
    ui->check_data_button->setEnabled(true);
    ui->save_button->setEnabled(true);
    if(engine.get_objects().size() > 1)
        ui->sort_button->setEnabled(true);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::set_label(bool not_empty, std::string file_name){
    auto it = file_name.rfind('/');
    if(it != 0)
        file_name = file_name.substr(it + 1);
    if(not_empty)
        ui->log_label->setText("Загружен файл " + QString::fromStdString(file_name));
     else
        ui->log_label->setText("Файл (" + QString::fromStdString(file_name) + ") пуст");
}
void MainWindow::set_label(bool not_empty, QString file_name){
    auto it = file_name.lastIndexOf('/');
    if(it != 0)
        file_name = file_name.mid(it + 1, file_name.size());
    if(not_empty)
        ui->log_label->setText("Загружен файл " + file_name);
     else
        ui->log_label->setText("Файл (" + file_name + ") пуст");
}




