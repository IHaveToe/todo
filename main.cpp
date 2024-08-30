#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unistd.h>

using namespace std;

class TodoList {
    private:
        vector<string> tasks;
        string path;

        // helper function to read tasks from file
        void readFromFile() {
            ifstream todoFile(path);
            if (!todoFile.is_open()) {
                cerr << "Error: unable to open file" << endl;
                todoFile.close();
                return;
            }

            string line;

            while (getline(todoFile, line)) {
                tasks.push_back(line);
            }
            todoFile.close();
        }

        //helper fuction for writing all tasks to file
        void writeToFile() {
            ofstream todoFile(path, ios::trunc);

            if (!todoFile.is_open()) {
                cerr << "Error: unable to open file" << endl;
                todoFile.close();
                return;
            }

            for (const string &task : tasks) {
                todoFile << task << endl;
            }
            todoFile.close();
        }


        void appendToFile() {
            ofstream todoFile(path, ios::app);
            if (!todoFile.is_open()) {
                cerr << "Error: unable to open file" << endl;
                todoFile.close();
                return;
            }
            // appends last item in tasks vector
            int taskIndex = tasks.size() -1;
            todoFile << tasks[taskIndex] << endl;
            todoFile.close();
        }

        void replaceInFile(string lineToDelete) {
            ofstream writer(path, ios::trunc);
            ifstream reader(path);

            if (!writer.is_open()) {
                cerr << "Error: unable to open file for writing" << endl;
                return;
            }

            else if (!reader.is_open()) {
                cerr << "Error: unable to open file for reading" << endl;
                return;
            }

            string line;
            bool passed = false;
            while (getline(reader, line)) {
                if (!(line == lineToDelete)){
                    writer << line << endl;
                }
            }

            writer.close();
            reader.close();

        }

    public:
        //constructor for initilizing TodoList object
        TodoList(const string user) {
            path = "/home/" + user + "/todo.md";
            readFromFile();
        }

        // add a task
        void addTask (const string task) {
            tasks.push_back(task);
            appendToFile();
        }

        // list all tasks
        void listTasks () {
            if (tasks.empty()) {
                cout << "No tasks found" << endl;
                return;
            }
            for (int index = 0; index < tasks.size(); index++) {
                cout << index+1 << ". " << tasks.at(index) << endl;
            }
        }

        void removeTask (const string task) {
            readFromFile();

            // iterator object to move through todo vector
            vector<string>::iterator iter = tasks.begin();

            int index = 0;

            // find and delete task from tasks vector
            while(iter != tasks.end()) {
                if (!tasks[index].compare(task)) {
                    iter = tasks.erase(iter);
                    replaceInFile(tasks[index]);
                }
                else {
                    ++iter;
                    index++;
                }
            }
        }


};

int main (int argc, char *argv[]) {

    // no arguments
    if (argc == 1) {
        cout << "incorrect usage ('-h' or '--help' for help)" << endl;
    }

    // vector to hold comandline arguments
    vector<string> args(argv + 1, argv+argc);
    const string command = args[0];

    string user = getlogin();
    TodoList todoList(user);

    //help argument
    if (command == "-h" | command == "--help") {
        cout << "Usage:" << endl;
        cout << "-a <task> : Add a task" << endl;
        cout << "-l        : List all tasks" << endl;
        cout << "-r <task> : Remove a task" << endl;
    }

    //append 
    else if (command == "-a") {
        // check if name of task is provided
        if (args.size() >= 2) {
            
            // get name of task
            string nameOfTask;
            for(int index = 1; index < args.size(); index++) {
                
                nameOfTask.append(args[index]);
                //only add space if argument is not last
                if (index < (args.size()-1)){
                    nameOfTask.append(" ");
                }
            }

            todoList.addTask(nameOfTask);
        }
        //no name of task provided
        else {
            cout << "incorrect usage, please provide name of task you want to add ('-h' or '--help' for help)";
        }
    }

    //list tasks
    else if (!args[0].compare("-l")) {
        todoList.listTasks();
    }

    //remove task
    else if (!args[0].compare("-r")) {
        //check if name of task is provided
        if (args.size() >= 2) {
            
            // get name of task
            string nameOfTask;
            for(int index = 1; index < args.size(); index++) {
                
                nameOfTask.append(args[index]);
                //only add space if argument is not last
                if (index < (args.size()-1)){
                    nameOfTask.append(" ");
                }
            }
            todoList.removeTask(nameOfTask);
        }
        //no name of task provided
        else {
            cout << "incorrect usage, please provide name of task you want to remove ('-h' or '--help' for help)";
            return 3;
        }
    }

    else {
        cout << "Unknown command ('-h' or '--help' for help)" << endl;
    }

    return 0;
}