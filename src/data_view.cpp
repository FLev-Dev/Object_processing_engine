#include "include/data_view.h"
#include "include/sort_window.h"


Data_view::Data_view(Object_processing_engine* _engine, QWidget *parent)
    : QWidget(parent)
    , engine(_engine){

    this->resize(520, 500);

    auto sort_window = new Sort_window(engine);
    main_layout = new QGridLayout;

    back_button = new QPushButton("Назад");
    auto sort_button = new QPushButton("Сортировать");
    table_parent = new QWidget;
    table_layout = new QGridLayout;
    table_parent->resize(520, this->height()-50);
    table_parent->setLayout(table_layout);

    main_layout->addWidget(back_button, 0, 0);
    main_layout->addWidget(sort_button, 0, 4);
    main_layout->addWidget(table_parent, 1, 0, 5, 5);


    this->setLayout(main_layout);
    connect(sort_button, &QPushButton::released, sort_window, &Sort_window::show);
    connect(sort_window, &Sort_window::finished, this, &Data_view::update_sort);
}

void Data_view::update_sort(){
    this->hide();
    set_tabs();
    this->show();
}

void Data_view::show_data_view(){
    set_tabs();
    this->show();
}

void Data_view::set_tabs(){
    qDeleteAll(table_parent->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
    engine->sort(engine->get_decrease());
    auto tabs = engine->mk_groups();
    int i = 0;
    std::vector<QTableWidget*> tables;
    auto main_tab = new QTabWidget(table_parent);
    main_tab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    for(const auto& tab_data: tabs){
        tables.push_back(new QTableWidget(tab_data.second.size(), 5));
        create_table(tables[i], tab_data.second);
        main_tab->addTab(tables[i++], QString::fromStdString(tab_data.first));
    }
    table_layout->addWidget(main_tab);
}

void Data_view::create_table(QTableWidget* table, const std::vector<Object>& objects){
    int i = 0;
    for(const auto& object: objects){
        std::vector<QTableWidgetItem*> vec;

        std::tm* temp = std::localtime(&object.time);
        QString mday, mon;
        if(temp->tm_mday < 10) mday = "0" + QString::number(temp->tm_mday);
        else mday = QString::number(temp->tm_mday);
        if(temp->tm_mon < 10) mon = "0" + QString::number(temp->tm_mon);
        else mon = QString::number(temp->tm_mon);
        QString time = mday + "." + mon + "."
                + QString::number(temp->tm_year+1900);

        vec.push_back(new QTableWidgetItem(QString::fromStdString(object.name)));
        vec.push_back(new QTableWidgetItem(QString::number(object.x)));
        vec.push_back(new QTableWidgetItem(QString::number(object.y)));
        vec.push_back(new QTableWidgetItem(QString::fromStdString(object.type)));
        vec.push_back(new QTableWidgetItem(time));

        int j = 0;
        for(auto item: vec){
            table->setItem(i, j++, item);
        }
        i++;
    }

    const QStringList names = {"Имя", "X", "Y", "Тип", "Дата создания"};
    table->setHorizontalHeaderLabels(names);
}
