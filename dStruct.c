// dStruct.c

#include "dStruct.h"
#include <stdio.h>
#include <stdlib.h>

void dStruct_init(dStruct* ds) {
    ds->head = NULL;
    ds->size = 0;
}

void dStruct_destroy(dStruct* ds) {
    while (ds->head != NULL) {
        dStructEntry* temp = ds->head;
        ds->head = ds->head->next;
        free(temp);
    }
    ds->size = 0;
}

bool dStruct_pushEntry(dStruct* ds, int pid, int status, int niceness, float cputime, float procTime) {
    dStructEntry* newEntry = (dStructEntry*)malloc(sizeof(dStructEntry));
    if (newEntry == NULL) {
        printf("Memory allocation failed.\n");
        return false; // Failure
    }

    newEntry->pid = pid;
    newEntry->status = status;
    newEntry->niceness = niceness;
    newEntry->cputime = cputime;
    newEntry->procTime = procTime;

    // Insert at the beginning of the linked list
    newEntry->next = ds->head;
    ds->head = newEntry;
    ds->size++;

    return true; // Success
}

void dStruct_popEntry(dStruct* ds) {
    if (ds->head != NULL) {
        dStructEntry* temp = ds->head;
        ds->head = ds->head->next;
        free(temp);
        ds->size--;
    }
    printf("\nAn entry has been popped\n");
}

dStructEntry* dStruct_getEntryByPid(const dStruct* ds, int pid) {
    dStructEntry* current = ds->head;
    while (current != NULL) {
        if (current->pid == pid) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

dStructEntry* dStruct_getEntryByIndex(const dStruct* ds, unsigned int index) {
    dStructEntry* current = ds->head;
    unsigned int i = 0;
    while (current != NULL && i < index) {
        current = current->next;
        i++;
    }
    return current;
}

float dStruct_getCputimeByPid(const dStruct* ds, int pid) {
    dStructEntry* entry = dStruct_getEntryByPid(ds, pid);
    if (entry != NULL) {
        return entry->cputime;
    }
    return -1.0; // Indicate an error
}

void dStruct_setCputimeByPid(dStruct* ds, int pid, float cputime) {
    dStructEntry* entry = dStruct_getEntryByPid(ds, pid);
    if (entry != NULL) {
        entry->cputime = cputime;
    }
    printf("\nCputime has been changed\n");
}

void dStruct_printAllEntries(const dStruct* ds) {
    printf("\nAll Entries:\n");
    dStructEntry* current = ds->head;
    while (current != NULL) {
        printf("PID: %d, Status: %d, Niceness: %d, Cputime: %.2f, ProcTime: %.2f\n",
               current->pid, current->status, current->niceness,
               current->cputime, current->procTime);
               current = current->next;
    }
}

void dStruct_printEntriesByNicenessAndStatus(const dStruct* ds, int niceness, int status){
    printf("\nPrinting entries by Niceness: %d and Status: %d\n",niceness,status);
    dStructEntry* current = ds->head;

    while(current != NULL){

        if (current->niceness == niceness || current->status == status){
            printf("PID: %d, Status: %d, Niceness: %d, Cputime: %.2f, ProcTime: %.2f\n",
               current->pid, current->status, current->niceness,
               current->cputime, current->procTime);
        }
        current = current->next;

    }

}

void dStruct_printEntryByPid(const dStruct* ds, int pid){
    printf("\nPrinting entry by Pid:\n");

    dStructEntry* current = ds->head;
    while (current->pid != pid) {
        current = current->next;
    }
    printf("PID: %d, Status: %d, Niceness: %d, Cputime: %.2f, ProcTime: %.2f\n",
               current->pid, current->status, current->niceness,
               current->cputime, current->procTime);



}

bool dStruct_searchByPid(const dStruct* ds, int pid){

    dStructEntry* current = ds->head;
    while (current != NULL) {
        if (current->pid == pid) {
            return true;
        }
        current = current->next;
    }
    return NULL;
}
bool dStruct_searchByNicenessAndStatus(const dStruct* ds, int niceness, int status){

    dStructEntry* current = ds->head;
    while (current != NULL) {
        if (current->niceness == niceness || current->status == status) {
            return true;
        }
        current = current->next;
    }
    return NULL;
}

int dStruct_getNicenessByPid(const dStruct *ds, int pid){
    dStructEntry* entry = dStruct_getEntryByPid(ds,pid);
    if (entry != NULL) {
        return entry->niceness;
    }
}

void dStruct_setNicenessByPid(dStruct *ds, int pid, int niceness) {
    dStructEntry* entry = dStruct_getEntryByPid(ds, pid);
    if (entry != NULL) {
        entry->niceness = niceness;
    }
    printf("\nNiceness has been changed\n");
}


int dStruct_getStatusByPid(const dStruct *ds, int pid){
    dStructEntry* entry = dStruct_getEntryByPid(ds,pid);
    if (entry != NULL) {
        return entry->status;
    }
}

void dStruct_setStatusByPid(dStruct *ds, int pid, int status) {
    dStructEntry* entry = dStruct_getEntryByPid(ds, pid);
    if (entry != NULL) {
        entry->status = status;
    }
    printf("\nStatus has been changed\n");
}


