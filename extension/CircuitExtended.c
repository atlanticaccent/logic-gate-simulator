#include "headerExtended.h"

/**
 * Creates a circuit
 * @return a pointer to memory allocated for a circuit structure
 */
Circuit* makeCircuit() {
    Circuit* newCircuit = malloc(sizeof(Circuit));
    newCircuit -> gates = malloc(sizeof(ListHeader));                           //Malloc a list to hold all gates
    newCircuit -> gates -> length = 0;                                          //Manually initialise the length of the list to one
    addGateToCircuit(newCircuit, makeGate("zero", "IN", 0, 0));                 //Add default one and zero wires to list of gates and not inputs to prevent truth table evaluation from modifying them
    Gate* oneGate = makeGate("one", "IN", 0, 0);
    oneGate -> output = true;
    addGateToCircuit(newCircuit, oneGate);                                      //Their presence in the general list of gates does ensure that they can be referred to by a circuit definition

    newCircuit -> inputs = malloc(sizeof(ListHeader));                          //Create a list that tracks input gates
    newCircuit -> inputs -> length = 0;

    return newCircuit;
}

/**
 * Adds a gate to a circuit
 * @param circuit the Circuit to be added to
 * @param gate    the Gate to be added
 */
void addGateToCircuit(Circuit* circuit, Gate* gate) {
    addNode(circuit -> gates, gate);
}

/**
 * Adds an IN gate to the list of inputs
 * @param circuit the Circuit to add to
 * @param gate    the IN Gate to add
 */
void addINToInputs(Circuit* circuit, Gate* gate) {
    addNode(circuit -> inputs, gate);
}

/**
 * Links all gates once all gates have been found in the given input
 * @param circuit the Circuit to iterate through
 */
int linkGates(Circuit* circuit) {
    bool ANotLinked, BNotLinked;                                                //Bools tracking if the current gate has been amended to point to its actual inputs
    for (ListNode* current = circuit -> gates -> head; current /* if current is null then end */; current = current -> next) {
        ANotLinked = current -> data -> type > 0;                               //Bools denote whether function should try finding the gate that provides inputA - only gate with no inputs is IN gate
        BNotLinked = current -> data -> type > 1;                               //Gate with only one input is NOT gate

        for (ListNode* inner = circuit -> gates -> head; (ANotLinked || BNotLinked) && inner; inner = inner -> next) {          //Continue until list is exhausted or until both inputs have been amended (or there was no need to amend them in the first place)
            if (ANotLinked && strcmp((char*)current -> data -> inputGateA, inner -> data -> name) == 0) {                       //Check if this input needs to be found, then search all gates for the string/name copied down earlier
                free(current -> data -> inputGateA);                            //If the name was found, then free the string that was copied into this pointer
                current -> data -> inputGateA = inner -> data;                  //Write the pointer of the gate currently iterated to for later dereferencing
                ANotLinked = false;                                             //Indicate that the A input to the gate has been found and linked
            } else if (BNotLinked && strcmp((char*)current -> data -> inputGateB, inner -> data -> name) == 0) {
                free(current -> data -> inputGateB);
                current -> data -> inputGateB = inner -> data;
                BNotLinked = false;
            }
        }
        if (ANotLinked || BNotLinked) {
            printf("A gate specified an input that was not specified, or the gate linker has failed\n");
            return 1;
        }
    }
    return 0;
}

/**
 * Reset the ouputs of all wires in a circuit
 * @param circuit the Circuit whose wires are to be reset
 */
void resetCircuit(Circuit* circuit) {
    for (ListNode* current = circuit -> gates -> head; current; current = current -> next) {        //Iterate through the listof wires
        if (current -> data -> type) {                                          //So long as the gate is not of type IN
            current -> data -> output = 0;                                      //Set output to 0
            current -> data -> inputA = 0;                                      //Set inputs to 0
            current -> data -> inputB = 0;
        }
    }
}

/**
 * Evaluates whether a circuit stabilises or not
 * @param  circuit the Circuit to check for stabilisation
 * @return         true if the circuit stabilises, false if it doesn't
 */
bool evaluateCircuit(Circuit* circuit) {
    int previousState[circuit -> gates -> length];                              //Array representing the previous state of the circuit
    // memset(previousState, -1, sizeof(previousState));
    int currentState[circuit -> gates -> length];                               //Array representing the new state of the circuit after evaluation
    memset(currentState, -1, sizeof(currentState));                             //Initialise currentState to impossible values, guaranteeing first eval fails, as this gets copied into previousState -- first eval compares against a state that is undefined, as on first run there is not previous state
    ListNode* currentNode;
    int index;
    for (int i = 0; i < (int)pow(2, circuit -> gates -> length); i++) {         //Exhaustive check means running the stabilisation check 2^gates times
        //Iterate through the list of gates and through the array of wire outputs
        for (currentNode = circuit -> gates -> head, index = 0; currentNode; currentNode = currentNode -> next, index++) {
            if (currentNode -> data -> logicFunction) currentNode -> data -> output = currentNode -> data -> logicFunction(currentNode -> data -> inputA, currentNode -> data -> inputB);           //Retrieve the
            previousState[index] = currentState[index];                         //Copy current into previous state for comparison later, to check for stabilisation
            currentState[index] = currentNode -> data -> output;                //Set current state to the actual current output
        }

        //Iterate through all gates and set their inputs to the new outputs
        for (ListNode* currentNode = circuit -> gates -> head; currentNode; currentNode = currentNode -> next) {
            if (currentNode -> data -> inputGateA) currentNode -> data -> inputA = ((Gate*)(currentNode -> data -> inputGateA)) -> output;
            if (currentNode -> data -> inputGateB) currentNode -> data -> inputB = ((Gate*)(currentNode -> data -> inputGateB)) -> output;
        }

        if (!memcmp(previousState, currentState, sizeof(currentState))) {       //Check if the circuit has stabilised yet, by comparing the two wire state arrays for parity
            return true;
        }
    }
    return false;                                                               //If the main loop above completes without returning true at some point, then the circuit will never stabilise so return false
}

/**
 * Modifies the inputs of a circuit to a new set of values
 * @param circuit     the Circuit to whose input gates are to be changed
 * @param totalInputs the total number of input gates there are
 * @param newInputs   the new input values
 */
void modifyInputs(Circuit* circuit, int totalInputs, bool* newInputs) {
    int index = totalInputs - 1;                                                //Reversing through the array so total - 1 is the valid top index of the array
    //Iterate through the list of input gates and set their ouputs to the value found in the given array
    for (ListNode* current = circuit -> inputs -> head; current; current = current -> next, index--) {      //Reversing through the array due to using bit-shifting to generate new inputs
        current -> data -> output = newInputs[index];
    }
}

/**
 * Frees the memory allocated to a circuit structure
 * @param circuit the Circuit being freed
 */
void destroyCircuit(Circuit* circuit) {
    destroyList(circuit -> gates, true);                                        //Free the gate list
    destroyList(circuit -> inputs, false);                                      //Input list is a subset of gate list, so can't free gates in this list as they already have been in the line before
    free(circuit);                                                              //Fre overall structure
}
