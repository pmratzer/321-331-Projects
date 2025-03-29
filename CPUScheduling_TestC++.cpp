#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <algorithm>
#include <ctime>
#include <chrono>
using namespace std;

struct Process {
    int id, arrival_time, process_completion, wait_time, recorded_arrival;  //replacement for list in python
    Process(int i, int a, int b) : id(i), arrival_time(a), process_completion(b), wait_time(0), recorded_arrival(0) {}
};

bool compareArrival(const Process& a, const Process& b) {
    return a.arrival_time < b.arrival_time;
}

void create_process_list(int num_processes, vector<Process>& process_list) {
    for (int i = 0; i < num_processes; i++) {
        int arrival_time = rand() % 2000;
        int process_completion = rand() % 200 + 1;
        process_list.push_back(Process(i, arrival_time, process_completion)); //create new process and put it into list
    }
    sort(process_list.begin(), process_list.end(), compareArrival);  //sort the list from beginning to end by comparing their arrival times (passed by reference)
}

void FCFS(int clock_cycles, vector<Process> process_list) { //don't pass by reference so we dont alter the original
    deque<Process> queue;
    int TAT_total = 0;
    int added_processes = 0;

    for (int i = 0; i < clock_cycles; i++) {
        for (int p = added_processes; p < process_list.size(); ++p) {
            if (process_list[p].arrival_time == i) {    //if it's the processes arrival time
                process_list[p].recorded_arrival = i;   //record it
                queue.push_back(process_list[p]);            //push onto process queue
                added_processes += 1;                   //increment added processes
            }
        }
        if (!queue.empty()) {
            queue.front().process_completion--; //decrement burst time
            if (queue.front().process_completion == 0) {  //if burst time is done
                TAT_total += (i - queue.front().recorded_arrival);  //calculate turnaround time
                queue.pop_front();                                        //remove from front of queue
            }
        }
    }
    cout << "Average Turnaround Time: " << (double)TAT_total / 2000 << endl;
}

void RR(int clock_cycles, vector<Process> process_list) {
    deque<Process> queue;
    int added_processes = 0;
    int TAT_total = 0;
    for (int i = 0; i < clock_cycles; i++) {
        for (int p = added_processes; p < process_list.size(); ++p) {
            if (process_list[p].arrival_time == i) {    //if it's the processes arrival time
                queue.push_front(process_list[p]);            //push onto process queue front
                process_list[p].recorded_arrival = i;   //record it
                added_processes += 1;                   //increment added processes
            }
        }
        if (!queue.empty()) {
            queue.front().process_completion--; //decrement burst time
            if (queue.front().process_completion > 0)
                queue.push_back(queue.front());
            else
                TAT_total += (i - queue.front().recorded_arrival);
            queue.pop_front();
        }
    }
    cout << "Average Turnaround Time: " << (double)TAT_total / 2000 << endl;
}

void SPN(int clock_cycles, vector<Process> process_list) {
    deque<Process> queue;
    int added_processes = 0, TAT_total = 0;
    for (int i = 0; i < clock_cycles; i++) {
        for (int p = added_processes; p < process_list.size(); ++p) {
            if (process_list[p].arrival_time == i) {    //if it's the processes arrival time
                process_list[p].recorded_arrival = i;   //record it
                if (!queue.empty()) {
                    int insert_spot = 1;
                    for (int j = 1; j < queue.size(); j++) {
                        if (process_list[p].process_completion > queue[j].process_completion) {
                            insert_spot += 1;
                            break;
                        }
                    }
                    queue.insert(queue.begin() + insert_spot, process_list[p]);
                }
                else {
                    queue.push_front(process_list[p]);
                }
                added_processes += 1;
            }
        }
        if (!queue.empty()) {
            queue[0].process_completion--;
            if (queue[0].process_completion == 0) {
                TAT_total += (i - queue[0].recorded_arrival);
                queue.pop_front();
            }
        }
    }
    cout << "Average Turnaround Time: " << (double)TAT_total / 2000 << endl;
}

void SRT(int clock_cycles, vector<Process> process_list) {
    deque<Process> queue;
    int added_processes = 0;
    int TAT_total = 0;
    for (int i = 0; i < clock_cycles; i++) {
        for (int p = added_processes; p < process_list.size(); ++p) {
            if (process_list[p].arrival_time == i) {    //if it's the processes arrival time
                process_list[p].recorded_arrival = i;   //record it
                if (!queue.empty()) {
                    int insert_spot = 0;
                    for (int j = 1; j < queue.size(); j++) {
                        if (process_list[p].process_completion > queue[j].process_completion) {
                            insert_spot += 1;
                            break;
                        }
                    }
                    queue.insert(queue.begin() + insert_spot, process_list[p]);
                }
                else {
                    queue.push_front(process_list[p]);
                }
                added_processes += 1;
            }
        }
        if (!queue.empty()) {
            queue[0].process_completion--;
            if (queue[0].process_completion == 0) {
                TAT_total += (i - queue[0].recorded_arrival);
                queue.pop_front();
            }
        }
    }
    cout << "Average Turnaround Time: " << (double)TAT_total / 2000 << endl;
}

void HRRN(int clock_cycles, vector<Process> process_list) {
    deque<Process> queue;
    int added_processes = 0;
    int TAT_total = 0;
    for (int i = 0; i < clock_cycles; i++) {
        for (int p = added_processes; p < process_list.size(); ++p) {
            if (process_list[p].arrival_time == i) {    //if it's the processes arrival time
                queue.push_front(process_list[p]);            //push onto process queue front
                process_list[p].recorded_arrival = i;   //record it
                added_processes += 1;                   //increment added processes
            }
        }
        if (!queue.empty()) {
            queue[0].process_completion--;
            if (queue[0].process_completion == 0) {
                TAT_total += (i - queue[0].recorded_arrival);
                queue.pop_front();
                int highest_response_ratio = -1;
                int best_process_index = -1;
                for (int j = 0; j < queue.size(); j++) {
                    queue[j].wait_time += 1;    //increment all processes wait time in queue
                    if (queue[j].recorded_arrival != 0) {
                        double response_ratio = ((queue[j].wait_time + queue[j].recorded_arrival) / queue[j].recorded_arrival);
                        if (response_ratio > highest_response_ratio) {
                            highest_response_ratio = response_ratio;
                            best_process_index = j;
                        }
                    }
                }
                if (best_process_index > 0) {
                    Process best_process = queue[best_process_index];  //copy best process into temp var
                    queue.erase(queue.begin() + best_process_index);   //erase the old version of process
                    queue.push_front(best_process);                    //put copy in front of queue
                }
            }
        }
    }
    cout << "Average Turnaround Time: " << (double)TAT_total / 2000 << endl;
}

void Feedback(int clock_cycles, vector<Process> process_list) {
    deque<Process> queue1;
    deque<Process> queue2;
    deque<Process> queue3;
    deque<Process> queue4;
    int added_processes = 0;
    int TAT_total = 0;
    for (int i = 0; i < clock_cycles; i++) {
        for (int p = added_processes; p < process_list.size(); ++p) {
            if (process_list[p].arrival_time == i) {    //if it's the processes arrival time
                queue1.push_front(process_list[p]);            //push onto process queue front
                process_list[p].recorded_arrival = i;   //record it
                added_processes += 1;                   //increment added processes
            }
        }
        if (!queue1.empty()) {
            queue1.front().process_completion--; //decrement burst time
            if (queue1.front().process_completion > 0)
                queue1.push_back(queue1.front());
            else
                TAT_total += (i - queue1.front().recorded_arrival);
            queue1.pop_front();
        }
        else if (!queue2.empty()) {
            queue2.front().process_completion--; //decrement burst time
            if (queue2.front().process_completion > 0)
                queue2.push_back(queue2.front());
            else
                TAT_total += (i - queue2.front().recorded_arrival);
            queue2.pop_front();
        }
        else if (!queue3.empty()) {
            queue3.front().process_completion--; //decrement burst time
            if (queue3.front().process_completion > 0)
                queue3.push_back(queue3.front());
            else
                TAT_total += (i - queue3.front().recorded_arrival);
            queue3.pop_front();
        }
        else if (!queue4.empty()) {
            queue4.front().process_completion--; //decrement burst time
            if (queue4.front().process_completion > 0)
                queue4.push_back(queue4.front());
            else
                TAT_total += (i - queue4.front().recorded_arrival);
            queue4.pop_front();
        }
    }
    cout << "Average Turnaround Time: " << (double)TAT_total / 2000 << endl;
}

int main() {
    srand(time(0));
    int num_processes = 2000;
    vector<Process> process_list;
    create_process_list(num_processes, process_list);
    int total_cycles = 0;
    for (int i = 0; i < process_list.size(); ++i) {
        total_cycles += process_list[i].process_completion;
        //the appends in the python are taken care of in the struct
    }
    cout << "Total Cycles: " << total_cycles << "\n\n";

    auto start_time = chrono::high_resolution_clock::now();  //the syntax for using chrono was done by ChatGPT
    cout << "FCFS\n";
    FCFS(total_cycles, process_list);
    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> execution_time = end_time - start_time;
    cout << "Execution time: " << execution_time.count() << " Seconds\n";

    start_time = chrono::high_resolution_clock::now();
    cout << "RR\n";
    RR(total_cycles, process_list);
    end_time = chrono::high_resolution_clock::now();
    execution_time = end_time - start_time;
    cout << "Execution time: " << execution_time.count() << " Seconds\n";

    start_time = chrono::high_resolution_clock::now();
    cout << "SPN\n";
    SPN(total_cycles, process_list);
    end_time = chrono::high_resolution_clock::now();
    execution_time = end_time - start_time;
    cout << "Execution time: " << execution_time.count() << " Seconds\n";

    start_time = chrono::high_resolution_clock::now();
    cout << "SRT\n";
    SRT(total_cycles, process_list);
    end_time = chrono::high_resolution_clock::now();
    execution_time = end_time - start_time;
    cout << "Execution time: " << execution_time.count() << " Seconds\n";

    start_time = chrono::high_resolution_clock::now();
    cout << "HRRN\n";
    HRRN(total_cycles, process_list);
    end_time = chrono::high_resolution_clock::now();
    execution_time = end_time - start_time;
    cout << "Execution time: " << execution_time.count() << " Seconds\n";

    start_time = chrono::high_resolution_clock::now();
    cout << "Feedback\n";
    Feedback(total_cycles, process_list);
    end_time = chrono::high_resolution_clock::now();
    execution_time = end_time - start_time;
    cout << "Execution time: " << execution_time.count() << " Seconds\n";

    return 0;
}
