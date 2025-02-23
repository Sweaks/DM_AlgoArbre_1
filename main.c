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
    if(!(*lst)) {
        *lst = cell;
    } else {
        cell->suivant = *lst;
        *lst = cell;
    }
}

void insere_en_queue(Liste * lst, Cellule * cell){
    if(!(*lst)) {
        *lst = cell;
    } else {
        Liste temp = *lst;
        while(temp->suivant) {
            temp = temp->suivant;
        }
        temp->suivant = cell;
    }
}

Cellule *extrait_tete(Liste *lst){
    if(!(*lst)) return NULL;   

    Cellule * cellule_extraite = *lst;
    *lst = (*lst)->suivant;
    cellule_extraite->suivant = NULL;

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
    } else {
        f->fin->suivant = new_cell;
        f->fin = new_cell;
    }
    f->taille++;
    return 1;
}

int defiler(File f, Noeud ** sortant){
    /* Je ne suis pas vraiment sur de la fonction par contre
       A voir seulement quoi */
    
    if(est_vide(f) > 0) return 0; // Si la file est vide
    Cellule * cellule_extraite = extrait_tete(&(f)->debut);
    if(!cellule_extraite) { //Si rien n'extrait
        *sortant = NULL;
        return 0;
    }
    *sortant = cellule_extraite->noeud;
    free(cellule_extraite);
    f->taille--;

    if (f->debut == NULL) {  //Si f->debut est vide après suppression 
        f->fin = NULL; // On doit s'assurer que f->fin soit NULL aussi
    }

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


int construit_filiforme_aleatoire(int h, Arbre * a, int graine) {
    if (h < 0) return 0;

    srand(graine);

    *a = (Noeud*)malloc(sizeof(Noeud));
    if (*a == NULL) return 0;

    (*a)->valeur = 1;
    (*a)->fg = (*a)->fd = NULL;

    Noeud* parent = *a;

    for (int i = 2; i <= h + 1; i++) {
        Noeud* enfant = (Noeud*)malloc(sizeof(Noeud));

        if (enfant == NULL) {
            // Libérer toute la mémoire allouée en cas d'échec (à voir si on met pas juste free(*a))
            while (*a) {
                Noeud* temp = *a;
                *a = (*a)->fg ? (*a)->fg : (*a)->fd;
                free(temp);
            }
            return 0;
        }

        enfant->valeur = i;
        enfant->fg = enfant->fd = NULL;

        if (rand() % 2) {
            parent->fg = enfant;
        } 

        else {
            parent->fd = enfant;
        }

        parent = enfant;
    }

    return 1;
}


int insere_niveau(Arbre a, int niv, Liste * lst){
    if(!a) return 1; //Si l'arbre est vide , il y a aucun element dans la liste
    if(niv == 0){ // Si on est arrivé au niveau voulu
        Cellule * cell = alloue_cellule(a);
        if(!cell) return 0; //allocation ratée
        insere_en_queue(lst, cell);
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
    int h = hauteur(a);
    for (int niv = 0; niv < h; niv++) {
        insere_niveau(a, niv, lst);
    }
    return 1;
}



int parcours_largeur(Arbre a, Liste * lst) {
    File f = initialisation();

    enfiler(f, a);

    while(f->taille > 0) {
        Noeud* n;
        defiler(f, &n);

        if (n) {
            if (n->fg) {
                enfiler(f, n->fg);
            }

            if (n->fd) {
                enfiler(f, n->fd);
            }

            Cellule * cell = alloue_cellule(n);
            insere_en_queue(lst, cell);

        }
    }
    return 1;
}

int insere_niveau_V2(Arbre a, int niv, Liste * lst, int * nb_visite){
    if(!a) return 1; //Si l'arbre est vide , il y a aucun element dans la liste
    if(niv == 0){ // Si on est arrivé au niveau voulu
        Cellule * cell = alloue_cellule(a);
        insere_en_tete(lst, cell);
        (*nb_visite)++; //on incremente nb_visite apres chaque insertion dans lst
    }
    return insere_niveau_V2(a->fg, niv-1, lst, nb_visite) + insere_niveau_V2(a->fd,niv-1, lst, nb_visite);

}

int parcours_largeur_naif_V2(Arbre a, Liste *lst, int *nb_visite){
    /*
    test/Test_Parcours.c:67:test_parcours_largeur_naif_V2:FAIL: Expected 1 Was 3
    A corriger
    */
    int h = hauteur(a);
    *nb_visite = 1;
    int nb_noeud;
    for (int niv = 0; niv < h; niv++) {
        insere_niveau_V2(a, niv, lst, nb_visite);
    }
    nb_noeud = *nb_visite;
    return nb_noeud;
}


int parcours_largeur_V2(Arbre a, Liste * lst, int * nb_visite) {
    File f = initialisation();

    enfiler(f, a);

    while(f->taille > 0) {
        Noeud* n;
        defiler(f, &n);

        if (n) {
            if (n->fg) {
                enfiler(f, n->fg);
            }

            if (n->fd) {
                enfiler(f, n->fd);
            }

            Cellule * cell = alloue_cellule(n);
            insere_en_queue(lst, cell);
            (*nb_visite)++;
        }
    }
    return 1;
}
 
int main() {
    Arbre arbre = NULL;
    Liste lst = NULL;
    int nb_visites = 0;
    int nb_visites2 = 0;
    int nb_visites3 = 0;
    int nb_visites4 = 0;
    

    // Construire un arbre complet de hauteur 2
    if (construit_complet(4, &arbre)) {
        printf("Arbre construit avec succès.\n");
    } else {
        printf("Échec de la construction de l'arbre.\n");
        return 1;
    }

    // Parcourir l'arbre en largeur et stocker les valeurs dans la liste
    if (parcours_largeur_naif(arbre, &lst)) {
        printf("Parcours en largeur réussi.\n");
        //affiche_liste(lst);
    } else {
        printf("Échec du parcours en largeur.\n");
    }


    // test construit filiforme
    Arbre arbre2 = NULL;
    Liste lst2 = NULL;
    Liste lst3 = NULL;

    if (construit_filiforme_aleatoire(4, &arbre2, 42)) {
        printf("Arbre construit avec succès.\n");
    } else {
        printf("Échec de la construction de l'arbre.\n");
        return 1;
    }
    // afficher l'arbre construit
    //parcours_largeur_naif(arbre2, &lst2);
    //affiche_liste(lst2);

    
    // test parcours largeur naif V2 avec arbre de recherche complet
    //et test parcours largeur V2 avec arbre de recherche filiforme aléatoire
    printf("Parcours en largeur de l'arbre 1:\n");
    parcours_largeur_naif_V2(arbre, &lst2, &nb_visites);
    parcours_largeur_V2(arbre, &lst3, &nb_visites2);
    affiche_liste(lst2);
    affiche_liste(lst3);
    printf("Nombre de noeuds visités (parcours_largeur_naif_V2): %d\n", nb_visites);
    printf("Nombre de noeuds visités (parcours_largeur_V2): %d\n", nb_visites2);

    Liste lst4 = NULL;
    Liste lst5 = NULL;

    // test parcours largeur naif V2 avec arbre de recherche filiforme aléatoire
    //et test parcours largeur (naif) V2 avec arbre de recherche filiforme aléatoire
    printf("Parcours en largeur de l'arbre 2:\n");
    parcours_largeur_naif_V2(arbre2, &lst4, &nb_visites3);
    parcours_largeur_V2(arbre2, &lst5, &nb_visites4);
    affiche_liste(lst4);
    affiche_liste(lst5);
    printf("Nombre de noeuds visités (parcours_largeur_naif_V2): %d\n", nb_visites3);
    printf("Nombre de noeuds visités (parcours_largeur_V2): %d\n", nb_visites4);

    return 0;
}
