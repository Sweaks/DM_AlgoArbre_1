#include <stdio.h>
#include <stdlib.h>

typedef struct _noeud {
    int valeur;
    struct _noeud *fg, *fd;
} Noeud, * Arbre;

typedef struct cell {
    Noeud * noeud;
    struct cell * suivant;
} Cellule, * Liste;

typedef struct file {
    Liste debut;
    Liste fin;
    int taille;
} Queue, * File;

Cellule * alloue_cellule(Noeud * n){
    Cellule * cell =(Cellule*)malloc(sizeof(Cellule));
    cell->noeud = n;
    cell->suivant = NULL;
    return cell; 
}

void insere_en_tete(Liste * lst, Cellule * cell){
    if(!(*lst)) *lst = cell;
    cell->suivant = *lst;
    *lst = cell;
}

Cellule * extrait_tete(Liste *lst){
    if(!(*lst)) return NULL;
    Cellule * cellule_extraite = *lst;
    *lst = (*lst)->suivant;
    return cellule_extraite;
}

void affiche_liste(Liste lst){
    // J'ai laissé cette fonction par rapport 
    // aux différents versions du prototype(et du pdf aussi)
    // Elle pourrait etre supprimé
    while(lst){
        printf("%d ", lst->noeud->valeur);
        lst = lst->suivant;
    }
    printf("\n");
}

void affiche_liste_renversee(Liste lst){
    if(lst){
        affiche_liste_renversee(lst->suivant);
        printf("%d ", lst->noeud->valeur);
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

int defiler(File f, Noeud ** sortant){
    /* Je ne suis pas vraiment sur de la fonction par contre
       A voir seulement quoi */
    
    if(est_vide(f) > 0) return 0; // Si la file est vide
    Cellule * cellule_extraite = extrait_tete(&(f)->debut);
    *sortant = cellule_extraite->noeud;
    free(cellule_extraite);
    f->taille--;
    return 1;
}

Noeud * alloue_noeud(int val, Arbre fg, Arbre fd){
    Noeud *noeud =(Noeud*)malloc(sizeof(Noeud));
    if(!noeud) return NULL;
    noeud->valeur = val;
    noeud->fg = fg;
    noeud->fd = fd;
    return noeud;
}


int construit_complet(int h, Arbre *a) {
    if (h < 0) return 0; // Hauteur invalide

    int total_noeuds = (1 << (h + 1)) - 1; // 2^(h+1) - 1
    Noeud** file = (Noeud**)malloc(total_noeuds * sizeof(Noeud*));
    if (file == NULL) return 0; // Échec d'allocation

    int val = 1, debut = 0, fin = 0;

    // Création de la racine
    *a = (Noeud*)malloc(sizeof(Noeud));
    if (*a == NULL) {
        free(file);
        return 0;
    }
    (*a)->valeur = val++;
    (*a)->fg = (*a)->fd = NULL;
    file[fin++] = *a;

    // Création des nœuds restants
    while (val <= total_noeuds) {
        Noeud* parent = file[debut++];

        // Création du fils gauche
        parent->fg = (Noeud*)malloc(sizeof(Noeud));
        if (parent->fg == NULL) {
            //liberer_arbre(a);
            free(file);
            return 0;
        }
        parent->fg->valeur = val++;
        parent->fg->fg = parent->fg->fd = NULL;
        file[fin++] = parent->fg;

        // Création du fils droit (si on n'a pas dépassé le total des nœuds)
        if (val <= total_noeuds) {
            parent->fd = (Noeud*)malloc(sizeof(Noeud));
            if (parent->fd == NULL) {
                //liberer_arbre(a);
                free(file);
                return 0;
            }
            parent->fd->valeur = val++;
            parent->fd->fg = parent->fd->fd = NULL;
            file[fin++] = parent->fd;
        }
    }

    free(file);
    return 1;
}



int insere_niveau(Arbre a, int niv, Liste * lst){
    if(!a) return 0;
    if(niv == 0){ // Si on est arrivé au niveau voulu
        Cellule * cell = alloue_cellule(a);
        insere_en_tete(lst, cell);
        return 1;
    }
    insere_niveau(a->fg, niv-1, lst);
    insere_niveau(a->fd,niv-1, lst);
    return 1;

}

int hauteur(Arbre a){
    if(!a) return 0;
    // On ajoute 1 à la hauteur maximale obtenue entre
    // le sous-arbre gauche et le sous-arbre droit
    return 1 + (hauteur(a->fg) > hauteur(a->fd) ? hauteur(a->fg) : hauteur(a->fd) );
}

int parcours_largeur_naif(Arbre a, Liste * lst){
    if(!a) return 0;
    int niv = hauteur(a);
    insere_niveau(a, niv, lst);
    return 1;
}
 
int main() {
    Arbre arbre = NULL;
    Liste lst = NULL;

    // Construire un arbre complet de hauteur 2
    if (construit_complet(2, &arbre)) {
        printf("Arbre construit avec succès.\n");
    } else {
        printf("Échec de la construction de l'arbre.\n");
        return 1;
    }

    // Parcourir l'arbre en largeur et stocker les valeurs dans la liste
    if (parcours_largeur_naif(arbre, &lst)) {
        printf("Parcours en largeur réussi.\n");
        affiche_liste(lst);
    } else {
        printf("Échec du parcours en largeur.\n");
    }

    return 0;
}
