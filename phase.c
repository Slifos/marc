
#include <stdio.h>
#include "loc.h"
#include "moves.h"
#include "phase.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include "map.h"

/*int* filtrerListe(int *source, int taille, int *positionsASupprimer, int tailleSuppr, int*destination) {
    int k = 0; // Index pour la nouvelle liste
    for (int i = 0; i < taille; i++) {
        // Vérifier si `i` est dans la liste des positions à supprimer
        int supprimer = 0;
        for (int j = 0; j < tailleSuppr; j++) {
            if (i == positionsASupprimer[j]) {
                supprimer = 1;
                break;
            }
        }
        if (!supprimer) {
            destination[k++] = source[i];
        }
    }
    return destination;
}*/

int* getMoves(int *moves,int index,int num_children) {
    //renvoie la liste des mouvements possibles aux nouveaux noeuds

    int* new_moves = malloc(num_children*sizeof(int*));
    for(int i=0;i<index;i++) {
        new_moves[i]=moves[i];
        printf("\n%d",new_moves[i]);
    }
    for(int i=index;i<num_children-1;i++){new_moves[i]=moves[i+1];printf("\n%d",new_moves[i]);}
    printf("\n");
    return new_moves;

}

void print_tree(Node* node, int level){
    // Affiche l'indentation et la valeur du nœud
    for (int i = 0; i < level; i++) {
        printf("  ");
    }


    // Afficher les enfants
    for (int i = 0; i < node->num_children; i++) {
        print_tree(node->children[i], level + 1);
    }
}
int num_move (int i, t_chance * chance){
    if (i <= chance->F_10 && chance->F_10!=0){
        chance->F_10--;
        return 1;
    }
    else if(i<=chance->F_20+chance->F_10 && chance->F_20!=0){
        chance->F_20--;
        return 2;
    }
    else if(i<=chance->F_20+chance->F_10+chance->F_30 && chance->F_30!=0){
        chance->F_30--;
        return 3;
    }
    else if(i<=chance->F_20+chance->F_10+chance->F_30+chance->B_10 && chance->B_10!=0){
        chance->B_10--;
        return 4;
    }
    else if(i<=chance->F_20+chance->F_10+chance->F_30+chance->B_10+chance->T_LEFT && chance->T_LEFT!=0){
        chance->T_LEFT--;
        return 5;
    }
    else if(i<=chance->F_20+chance->F_10+chance->F_30+chance->B_10+chance->T_LEFT+chance->T_RIGHT && chance->T_RIGHT!=0){
        chance->T_RIGHT--;
        return 6;
    }
    else if(i<=chance->F_20+chance->F_10+chance->F_30+chance->B_10+chance->T_LEFT+chance->T_RIGHT+chance->U_TURN && chance->U_TURN!=0){
        chance->U_TURN--;
        return 7;
    }
    else{
        return 8;
    }
}

t_move move_num(int i){
    switch (i) {
        case 1 : return F_10;
        case 2 : return F_20;
        case 3 : return F_30;
        case 4 : return B_10;
        case 5 : return T_LEFT;
        case 6 : return T_RIGHT;
        case 7 : return U_TURN;
    }
}
// Fonction pour créer un nœud
Node* create_node(int value, int num_children) {

    Node* node = (Node*)malloc(sizeof(Node));

    if (node == NULL) {
        return NULL;

    }
    node->value = value;
    node->num_children = num_children;

    // Allouer de la mémoire pour les enfants si le nœud a des enfants
    if (num_children > 4) {

        node->children = malloc(num_children*sizeof(Node*));

        if (node->children == NULL) {
            return NULL;

        }
    } else {
        node->children = NULL;
        // Pas d'enfants pour ce nœud
    }

    return node;
}

// Fonction récursive pour construire l'arbre
void build_tree(Node* node, t_map map, t_localisation loc, int*moves,int reg) {
    int new_reg=reg;

    if (node->num_children < 5+reg) {
        node->children = NULL;
        return; // Pas d'enfants à créer
    }

    for (int i = 0; i < node->num_children; i++){
        // Créer un enfant avec un niveau inférieur
        int* new_moves = getMoves(moves,i,node->num_children);
        t_localisation new_loc = move(loc, move_num(new_moves[i]));

        int collision = verif_collision(new_loc);
        //printf("\nbefore collision");
        if (collision){


            if (map.costs[loc.pos.x][loc.pos.y]==4){
                new_reg=1;
            }
            //printf("\ncollision1");
            node->children[i] = create_node(node->value = map.costs[loc.pos.x][loc.pos.y], node->num_children - 1);
            build_tree(node->children[i],map, loc, new_moves,new_reg);
        }
        else {
            //printf("\nbefore collision2");
            if (map.costs[new_loc.pos.x][new_loc.pos.y]==4) {
                new_reg=1;
            }

            node->children[i] = create_node(node->value = map.costs[new_loc.pos.x][new_loc.pos.y], node->num_children - 1);
            //printf("\ncollision2");
            build_tree(node->children[i],map, new_loc, new_moves,new_reg);

        }
        /*printf("\ncollision");
        printf("map cost-> %d",map.costs[new_loc.pos.x][new_loc.pos.y]);

        printf("\nreg");
        node->value = map.costs[new_loc.pos.x][new_loc.pos.y];
        printf("\nbefore node");

        //printf("\nnum child %d ",node->num_children);
        build_tree(node->children[i],map, new_loc, moves,new_reg);*/// Construire l'arbre pour cet enfant
    }
}

void phase(t_localisation loc, t_chance chance, t_map map){
    t_localisation loc2 = loc;
    srand(time(NULL));
    //int chemin[5] = {-1,-1,-1,-1,-1};
    int* moves = base_moves(chance);


    Node *node = create_node(-1, 9);
    printf("\n node value : %d",node->value);
    build_tree(node,map,loc, moves,0);
    printf("\n node value : %d",node->value);

    best_way(node);
}

int* best_way(Node* node) {
    //cherche le chemin avec le coût le moins élevé possible à travers l'arbre
    //pour trouver le meilleur chemin à parcourir
    int b,c,d,e,f,min = 100000;
    int* cheminfinal = malloc(5*sizeof(int));
    for (b=0;b<9;b++){
        for (c=0;c<8;c++){
            for (d=0;d<7;d++){
                for (e=0;e<6;e++){
                    for (f=0;f<5;f++){
                        //printf("\n%d %d %d %d %d",node->children[b]->value,node->children[b]->children[c]->value,node->children[b]->children[c]->children[d]->value,node->children[b]->children[c]->children[d]->children[e]->value,node->children[b]->children[c]->children[d]->children[e]->children[f]->value);
                        if((node->children[b]->value + node->children[b]->children[c]->value + node->children[b]->children[c]->children[d]->value + node->children[b]->children[c]->children[d]->children[e]->value + node->children[b]->children[c]->children[d]->children[e]->children[f]->value)<min){
                            min = node->children[b]->value + node->children[b]->children[c]->value + node->children[b]->children[c]->children[d]->value + node->children[b]->children[c]->children[d]->children[e]->value + node->children[b]->children[c]->children[d]->children[e]->children[f]->value;
                            printf("\nmin : %d", min);
                            printf("\nchemin : %d %d %d %d %d",b,c,d,e,f);
                            cheminfinal[0] = b;
                            cheminfinal[1] = c;
                            cheminfinal[2] = d;
                            cheminfinal[3] = e;
                            cheminfinal[4] = f;

                            printf("\nChemin en prix : %d %d %d %d %d\n",node->children[b]->value,node->children[b]->children[c]->value,node->children[b]->children[c]->children[d]->value,node->children[b]->children[c]->children[d]->children[e]->value,node->children[b]->children[c]->children[d]->children[e]->children[f]->value);
                            printf("\n");
                        }
                    }
                }
            }
        }
    }


}
int* base_moves(t_chance chance) {
    //génére les enfants de la racine original avec les mouvements associés
    int* moves = (int*)malloc(9*sizeof(int));
    for (int i=0; i < 9 ;i++){
        int total = chance.U_TURN + chance.T_RIGHT + chance.T_LEFT + chance.B_10 + chance.F_10 +chance.F_20 +chance.F_30;
        int p = floor(((rand()%(101)))*((float)total/100));
        printf("%d ",p);
        moves[i] = num_move(p,&chance);
        printf("%d & %d \n",total ,moves[i]);
    }

    return moves;
}
int verif_collision(t_localisation new_loc) {
    //renvoie si le robot rentre en collision avec la limite du terrain
    //en fonction de sa nouvelle localisation
    if (new_loc.pos.x > 6 || new_loc.pos.x<0 || new_loc.pos.y > 6 || new_loc.pos.y <0) {
        return 1;
    }
    return 0;


}