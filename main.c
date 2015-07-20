#include "main.h"

struct posn target = {2, 25};

int main(){
    Gen new_gen = CreateInitialPopulation();
    SetTargetOrg(new_gen);
    int num_generations = OneRun(new_gen);
    printf("%d",num_generations);
    freeMem(new_gen);
   }

void draw_path(char* directions){
    int x_max, y_max = 30;
    bool visited[30][30] = {{0}};
    struct posn cur_loc = {0, 0};
    // find all visited points
    for(int i = 0; i<20; i++){
        if(i%2 == 0){
            for(int j = cur_loc.x; j<=cur_loc.x+directions[i];j++){
                visited[cur_loc.y][j] = true;
            }
            cur_loc.x += directions[i];
        }
        else{
            for(int j = cur_loc.y; j<=cur_loc.y+directions[i];j++){
                visited[j][cur_loc.x] = true;
            }
            cur_loc.y += directions[i];
        }
    }
    // print map
    printf("PATH: \n");
    for(int i = 0; i < 30; i++){
        for(int j = 0; j< 30; j++){
            if(visited[i][j] && i == target.y && j == target.x) printf("X");
            else if(visited[i][j]) printf("O");
            else printf(".");
        }
        printf("\n");
    }
}

void freeMem(Gen g){
    for(int i = 0; i<NUMBER_ORGANISMS; i++){
        free(g->cur_generation[i]);
        free(g->next_generation[i]);
    }
    free(g->cur_generation);
    free(g->next_generation);
    free(g->target_org);
    free(g->org_fitnesses);
    free(g);

}

Gen CreateInitialPopulation(){
    Gen new_gen = malloc(sizeof(struct genetic));
    new_gen->cur_generation = malloc(sizeof(char *)*NUMBER_ORGANISMS);
    new_gen->next_generation = malloc(sizeof(char *)*NUMBER_ORGANISMS);
    new_gen->target_org = malloc(sizeof(char)*NUMBER_GENES);
    new_gen->org_fitnesses = malloc(sizeof(int)*NUMBER_ORGANISMS);
    for(int i = 0; i<NUMBER_ORGANISMS; i++){
        new_gen->cur_generation[i] = malloc(sizeof(char)*NUMBER_GENES);
        new_gen->next_generation[i] = malloc(sizeof(char)*NUMBER_GENES);
        //Set initial genes
        for(int gene_num = 0; gene_num < NUMBER_GENES; ++gene_num){
            new_gen->cur_generation[i][gene_num] = rand() % ALLELES;
            //print initial population
            printf("%d",new_gen->cur_generation[i][gene_num]);
        }
        printf("\n");
    }
    return new_gen;
}

void SetTargetOrg(Gen g){
    printf("\n Target Org \n");
    for(int i = 0; i < NUMBER_GENES; ++i){
        g->target_org[i] = rand() % ALLELES;
        printf("%d",g->target_org[i]);
    }
    printf("\n");
}

int OneRun(Gen g){
    int num_generations = 1;
    bool match_found = false;
    while(1){
        match_found = EvaluateOrganismsPath(g);
        if(match_found){
            return num_generations;
        }
        ProduceNextGeneration(g);
        num_generations++;
    }
}

bool EvaluateOrganisms(Gen g){
    bool exact_match = false;
    int matches = 0;
    g->totalFitness = 0;
    int exact_match_count = 0;
    for(int org_num = 0; org_num < NUMBER_ORGANISMS; ++org_num){
        matches = 0;
        for(int gene_num = 0;gene_num<NUMBER_GENES; ++gene_num){
            if(g->cur_generation[org_num][gene_num] == g->target_org[gene_num]){
                matches++;
            }
        }
        g->org_fitnesses[org_num] = matches;
        g->totalFitness+=matches;
        if(matches == NUMBER_GENES){
            exact_match = true;
            exact_match_count++;
            printf("Matched Organism: ");
            //for(int i = 0; i < NUMBER_GENES; i++){
            //    printf("%d", g->cur_generation[org_num][i]);
            //}
            //printf("\n");
        }
    }
    // return if all organisms match
    printf("total matches: %d\n", exact_match_count);
    return (exact_match_count >= 0.8 * NUMBER_ORGANISMS);
}

void ProduceNextGeneration(Gen g){
    for(int org_num = 0; org_num < NUMBER_ORGANISMS; ++org_num){
        int parent1 = SelectOrganism(g);
        int parent2 = SelectOrganism(g);
        int crossover_point = rand() % NUMBER_GENES;
        int mutate;

        for(int gene_num = 0; gene_num < NUMBER_GENES; ++gene_num){
            mutate = rand() % 1000;
            if(mutate == 0){
                //printf("mutated\n");
                g->next_generation[org_num][gene_num] = rand() % ALLELES;
            }
            else{
                //printf("not mutated\n");
                if(gene_num < crossover_point){
                    g->next_generation[org_num][gene_num] = g->cur_generation[parent1][gene_num];
                }
                else g->next_generation[org_num][gene_num] = g->cur_generation[parent2][gene_num];
            }
        }
    }
    // copy next_gen to cur_gen
    for(int i = 0; i < NUMBER_ORGANISMS; ++i){
        for(int j = 0; j<NUMBER_GENES; ++j){
            g->cur_generation[i][j] = g->next_generation[i][j];
        }
    }
    printf("Sample Organism: ");
    for(int i = 0; i<NUMBER_GENES; i++){
       printf("%d",g->cur_generation[2][i]);
    }
    printf("\n");
}

int SelectOrganism(Gen g){
    int select_point = rand() % (g->totalFitness + 1);
    int sum_fitness = 0;
    for(int i = 0; i<NUMBER_ORGANISMS; i++){
        sum_fitness+=g->org_fitnesses[i];
        if(sum_fitness >= select_point){
            return i;
        }
    }
}

bool EvaluateOrganismsSum(Gen g){
    g->totalFitness = 0;
    int exact_match_count = 0;
    int cur_sum = 0;
    int cur_fitness = 0;
    for(int org_num = 0; org_num < NUMBER_ORGANISMS; ++org_num){
        cur_sum = 0;
        for(int gene_num = 0;gene_num<NUMBER_GENES; ++gene_num){
            cur_sum+=g->cur_generation[org_num][gene_num];
        }
        cur_fitness = TARGET_SUM - abs(TARGET_SUM - cur_sum);
        g->org_fitnesses[org_num] = cur_fitness;
        g->totalFitness+=cur_fitness;
        if(cur_fitness == TARGET_SUM){
            exact_match_count++;
            //printf("Matched Organism: ");
            //for(int i = 0; i < NUMBER_GENES; i++){
            //    printf("%d", g->cur_generation[org_num][i]);
            //}
            //printf("\n");
        }
    }
    // return if all organisms match
    printf("total matches: %d\n", exact_match_count);
    return (exact_match_count >= 0.8* NUMBER_ORGANISMS);
}

bool EvaluateOrganismsPath(Gen g){
    g->totalFitness = 0;
    int exact_match = false;
    struct posn cur_loc = {0,0};
    int cur_fitness = 0;
    int distance = 0;
    for(int org_num = 0; org_num < NUMBER_ORGANISMS; ++org_num){
        cur_loc.x = 0;
        cur_loc.y = 0;
        for(int gene_num = 0;gene_num<NUMBER_GENES; ++gene_num){
            if(gene_num%2 == 0){
                cur_loc.x += g->cur_generation[org_num][gene_num];
            }
            else cur_loc.y += g->cur_generation[org_num][gene_num];
        }
        distance = (int) sqrt((cur_loc.x - target.x)*(cur_loc.x - target.x)+(cur_loc.y - target.y)*(cur_loc.y - target.y));
        cur_fitness = MAX_DISTANCE - distance;
        g->org_fitnesses[org_num] = cur_fitness;
        g->totalFitness+=cur_fitness;
        //if(distance <= 3){
        //        printf("Close Match\n");
        //        print_map(cur_loc);
        //        printf("\n");
        //}
        if(distance == 0){
            exact_match = true;
            printf("Matched Organism: ");
            for(int i = 0; i < NUMBER_GENES; i++){
               printf("%d", g->cur_generation[org_num][i]);
            }
            printf("\n");
            draw_path(g->cur_generation[org_num]);
        }
    }
    // return if all organisms match
    return (exact_match);
}
