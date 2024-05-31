/*
* FILENAME: tutorial.c
* Overview: BDD tutorial
* AUTHOR: David Kebo Houngninou
*/

#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "cudd.h"

/**Speicherzugriffsfehler (Speicherabzug geschrieben)
 * pr = 0 : prints nothing
 * pr = 1 : prints counts of nodes and minterms
 * pr = 2 : prints counts + disjoint sum of product
 * pr = 3 : prints counts + list of nodes
 * pr > 3 : prints counts + disjoint sum of product + list of nodes
 * @param the dd node
 */
void print_dd (DdManager *gbm, DdNode	*dd, int n, int pr )
{
    printf("DdManager nodes: %ld | ", Cudd_ReadNodeCount(gbm)); /*Reports the number of live nodes in BDDs and ADDs*/
    printf("DdManager vars: %d | ", Cudd_ReadSize(gbm) ); /*Returns the number of BDD variables in existance*/
    printf("DdManager reorderings: %d | ", Cudd_ReadReorderings(gbm) ); /*Returns the number of times reordering has occurred*/
    printf("DdManager memory: %ld \n", Cudd_ReadMemoryInUse(gbm) ); /*Returns the memory in use by the manager measured in bytes*/
    Cudd_PrintDebug(gbm, dd, n, pr);	// Prints to the standard output a DD and its statistics: number of nodes, number of leaves, number of minterms.
}


/**
 * Writes a dot file representing the argument DDs
 * @param the node object
 */
void write_dd (DdManager *gbm, DdNode	*dd, char* filename)
{
    FILE *outfile; // output file pointer for .dot file
    outfile = fopen(filename,"w");
    DdNode **ddnodearray = (DdNode **)malloc(sizeof(DdNode*)); // initialize the function array
    ddnodearray[0] = dd;
    Cudd_DumpDot(gbm, 1, ddnodearray, NULL, NULL, outfile);	// dump the function to .dot file
    free(ddnodearray);
    fclose (outfile); // close the file */
}

int main(int argc, char *argv[]) {
    char filename[30];
    DdManager *gbm; /* Global BDD manager. */
    gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0); /* Initialize a new BDD manager. */
    DdNode *x1, *x2, *x3, *y;

    x1 = Cudd_bddNewVar(gbm); /* Create a new BDD variable x1 */
    x2 = Cudd_bddNewVar(gbm); /* Create a new BDD variable x2 */
    x3 = Cudd_bddNewVar(gbm); /* Create a new BDD variable x3 */

    // Create negation of x1 and x2
    DdNode *not_x1 = Cudd_Not(x1);
    DdNode *not_x2 = Cudd_Not(x2);

    // Create the first term (¬x1 ∨ ¬x2)
    DdNode *term1 = Cudd_bddOr(gbm, not_x1, not_x2);

    // Create the second term (x1 ∨ x3)
    DdNode *term2 = Cudd_bddOr(gbm, x1, x3);

    // Create the final BDD (y = term1 ∧ term2)
    y = Cudd_bddAnd(gbm, term1, term2);

    Cudd_Ref(y); /* Update the reference count for the node just created. */
    y = Cudd_BddToAdd(gbm, y); /* Convert BDD to ADD for display purpose */
    print_dd(gbm, y, 2, 4);   /* Print the dd to standard output */
    sprintf(filename, "./bdd/y2_function.dot"); /* Write .dot filename to a string */
    write_dd(gbm, y, filename);  /* Write the resulting cascade dd to a file */
    Cudd_Quit(gbm);
    return 0;
}
