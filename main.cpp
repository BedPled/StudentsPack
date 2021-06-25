#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace std;
//Итеративно разработать приложение для работы со студентами. В рамках задания будут появляться новые требования.
// Необходимо иногда рефакторить код (см low coupling, strong cohesion, SOLID, KISS, YAGNI, DRY и тд) под специфику
// требований, увеличивая сопровождаемость на будущее.

//Этап 1
//В предметной области есть Студент, студент может получать отметки.
//
//Необходимо произвольной формулой вычислить является ли студент отличником.
//
//Архитектурный тест 1
//Система должна позволять добавлять новых студентов, выставлять им оценки, определять кто отличник, а кто нет
int count_child = 0;
class Student {
public:
    vector<int> rating;
    int id;
    Student(){
        count_child++;
        id = count_child;
    }

    Student(const Student &student){
        id = student.id;
        rating = student.rating;
    }

    bool isExcellent () { // проверка на отличника
        for (int i = 0; i < rating.size(); ++i) {
            if (rating[i] != 5) return false;
        }
        return true;
    };

    void setRating (vector<int> rating) {
        this->rating = rating;
    }


    void addRating (int grade) { // заносим оценку в рейтинг
        rating.push_back(grade);
    }

    void  delGrade (int grade) { // заносим оценку в рейтинг
        for (int i = 0; i < rating.size(); ++i) {
            if (grade == rating[i]) {
                rating.erase(rating.begin() + i);
                break;
            }
        }
    }
};

//Этап 2
//В предметной области появляется Преподаватель, который может выставлять студентам отметки.
//
//Архитектурный тест 2
//Система должна позволять добавлять новых преподавателей. Преподаватель может выставлять оценки произвольным студентам


//Этап 3
//У преподавателя есть настроение. От настроения преподавателя зависит оценка, которую он поставит студенту.
// Также на оценку влияет тот, кому преподаватель ставит эту оценку.
//
//Пример формул расчёта итоговой оценки:
//хорошее настроение + отличник = 5
//плохое настроение + отличник = 4 или 5 (равновероятно)
//хорошее настроение + не отличник = 4
//плохое настроение + не отличник = 2 или 3 (равновероятно)

enum TEACHER {
    tMOOD,
    t2,
    t3,
    t4,
    t5,
};

class Teacher {
    bool mood = true;
    int moodCounter = 0;
    int moodFrequency = 5;
    TEACHER teacherType = tMOOD;
public:
    string name;

    void setMoodFrequency(int moodFrequency) {
            if (moodFrequency > 0) {
                throw logic_error("Некорректная частота смены настроения");
            }
            Teacher::moodFrequency = moodFrequency;
    }

    void giveRating(Student student, int grade) {
        student.addRating(grade);
    }

    void setTeacherType(TEACHER teacherType) {
        Teacher::teacherType = teacherType;
    }

    void setMood(bool mood) {
        Teacher::mood = mood;
    }

    int giveBananaRating (Student student) {
        int grade;
        switch (teacherType) {
            case tMOOD:
                if (mood) {
                    if (student.isExcellent()) {
                        grade = 5;
                        giveRating(student, grade);
                    } else {
                        grade = 4;
                        giveRating(student, grade);
                    }
                } else {
                    if (student.isExcellent()) {
                        grade = 4 + rand() % 2;
                        giveRating(student, grade);
                    } else {
                        grade = 2 + rand() % 2;
                        giveRating(student, grade);
                    }
                }

                moodCounter++;
                if (moodCounter % moodFrequency == 0) {
                    mood = rand() % 2;
                }
                break;
            case t2:
                grade = 2;
                giveRating(student, grade);
                break;
            case t3:
                grade = 3;
                giveRating(student, grade);
                break;
            case t4:
                grade = 4;
                giveRating(student, grade);
                break;
            case t5:
                grade = 5;
                giveRating(student, grade);
                break;
        }
        return grade;
    }
};

//Этап 4
//В предметной области появляется Занятие. На занятии присутствует один преподаватель и некоторые студенты.
// На занятии преподаватель выставляет произвольное количество оценок(внимая своему настроению) произвольным студентам,
// которые пришли на занятие.
//
//Архитектурный тест 4
//Система должна позволять добавлять новые занятия.

class Lesson {
public:
    Teacher teacher;
    vector<Student> students;
    vector<Student> studentsWhoGetRating;
    vector<int> GetRating;


    void addStudent (Student student) { // заносим оценку в рейтинг
        students.push_back(student);
    }

    void setRandomRating () {
        int count = rand() % students.size() + 1;
        for (int i = 0; i < count; ++i) {
            GetRating.push_back(teacher.giveBananaRating(students[i]));
            studentsWhoGetRating.push_back(students[i]);
        }
    }

    bool isHaveGrade (Student student) { // возвращает index первой оценки
        for (int i = 0; i < studentsWhoGetRating.size(); ++i) {
            if (student.id == studentsWhoGetRating[i].id) {
                return true;
            }
        }
        return false;
    }

};


//Этап 5
//Есть преподаватели, которые ставят 5, вне зависимости от настроения и от того, кому они ставят оценку.
//
//Архитектурный тест 5
//Архитектура предусматривает появление и тех преподавателей, которые будут всегда ставить 2.

//Этап 6
//Каждую пятую выставленную оценку у преподавателя меняется настроение (случайным образом, не обязательно на противоположное)
//
//Архитектурный тест 6
//Архитектура предусматривает появление и тех преподавателей, настроение у которых меняется чаще или реже


//Этап 7
//В предметной области появляется Родитель. У родителя может быть настроение. Родитель может иметь несколько детей.
// Родитель может выполнять следующие функции:
//- Рассказать о каждом своём ребёнке что-нибудь (зависит от настроения и от того отличник он или нет)
//- Рассказать об одном случайном своём ребёнке что-нибудь (зависит от настроения и от того отличник он или нет)
//- Рассказать в общем, суммарно, обо всех своих детях (зависит от настроения и от того отличники они в среднем или нет)
//- Рассказать о конкретном своём ребёнке (зависит от настроения и от того отличники они в среднем или нет).
// Если это не ребёнок родителя, то сообщить об ошибке

class Parent {
public:
    string name;

    bool mood = true;
    vector<Student> childs;
    void setMood(bool mood) {
        Parent::mood = mood;
    }

    void addChild (Student child) { // заносим оценку в рейтинг
        childs.push_back(child);
    }

    virtual void tellAbout (Student child) {
    bool my = false;

            for (int i = 0; i < childs.size(); ++i) {
                if (child.id == childs[i].id) {
                    my = true;
                }
            }
            if (!my) {
                throw logic_error("Подкинули чужого амогуса");
            }

            if (mood) {
                if (child.isExcellent()) {
                    cout << " просто супер" << endl;
                } else {
                    cout << " молодец" << endl;
                }
            } else {
                if (child.isExcellent()) {
                    if (rand() % 2) {
                        cout << " просто супер" << endl;
                    } else {
                        cout << " молодец" << endl;
                    }
                } else {
                    if (rand() % 2) {
                        cout << " может и лучше" << endl;
                    } else {
                        cout << " балбес" << endl;
                    }
                }
            }
    }

    void tellAboutEachChild () {
        for (int i = 0; i < childs.size(); ++i) {
            cout << "Амогус " << childs[i].id;
            tellAbout(childs[i]);
        }
    }

    void tellAboutChild () {
        int countChild = rand() % childs.size();
        cout << "Амогус " << childs[countChild].id;
        tellAbout(childs[countChild]);
    }

    void tellAboutAllChild () {
        int countExcellent = 0;
        for (int i = 0; i < childs.size(); ++i) {
            if (childs[i].isExcellent()) {
                countExcellent++;
            }
        }
        cout << "Мои Амогусы ";
        if (mood) {
            if (childs.size() - countExcellent > childs.size()) {
                cout << " просто супер" << endl;
            } else {
                cout << " молодцы" << endl;
            }
        } else {
            if (childs.size() - countExcellent > childs.size()) {
                if (rand() % 2) {
                    cout << " просто супер" << endl;
                } else {
                    cout << " молодцы" << endl;
                }
            } else {
                if (rand() % 2) {
                    cout << " могли быть и лучше" << endl;
                } else {
                    cout << " балбесы" << endl;
                }
            }
        }

    }

    void tellAboutTheChild (int num) {
        cout << "Амогус " << childs[num].id;
        tellAbout(childs[num]);
    }
};

//Этап 8
//В предметной области появляется Собрание. На собрании участвуют Преподаватели, Родители.
//
// Они обсуждают все прошедшие занятия и родители по каждому из своих детей, которые получали за эти
// занятия хотя бы одну оценку, высказываются "о конкретном своём ребёнке".
//
// Если на собрании нет преподавателя, который проводил Занятие, то эти выставленные
// оценки не учитываются при необходимости обсуждения ребёнка.
//
// Если на собрании нет родителя ребёнка, то список таких
// детей формируется в конце собрания и должен быть оглашён.
//
//Архитектурный тест 8
//Архитектура позволяет проводить несколько собраний по различному набору Занятий, меняя состав преподавателей и родителей.

class Meeting {
public:
    vector<Teacher> teachers;
    vector<Parent> parents;
    vector<Lesson> lessons;
    vector<Student> studentNoParents;

    void addTeacher (Teacher teacher) {
        teachers.push_back(teacher);
    }

    void addParent (Parent parent) {
        parents.push_back(parent);
    }

    void addLesson (Lesson lesson) {
        lessons.push_back(lesson);
    }

    bool isHereTeacher (Teacher teacher) {
        for (int i = 0; i < teachers.size(); ++i) {
            if (teachers[i].name == teacher.name) {
                return true;
            }
        }
        return false;
    }

    bool isHereParent (Student student) {
        for (int i = 0; i < parents.size(); ++i) {
            for (int j = 0; j < parents[i].childs.size(); ++j) {
                if (student.id == parents[i].childs[j].id) {
                    return true;
                }
            }
        }
        return false;
    }

    void meet () {
        // Они обсуждают все прошедшие занятия и родители по каждому из своих детей, которые получали за эти
        // занятия хотя бы одну оценку, высказываются "о конкретном своём ребёнке".
        //
        // Если на собрании нет преподавателя, который проводил Занятие, то эти выставленные
        // оценки не учитываются при необходимости обсуждения ребёнка.
        //
        // Если на собрании нет родителя ребёнка, то список таких
        // детей формируется в конце собрания и должен быть оглашён.
        Student student;
        bool breaker;

        for (int i = 0; i < parents.size(); ++i) { // перебираем родителей
            for (int j = 0; j < parents[i].childs.size(); ++j) { // перебираем детей
                student = parents[i].childs[j];
                breaker = false;
                for (int k = 0; k < lessons.size(); ++k) { // перебираем уроки
                    if (lessons[k].isHaveGrade(student)) {
                        breaker = true;
                    }
                    if (!isHereTeacher(lessons[k].teacher)) { // учитетель, ты тут?
                        if (lessons[k].isHaveGrade(student)) { // О, а оценочку поставили?
                            for (int l = 0; l < lessons[k].studentsWhoGetRating.size(); ++l) {
                                if (student.id == lessons[k].studentsWhoGetRating[l].id) {
                                    student.delGrade(lessons[k].GetRating[l]);
                                }
                            }
                        }
                    }
                }
                if (breaker) {
                    parents[i].tellAbout(student); // говорим о ребёнке
                }
            }
        }


        bool flag;
        for (int i = 0; i < lessons.size(); ++i) {
            for (int j = 0; j < lessons[i].students.size(); ++j) {
                flag = true;
                if(!isHereParent(lessons[i].students[j])) {
                   for (int k = 0; k < studentNoParents.size(); ++k) {
                       if (studentNoParents[k].id == lessons[i].students[j].id) {
                           flag = false;
                       }
                   }
                   if (flag) {
                       studentNoParents.push_back(lessons[i].students[j]);
                   }
               }
            }
        }

        for (auto & studentNoParent : studentNoParents) {
            cout << studentNoParent.id << endl;
        }
    }
};

//Этап 9
//В предметной области появляется Бабушка, которая являясь родителем/опекуном ребёнка может выступать на собраниях
//и обладает теми же функциями, что и Родитель. Единственное, что у Бабушка всегда о своих детях говорит хорошо, а о чужих по-настроению.
class grandmother : public Parent {
    virtual void tellAbout (Student child) {
        bool my = false;

        for (int i = 0; i < childs.size(); ++i) {
            if (child.id == childs[i].id) {
                my = true;
            }
        }
        if (!my) {
            if (mood) {
                if (child.isExcellent()) {
                    cout << " лапочка" << endl;
                } else {
                    cout << " молодец" << endl;
                }
            } else {
                if (child.isExcellent()) {
                    if (rand() % 2) {
                        cout << " лапочка" << endl;
                    } else {
                        cout << " молодец" << endl;
                    }
                } else {
                    if (rand() % 2) {
                        cout << " может и лучше" << endl;
                    } else {
                        cout << " балбес" << endl;
                    }
                }
            }
        } else {
            cout << " лапочка" << endl;
        }
    }

};


int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}



