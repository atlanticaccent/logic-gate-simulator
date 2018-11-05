#ifndef header
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef enum gateType { IN = 0, NOT = 1, AND = 2, NAND = 2, OR = 2, NOR = 2, XOR = 2, EQ = 2 } gateType;            //gateType enum indicates a gate and evaluates to the number of inputs that a gate has

typedef struct Gate{
    char* name;
    gateType type;
    bool (*logicFunction)(bool, bool);
    void* inputGateA;                                                           //Abuse void pointer to get discount polymorphism - store actual name of input gate (known at read time) then replace with pointer of gate (only certain at file end)
    bool inputA;
    void* inputGateB;
    bool inputB;
    bool output;
} Gate;

typedef struct ListNode{
    Gate* data;
    struct ListNode* next;
} ListNode;

typedef struct ListHeader {
    ListNode* head;
    ListNode* tail;
    int length;
} ListHeader;

typedef struct Circuit{
    ListHeader* gates;
    ListHeader* inputs;
    Gate* out;
} Circuit;

Circuit* parseStream();

Circuit* makeCircuit();

void addGateToCircuit(Circuit* circuit, Gate* gate);

void addINToInputs(Circuit* circuit, Gate* gate);

int linkGates(Circuit* circuit);

bool evaluateCircuit(Circuit* circuit);

void modifyInputs(Circuit* circuit, int totalInputs, bool* inputs);

void resetCircuit(Circuit* circuit);

void addNode(ListHeader* list, Gate* data);

Gate* makeGate(char* gateIdentifier, char* gateType, char* inputA, char* inputB);

bool notLogic(bool input, bool unused);

bool andLogic(bool inputA, bool inputB);

bool nandLogic(bool inputA, bool inputB);

bool orLogic(bool inputA, bool inputB);

bool norLogic(bool inputA, bool inputB);

bool xorLogic(bool inputA, bool inputB);

bool eqLogic(bool inputA, bool inputB);

void destroyGate(Gate* gate);

void destroyList(ListHeader* list, bool freeData);

void destroyCircuit(Circuit* circuit);

#endif
