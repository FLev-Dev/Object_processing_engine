#include "include/sort_window.h"
//Qt libs
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

Sort_window::Sort_window(Object_processing_engine* _engine): engine(_engine){
    this->setWindowTitle("Выберите тип сортировки");
    this->setFixedSize(250, 100);
    this->setModal(true);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    choose_sort = new QComboBox;
    for(auto& type: sort_types)
        choose_sort->addItem(type);
    choose_sort->setCurrentIndex(0);
    decrease_check = new QCheckBox("По убыванию");
    auto accept_sort_button = new QPushButton("Принять");

    auto layout = new QVBoxLayout;
    layout->addWidget(choose_sort);
    layout->addWidget(decrease_check);
    layout->addWidget(accept_sort_button);
    this->setLayout(layout);

    connect(accept_sort_button, &QPushButton::released, this, &Sort_window::call_sort);
}

void Sort_window::call_sort(){
    engine->sort(decrease_check->isChecked(), choose_sort->currentIndex());
    auto message = new QMessageBox(QMessageBox::Information, tr("Успешно"),
                                   tr("Сортировка произведена"), QMessageBox::Ok, this);

    connect(message, &QMessageBox::buttonClicked, this, &Sort_window::close);
    message->show();

}
