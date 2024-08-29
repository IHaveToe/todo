#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unistd.h>

using namespace std;


vector<string> todo;

//filepath for todo list file
string user = getlogin();
string path = {"/home/" + user + "/todo.md"};

void appendList (string task) {

    //create file object
    fstream todoFile;
    todoFile.open(path, ios::app);

    todoFile << task << "\n";

    todoFile.close();
}

void readList () {
    //create file object
    string user = getlogin();
    fstream todoFile(path);
    
    // string to hold line before giving to vector
    string buf;

    //push line to end of vector
    while (getline(todoFile, buf)){
        todo.push_back(buf);
    }

    todoFile.close();
}

void removeTask (string task) {
    // remove the task specifed by its string parameter
   
    readList();

    // iterator object to move through todo vector
    vector<string>::iterator iter = todo.begin();

    int index = 0;

    // find and delete task from todo vector
    while(iter != todo.end()) {
        if (!todo[index].compare(task)) {
            iter = todo.erase(iter);
        }
        else {
            ++iter;
            index++;
        }
    }

    //delete old to-do file
    const char* path_ptr = path.c_str();
    remove(path_ptr);
    
    //create file object
    fstream todoFile;
    todoFile.open(path, ios::app);

    // write todo list into file
    for (int index = 0; index < todo.size(); index++) {

        todoFile << todo[index] << endl;
    }

}

int main (int argc, char *argv[]) {
    // vector to hold comandline arguments
    vector<string> args(argv + 1, argv+argc);
    
    // no arguments
    if (argc == 1) {
        cout << "incorrect usage ('-h' or '--help' for help)" << endl;
        return 1;
    }

    //help argument
    else if (!args[0].compare("-h") | !args[0].compare("--help")) {
        cout << "will add later bb" << endl;
        return 2;
    }

    //append 
    else if (!args[0].compare("-a")) {
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

            appendList(nameOfTask);
            return 0;
        }
        //no name of task provided
        else {
            cout << "incorrect usage, please provide name of task you want to add ('-h' or '--help' for help)";
            return 3;
        }
    }

    //list tasks
    else if (!args[0].compare("-l")) {
        readList();
        //print tasks to console
        for (int i = 0; i < todo.size(); i++) {
        cout << todo.at(i) << endl;
        }
        return 0;
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
            removeTask(nameOfTask);
            return 0;
        }
        //no name of task provided
        else {
            cout << "incorrect usage, please provide name of task you want to remove ('-h' or '--help' for help)";
            return 3;
        }
    }


}