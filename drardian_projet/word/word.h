//  Interface pour la spécification du TDA mot dans le cas d'une structure de 
//    données représantant un mot ainsi que les lexiques dans lesquelles on le
//    trouve et aussi les lignes sur lesquelles il apparait dans un hypotétique
//    texte non spécifié dans ce module.

#ifndef WORD__H
#define WORD__H

#include <stddef.h>
#include <stdbool.h>

//  struct word, word: structure permettant de gérer un mot
//  L'utilisateur doit garantir que l'adresse de chaque objet fournie aux
//    diverses fonctions est valide durant toute la durée de la présence de
//    l'objet dans le mot. Si des opérations d'allocation dynamique
//    sont effectuées, elles le sont pour la gestion propre de la structure de
//    données, et en aucun cas pour réaliser de copies ou des destructions
//    d'objets

typedef struct word word;

//  Les fonctions qui suivent ont un comportement indéterminié si l'adresse de 
//    leur paramètre de type word * n'est pas l'adresse d'un objet préalablement
//    renvoyé par la fonction word_create et non révoqué depuis par la fonction
//    word_dispose
//  Les lignes et lexiques dans lesquelles apparait le mot sont gérés par des
//    liste provenant du module list dont l'inteface est défini dans le fichier 
//    list.h, aussi l'uttilisation des fonctions suivantes peuvent être déduites
//    prèsque immédiatement de l'inteface list.h à ceci près que ces fonction
//    ont été ôté de leur caractère polymorphe pour une uttilisation simplifiée:
//    word_next_line, word_can_next_line, word_reset_current_line,
//    word_next_glosary, word_can_next_glosary, word_reset_current_glosary 

//  word_create: crée une structure de donnée correspondant un mot ayant pour
//    nom la chaine de caractères représenté par value et renvois son adresse ou
//    NULL si il y a eu dépassement de capacitée
extern word *word_create(char *value);

//  word_add_line: ajoute aux lignes sur lesquelles on peut trouver le mot w
//    dans le texte, la ligne line et renvois true en cas de succès, false en 
//    cas de dépassement de capacité
extern bool word_add_line(word *w, size_t line);

//  word_add_glosary: ajoute aux lexiques dans lesquels le mot w apparait,
//    le lexique représenté par la chaine de caratère glosary et renvois true
//    en cas de succès, false en cas de débordement de capacité
//  Il est bon à noter qu'aucun test n'est fait sur la chaine de caractère 
//    glosary, aussi est il possible d'ajouter NULL au mot word. Cela étant
//    due aux module list uttilisé, et souhaitant une performance optimale,
//    dans ce cas-ci, la fonction renverra false et il ne sera pas possible de
//    différencier un débordement de capacitée d'une execution normale autrement
//    qu'en vérifiant manuellement la fin des lexiques a l'aide des fonctions 
//    word_next_line et word_can_next_line, pour vérifier que NULL y est bien 
//    présent. Pour cette raison, il est fortement déconseillé de passer NULL 
//    comme valeure associée à la chaine de caractère glosary
extern bool word_add_glosary(word *w, const char *glosary);

//  word_next_line: renvois la ligne courante de w et passe à la suivante.
//    Cette fonction doit être éxécutée seulement si word_can_next_line(w) a
//    renvoyé true après tout autre appel à word_next_line(w) antérieur à 
//    l'appel actuel de la fonction, ou si pour une quelquonque raison 
//    vous avez une preuve que l'objet courant existe bel et bien.
//    Pour plus de clartée sur cette fonction, se référer à la fonction 
//    list_next du module list comme spécifié dans l'entête de ce fichier
extern size_t word_next_line(word *w);

//  word_can_next_line: indique si l'uttilisation de word_next_line(w) est 
//    autorisé sur w, si l'uttilisation de word_next_line(w) n'est pas autorisé,
//    il est garanti que la cause de cette interdiction ne peut être que 
//    parceque la ligne courante est la dernière dans w
//    Pour plus de clartée sur cette fonction, se référer à la fonction 
//    list_can_next du module list comme spécifié dans l'entête de ce fichier.
extern bool word_can_next_line(word *w);

//  word_rest_current_line: remet la ligne courante sur la première ligne
//    Pour plus de clartée sur cette fonction, se référer à la fonction 
//    list_reset_current du module list comme spécifié dans l'entête de ce 
//    fichier.
extern void word_reset_current_line(word *w);

//  word_next_glosary: comportement comparable à word_next_line avec pour objet
//    les lexiques en lieu et place des lignes et la fonction 
//    word_can_next_glosary(w) en remplacement à word_can_next_line(w)
//    Pour plus de clartée sur cette fonction, se référer à la fonction 
//    list_next du module list comme spécifié dans l'entête de ce fichier.
extern char *word_next_glosary(word *w);

//  word_can_next_glosary: comportement comparable à word_can_next_line avec
//    pour objet les lexiques en lieu et place des lignes et la fonction 
//    word_next_glosary(w) en remplacement à word_next_line(w)
//    Pour plus de clartée sur cette fonction, se référer à la fonction 
//    list_can_next du module list comme spécifié dans l'entête de ce fichier.
extern bool word_can_next_glosary(word *w);

//  word_rest_current_glosary: remet la ligne courante sur la premièr lexique
//    Pour plus de clartée sur cette fonction, se référer à la fonction 
//    list_reset_current du module list comme spécifié dans l'entête de ce 
//    fichier.
extern void word_reset_current_glosary(word *w);

//  word_dispose: libère toutes les ressources associés à la structure de *w 
//    puis affecte à *w la valeur NULL
extern void word_dispose(word **w);
#endif
