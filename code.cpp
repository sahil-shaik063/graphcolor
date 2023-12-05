#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <limits>

using namespace std;

unordered_map<string, int> scheduleCourses(const unordered_map<string, unordered_set<string>>& courses, int maxPeriods) {
    unordered_map<string, unordered_set<string>> graph;
    unordered_map<string, int> timetable;

    for (const auto& course : courses) {
        const string& currentCourse = course.first;
        graph[currentCourse] = {};
        for (const auto& otherCourse : courses) {
            const string& comparedCourse = otherCourse.first;
            if (currentCourse != comparedCourse) {
                if (any_of(course.second.begin(), course.second.end(),
                           [&otherCourse](const string& student) {
                               return otherCourse.second.find(student) != otherCourse.second.end();
                           })) {
                    graph[currentCourse].insert(comparedCourse);
                }
            }
        }
    }

    for (const auto& course : courses) {
        const string& currentCourse = course.first;
        int earliestSlot = numeric_limits<int>::max();

        for (int i = 0; i < maxPeriods; ++i) {
            if (all_of(graph[currentCourse].begin(), graph[currentCourse].end(),
                       [&timetable, &i](const string& neighbor) {
                           return timetable.find(neighbor) == timetable.end() || timetable[neighbor] != i;
                       })) {
                earliestSlot = min(earliestSlot, i);
            }
        }

        timetable[currentCourse] = earliestSlot;
    }

    unordered_set<int> uniqueSlots;
    for (const auto& slot : timetable) {
        uniqueSlots.insert(slot.second);
    }

    cout << "Minimum number of time slots = " << uniqueSlots.size() << endl;

    return timetable;
}

int main() {
    unordered_map<string, unordered_set<string>> courses = {
        {"Physics", {"Mathematics", "Computer Science", "Electronics"}},
        {"Chemistry", {"Mathematics", "Physics", "Computer Science"}},
        {"Mathematics", {"Physics", "Chemistry", "Computer Science", "Statistics"}},
        {"Economics", {"Mathematics", "Statistics", "Computer Science"}}
    };

    int maxPeriods = 8;
    auto timetable = scheduleCourses(courses, maxPeriods);

    for (const auto& course : timetable) {
        cout << "Course: " << course.first << ", Time Slot: " << course.second << endl;
    }

    return 0;
}