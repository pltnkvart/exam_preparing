#include <vector>
#include <iostream>
#include <thread>

// Перечисляемый тип факультетов
enum Faculty {
    graphfinder, czerin, configrun, pythondui, unknown
};

// Класс студента (реализация не важна для текущей задачи)
class Student {
public:
    Faculty faculty;

    Student() {
        faculty = Faculty::unknown;
    }
};

// Функция, записывающая студента на заданный факультет
void assignFaculty(Student student, Faculty faculty) {
    student.faculty = faculty;
}

// Функция, отправляющая запрос распределяющей сети под номером N
// Возвращает самый подходящий факультет для переданного студента
// Если сервер перегрелся, возвращает значение unknown
Faculty runNN(const Student &student, int N) {
    return Faculty(rand() % 4);
}


// Функция для распределения вектора студентов по факультетам
// Вам необходимо реализовать эту функцию!
// Функция для распределения вектора студентов по факультетам
void distribute(const std::vector<Student> &students) {
//    const int numberNetworks = 24;
    std::vector<int> count(4, 0);

    std::mutex mutex;
    std::vector<std::thread> threads;

    auto assignThread = [&mutex, &count, &students](const Student &student, int i) {
        int selected = runNN(student, i);
        mutex.lock();
        if (count[selected] >= students.size() / 3) {
            selected = std::distance(std::begin(count), std::min_element(count.begin(), count.end()));
        }
        count[selected]++;
        assignFaculty(student, static_cast<Faculty> (selected));
        mutex.unlock();
    };

    for (int i = 0; i < students.size(); i++) {
        threads.emplace_back(assignThread, students[i], i);
    }

    for (auto &thread: threads) {
        thread.join();
    }
}

int main() {
    srand(time(nullptr));

    std::vector<Student> students(25);

    distribute(students);

    for (auto &s: students) {
        std::cout << s.faculty << std::endl;
    }

    return 0;
}
