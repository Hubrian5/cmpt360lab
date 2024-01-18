// dStruct.h

#ifndef DSTRUCT_H
#define DSTRUCT_H

#include <stdbool.h>

// Define the structure to store information
typedef struct dStructEntry {
    int pid;
    int status;
    int niceness;
    float cputime;
    float procTime;
    struct dStructEntry* next;
} dStructEntry;

// Define the dynamic structure
typedef struct {
    dStructEntry* head;
    unsigned int size;
} dStruct;

// Function prototypes

// Initialize and destroy functions
void dStruct_init(dStruct* ds);
void dStruct_destroy(dStruct* ds);

// Entry manipulation functions
bool dStruct_pushEntry(dStruct* ds, int pid, int status, int niceness, float cputime, float procTime);
void dStruct_popEntry(dStruct* ds);

// Get entry functions
dStructEntry* dStruct_getEntryByPid(const dStruct* ds, int pid);
dStructEntry* dStruct_getEntryByIndex(const dStruct* ds, unsigned int index);

// Get and set functions
float dStruct_getCputimeByPid(const dStruct* ds, int pid);
void dStruct_setCputimeByPid(dStruct* ds, int pid, float cputime);

int dStruct_getNicenessByPid(const dStruct* ds, int pid);
void dStruct_setNicenessByPid(dStruct* ds, int pid, int niceness);

int dStruct_getStatusByPid(const dStruct* ds, int pid);
void dStruct_setStatusByPid(dStruct* ds, int pid, int status);

// Search functions
bool dStruct_searchByPid(const dStruct* ds, int pid);
bool dStruct_searchByNicenessAndStatus(const dStruct* ds, int niceness, int status);

// Print functions
void dStruct_printAllEntries(const dStruct* ds);
void dStruct_printEntryByPid(const dStruct* ds, int pid);
void dStruct_printEntriesByNicenessAndStatus(const dStruct* ds, int niceness, int status);

#endif // DSTRUCT_H
