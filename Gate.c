#include "header.h"

/**
 * makeGate takes a number of parameters and attempts to create a gate from them
 * @param  gateIdentifier the given name of the gate being created
 * @param  gateType       the type (IN, NOT, AND, etc) of the gate being created
 * @param  inputA         on creation, this is a string holding the given name of another gate whose output this gate operates on
 * @param  inputB         as inputA
 * @return                returns a gate structure representing a gate created to the parameters given, or 0 if a gate could not be created
 */
Gate* makeGate(char* gateIdentifier, char* gateType, char* inputA, char* inputB) {
    Gate* newGate = malloc(sizeof(Gate));

    //Following if else block checks if the given gate type is recognised, then sets if the new gate's type and logicFunction pointer accordingly
    if(!strcmp("IN", gateType)) {
        newGate -> type = IN;
    } else if (!strcmp("NOT", gateType)) {
        newGate -> type = NOT;
        newGate -> logicFunction = &notLogic;
    } else if (!strcmp("AND", gateType)) {
        newGate -> type = AND;
        newGate -> logicFunction = &andLogic;
    } else if (!strcmp("NAND", gateType)) {
        newGate -> type = NAND;
        newGate -> logicFunction = &nandLogic;
    } else if (!strcmp("OR", gateType)) {
        newGate -> type = OR;
        newGate -> logicFunction = &orLogic;
    } else if (!strcmp("NOR", gateType)) {
        newGate -> type = NOR;
        newGate -> logicFunction = &norLogic;
    } else if (!strcmp("XOR", gateType)) {
        newGate -> type = XOR;
        newGate -> logicFunction = &xorLogic;
    } else if (!strcmp("EQ", gateType)) {
        newGate -> type = EQ;
        newGate -> logicFunction = &eqLogic;
    } else {
        free(newGate);                                                          //If the above block fails to match, free the earlier malloced memory
        return 0;                                                               //If the given gateType was not recognised then return 0, to be caught by the calling function
    }

    newGate -> name = malloc(strlen(gateIdentifier) + 1);                       //Malloc the space for the string that will hold this gate's given name
    strcpy(newGate -> name, gateIdentifier);
    if (newGate -> type) {                                                      //If this gate has a gateType that is not IN (or 0/false) then copy the name of the gate the current gate uses for input
        newGate -> inputGateA = malloc(strlen(inputA) + 1);                     //This string will be used to search for the gate designated as this gate's input, then changed so that it equals the address of that input-gate struct
        strcpy((char*)newGate -> inputGateA, inputA);
    }
    if (newGate -> type > 1) {
        newGate -> inputGateB = malloc(strlen(inputB) + 1);
        strcpy((char*)newGate -> inputGateB, inputB);
    }
    newGate -> inputA = false;
    newGate -> inputB = false;
    newGate -> output = false;

    return newGate;
}

/**
 * The following (someGate)Logic functions hold the logic of each gate.
 * When a gate is created, a pointer to one of these functions is inserted into the gate so that a gate's type does not have to be determined again
*/

bool notLogic(bool input, bool unused) {                                        //Second parameter is present but unused as circuit evaluation makes the assumption that all gates have two inputs for generlisation purposes
    return !input;
}

bool andLogic(bool inputA, bool inputB) {
    return inputA && inputB;
}

bool nandLogic(bool inputA, bool inputB) {
    return !(inputA && inputB);
}

bool orLogic(bool inputA, bool inputB) {
    return inputA || inputB;
}

bool norLogic(bool inputA, bool inputB) {
    return !(inputA || inputB);
}

bool xorLogic(bool inputA, bool inputB) {
    return !(inputA == inputB) && (inputA || inputB);
}

bool eqLogic(bool inputA, bool inputB) {
    return inputA == inputB;
}

/**
 * Frees the memory allocated to a gate
 * @param gate the Gate to free
 */
void destroyGate(Gate* gate) {
    free(gate -> name);                                                         //The only pointer that should point to malloced memory by the program's end is the gate's name
    free(gate);
}
