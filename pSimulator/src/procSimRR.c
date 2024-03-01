#include "dStruct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> // directory control
#include <time.h> // get current date
#include <stdbool.h> // true,false
#include <unistd.h> // sleep

#define TIME_DT 0.1 // time increment
#define TIME_JIFFY 0.1 // time quantum
// Change their values to 1 and the others to 0 to run that specific algorithm
#define ALGOR_FIFO 0
#define ALGOR_SJF 0
#define ALGOR_RR 1

dStruct readyQueue;
dStruct runningQueue;

// Function prototype for initializing queues
void initializeQueues();
void addProcessToReadyQueue(int proctime, int niceness);
bool isFolderEmpty(const char *folderPath);
void processNewProcesses(dStruct* readyQueue);
void processAllProcesses(dStruct* readyQueue);
void createLogFile();
void transferProcessToRunning();
void incrementCpuTimeInRunningQueue();
void checkAndRemoveProcessFromRunningQueue();
void checkAndCompleteSimulation();
void logQueueStatus();
void sortReadyQueueByShortestProcTime();
void incrementCpuTimeInRunningQueueTimeQuantum();

// Global Variables
int nextPid = 1; // Counter for unique PID
float t = 0; // Global Time Variable
bool processesRemaining = true; // Flag to end loop

int main() {
    // Initialize the ready and running queues and log file
    initializeQueues();
    createLogFile();
    printf("Queues initialized. Logfile Created. Simulation ready to start.\n");

    if(ALGOR_FIFO == 1){
        while(processesRemaining == true){
            // Process files in from newProc and push into readyQueue.
            processNewProcesses(&readyQueue);

            // Transfer process from readyQueue to runningQueue is running is empty.
            transferProcessToRunning();

            // Log both the queues to the logfile
            logQueueStatus(); // Global time variable will log a completed process the way its currently set up

            // Check if process in running is complete (cputime>=proctime) then remove it.
            checkAndRemoveProcessFromRunningQueue();

            // Increment cputime at end of loop and global time
            incrementCpuTimeInRunningQueue();
            t += TIME_DT;
            printf("End of time leap\n");

            // Check if newProc is empty, running and ready queue is empty then end loop if it is.
            checkAndCompleteSimulation();
            sleep(1);
        }
    }

    else if (ALGOR_SJF == 1){
        while(processesRemaining == true){
            processAllProcesses(&readyQueue);
            dStruct_sortByShortestProcTime(&readyQueue); // Short readyQueue by shortest job first
            transferProcessToRunning();
            logQueueStatus();
            checkAndRemoveProcessFromRunningQueue();
            incrementCpuTimeInRunningQueue();
            t += TIME_DT;
            printf("End of time leap\n");
            checkAndCompleteSimulation();
            sleep(1);
        }
    }

    else if (ALGOR_RR == 1){
        while(processesRemaining == true){
            processAllProcesses(&readyQueue);
            roundRobinExecution(); // Transfers processes to respective queue and runs for time quantum and increments cputime
            t += TIME_DT;
            printf("End of time leap\n");
            checkAndCompleteSimulation();

        }
    }

    return 0;
}

void initializeQueues() {
    dStruct_init(&readyQueue);
    dStruct_init(&runningQueue);
}

void processNewProcesses(dStruct* readyQueue) {
    DIR *dir;
    struct dirent *ent;
    char *folderPath = "../newProc";

    if ((dir = opendir(folderPath)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            // Construct the full path for each directory entry
            char filePath[1024];
            snprintf(filePath, sizeof(filePath), "%s/%s", folderPath, ent->d_name);

            // Skip "." and ".." entries
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;
            }

            // Attempt to open the file
            FILE *file = fopen(filePath, "r");
            if (file) {
                int niceness;
                float proctime;
                // If the file is successfully opened, read proctime and niceness
                if (fscanf(file, "%f, %d", &proctime, &niceness) == 2) {
                    // Add the process to the ready queue
                    dStruct_pushEntry(readyQueue, nextPid++, 1, niceness, 0, proctime); // grabs proctime, gives unique pid, sets ready=1, stores niceness, sets cputime=0
                }
                fclose(file);
                // Remove the file after processing. Turn off while testing
                remove(filePath);

                // Exit after processing one file
                break; // remove this to process more than one file
            }
        }
        closedir(dir);
    } else {
        // Could not open directory
        perror("Unable to open directory");
    }
}

// This just processes more than one file into the readyQueue
void processAllProcesses(dStruct* readyQueue) {
    DIR *dir;
    struct dirent *ent;
    char *folderPath = "../newProc";

    if ((dir = opendir(folderPath)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            // Construct the full path for each directory entry
            char filePath[1024];
            snprintf(filePath, sizeof(filePath), "%s/%s", folderPath, ent->d_name);

            // Skip "." and ".." entries
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;
            }

            // Attempt to open the file
            FILE *file = fopen(filePath, "r");
            if (file) {
                int niceness;
                float proctime;
                // If the file is successfully opened, read proctime and niceness
                if (fscanf(file, "%f, %d", &proctime, &niceness) == 2) {
                    // Add the process to the ready queue
                    dStruct_pushEntry(readyQueue, nextPid++, 1, niceness, 0, proctime); // grabs proctime, gives unique pid, sets ready=1, stores niceness, sets cputime=0
                }
                fclose(file);
                // Remove the file after processing. Turn off while testing
                remove(filePath);

            }
        }
        closedir(dir);
    } else {
        // Could not open directory
        perror("Unable to open directory");
    }
}


/* Creates a log file with the format log-mm-dd-yy*/
void createLogFile() {
    // Buffer to hold the formatted date string
    char dateStr[15];

    // Get current time
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // Format date as mm-dd-yy
    strftime(dateStr, sizeof(dateStr), "RR.txt", t);

    // Construct log file path and name
    char logFilePath[50];
    snprintf(logFilePath, sizeof(logFilePath), "../log/log-%s", dateStr);

    // Open the log file for writing
    FILE *logFile = fopen(logFilePath, "w");
    if (logFile == NULL) {
        perror("Failed to create log file");
        return;
    }

    // Close the file
    fclose(logFile);
}

// Function to transfer a process from readyQueue to runningQueue if runningQueue is empty
void transferProcessToRunning() {
    if (runningQueue.size == 0 && readyQueue.size > 0) {
        // Capture the process details from the head of the readyQueue before popping
        dStructEntry* headProcess = readyQueue.head;
        if (headProcess != NULL) { // Ensure there is a process to transfer
            // Use the process details from the head of the readyQueue
            int pid = headProcess->pid;
            int status = headProcess->status;
            int niceness = headProcess->niceness;
            float cputime = headProcess->cputime;
            float procTime = headProcess->procTime;

            // Pop the process from the readyQueue
            dStruct_popEntry(&readyQueue);

            // Now push the captured process details onto the runningQueue. Increments cputime
            bool success = dStruct_pushEntry(&runningQueue, pid, status, niceness, cputime, procTime);
            //t += TIME_DT; // increment global time
            if (success) {
                printf("Process %d transferred from readyQueue to runningQueue.\n", pid);
            } else {
                printf("Failed to transfer process to runningQueue.\n");
            }
        }
    } else if (runningQueue.size > 0) {
        //printf("RunningQueue is not empty, no need to transfer.\n");
    } else {
        printf("ReadyQueue is empty, no process to transfer.\n");
    }
}

void incrementCpuTimeInRunningQueue() {
    dStructEntry* current = runningQueue.head;
    while (current != NULL) {
        current->cputime += 1.0; // Increment by 1.0 or another value as needed
        // Print the current process ID and its updated cputime
        //printf("Process ID %d, new cputime: %.2f\n", current->pid, current->cputime);
        current = current->next;
    }
}

void incrementCpuTimeInRunningQueueTimeQuantum() {
    dStructEntry* current = runningQueue.head;
    while (current != NULL) {
        current->cputime += TIME_JIFFY;
        current = current->next;
    }
}

// Function to check and remove the process from runningQueue if cputime >= procTime
void checkAndRemoveProcessFromRunningQueue() {
    if (runningQueue.head != NULL && runningQueue.head->cputime >= runningQueue.head->procTime) {
        // Print the current cputime for verification before popping
        printf("Process %d completed with cputime: %f\n", runningQueue.head->pid, runningQueue.head->cputime);

        // Can just pop entry because there should only be 1 entry in runningQueue
        dStruct_popEntry(&runningQueue);
    }
}

// Function to check if a folder is empty
bool isFolderEmpty(const char *folderPath) {
    int count = 0;
    struct dirent *entry;
    DIR *dir = opendir(folderPath);

    if (dir == NULL) {
        // Unable to open the directory
        perror("Unable to open directory");
        return true; // Assume empty if unable to open
    }

    while ((entry = readdir(dir)) != NULL) {
        // Skip the . and .. entries
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            count++;
            break; // Found an entry which is not . or .., no need to continue
        }
    }

    closedir(dir);
    return count == 0; // If count is 0, directory is empty
}

// Function to check if the simulation conditions are met for completion
void checkAndCompleteSimulation() {
    // Check if ../newProc is empty
    bool isFolderEmptyFlag = isFolderEmpty("../newProc");

    // Check if both readyQueue and runningQueue sizes are 0
    bool areQueuesEmpty = (readyQueue.size == 0) && (runningQueue.size == 0);

    // If both conditions are met, mark the simulation as complete
    if (isFolderEmptyFlag && areQueuesEmpty) {
        processesRemaining = false;
    }
}

// Function to log the status of processes in both queues. Also updates logfile status to complete if cputime >= proctime.
void logQueueStatus() {
    char filename[40]; // Adjusted size for the full path
    FILE *logfile;
    struct tm *tm_info;
    time_t now = time(NULL);

    tm_info = localtime(&now);
    // Updated line to format the filename with the correct directory and extension
    strftime(filename, sizeof(filename), "../log/log-%m-%d-%y.txt", tm_info);

    // Open the log file in append mode
    logfile = fopen(filename, "a");
    if (logfile == NULL) {
        perror("Error opening log file");
        return;
    }

    // Log the status for the process in the readyQueue, if any.
    for (dStructEntry* curr = readyQueue.head; curr != NULL; curr = curr->next) {
        fprintf(logfile, "%.2f, %d, 1, %d, %.2f, %.2f, 0\n",
            t, curr->pid, curr->niceness,
            curr->cputime, curr->procTime);
    }


    // Log the status for the process in the runningQueue, if any. Format: global time variable, PID, niceness, cputime, procTime, whichQueue(0 = ready, 1 = running)
    if (runningQueue.size > 0) {
        int status = (runningQueue.head->cputime >= runningQueue.head->procTime) ? 3 : 2; // This just makes status=3(complete) if cputime >= procTime
        fprintf(logfile, "%.2f, %d, %d, %d, %.2f, %.2f, 1\n",
                t, runningQueue.head->pid, status, runningQueue.head->niceness,
                runningQueue.head->cputime, runningQueue.head->procTime);
    }

    fclose(logfile); // Close the file after logging
}
void roundRobinExecution() {
    if (runningQueue.size == 0 && readyQueue.size > 0) {
        transferProcessToRunning();
    }

    dStructEntry* runningProcess = runningQueue.head;
    if (runningProcess != NULL) {
        // Increment cputime
        float potentialNewCputime = runningProcess->cputime + TIME_JIFFY; // Increment cputime

        if (potentialNewCputime >= runningProcess->procTime) {
            runningProcess->cputime = runningProcess->procTime; // Ensure cputime does not exceed procTime
            runningProcess->status = 3; // Mark as completed
        } else {
            runningProcess->cputime = potentialNewCputime; // Increment cputime normally
        }

        logQueueStatus(); // Log the queue status

        // Round Robin
        if (runningProcess->status == 3) {
            dStruct_popEntry(&runningQueue); // Remove the completed process
        } else {
            printf("Moving process %d from runningQueue back to readyQueue\n", runningProcess->pid);
            dStruct_pushEntry(&readyQueue, runningProcess->pid, runningProcess->status, runningProcess->niceness, runningProcess->cputime, runningProcess->procTime);
            dStruct_popEntry(&runningQueue); // Move the process back to the readyQueue for round-robin execution
        }
    }

    if (readyQueue.size > 0) {
        transferProcessToRunning();
    }
}
