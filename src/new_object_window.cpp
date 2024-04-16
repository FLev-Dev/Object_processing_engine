#include "include/new_object_window.h"
//Qt libs
#include <QLayout>
#include <QRegExpValidator>
#include <QPushButton>


New_object_window::New_object_window(Object_processing_engine* _engine): engine(_engine){
    //Контроль ввода
    QRegExp double_reg_exp("^-?\\d+[\.\,]?\\d*");
    QRegExp text_reg_exp("^[a-zA-Zа-яА-Я]*");
    QValidator* double_validator = new QRegExpValidator(double_reg_exp);
    QValidator* text_validator = new QRegExpValidator(text_reg_exp);


    //UI добавления объекта
    this->setWindowTitle("Добавление нового объекта");
    this->setFixedSize(400, 200);
    this->setModal(true);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    auto new_object_layout = new QGridLayout;

    auto label_object_name = new QLabel("Имя:");
    new_object_layout->addWidget(label_object_name, 0, 0, 1, 2);

    input_object_name = new QLineEdit;
    input_object_name->setPlaceholderText("Имя объекта");
    new_object_layout->addWidget(input_object_name, 1, 0, 1, 2);
    input_object_name->setValidator(text_validator);

    auto label_coords = new QLabel("Координаты:");
    new_object_layout->addWidget(label_coords, 2, 0, 1, 2);

    input_object_x = new QLineEdit;
    input_object_x->setPlaceholderText("Координата X");
    input_object_x->setValidator(double_validator);
    new_object_layout->addWidget(input_object_x, 3, 0);

    input_object_y = new QLineEdit;
    input_object_y->setPlaceholderText("Координата Y");
    input_object_y->setValidator(double_validator);
    new_object_layout->addWidget(input_object_y, 3, 1);

    auto label_object_type = new QLabel("Тип объекта:");
    new_object_layout->addWidget(label_object_type, 4, 0, 1, 2);
    choose_object_type = new QComboBox;
    cb_model = new QStringListModel;
    choose_object_type->setModel(cb_model);
    for(const auto &type : engine->get_object_types())
        choose_object_type->addItem(QString::fromStdString(type));
    new_object_layout->addWidget(choose_object_type, 5, 0);

    //
    add_new_type_ui(text_validator);
    //
    auto new_type_button = new QPushButton("Добавить тип");
    new_object_layout->addWidget(new_type_button, 5, 1);
    connect(new_type_button, &QPushButton::released, add_new_type_dialog, &QDialog::show);

    auto cancel = new QPushButton("Отмена");
    new_object_layout->addWidget(cancel, 6, 0);
    connect(cancel, &QPushButton::released, this, &QDialog::hide);

    auto accept = new QPushButton("Принять");
    new_object_layout->addWidget(accept, 6, 1);
    accept->setDefault(true);
    connect(accept, &QPushButton::released, this, &New_object_window::new_object_create);

    this->setLayout(new_object_layout);

    //UI окна ошибки при добавлении нового объекта
    error_new_obj = new QDialog(this);
    error_new_obj->setModal(true);
    error_new_obj->setWindowTitle("Ошибка");
    error_new_obj->setWindowFlags(error_new_obj->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    error_new_obj->setFixedSize(200, 100);

    auto error_obj_vlayout = new QVBoxLayout;

    //Текст меняется в методе new_object_create в зависимости от error_code
    new_obj_message_error = new QLabel("Неизвестно");
    error_obj_vlayout->addWidget(new_obj_message_error);

    auto error_obj_button = new QPushButton("Понятно");
    error_obj_vlayout->addWidget(error_obj_button);
    connect(error_obj_button, &QPushButton::released, error_new_obj, &QDialog::hide);

    error_new_obj->setLayout(error_obj_vlayout);

    //UI окна при успешном добавлении нового объекта
    sucess_new_obj = new QDialog;
    sucess_new_obj->setModal(true);
    sucess_new_obj->setWindowTitle("Успешно");
    sucess_new_obj->setFixedSize(200, 100);
    sucess_new_obj->setWindowFlags(sucess_new_obj->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    auto sucess_obj_vlayout = new QVBoxLayout;

    auto l_sucess_obj = new QLabel("Новый объект добавлен");
    sucess_obj_vlayout->addWidget(l_sucess_obj, 0);

    auto sucess_obj_button = new QPushButton("Хорошо");
    sucess_obj_vlayout->addWidget(sucess_obj_button, 1);
    connect(sucess_obj_button, &QPushButton::released, sucess_new_obj, &QDialog::hide);
    connect(sucess_new_obj, &QDialog::finished, this, &QDialog::hide);
    connect(sucess_obj_button, &QPushButton::released, this, &QDialog::hide);

    sucess_new_obj->setLayout(sucess_obj_vlayout);
}

void New_object_window::add_new_type_ui(QValidator*& text_validator){
    //UI создания нового типа
    add_new_type_dialog = new QDialog(this);
    add_new_type_dialog->setWindowFlags(add_new_type_dialog->windowFlags()
                                        & ~Qt::WindowContextHelpButtonHint);
    add_new_type_dialog->setFixedSize(250, 100);
    add_new_type_dialog->setWindowTitle("Добавление типа");

    auto new_type_layout = new QGridLayout;
    auto label_new_type = new QLabel("Новый тип:");
    new_type_layout->addWidget(label_new_type, 0, 0, 1, 2);

    input_new_type = new QLineEdit;
    input_new_type->setPlaceholderText("Название типа");
    input_new_type->setValidator(text_validator);
    new_type_layout->addWidget(input_new_type, 1, 0, 1, 2);

    auto new_type_cancel = new QPushButton("Отмена");
    new_type_layout->addWidget(new_type_cancel, 2, 0);
    connect(new_type_cancel, &QPushButton::released, add_new_type_dialog, &QDialog::hide);

    auto new_type_ok = new QPushButton("Добавить");
    new_type_ok->setDefault(true);
    new_type_layout->addWidget(new_type_ok, 2, 1);
    connect(new_type_ok, &QPushButton::released, this, &New_object_window::add_new_type);

    add_new_type_dialog->setLayout(new_type_layout);


    //UI окно ошибки при создании нового типа
    error_new_type = new QDialog(this);
    error_new_type->setModal(true);
    error_new_type->setWindowTitle("Ошибка");
    error_new_type->setFixedSize(200, 100);
    error_new_type->setWindowFlags(error_new_type->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    auto error_v_layout = new QVBoxLayout;
    auto l_error = new QLabel("Нельзя создать тип без названия!");
    error_v_layout->addWidget(l_error);
    auto error_button = new QPushButton("Понятно");
    error_v_layout->addWidget(error_button);

    connect(error_button, &QPushButton::released, error_new_type, &QDialog::hide);

    error_new_type->setLayout(error_v_layout);


    //UI окно успешного создания нового типа
    sucess_new_type = new QDialog;
    sucess_new_type->setModal(true);
    sucess_new_type->setWindowTitle("Успешно");
    sucess_new_type->setFixedSize(200, 100);
    sucess_new_type->setWindowFlags(sucess_new_type->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    auto sucess_v_layout = new QVBoxLayout;
    auto l_sucess = new QLabel("Новый тип создан");
    sucess_v_layout->addWidget(l_sucess, 0);

    auto sucess_button = new QPushButton("Хорошо");
    sucess_v_layout->addWidget(sucess_button, 1);
    connect(sucess_button, &QPushButton::released, sucess_new_type, &QDialog::hide);
    connect(sucess_new_type, &QDialog::finished, add_new_type_dialog, &QDialog::hide);
    connect(sucess_button, &QPushButton::released, add_new_type_dialog, &QDialog::hide);

    sucess_new_type->setLayout(sucess_v_layout);
}

void New_object_window::update_types(){
    for(const auto &type : engine->get_object_types()){
        auto qstr = QString::fromStdString(type);
        if(!cb_model->stringList().contains(qstr))
            choose_object_type->addItem(qstr);
    }
}

void New_object_window::add_new_type(){
    auto str = input_new_type->text();
    input_new_type->setText("");

    if(str.begin() != str.end()){
        str = str.toLower();
        str[0] = str[0].toUpper();
        engine->add_new_object_type(str.toStdString());
        choose_object_type->addItem(str);
        choose_object_type->setCurrentText(str);
        sucess_new_type->show();
    } else error_new_type->show();
}

void New_object_window::new_object_create(){
    int error_code = engine->add_object(input_object_name->text(),
                                        input_object_x->text(),
                                        input_object_y->text(),
                                        choose_object_type->currentText());
    if(error_code == 0){
        sucess_new_obj->show();
        emit include_buttons();
        emit change_label(true);
        return;
    }
    if(error_code == 2)
        new_obj_message_error->setText("Укажите имя объекта");
    if(error_code == 3)
        new_obj_message_error->setText("Укажите координаты");
    if(error_code == 4)
        new_obj_message_error->setText("Этот объект уже есть в базе данных/nВремя создания объекта не учитывается");
    if(error_code == 5)
        new_obj_message_error->setText("Укажите имя объекта и координаты");
    error_new_obj->show();
}
