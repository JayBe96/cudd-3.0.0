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
    gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);                          /* Initialize a new BDD manager. */
    //Create Nodes    
    DdNode *x1, *x2, *x3, *y1, *y2, *result;
    x1 = Cudd_bddNewVar(gbm); 
    x2 = Cudd_bddNewVar(gbm); 
    x3 = Cudd_bddNewVar(gbm);
    // Bdd 1 y = (x1 ∧ ¬x2) ∨ (x1 ∧ x3)
    DdNode *term1 = Cudd_bddAnd(gbm, x1, Cudd_Not(x2));
    DdNode *term2 = Cudd_bddAnd(gbm, Cudd_Not(x1), x3);
    y1 = Cudd_bddOr(gbm, term1, term2);
    Cudd_Ref(y1);
    // Bdd 2 y = (x1 ∨ x3) ∧ (¬x2 ∨ x3)
    DdNode *term3 = Cudd_bddOr(gbm, x1, x3);
    DdNode *term4 = Cudd_bddOr(gbm, Cudd_Not(x2), x3);
    y2 =  Cudd_bddAnd(gbm, term3, term4);
    Cudd_Ref(y2);
    // Prints to console wether the 2 bdds are equal.
    if (Cudd_bddLeq(gbm, y1, y2) == 1){
        printf("The 2 BDDs are equal.\n");
    } else {
        printf("The 2 BDDs are NOT equal.\n");
    }
    result = Cudd_bddXnor(gbm, y1, y2);
    Cudd_Ref(result); /* Update the reference count for the node just created. */
    result = Cudd_BddToAdd(gbm, result); /* Convert BDD to ADD for display purpose */
    print_dd(gbm, result, 2, 4);   /* Print the dd to standard output */
    sprintf(filename, "./bdd/equvivalent.dot"); /* Write .dot. */
    write_dd(gbm, result, filename);  /* Write the resulting cascade dd to a file */
    Cudd_Quit(gbm);
    return 0;
}
