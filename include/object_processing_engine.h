#ifndef OBJECT_PROCESSING_ENGINE_H
#define OBJECT_PROCESSING_ENGINE_H
//Std libs
#include <vector>
#include <iostream>
#include <ctime>
#include <map>
//Qt libs
#include <QString>

class Sort_type_error : public std::exception {
    const char* what() const noexcept override {
        return "An unknown sorting type was received";
    }
};

//operator== does not compare the time field!
struct Object{
    std::string name;
    double x, y;
    std::string type;
    std::time_t time;

    Object(){}
    Object(std::string _name, double _x, double _y, std::string _type, std::time_t _time)
        :name(_name), x(_x), y(_y), type(_type), time(_time){}

    bool operator==(const Object& other){
        if(name == other.name && x == other.x && y == other.y
                && type == other.type)
            return true;
        return false;
    }
};

namespace sort_type{
    enum sort_types {NAME = 0, TIME = 1, TYPE, DISTANCE};
}

class Object_processing_engine{


    std::vector<Object> objects;
    std::vector<std::string> object_types = {
        "Человек", "Машина", "Дерево", "Здание"};
    int current_sort = 0;
    bool current_decrease = false;

    /*
     * If the number of objects of a certain type
     *is below a specified threshold,
     *that type will not be included in the grouped results.
     */
    const int objects_for_group = 3;

public:

    int add_object(QString name, QString x, QString y, QString type);
    void sort(bool decrease = false, int sort_type = -1);
    std::map<std::string, std::vector<Object>> mk_groups(int sort_type = -1);

    Object_processing_engine();
    Object_processing_engine(const Object_processing_engine& other);
    Object_processing_engine& operator=(const Object_processing_engine& other);


    std::vector<Object> get_objects () const {return objects;};
    int get_sort_type () const {return current_sort;}
    bool get_decrease() const {return current_decrease;}
    std::vector<std::string> get_object_types () const {return object_types;}


    void set_sort_type(const int &new_sort);
    void set_objects(const std::vector<Object>& new_objects);
    void set_decrese (const bool &new_decrease);

    void add_new_object_types(const std::vector<std::string> &new_types);
    void add_new_object_type(const std::string &new_type);
    void add_new_object_type(const QString &new_type);
};

#endif // OBJECT_PROCESSING_ENGINE_H
