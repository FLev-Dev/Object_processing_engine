#include "include/file_manager.h"
//Std libs
#include "fstream"
//Qt libs
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDate>


File_manager::File_manager(Object_processing_engine* _engine, QWidget *parent)
    : QObject(parent)
    , engine(_engine){

    add_to_log(QDate::currentDate().toString()
               + QDateTime::currentDateTimeUtc().toString());
    //UI для ошибки
    error_dialog = new QDialog;
    error_dialog->setModal(true);
    error_dialog->setWindowTitle("Ошибка");
    error_dialog->setWindowFlags(error_dialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    error_dialog->setFixedSize(300, 100);

    auto error_vlayout = new QVBoxLayout;

    auto error_label = new QLabel("Ошибка, не удалось сохранить информацию в файл");
    error_vlayout->addWidget(error_label);

    auto error_button = new QPushButton("Понятно");
    error_vlayout->addWidget(error_button);
    connect(error_button, &QPushButton::released, error_dialog, &QDialog::hide);

    error_dialog->setLayout(error_vlayout);

    //UI для успешной записи
    sucess_dialog = new QDialog;
    sucess_dialog->setModal(true);
    sucess_dialog->setWindowTitle("Успешно");
    sucess_dialog->setWindowFlags(sucess_dialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    sucess_dialog->setFixedSize(200, 100);

    auto sucess_vlayout = new QVBoxLayout;

    auto sucess_label = new QLabel("Данные успешно записаны в файл");
    sucess_vlayout->addWidget(sucess_label);

    auto sucess_button = new QPushButton("Понятно");
    sucess_vlayout->addWidget(sucess_button);
    connect(sucess_button, &QPushButton::released, sucess_dialog, &QDialog::hide);

    sucess_dialog->setLayout(sucess_vlayout);
}

bool File_manager::read_from_file(std::string file_name){
    if(!file_name.empty()) default_file = file_name;
    std::vector<std::string> list_objects;

    std::fstream file(default_file);
    if(!file.is_open() || file.peek() == EOF) {
        std::string temp = "File opening error: " + default_file + ". ";

        !file.is_open() ? temp += "File not found" : temp += "File is empty";

        add_to_log(temp);
        emit change_label(false);
        return false;
    }
    std::vector<Object> result;
    std::vector<std::string> types;
    int unrecognized = 0;
    for (std::string str; file >> str;){
        Object object;
        object.name = str;
        file >> object.x;
        file >> object.y >> object.type;
        types.push_back(object.type);
        file >> object.time;
        if(object.name.empty() || object.x == 0 || object.y == 0
                || object.type.empty() || object.time < 0){
            unrecognized++;
            continue;
        }
        result.push_back(object);

    }
    if(unrecognized != 0)
        add_to_log("File \"" + default_file
                   + "\". Lines unrecognized:" + std::to_string(unrecognized));
    file.close();
    engine->add_new_object_types(types);
    engine->set_objects(result);
    emit include_buttons();
    emit change_label(true);
    return true;
}

void File_manager::save_to_file(){

    std::ofstream file(default_file);
    std::vector<Object> temp = engine->get_objects();

    if(!file.is_open()) {
        add_to_log("File opening error: " + default_file + ". ");
        error_dialog->show();
    } else if(temp.empty()){
        add_to_log(std::string("data is empty"));
        error_dialog->show();
    } else {
        for(const auto& obj: temp){
            file << obj.name << ' ';
            file << obj.x    << ' ';
            file << obj.y    << ' ';
            file << obj.type << ' ';
            file << obj.time << ' ';
            file << std::endl;
        }
        sucess_dialog->show();
    }
    file.close();
}

void File_manager::set_default_file(std::string new_file){default_file = new_file;}

std::string File_manager::get_default_file(){return default_file;}

void File_manager::add_to_log(std::string log_record) {
    std::fstream log_file;
    log_file.open("Error_log", std::ios::app);
    if(log_file)
        log_file << log_record << std::endl;
    log_file.close();
}
void File_manager::add_to_log(QString log_record){
    std::fstream log_file;
    log_file.open("Error_log", std::ios::app);
    if(log_file)
        log_file << log_record.toStdString() << std::endl;
    log_file.close();
}
