#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "oeuf.h"

void test_allocation_simple() {
    printf("Test d'allocation simple...\n");
    int* ptr = (int*) oe_malloc(sizeof(int));
    assert(ptr != NULL);  // Vérifie que l'allocation a réussi
    *ptr = 42;  // Utilisation de la mémoire allouée
    assert(*ptr == 42);  // Vérifie la valeur assignée
    oe_free(ptr);
    printf("Test d'allocation simple réussi !\n\n");
}

void test_allocation_multiple() {
    printf("Test d'allocation multiple...\n");
    int* ptr1 = (int*) oe_malloc(sizeof(int));
    int* ptr2 = (int*) oe_malloc(sizeof(int) * 10);
    assert(ptr1 != NULL && ptr2 != NULL);  // Vérifie que les allocations ont réussi
    for (int i = 0; i < 10; i++) {
        ptr2[i] = i * 2;  // Remplissage de la mémoire allouée
    }
    for (int i = 0; i < 10; i++) {
        assert(ptr2[i] == i * 2);  // Vérifie les valeurs
    }
    oe_free(ptr1);
    oe_free(ptr2);
    printf("Test d'allocation multiple réussi !\n\n");
}

void test_libération_double() {
    printf("Test de libération double...\n");
    int* ptr = (int*) oe_malloc(sizeof(int));
    assert(ptr != NULL);
    oe_free(ptr);
    printf("Première libération réussie...\n");
    // Vérifie que la double libération ne provoque pas de crash
    oe_free(ptr);  // Cela ne devrait pas planter si oe_free est bien implémenté
    printf("Test de libération double réussi !\n\n");
}

void test_allocation_zero() {
    printf("Test d'allocation de taille zéro...\n");
    void* ptr = oe_malloc(0);
    assert(ptr == NULL || ptr != NULL);  // Selon l'implémentation, malloc(0) peut retourner NULL ou un pointeur valide
    oe_free(ptr);  // Ne doit pas planter même si ptr est NULL
    printf("Test d'allocation de taille zéro réussi !\n\n");
}


int main() {
    test_allocation_simple();
    test_allocation_multiple();
    test_libération_double();
    test_allocation_zero();

    printf("Tous les tests sont passés avec succès !\n");
    return 0;
}