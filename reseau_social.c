#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*  reseau_social.c
 *  Linked-list social network in C
 *  Structures: post_t (inner list) + user_t (outer list) */

#define ID_MIN  1000
#define ID_MAX  9999

/* Data structures */

typedef struct post {
    int   id_post;
    char  contenu[100];
    int   likes;
    struct post *svt;
} post_t;

typedef struct user {
    int    id_user;
    char   nom[30];
    int    age;
    post_t *l_posts;      /* inner linked list of posts */
    struct user *svt;
} user_t;

typedef user_t* reseau_t;

/* Forward declarations */

reseau_t insere_user(reseau_t R, int id, char *nom, int age);
post_t*  insere_post(post_t *L, int id, char *contenu, int likes);
void     affiche_user(user_t U);
void     affiche_reseau(reseau_t R);
int      total_likes(reseau_t R);
float    moyenne_likes(reseau_t R);
reseau_t supprimer_user(reseau_t R, int id);
void     liberer_posts(post_t *L);
void     liberer_reseau(reseau_t R);

/* fill ONE post via pointer */

void saisie_post(post_t *P) {
    do {
        printf("  ID post (4 chiffres): ");
        scanf("%d", &P->id_post);
    } while (P->id_post < ID_MIN || P->id_post > ID_MAX);

    printf("  Contenu: ");
    scanf(" %[^\n]", P->contenu);

    do {
        printf("  Likes (>= 0): ");
        scanf("%d", &P->likes);
    } while (P->likes < 0);

    P->svt = NULL;
}

/* display ONE post by value */

void affiche_post(post_t P) {
    printf("    [%d] \"%s\"  likes: %d\n",
           P.id_post, P.contenu, P.likes);
}

/* fill ONE user via pointer */

void saisie_user(user_t *U) {
    do {
        printf("ID user (4 chiffres): ");
        scanf("%d", &U->id_user);
    } while (U->id_user < ID_MIN || U->id_user > ID_MAX);

    printf("Nom: ");
    scanf("%s", U->nom);

    do {
        printf("Age: ");
        scanf("%d", &U->age);
    } while (U->age <= 0 || U->age > 120);

    U->l_posts = NULL;
    U->svt     = NULL;

    /* Ask how many posts to add immediately */
    int nb;
    printf("Nombre de publications a ajouter: ");
    scanf("%d", &nb);
    for (int i = 0; i < nb; i++) {
        printf("  -- Publication %d --\n", i + 1);
        post_t *p = (post_t*)malloc(sizeof(post_t));
        if (!p) { fprintf(stderr, "Erreur malloc post\n"); exit(1); }
        saisie_post(p);
        U->l_posts = insere_post(U->l_posts, p->id_post,
                                 p->contenu, p->likes);
        free(p);
    }
}

/* display ONE user (posts included) */

void affiche_user(user_t U) {
    printf("User [%d] %s  age: %d\n",
           U.id_user, U.nom, U.age);
    post_t *cour = U.l_posts;
    while (cour != NULL) {
        affiche_post(*cour);
        cour = cour->svt;
    }
}

/* display every user */

void affiche_reseau(reseau_t R) {
    reseau_t cour = R;
    printf("\n=== Reseau Social ===\n");
    while (cour != NULL) {
        affiche_user(*cour);
        printf("\n");
        cour = cour->svt;
    }
}

/* sorted insert by id_post */

post_t* insere_post(post_t *L, int id, char *contenu, int likes) {
    /* check duplicate */
    post_t *cour = L;
    while (cour != NULL) {
        if (cour->id_post == id) {
            printf("Erreur: post ID %d existe deja.\n", id);
            return L;
        }
        cour = cour->svt;
    }

    /* create node */
    post_t *nouv = (post_t*)malloc(sizeof(post_t));
    if (!nouv) { fprintf(stderr, "Erreur malloc\n"); exit(1); }
    nouv->id_post = id;
    strcpy(nouv->contenu, contenu);
    nouv->likes = likes;
    nouv->svt   = NULL;

    /* insert at head if list empty or new id is smallest */
    if (L == NULL || id < L->id_post) {
        nouv->svt = L;
        return nouv;
    }

    /* find sorted position */
    cour = L;
    while (cour->svt != NULL && cour->svt->id_post < id)
        cour = cour->svt;
    nouv->svt = cour->svt;
    cour->svt = nouv;
    return L;
}

/* sorted insert by id_user */

reseau_t insere_user(reseau_t R, int id, char *nom, int age) {
    /* check duplicate */
    reseau_t cour = R;
    while (cour != NULL) {
        if (cour->id_user == id) {
            printf("Erreur: user ID %d existe deja.\n", id);
            return R;
        }
        cour = cour->svt;
    }

    /* create node */
    user_t *nouv = (user_t*)malloc(sizeof(user_t));
    if (!nouv) { fprintf(stderr, "Erreur malloc\n"); exit(1); }
    nouv->id_user = id;
    strcpy(nouv->nom, nom);
    nouv->age    = age;
    nouv->l_posts = NULL;
    nouv->svt    = NULL;

    if (R == NULL || id < R->id_user) {
        nouv->svt = R;
        return nouv;
    }

    cour = R;
    while (cour->svt != NULL && cour->svt->id_user < id)
        cour = cour->svt;
    nouv->svt = cour->svt;
    cour->svt = nouv;
    return R;
}

/* total likes across all users/posts */

int total_likes_posts(post_t *L) {
    if (L == NULL) return 0;
    return L->likes + total_likes_posts(L->svt);
}

int total_likes(reseau_t R) {
    if (R == NULL) return 0;
    return total_likes_posts(R->l_posts) + total_likes(R->svt);
}

/* recursive count of all users */

int nb_users(reseau_t R) {
    if (R == NULL) return 0;
    return 1 + nb_users(R->svt);
}

float moyenne_likes(reseau_t R) {
    int n = nb_users(R);
    if (n == 0) return 0;
    return (float)total_likes(R) / n;
}

/* remove by id */

reseau_t supprimer_user(reseau_t R, int id) {
    if (R == NULL) return NULL;

    if (R->id_user == id) {
        reseau_t temp = R->svt;
        liberer_posts(R->l_posts);
        free(R);
        return temp;
    }

    reseau_t cour = R;
    while (cour->svt != NULL) {
        if (cour->svt->id_user == id) {
            reseau_t a_suppr = cour->svt;
            cour->svt = a_suppr->svt;
            liberer_posts(a_suppr->l_posts);
            free(a_suppr);
            return R;
        }
        cour = cour->svt;
    }
    printf("User ID %d introuvable.\n", id);
    return R;
}

/* Memory cleanup */

void liberer_posts(post_t *L) {
    while (L != NULL) {
        post_t *tmp = L->svt;
        free(L);
        L = tmp;
    }
}

void liberer_reseau(reseau_t R) {
    while (R != NULL) {
        reseau_t tmp = R->svt;
        liberer_posts(R->l_posts);
        free(R);
        R = tmp;
    }
}

/* main */

int main(void) {
    reseau_t R = NULL;
    int choix;

    printf("=== Reseau Social — Saisie ===\n");
    do {
        user_t temp;
        saisie_user(&temp);
        R = insere_user(R, temp.id_user, temp.nom, temp.age);

        /* copy posts into the newly inserted node */
        reseau_t inserted = R;
        while (inserted && inserted->id_user != temp.id_user)
            inserted = inserted->svt;
        if (inserted) inserted->l_posts = temp.l_posts;

        printf("Ajouter un autre utilisateur? (1=oui / 0=non): ");
        scanf("%d", &choix);
    } while (choix == 1);

    affiche_reseau(R);

    printf("\nTotal likes: %d\n", total_likes(R));
    printf("Moyenne likes / user: %.2f\n", moyenne_likes(R));

    /* manual post insert for user 1234 (demo) */
    printf("\n=== Ajout d'un post manuellement ===\n");
    int uid;
    printf("ID utilisateur cible: ");
    scanf("%d", &uid);
    reseau_t cour = R;
    while (cour != NULL && cour->id_user != uid)
        cour = cour->svt;
    if (cour) {
        int pid, likes;
        char contenu[100];
        printf("  ID post: ");  scanf("%d", &pid);
        printf("  Contenu: "); scanf(" %[^\n]", contenu);
        printf("  Likes: ");   scanf("%d", &likes);
        cour->l_posts = insere_post(cour->l_posts, pid, contenu, likes);
        affiche_user(*cour);
    } else {
        printf("Utilisateur introuvable.\n");
    }

    liberer_reseau(R);
    return 0;
}
