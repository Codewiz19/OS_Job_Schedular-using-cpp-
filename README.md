CPU Scheduler Console Application
Description

This is a console-based application that simulates various CPU scheduling algorithms. The program allows users to input process data, select the scheduling algorithm, and view the scheduling results directly in the terminal. Supported algorithms include:

    First-Come-First-Serve (FCFS)
    Shortest Job First (SJF)
    Round Robin (RR)

Users can input process details such as arrival time and service time, and the system will display the execution schedule for each process based on the selected algorithm.
Features

    Console Input and Output for easy interaction.
    Support for multiple algorithms (FCFS, SJF, RR).
    Process scheduling visualization in the terminal.
    Input process details (name, arrival time, service time).
    Clear output results, showing the execution sequence and times for each process.

Technologies Used

    C++ as the core programming language.
    Standard C++ libraries for implementing scheduling algorithms and managing data.

Usage Instructions

    Select an Algorithm:
        Choose one of the available algorithms: FCFS, SJF, or RR from the terminal input.

    Input Data:
        Enter the Last Instant (the maximum simulation time).
        Enter the Number of Processes to simulate.
        For each process, input the Name, Arrival Time, and Service Time in the format Name,Arrival,Service.

    Run the Scheduler:
        After entering the data, press Enter to start the scheduling simulation.

    View Results:
        The output will display the scheduling results, showing the sequence of processes, their start time, finish time, and turn-around time.

Algorithm Details
1. First-Come-First-Serve (FCFS)

    Processes are executed in the order of their arrival times.
    No preemption occurs.
    The algorithm is simple but can result in a convoy effect (i.e., long processes may delay others).

2. Shortest Job First (SJF)

    The process with the shortest service time is executed next.
    Non-preemptive version (not SRTF).
    SJF minimizes average waiting time but may suffer from the starvation problem (longer processes may never execute).

3. Round Robin (RR)

    Each process is assigned a fixed time quantum.
    Processes are executed in a cyclic order.
    If a process doesn’t finish within the quantum, it is placed back in the queue.
    RR ensures fairness but may have higher turnaround time due to frequent context switching.

Contributing

Feel free to fork this repository, make changes, and submit pull requests for improvements. All contributions are welcome!
