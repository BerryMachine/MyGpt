 #include "tokenizer.h"
 #include <stdio.h>
 #include <string.h>
 

 void tokenize() {
    const char* text = "An implementation of Kenneth Stanley's NeuroEvolution of Augmenting Topologies (NEAT). In this implementation, ant colonies compete against each other for food. The best colonies produce similar colonies, making the colonies progressively better. It is able to simulate fighting, food, pheromones, offspring, and nest raids.";

    int size = strlen(text);
    for (int i = 0; i < size - 1; i++) {
        char a = text[i];
        char b = text[i + 1];
        printf("%c%c\n", a, b);
    }

    


    return;
 }