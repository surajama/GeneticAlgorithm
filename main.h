
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUMBER_ORGANISMS 100
#define NUMBER_GENES     20
#define ALLELES          4
#define MUTATION_RATE    0.001
#define TARGET_SUM 30
#define MAX_DISTANCE 43
#include <math.h>

struct posn{
int x;
int y;};



struct genetic {
    char **cur_generation;
    char **next_generation;
    char *target_org;
    int *org_fitnesses;
    int totalFitness;};
typedef struct genetic * Gen;

// function declarations
Gen CreateInitialPopulation(void);
void freeMem(Gen g);
void SetTargetOrg(Gen g);
int OneRun(Gen g);
  bool EvaluateOrganisms(Gen g);
  bool EvaluateOrganismsSum(Gen g);
  bool EvaluateOrganismsPath(Gen g);
  void ProduceNextGeneration(Gen g);
    int SelectOrganism(Gen g);
