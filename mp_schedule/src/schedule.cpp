/**
 * @file schedule.cpp
 * Exam scheduling using graph coloring
 */

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <queue>
#include <unordered_map>

#include "schedule.h"
#include "utils.h"
#include <algorithm>

#include <cassert>

/**
 * Given a filename to a CSV-formatted text file, create a 2D vector of strings where each row
 * in the text file is a row in the V2D and each comma-separated value is stripped of whitespace
 * and stored as its own string. 
 * 
 * Your V2D should match the exact structure of the input file -- so the first row, first column
 * in the original file should be the first row, first column of the V2D.
 *  
 * @param filename The filename of a CSV-formatted text file. 
 */
V2D file_to_V2D(const std::string & filename){
    // String of file contents
    std::string fileStr = file_to_string(filename);
    
    // Vector with each line of fileStr as an element
    std::vector<std::string> lines;
    int rows = SplitString(fileStr, '\n', lines);
    
    // End result
    V2D ret;
    for (int i = 0; i < rows; i++) {
        std::vector<std::string> temp;
        SplitString(lines[i], ',', temp);
        for (auto& str : temp) {
            str = Trim(str);
        }
        ret.push_back(temp);
    }
    return ret;
}

int getStudentIdx (const std::string & student, const V2D & studentFile) {
    for (unsigned i = 0; i < studentFile.size(); i++) {
        if (studentFile[i][0] == student) {
            return i;
        }
    }
    return -1;
}

/**
 * Given a course roster and a list of students and their courses, 
 * perform data correction and return a course roster of valid students (and only non-empty courses).
 * 
 * A 'valid student' is a student who is both in the course roster and the student's own listing contains the course
 * A course which has no students (or all students have been removed for not being valid) should be removed
 * 
 * @param cv A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param student A 2D vector of strings where each row is a student ID followed by the courses they are taking
 */
V2D clean(const V2D & cv, const V2D & student){
    // To be returned
    V2D cleanRF = cv;

    // Go through each course
    for (int i = 0; i < (int)cleanRF.size(); i++) {
        // Get course name
        std::string course = cleanRF[i][0];
        // std::cout << "COURSE: " << course << std::endl;
        // Go through each student enrolled in each course
        for (int j = 1; j < (int)cleanRF[i].size(); j++) {
            // Get idx of a student in V2D student
            int studentIdx = getStudentIdx(cleanRF[i][j], student);
            // std::cout << cleanRF[i][j] << " idx: " << getStudentIdx(cleanRF[i][j], student) << std::endl;
            // Check if student exists and is enrolled in course
            if (studentIdx == -1 || std::find(student[studentIdx].begin(), student[studentIdx].end(), course) == student[studentIdx].end()) {
                cleanRF[i].erase(cleanRF[i].begin() + j);
                j--;
            }
        }
        // Check if a course has no students
        if (cleanRF[i].size() == 1) {
            cleanRF.erase(cleanRF.begin() + i);
            i--;
        }
    }
    return cleanRF;
}

void getGraph(const V2D &courses, std::map<std::string, std::vector<std::string>> &graph) {
    // Go through each course
    for (unsigned i = 0; i < courses.size() - 1; i++) {
        std::string course1 = courses[i][0];
        // std::cout << "COURSE ONE: " << course1 << std::endl;
        // Go through other courses
        for (unsigned j = i + 1; j < courses.size(); j++) {
            std::string course2 = courses[j][0];
            // std::cout << "  COURSE TWO: " << course2 << std::endl;
            // Check if course1 and course2 share students
            for (unsigned k = 1; k < courses[i].size(); k++) {
                // std::cout << "      STUDENT : " << courses[i][k];
                if (std::find(courses[j].begin() + 1, courses[j].end(), courses[i][k]) != courses[j].end()) {
                    // std::cout << "  TRUE" << std::endl;
                    graph[course1].push_back(course2);
                    graph[course2].push_back(course1);
                    break;
                }
                // std::cout << std::endl;
            }
        }
    }
}

/**
 * Given a collection of courses and a list of available times, create a valid scheduling (if possible).
 * 
 * A 'valid schedule' should assign each course to a timeslot in such a way that there are no conflicts for exams
 * In other words, two courses who share a student should not share an exam time.
 * Your solution should try to minimize the total number of timeslots but should not exceed the timeslots given.
 * 
 * The output V2D should have one row for each timeslot, even if that timeslot is not used.
 * 
 * As the problem is NP-complete, your first scheduling might not result in a valid match. Your solution should 
 * continue to attempt different schedulings until 1) a valid scheduling is found or 2) you have exhausted all possible
 * starting positions. If no match is possible, return a V2D with one row with the string '-1' as the only value. 
 * 
 * @param courses A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param timeslots A vector of strings giving the total number of unique timeslots
 */
V2D schedule(const V2D &courses, const std::vector<std::string> &timeslots){
    // Adjacency List
    std::map<std::string, std::vector<std::string>> graph;
    // Initialize graph
    getGraph(courses, graph);
    // returned schedule
    V2D ret;
    for (auto timeslot : timeslots) {
        ret.push_back(std::vector<std::string>{timeslot});
    }
    
    // Start BFS
    std::set<std::string> visited;
    // Start Graph Coloring
    for (auto pair : graph) {
        if (visited.find(pair.first) != visited.end()) {
            continue;
        }
        std::queue<std::string> queue;
        queue.push(pair.first);
        visited.insert(pair.first);

        // BFS
        while (!queue.empty())
        {
            std::string curr = queue.front();
            queue.pop();

            // Find first available timeslot
            std::vector<bool> openTimeslot(timeslots.size(), true);
            for (auto neighbor : graph[curr]) {
                // Push neighbors onto queue
                if (visited.find(neighbor) == visited.end())
                {
                    queue.push(neighbor);
                    visited.insert(neighbor);
                }
                // Determine closed timeslots
                for (unsigned i = 0; i < ret.size(); i++) {
                    if (std::find(ret[i].begin() + 1, ret[i].end(), neighbor) != ret[i].end()) {
                        openTimeslot[i] = false;
                    }
                }
            }
            // Find open timeslot
            int timeslotIdx = -1;
            for (unsigned i = 0; i < openTimeslot.size(); i++) {
                if (openTimeslot[i]) {
                    timeslotIdx = i;
                    break;
                }
            }
            // No Possible Coloring
            if (timeslotIdx == -1) {
                return V2D {{"-1"}};
            }
            // Assign timeslot
            ret[timeslotIdx].push_back(curr);
            // Testing
            /*std::cout << curr << ": {";
            for (unsigned i = 0; i < openTimeslot.size(); i++) {
                std::cout << openTimeslot[i] << ", ";
            }
            std::cout << "} \n";*/
        }
        
    }

    // Testing
    /*for (auto vec : ret) {
        std::cout << "TIMESLOT " << vec[0] << ": {";
        for (unsigned i = 1; i < vec.size(); i++) {
            std::cout << vec[i] << ", ";
        }
        std::cout << "}" << std::endl; 
    }*/

    return ret;
}