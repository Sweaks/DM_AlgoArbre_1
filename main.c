#include <stdio.h>
#include <stdlib.h>

typedef struct _noeud {
    int valeur;
    struct _noeud *fg, *fd;
} Noeud, * Arbre;

typedef struct cell {
    Noeud * noeud;
    struct cell * suivant;
    struct cell * precedent;
} Cellule, * Liste;

typedef struct file {
    Liste debut;
    Liste fin;
    int taille;
} Queue, * File;

Cellule * alloue_cellule(Noeud * n){
    Cellule * cell =(Cellule*)malloc(sizeof(Cellule));
    cell->noeud = n;
    cell->precedent = NULL;
    cell->suivant = NULL;
    return cell; 
}

void insere_en_tete(Liste * lst, Cellule * cell){
    if(!(*lst)) *lst = cell;
    cell->suivant = *lst;
    cell->precedent = NULL;
    (*lst)->precedent = cell;
    *lst = cell;
}

Cellule * extrait_tete(Liste *lst){
    if(!(*lst)) return NULL;
    Cellule * cellule_extraite = *lst;
    *lst = (*lst)->suivant;
    (*lst)->precedent = NULL;
    return cellule_extraite;
}

void affiche_liste(Liste lst){
    while(lst){
        printf("%d ", lst->noeud->valeur);
        lst = lst->suivant;
    }
    printf("\n");
}

File initialisation(void){
    Queue *queue =(Queue*)malloc(sizeof(Queue));
    queue->taille = 0;
    queue->debut = NULL;
    queue->fin = NULL;
    return queue;
}

int est_vide(File f){
    if(f->taille == 0) return 1; //Si la file est vide
    return 0;
}

int enfiler(File f, Noeud * n){
    Cellule *new_cell = alloue_cellule(&(*n));
    if(!new_cell) return 0; // Cas si le malloc n'a pas marché
    
    if(est_vide(f) > 0){ // Si la file est vide
        f->debut = new_cell;
        f->fin = new_cell;
    }else{
        insere_en_tete(&(f)->debut, new_cell);
        f->taille++;
    }
    return 1;
}

// int defiler(File f, Noeud ** sortant){


// }

Noeud * alloue_noeud(int val, Arbre fg, Arbre fd){
    Noeud *noeud =(Noeud*)malloc(sizeof(Noeud));
    if(!noeud) return NULL;
    noeud->valeur = val;
    noeud->fg = fg;
    noeud->fd = fd;
    return &noeud;
}

