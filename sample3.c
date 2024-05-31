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

int main (int argc, char *argv[]) {
  char filename[30];
  DdManager *gbm; /* Global BDD manager. */
  gbm = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0); /* Initialize a new BDD manager. */

  // Define variable IDs (assuming x1, x2, x3, and x4)
  DdNode* x1 = Cudd_bddNewVar(gbm); /* Create a new BDD variable x1 */
  DdNode* x2 = Cudd_bddNewVar(gbm); /* Create a new BDD variable x2 */
  DdNode* x3 = Cudd_bddNewVar(gbm); /* Create a new BDD variable x3 */
  DdNode* x4 = Cudd_bddNewVar(gbm); /* Create a new BDD variable x4 */

  // Build individual clauses
  DdNode* clause1 = Cudd_Not(Cudd_bddAnd(gbm, Cudd_Not(x1), Cudd_bddAnd(gbm, Cudd_Not(x2), Cudd_bddAnd(gbm, Cudd_Not(x3), x4))));
  DdNode* clause2 = Cudd_bddAnd(gbm, x1, Cudd_bddAnd(gbm, x2, Cudd_bddAnd(gbm, x3, Cudd_Not(x4))));
  DdNode* clause3 = Cudd_Not(Cudd_bddAnd(gbm, Cudd_Not(x1), Cudd_bddAnd(gbm, Cudd_Not(x2), Cudd_bddAnd(gbm, x3, Cudd_Not(x4)))));
  DdNode* clause4 = Cudd_bddAnd(gbm, x1, Cudd_bddAnd(gbm, x2, Cudd_bddAnd(gbm, x3, x4)));

  // Combine clauses using OR
  DdNode* bdd = Cudd_bddOr(gbm, clause1, Cudd_bddOr(gbm, clause2, Cudd_bddOr(gbm, clause3, clause4)));

  Cudd_Ref(bdd);          /*Update the reference count for the node just created.*/
  bdd = Cudd_BddToAdd(gbm, bdd); /*Convert BDD to ADD for display purpose*/

  // Print the BDD
  print_dd(gbm, bdd, 4, 2); // Print counts + disjoint sum of product

  sprintf(filename, "./bdd/f_graph.dot");
  write_dd(gbm, bdd, filename);

  Cudd_Quit(gbm);
  return 0;
}