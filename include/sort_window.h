#ifndef SORT_WINDOW_H
#define SORT_WINDOW_H
#include "include/object_processing_engine.h"
//Qt libs
#include <QDialog>
#include <QComboBox>
#include <QCheckBox>


class Sort_window : public QDialog{

    Object_processing_engine* engine;
    QComboBox* choose_sort;
    QCheckBox* decrease_check;

    /*
     * For this vector, the order of the sorting types should match t
     * he enumeration values sort_types,
     * which is defined in object_processing_engine.h.
     */
    const std::vector<QString> sort_types = {
        "По имени", "По времени создания", "По типу", "По расстоянию"};

public:
    Sort_window(Object_processing_engine* _engine);
    ~Sort_window(){delete engine;}
public slots:
    void call_sort();
};

#endif // SORT_WINDOW_H
