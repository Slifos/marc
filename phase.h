//
// Created by gaspa on 17/11/2024.
//

#ifndef UNTITLED1_PHASE_H
#define UNTITLED1_PHASE_H
#include "moves.h"
#include "map.h"

typedef struct Node {
    int value;                // Valeur du nœud (facultatif)
    int num_children;         // Nombre d'enfants pour ce nœud
    struct Node** children;
    int isReg;// Tableau de pointeurs vers les enfants
} Node;

typedef struct s_chance{
    int T_LEFT;
    int T_RIGHT;
    int U_TURN;
    int F_10;
    int F_20;
    int F_30;
    int B_10;
}t_chance;


int* getMoves(int *,int,int);
t_move move_num(int );
Node* create_node(int , int ,int);
void build_tree(Node* , t_map , t_localisation , int*,int);
void phase(t_localisation , t_chance , t_map );
int num_move (int , t_chance * );
int* base_moves(t_chance chance);
int* best_way(Node* node);
int verif_collision(t_localisation);
#endif //UNTITLED1_PHASE_H
