#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

// Task class representing a task with a name and priority
class Task {
public:
    std::string name;
    int priority;

    Task(std::string name, int priority) : name(name), priority(priority) {}

    bool operator<(const Task& other) const {
        return priority > other.priority; // For min-heap, higher priority tasks come first
    }
};

// ToDoList class managing the tasks
class ToDoList {
private:
    std::vector<Task> tasks;
    std::priority_queue<Task> priorityQueue;
    std::stack<std::pair<std::string, Task>> taskStack;

public:
    void addTask(const std::string& taskName, int priority) {
        Task task(taskName, priority);
        tasks.push_back(task);
        priorityQueue.push(task);
        taskStack.push({"add", task});
    }

    void removeTask(const std::string& taskName) {
        auto it = std::find_if(tasks.begin(), tasks.end(), [&](Task& task) { return task.name == taskName; });
        if (it != tasks.end()) {
            Task task = *it;
            tasks.erase(it);
            rebuildPriorityQueue();
            taskStack.push({"remove", task});
        } else {
            std::cout << "Task not found" << std::endl;
        }
    }

    void markCompleted(const std::string& taskName) {
        auto it = std::find_if(tasks.begin(), tasks.end(), [&](Task& task) { return task.name == taskName; });
        if (it != tasks.end()) {
            Task task = *it;
            tasks.erase(it);
            rebuildPriorityQueue();
            taskStack.push({"complete", task});
        } else {
            std::cout << "Task not found" << std::endl;
        }
    }

    void listTasks() const {
        for (const auto& task : tasks) {
            std::cout << "Task: " << task.name << ", Priority: " << task.priority << std::endl;
        }
    }

    void listTasksByPriority() {
        std::priority_queue<Task> tempQueue = priorityQueue;
        while (!tempQueue.empty()) {
            Task task = tempQueue.top();
            std::cout << "Task: " << task.name << ", Priority: " << task.priority << std::endl;
            tempQueue.pop();
        }
    }

    void undo() {
        if (taskStack.empty()) {
            std::cout << "No operations to undo" << std::endl;
            return;
        }

        auto lastAction = taskStack.top();
        taskStack.pop();

        if (lastAction.first == "add") {
            auto it = std::find_if(tasks.begin(), tasks.end(), [&](Task& task) { return task.name == lastAction.second.name; });
            if (it != tasks.end()) {
                tasks.erase(it);
                rebuildPriorityQueue();
            }
        } else if (lastAction.first == "remove" || lastAction.first == "complete") {
            tasks.push_back(lastAction.second);
            priorityQueue.push(lastAction.second);
        }
    }

private:
    void rebuildPriorityQueue() {
        while (!priorityQueue.empty()) priorityQueue.pop();
        for (const auto& task : tasks) {
            priorityQueue.push(task);
        }
    }
};

// Function to display the menu
void displayMenu() {
    std::cout << "\nTo-Do List Menu:\n";
    std::cout << "1. Add a Task\n";
    std::cout << "2. Remove a Task\n";
    std::cout << "3. Mark Task as Completed\n";
    std::cout << "4. List All Tasks\n";
    std::cout << "5. List Tasks by Priority\n";
    std::cout << "6. Undo Last Operation\n";
    std::cout << "7. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    ToDoList todoList;
    int choice;
    
    do {
        displayMenu();
        std::cin >> choice;

        std::string taskName;
        int priority;

        switch (choice) {
            case 1:
                std::cout << "Enter task description: ";
                std::cin.ignore(); // To ignore the newline character left by std::cin
                std::getline(std::cin, taskName);
                std::cout << "Enter task priority: ";
                std::cin >> priority;
                todoList.addTask(taskName, priority);
                break;
            case 2:
                std::cout << "Enter task name to remove: ";
                std::cin.ignore();
                std::getline(std::cin, taskName);
                todoList.removeTask(taskName);
                break;
            case 3:
                std::cout << "Enter task name to mark as completed: ";
                std::cin.ignore();
                std::getline(std::cin, taskName);
                todoList.markCompleted(taskName);
                break;
            case 4:
                std::cout << "All tasks:\n";
                todoList.listTasks();
                break;
            case 5:
                std::cout << "Tasks by priority:\n";
                todoList.listTasksByPriority();
                break;
            case 6:
                todoList.undo();
                break;
            case 7:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 7);

    return 0;
}
