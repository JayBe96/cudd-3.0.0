#include <stdio.h> 
#include "util.h"
#include "cudd.h"

int main (int argc, char *argv[])
{
    FILE *fp = fopen("cudd_info_out.txt", "w");
    DdManager *gbm; /* Global BDD manager. */
    char filename[30];
    gbm = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0); /* Initialize a new BDD manager. */
    DdNode *bdd = Cudd_bddNewVar(gbm); /*Create a new BDD variable*/
    Cudd_Ref(bdd); /*Increases the reference count of a node*/
    printf("DdManager nodes: %ld | ", Cudd_ReadNodeCount(gbm)); /*Reports the number of live nodes in BDDs and ADDs*/
    printf("DdManager vars: %d | ", Cudd_ReadSize(gbm) ); /*Returns the number of BDD variables in existance*/
    printf("DdManager reorderings: %d | ", Cudd_ReadReorderings(gbm) ); /*Returns the number of times reordering has occurred*/
    printf("DdManager memory: %ld |\n\n", Cudd_ReadMemoryInUse(gbm) ); /*Returns the memory in use by the manager measured in bytes*/
    Cudd_PrintDebug(gbm, bdd, 2, 4);	// Prints to the standard output a DD and its statistics: number of nodes, number of leaves, number of minterms.
    Cudd_PrintInfo(gbm, fp);
    Cudd_Quit(gbm);
    fclose(fp);
    return 0;
}
