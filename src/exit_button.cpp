#include "include/exit_button.h"
//Qt libs
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QStyleFactory>

Exit_button::Exit_button(const QString &text, QWidget *parent): QPushButton(parent){
    this->setText(text);
    connect(this, &Exit_button::released, this, &Exit_button::show_dialog);
}

void Exit_button::show_dialog(){
    background = new QPushButton(this->parentWidget());
    background->setFixedSize(parentWidget()->width(), parentWidget()->height());
    background->setStyleSheet("background-color: rgba(0, 0, 0, 80); border: none;");

    dialog = new QDialog(this->parentWidget());
    dialog->setFixedSize(300, 100);
    dialog->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    auto layout = new QGridLayout(dialog);
    auto* text = new QLabel("Вы действительно хотите выйти?\nНесохранённые данные могут быть потеряны");
    layout->addWidget(text, 0, 0, 1, 2, Qt::AlignCenter);
    auto* cancel_button = new QPushButton("Отмена");
    layout->addWidget(cancel_button, 1, 0);
    auto* exit_button = new QPushButton("Выход");
    exit_button->setDefault(true);
    layout->addWidget(exit_button, 1, 1);

    connect(cancel_button, &QPushButton::released, this, &Exit_button::delete_dialog);
    connect(exit_button, &QPushButton::released, this, [](){exit(0);});
    connect(background, &QPushButton::released, this, &Exit_button::delete_dialog);
    connect(dialog, &QDialog::finished, this, [this](){background->deleteLater();});

    background->show();
    dialog->show();
}

void Exit_button::delete_dialog(){
    background->deleteLater();
    dialog->deleteLater();
}
