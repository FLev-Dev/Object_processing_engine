#include "include/object_processing_engine.h"
//Std libs
#include <map>
#include <algorithm>
#include <cmath>
#include <locale>
//Qt libs
#include <QRegularExpression>

class Add_object_error : public std::exception {
    const char* what() const noexcept override {
        return "Cant add new object. Type is empty!";
    }
};

struct Time{
    int y_day, week, mon, year;
    Time(int _y_day, int _week, int _mon, int _year){
        y_day = _y_day;
        week = _week;
        mon = _mon;
        year = _year;
    }
    std::string get_compare_period(Time other){
        if(this->year == other.year){
            if(other.y_day == this->y_day)
                return "Сегодня";
            if(other.y_day == this->y_day - 1)
                return "Вчера";
            if(other.week == this->week)
                return "На этой неделе";
            if(other.mon == this->mon)
                return "В этом месяце";
            return "В этом году";
        }
        return "Ранее";
    }
};

Object_processing_engine::Object_processing_engine(){}

Object_processing_engine::Object_processing_engine(const Object_processing_engine &other)
    : objects(other.objects){}

Object_processing_engine &Object_processing_engine::operator=(const Object_processing_engine &other){
    if(&other != this){
        objects = other.objects;
    }
    return *this;
}

int Object_processing_engine::add_object(QString name, QString x, QString y, QString type){
    int error_code = 0;
    if(name.isEmpty()) error_code += 2;
    if(x.isEmpty() || y.isEmpty()) error_code += 3;
    if(type.isEmpty()) throw Add_object_error();
    if(error_code != 0)return error_code;

    auto time = std::time(nullptr);
    Object new_obj(name.toStdString(), x.toDouble(), y.toDouble(), type.toStdString(), time);
    for(const auto& obj: objects)
        if(new_obj == obj)
            return 4;

    objects.push_back(new_obj);
    return 0;
}

void Object_processing_engine::sort(bool decrease, int sort_type){
    if (sort_type == -1) sort_type = current_sort;
    else current_sort = sort_type;

    switch (sort_type){
    case sort_type::NAME: {
        std::sort(objects.begin(), objects.end(),[](const Object &o1, const Object &o2) {
            return o1.name < o2.name;
        });
        break;
    }
    case sort_type::DISTANCE: {
        std::sort(objects.begin(), objects.end(),[](const Object &o1, const Object &o2){
            return sqrt(o1.x*o1.x + o1.y*o1.y) < sqrt(o2.x*o2.x + o2.y*o2.y);
        });
        break;
    }
    case sort_type::TYPE:{
        std::sort(objects.begin(), objects.end(),[](const Object &o1, const Object &o2) {
            if(o1.type != o2.type)
                return o1.type < o2.type;
            return o1.name < o2.name;
        });
        if(decrease)
            std::reverse(objects.begin(), objects.end());
        int count = 1;
        for(auto it = objects.begin(); it != objects.end(); it++){
            if(it != objects.begin() && it->type != (it-1)->type){
                if(count != 0 && count < objects_for_group){
                    for(int k = 1; k < count; k++){
                        std::rotate(it-k, it, objects.end() - k + 1);
                    }
                    count = 0;
                    continue;
                }
                count = 1;
            }else count++;
        }
        break;
    }
    case sort_type::TIME:{
        std::sort(objects.begin(), objects.end(),
                    [](const Object &o1, const Object &o2) {return o1.time < o2.time;});
        break;
    }
    default:{
        throw Sort_type_error();
    }
    }
    if(decrease && sort_type != sort_type::TYPE)
        std::reverse(objects.begin(), objects.end());
    current_decrease = decrease;
}

std::map<std::string, std::vector<Object>> Object_processing_engine::
        mk_groups(int sort_type){

    if (sort_type == -1) sort_type = current_sort;
    else current_sort = sort_type;
    std::map<std::string, std::vector<Object>> result_groups;
    std::string group_name;
    std::vector<Object> group_obects;

    switch (sort_type){
    case sort_type::NAME: {
        QString temp = QString::fromStdString(objects[0].name).at(0);
        group_name = temp.toStdString();
        for(const auto& object: objects){
            if(!QString::fromStdString(object.name).contains(QRegularExpression("^[А-Яа-я].*")))
                result_groups["#"].push_back(object);
            else if(QString::fromStdString(object.name).at(0) != temp){
                if(!group_obects.empty())
                    result_groups[group_name] = std::move(group_obects);
                temp = QString::fromStdString(object.name).at(0);
                group_name = temp.toStdString();
                group_obects.push_back(object);
            } else
                group_obects.push_back(object);
        }
        if(!group_obects.empty()) result_groups[group_name] = std::move(group_obects);
        break;
    }
    case sort_type::DISTANCE: {
        const double max = 10000;
        for(const auto& object: objects){
            double dist = sqrt(object.x * object.x + object.y * object.y);
            if(dist > max) {
                group_obects.push_back(object);
                continue;
            }
            for(double temp = 100; temp <= max; temp *= 10){
                group_name = "До " + std::to_string((int)temp) + " ед.";
                if(dist < temp){
                    result_groups[group_name].push_back(object);
                    break;
                }
            }
        }
        if(!group_obects.empty()) {
            result_groups["Слишком далеко"] = std::move(group_obects);
        }
        break;
    }
    case sort_type::TYPE:{
        group_name = objects[0].type;
        int count = 0;
        for(const auto& object: objects){
            count++;
            if(object.type != group_name){
                if(count < objects_for_group){
                    for(auto& obj: group_obects){
                        group_name = "Разное";
                        result_groups[group_name].push_back(obj);
                    }
                    group_obects.clear();
                    count = 0;
                } else {
                result_groups[group_name] = std::move(group_obects);
                group_name = object.type;
                count = 0;
                }
            }
            group_obects.push_back(object);
        }
        if(!group_obects.empty()) {
            for(auto& obj: group_obects){
                result_groups[group_name].push_back(obj);
            }
            group_obects.clear();
        }
        if(result_groups.find("Разное") != result_groups.end()){
            std::sort(result_groups["Разное"].begin(), result_groups["Разное"].end(),[](const Object &o1, const Object &o2) {
                if(o1.type != o2.type)
                    return o1.type < o2.type;
                return o1.name < o2.name;
            });
        if(current_decrease)
            std::reverse(result_groups["Разное"].begin(), result_groups["Разное"].end());
        }
        break;
    }
    case sort_type::TIME:{
        auto calc_week = [](std::tm* time, int& week){
            week = (time->tm_yday + 1) / 7;
            if ((time->tm_yday + 1) % 7) ++week;
        };
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);
        int week;
        calc_week(now, week);
        Time now_date(now->tm_yday, week, now->tm_mon, now->tm_year);
        for(const auto& object: objects){
            now = std::localtime(&object.time);
            calc_week(now, week);
            Time obj_date(now->tm_yday, week, now->tm_mon, now->tm_year);
            result_groups[now_date.get_compare_period(obj_date)].push_back(object);
        }
        break;
    }
    default:{
        throw Sort_type_error();
    }
    }
    return result_groups;
}


void Object_processing_engine::set_objects(const std::vector<Object>& new_objects){
    if(new_objects.begin() != new_objects.end())
        objects = new_objects;
}

void Object_processing_engine::set_decrese(const bool &new_decrease){
    if (new_decrease != current_decrease) current_decrease = new_decrease;
}


void Object_processing_engine::set_sort_type(const int &new_sort){
    if(new_sort > 4 || new_sort <= 0) throw Sort_type_error();
    current_sort = new_sort;
}

void Object_processing_engine::add_new_object_types(const std::vector<std::string> &new_types){
    for(const auto& new_type: new_types){
        if(std::find(object_types.begin(), object_types.end(), new_type)
                == object_types.end()){
            object_types.push_back(new_type);
        }
    }
}
void Object_processing_engine::add_new_object_type(const std::string &new_type){
    if(std::find(object_types.begin(), object_types.end(), new_type)
            != object_types.end())
        object_types.push_back(new_type);
}
void Object_processing_engine::add_new_object_type(const QString &new_type_q){
    std::string new_type = new_type_q.toStdString();
    if(std::find(object_types.begin(), object_types.end(), new_type)
            != object_types.end())
        object_types.push_back(new_type);
}
