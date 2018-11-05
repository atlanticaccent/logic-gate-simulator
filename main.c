#include "header.h"

int main() {
    Circuit* circuit = parseStream();
    linkGates(circuit);

    for (ListNode* current = circuit -> inputs -> head; current; current = current -> next) {
        printf("%s ", current -> data -> name);
    }
    printf("out\n");


    bool inputs[circuit -> inputs -> length];
    bool stabilises;
    int bit;
    for (unsigned long long i = 0; i < pow(2, circuit -> inputs -> length); i++) {    //i doubles as a truth table row counter, and as such can also double as a bit source for IN gate outputs later - to be an appropriate bit source i must be unsigned, and making it a long long guarantess a minimum of 64 bits and so support for 64 inputs
        for (int j = 0; j < circuit -> inputs -> length; j++) {                 //Loop writes individual bits of an unsigned integer to the outputs of all IN gates to represent changing inputs of a truth table
            bit = (i >> j) & 1;                                                 //Bit masking technique courtesy of user Joe at https://stackoverflow.com/a/26230537
            inputs[j] = bit;
        }
        resetCircuit(circuit);                                                  //Sets the values of all gate outputs to zero to refresh the circuit for the next set of inputs
        modifyInputs(circuit, circuit -> inputs -> length, inputs);             //Modifies the inputs in the circuit to new values
        stabilises = evaluateCircuit(circuit);                                  //Check if the circuit stabilises with the current set of inputs
        //Iterate through the input gates and print their current output values
        for (ListNode* current = circuit -> inputs -> head; current; current = current -> next) {
            printf("%d ", current -> data -> output);
        }
        if (stabilises) {
            printf("%d\n", circuit -> out ? circuit -> out -> output : 0);      //Print the ouput of the circuit if it stabilises, or zero if it does stabilise but there is no labelled output wire
        } else {
            printf("?\n");                                                      //Print "?" if the circuit does not stabilise, regardless of presence of output wire
        }
    }
    destroyCircuit(circuit);                                                    //Frees all memory allocated to structs over the course of operation
}
