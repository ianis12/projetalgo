//  Interface pour la spécification du module option permettant la gestion
//    des options tel que spécifié ci-contre :
//    --help : aﬃcher l’aide selon le format usuel; 
//    -S ou --sort : aﬃcher la liste dans l’ordre lexicographique. 
//    --case=VALUE : convertir tous les caractères lus en minuscule si VALUE 
//    vaut lower, en majuscules si VALUE vaut upper, les laisser tels quels si 
//    VALUE vaut as-is; 
//    -l : équivalent à --case=lower; 
//    -s : équivalent à --case=as-is; 
//    -u : équivalent à --case=upper; 
//    -i FILE ou --input=FILE : au lieu de l’entrée standard, lire le texte dans
//    le ﬁchier FILE; 
//    -o FILE ou --ouput=FILE : au lieu de la sortie standard, écrire résultat dans
//    le ﬁchier FILE.
#ifndef OPTIONS__H
#define OPTIONS__H

#define OPTION_WORD_LENGTH_MAX 63
#define OPTION_FUN_FAILURE -1
#define OPTION_FUN_SUCCESS 0

extern int option(int argc, char** argv, bool* sort, bool* lowerCase,
    bool* upperCase, bool* asIsCase, char **inputFileName, 
    char **outputFileName, list *glosaryFileNamesList);
    
#endif
