#include "header.h"

/**
 * parseStream takes the standard input stream and parses each line for gate definitions until it is given an end of file character
 * For the base specification parseStream is hardcoded to only use stdin
 * @return a Circuit struct representing the overall assembly of gates that the function has parsed
 */
Circuit* parseStream() {
    //FILE* inputFile = fopen(fileName, "r");                                   //Shunt this to command-line argument interpreting function for extension

    Circuit* newCircuit = makeCircuit();

    size_t charsRead = 0;
    char* inputLine = 0;                                                        //Free me!
    char* workingPointer;
    char* token;
    int index;
    char* tokenPointers[4];
    int i;
    for(i = getline(&inputLine, &charsRead, stdin); i != -1; inputLine = 0, charsRead = 0, i = getline(&inputLine, &charsRead, stdin)) {           //Change stdin to a file stream type parameter to allow switching between console or file input in extension
        workingPointer = inputLine;

        for (token = strtok(workingPointer, " \n"), index = 0; token && index < 4; token = strtok(0, " \n"), index++) {
            tokenPointers[index] = token;
        }

        Gate* gate = makeGate(tokenPointers[0], tokenPointers[1], tokenPointers[2], tokenPointers[3]);
        if (gate) {                                                             //Checks that makeGate succeeded, so further checks on the newly created gate won't segfault due to the new gate not actually existing
            addGateToCircuit(newCircuit, gate);                                 //Add the gate to the circuit
            if (!(gate -> type)) addINToInputs(newCircuit, gate);               //Add the gate to the circuit's list of inputs if it has a type of IN (which evaluates to zero, which when negated is true)
            if (strcmp(gate -> name, "out") == 0) newCircuit -> out = gate;
        }
        free(inputLine);                                                        //Frees inputLine for reuse
    }
    free(inputLine);                                                            //getLine will not free the buffer it makes even on failure, so an extra free is required
                                                                                //(as the loop only stops after getLine allocates a buffer and fails to write to it due to reaching EOF)
    return newCircuit;
}
