from copy import deepcopy
import time
import random

#NOTE: assume for every scheduling algorithm that processes can be scheduled on the same cycle they arrive
def FCFS(clock_cycles, process_list_orig):
    process_list = deepcopy(process_list_orig)
    queue = []
    TAT_total = 0
    added_processes = 0     #shortens the amount of times we iterate over process_list

    for i in range(clock_cycles):
        for process in process_list[added_processes:]:  #each process in order of arrival time
            if process[1] == i:
                process[4] = i  #arrival time put into process[4]
                queue.append(process)
                added_processes += 1
                
        if len(queue) > 0:
            queue[0][2] -= 1
            if queue[0][2] == 0:
                TAT_total += (i - queue[0][4])  #add to our total for Turnaround time (completion time - arrival time)
                queue.pop(0)

    print("Average turnaround time: ", TAT_total/2000)


def RR(clock_cycles, process_list_orig): #prioritize arriving processes (schedule at arrival)
    process_list = deepcopy(process_list_orig)
    queue = []
    added_processes = 0     #shortens the amount of times we iterate over process_list
    TAT_total = 0


    for i in range(clock_cycles):
        for process in process_list[added_processes:]: #this for loop builds the queue as processes come in
            if process[1] == i: #if arrival time is current clock cycle
                queue.insert(0, process)   #add to queue (at front to schedule arriving processes)
                process[4] = i  #arrival time put into process[4]
                added_processes += 1

        if (len(queue) > 0):    #if queue isnt empty
            queue[0][2] -= 1 #required cycles decremented
            if (queue[0][2] > 0):
                queue.append(queue[0])
            else:
                TAT_total += (i - queue[0][4])  #add to our total for Turnaround time (completion time - arrival time)
            queue.pop(0)

    print("Average turnaround time: ", TAT_total/2000)



def SPN(clock_cycles, process_list_orig):
    process_list = deepcopy(process_list_orig)
    queue = []
    added_processes = 0     #shortens the amount of times we iterate over process_list
    TAT_total = 0

    for i in range(clock_cycles):
        for process in process_list[added_processes:]: #this for loop builds the queue as processes come in
            if process[1] == i: #if arrival time is current clock cycle
                process[4] = i  #arrival time put into process[4]
                if (len(queue) > 0):
                    insert_spot = 1
                    for p in queue[1:]:
                        if (process[2] > p[2]): #find place this process belongs in the queue
                            insert_spot += 1
                            break
                    queue.insert(insert_spot, process)
                else:
                    queue.insert(0, process)   #add to queue (at front to schedule arriving processes)
                added_processes += 1


        if (len(queue) > 0):    #if queue isnt empty
            queue[0][2] -= 1 #required cycles decremented
            if (queue[0][2] == 0):
                TAT_total += (i - queue[0][4])  #add to our total for Turnaround time (completion time - arrival time)
                queue.pop(0)
    print("Average turnaround time: ", TAT_total/2000)



def SRT(clock_cycles, process_list_orig):   #prioritizes newer processes
    process_list = deepcopy(process_list_orig)
    queue = []
    added_processes = 0     #shortens the amount of times we iterate over process_list
    TAT_total = 0
    for i in range(clock_cycles):
        for process in process_list[added_processes:]: #this for loop builds the queue as processes come in
            if process[1] == i: #if arrival time is current clock cycle
                process[4] = i  #arrival time put into process[4]
                if (len(queue) > 0):
                    insert_spot = 0
                    for p in queue:
                        if (process[2] > p[2]): #find place this process belongs in the queue
                            insert_spot += 1
                            break
                    queue.insert(insert_spot, process)
                
                else:
                    queue.insert(0, process)   #add to queue (at front to schedule arriving processes)
                added_processes += 1


        if (len(queue) > 0):    #if queue isnt empty
            queue[0][2] -= 1 #required cycles decremented
            if (queue[0][2] == 0):
                TAT_total += (i - queue[0][4])  #add to our total for Turnaround time (completion time - arrival time)
                queue.pop(0)

    print("Average turnaround time: ", TAT_total/2000)


def HRRN(clock_cycles, process_list_orig):
    process_list = deepcopy(process_list_orig)
    queue = []
    added_processes = 0     #shortens the amount of times we iterate over process_list
    TAT_total = 0
    for i in range(clock_cycles):
        for process in process_list[added_processes:]:
            if process[1] == i: #if arrival time is current clock cycle
                process[4] = i  #arrival time put into process[4]
                queue.append(process)   #add to queue 
                added_processes += 1


        if (len(queue) > 0):    #if queue isnt empty
            queue[0][2] -= 1 #required cycles decremented

            if (queue[0][2] == 0):      #go to next process (HRRN)
                TAT_total += (i - queue[0][4])  #add to our total for Turnaround time (completion time - arrival time)
                queue.pop(0)
                highest_response_ratio = -1
                best_process_index = -1  

                for index, p in enumerate(queue):  
                    p[3] += 1  # Increment wait time
                    response_ratio = (p[3] + p[2]) / p[2]  # Compute HRR

                    if response_ratio > highest_response_ratio:
                        highest_response_ratio = response_ratio
                        best_process_index = index  # Track the best process

                # Move the highest response ratio process to the front
                if best_process_index > 0:
                    queue.insert(0, queue.pop(best_process_index))
    print("Average turnaround time: ", TAT_total/2000)



def FEEDBACK(clock_cycles, process_list_orig): #feedback with 4 queues of priority
    process_list = deepcopy(process_list_orig)
    queue1 = []
    queue2 = []
    queue3 = []
    queue4 = []
    added_processes = 0     #shortens the amount of times we iterate over process_list
    TAT_total = 0
    for i in range(clock_cycles):
        for process in process_list[added_processes:]: #this for loop builds the queue as processes come in
            if process[1] == i: #if arrival time is current clock cycle
                process[4] = i  #arrival time put into process[4]
                queue1.insert(0, process)   #add to queue (at front to schedule arriving processes)
                added_processes += 1

        if (len(queue1) > 0):    #if queue isnt empty
            queue1[0][2] -= 1 #required cycles decremented
            if (queue1[0][2] > 0):
                queue2.append(queue1[0])
            else:
                TAT_total += (i - queue1[0][4])  #add to our total for Turnaround time (completion time - arrival time)
            queue1.pop(0)
        elif (len(queue2) > 0):
            queue2[0][2] -= 1 #required cycles decremented
            if (queue2[0][2] > 0):
                queue3.append(queue2[0])
            else:
                TAT_total += (i - queue2[0][4])  #add to our total for Turnaround time (completion time - arrival time)
            queue2.pop(0)
        elif (len(queue3) > 0):
            queue3[0][2] -= 1 #required cycles decremented
            if (queue3[0][2] > 0):
                queue4.append(queue3[0])
            else:
                TAT_total += (i - queue3[0][4])  #add to our total for Turnaround time (completion time - arrival time)
            queue3.pop(0)
        elif (len(queue4) > 0):
            queue4[0][2] -= 1 #required cycles decremented
            if (queue4[0][2] > 0):
                queue4.append(queue4[0])
            else:
                TAT_total += (i - queue4[0][4])  #add to our total for Turnaround time (completion time - arrival time)
            queue4.pop(0)
    print("Average turnaround time: ", TAT_total/2000)



def create_process_list(num_processes, process_list):
    process_counter = 0
    while(num_processes != 0):

        process_name = process_counter

        process_arrival = random.randint(0, 2000)

        process_completion = random.randint(1, 200)

        #processes are in a list containing name, arrival time, and needed runtime
        new_process = [process_name, process_arrival, process_completion]
        process_list.append(new_process)

        process_counter += 1;
        num_processes -= 1

    #bubble sort by process arrival time
    for i in range(len(process_list)):
        for j in range(0, len(process_list) - i - 1):
            if process_list[j][1] > process_list[j + 1][1]:
                process_list[j], process_list[j + 1] = process_list[j + 1], process_list[j]


num_processes = 2000
process_list_orig = []
keep_going = True
create_process_list(num_processes, process_list_orig)
total_cycles = 0
for process in process_list_orig:
    total_cycles += process[2]
    process.append(0)   #give every process a waiting time in [3]
    process.append(0)   #give every process an arrival time in [4] (for computing TAT)
print("Total Cycles: ", total_cycles)
print()
    

print()
start_time = time.time()
print("FCFS")
FCFS(total_cycles, process_list_orig.copy())
end_time = time.time()
print("Execution time: ", end_time - start_time, " Seconds")

print()
start_time = time.time()
print("RR")
RR(total_cycles, process_list_orig)
end_time = time.time()
print("Execution time: ", end_time - start_time, " Seconds")

print()
start_time = time.time()
print("SPN")
SPN(total_cycles, process_list_orig)
end_time = time.time()
print("Execution time: ", end_time - start_time, " Seconds")

print()
start_time = time.time()
print("SRT")
SRT(total_cycles, process_list_orig)
end_time = time.time()
print("Execution time: ", end_time - start_time, " Seconds")

print()
start_time = time.time()
print("HRRN")
HRRN(total_cycles, process_list_orig)
end_time = time.time()
print("Execution time: ", end_time - start_time, " Seconds")

print()
start_time = time.time()
print("FB")
FEEDBACK(total_cycles, process_list_orig)
end_time = time.time()
print("Execution time: ", end_time - start_time, " Seconds")

