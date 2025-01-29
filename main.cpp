#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <algorithm>
#include <queue>
#include <climits>
#include <iomanip>
#include <set>

using namespace std;

struct Process {
    string name;
    int arrival_time;
    int service_time;
    int remaining_time;
    int start_time;
    int completion_time;
    int waiting_time;
    int turn_around_time;
    int priority;
    
    Process() : arrival_time(0), service_time(0), remaining_time(0),
                start_time(-1), completion_time(0), waiting_time(0),
                turn_around_time(0), priority(0) {}
};

class CPUScheduler {
private:
    vector<Process> processes;
    int process_count;

    void printResults() const {
        cout << "\nScheduling Results:\n";
        cout << setw(10) << "Process" << setw(10) << "Arrival" << setw(10) << "Service"
             << setw(12) << "Start Time" << setw(15) << "Completion" << setw(10) << "Waiting"
             << setw(12) << "Turnaround" << endl;
        cout << string(79, '-') << endl;

        for (const auto& p : processes) {
            cout << setw(10) << p.name << setw(10) << p.arrival_time << setw(10) << p.service_time
                 << setw(12) << p.start_time << setw(15) << p.completion_time << setw(10) << p.waiting_time
                 << setw(12) << p.turn_around_time << endl;
        }

        double avg_waiting = 0, avg_turnaround = 0;
        for (const auto& p : processes) {
            avg_waiting += p.waiting_time;
            avg_turnaround += p.turn_around_time;
        }
        avg_waiting /= process_count;
        avg_turnaround /= process_count;

        cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << avg_waiting << endl;
        cout << "Average Turnaround Time: " << avg_turnaround << endl;
    }

public:
    void takeInput() {
        cout << "Enter the number of processes: ";
        cin >> process_count;
        
        if (process_count <= 0) {
            throw runtime_error("Number of processes must be positive");
        }
        
        processes.resize(process_count);

        for (int i = 0; i < process_count; i++) {
            cout << "\nProcess " << (i + 1) << ":\n";
            cout << "Enter Process Name: ";
            cin >> processes[i].name;

            cout << "Arrival Time: ";
            cin >> processes[i].arrival_time;
            
            cout << "Service Time: ";
            cin >> processes[i].service_time;

            if (processes[i].arrival_time < 0 || processes[i].service_time <= 0) {
                throw runtime_error("Invalid arrival time or service time");
            }

            processes[i].remaining_time = processes[i].service_time;
        }
    }

    void firstComeFirstServe() {
        sort(processes.begin(), processes.end(), 
             [](const Process& a, const Process& b) { return a.arrival_time < b.arrival_time; });

        int current_time = 0;
        for (auto& p : processes) {
            if (current_time < p.arrival_time) {
                current_time = p.arrival_time;
            }
            p.start_time = current_time;
            current_time += p.service_time;
            p.completion_time = current_time;
            p.waiting_time = p.start_time - p.arrival_time;
            p.turn_around_time = p.completion_time - p.arrival_time;
        }
        printResults();
    }

    void shortestJobFirst() {
        vector<bool> completed(process_count, false);
        int current_time = 0;
        int completed_count = 0;

        while (completed_count < process_count) {
            int shortest_job = -1;
            int min_burst_time = INT_MAX;

            for (int i = 0; i < process_count; i++) {
                if (!completed[i] && processes[i].arrival_time <= current_time && 
                    processes[i].service_time < min_burst_time) {
                    min_burst_time = processes[i].service_time;
                    shortest_job = i;
                }
            }

            if (shortest_job == -1) {
                current_time++;
                continue;
            }

            auto& p = processes[shortest_job];
            p.start_time = current_time;
            p.completion_time = current_time + p.service_time;
            p.waiting_time = p.start_time - p.arrival_time;
            p.turn_around_time = p.completion_time - p.arrival_time;
            
            current_time = p.completion_time;
            completed[shortest_job] = true;
            completed_count++;
        }
        printResults();
    }

    void shortestRemainingTime() {
        vector<Process> temp = processes;
        int current_time = 0;
        int completed_count = 0;

        while (completed_count < process_count) {
            int shortest_remaining = -1;
            int min_remaining_time = INT_MAX;

            for (int i = 0; i < process_count; i++) {
                if (temp[i].arrival_time <= current_time && temp[i].remaining_time > 0 && 
                    temp[i].remaining_time < min_remaining_time) {
                    min_remaining_time = temp[i].remaining_time;
                    shortest_remaining = i;
                }
            }

            if (shortest_remaining == -1) {
                current_time++;
                continue;
            }

            if (temp[shortest_remaining].start_time == -1) {
                temp[shortest_remaining].start_time = current_time;
                processes[shortest_remaining].start_time = current_time;
            }

            temp[shortest_remaining].remaining_time--;
            current_time++;

            if (temp[shortest_remaining].remaining_time == 0) {
                temp[shortest_remaining].completion_time = current_time;
                processes[shortest_remaining].completion_time = current_time;
                processes[shortest_remaining].waiting_time = 
                    current_time - processes[shortest_remaining].arrival_time - processes[shortest_remaining].service_time;
                processes[shortest_remaining].turn_around_time = 
                    current_time - processes[shortest_remaining].arrival_time;
                completed_count++;
            }
        }
        printResults();
    }

    void roundRobin(int quantum) {
        if (quantum <= 0) {
            throw runtime_error("Time quantum must be positive");
        }

        vector<Process> temp = processes;
        queue<int> ready_queue;
        set<int> in_queue;
        int current_time = 0;
        int completed_count = 0;

        while (completed_count < process_count) {
            // Add newly arrived processes to ready queue
            for (int i = 0; i < process_count; i++) {
                if (temp[i].arrival_time <= current_time && 
                    temp[i].remaining_time > 0 && 
                    in_queue.find(i) == in_queue.end()) {
                    ready_queue.push(i);
                    in_queue.insert(i);
                }
            }

            if (ready_queue.empty()) {
                current_time++;
                continue;
            }

            int current_process = ready_queue.front();
            ready_queue.pop();
            in_queue.erase(current_process);

            if (temp[current_process].start_time == -1) {
                temp[current_process].start_time = current_time;
                processes[current_process].start_time = current_time;
            }

            int execution_time = min(quantum, temp[current_process].remaining_time);
            temp[current_process].remaining_time -= execution_time;
            current_time += execution_time;

            if (temp[current_process].remaining_time == 0) {
                temp[current_process].completion_time = current_time;
                processes[current_process].completion_time = current_time;
                processes[current_process].waiting_time = 
                    current_time - processes[current_process].arrival_time - processes[current_process].service_time;
                processes[current_process].turn_around_time = 
                    current_time - processes[current_process].arrival_time;
                completed_count++;
            } else {
                ready_queue.push(current_process);
                in_queue.insert(current_process);
            }
        }
        printResults();
    }

    void priorityScheduling() {
        vector<bool> completed(process_count, false);
        int current_time = 0;
        int completed_count = 0;

        cout << "Enter priorities for each process (lower number = higher priority):\n";
        for (int i = 0; i < process_count; i++) {
            cout << "Priority for process " << processes[i].name << ": ";
            cin >> processes[i].priority;
        }

        while (completed_count < process_count) {
            int highest_priority_job = -1;
            int highest_priority = INT_MAX;

            for (int i = 0; i < process_count; i++) {
                if (!completed[i] && processes[i].arrival_time <= current_time && 
                    processes[i].priority < highest_priority) {
                    highest_priority = processes[i].priority;
                    highest_priority_job = i;
                }
            }

            if (highest_priority_job == -1) {
                current_time++;
                continue;
            }

            auto& p = processes[highest_priority_job];
            p.start_time = current_time;
            p.completion_time = current_time + p.service_time;
            p.waiting_time = p.start_time - p.arrival_time;
            p.turn_around_time = p.completion_time - p.arrival_time;
            
            current_time = p.completion_time;
            completed[highest_priority_job] = true;
            completed_count++;
        }
        printResults();
    }
};

int main() {
    CPUScheduler scheduler;
    try {
        scheduler.takeInput();

        cout << "\nSelect Scheduling Algorithm:\n"
             << "1. First-Come-First-Serve (FCFS)\n"
             << "2. Shortest Job First (SJF)\n"
             << "3. Shortest Remaining Time (SRT)\n"
             << "4. Round Robin\n"
             << "5. Priority Scheduling\n"
             << "Enter your choice (1-5): ";
        
        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                scheduler.firstComeFirstServe();
                break;
            case 2:
                scheduler.shortestJobFirst();
                break;
            case 3:
                scheduler.shortestRemainingTime();
                break;
            case 4: {
                int quantum;
                cout << "Enter time quantum for Round Robin: ";
                cin >> quantum;
                scheduler.roundRobin(quantum);
                break;
            }
            case 5:
                scheduler.priorityScheduling();
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}