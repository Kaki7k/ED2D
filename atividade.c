//Thiago Henrique Rodrigues Arakaki 2261286
//Gabriel de Souza Muniz 2475430
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define true 1
#define false 0
typedef int bool;
typedef int TIPOPESO;

typedef struct Aresta
{
    int destino;
    TIPOPESO peso;
    struct Aresta *proxima;
} Aresta;

typedef struct Vertice
{
    Aresta *listaAdj;
} Vertice;

typedef struct Grafo
{
    int numArestas;
    int numVertices;
    Vertice *vertices;
} Grafo;

Grafo *criarGrafo(int numVertices)
{
    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));
    grafo->numVertices = numVertices;
    grafo->numArestas = 0;
    grafo->vertices = (Vertice *)malloc(numVertices * sizeof(Vertice));

    for (int i = 0; i < numVertices; i++)
    {
        grafo->vertices[i].listaAdj = NULL;
    }

    return grafo;
}

bool adicionarAresta(Grafo *grafo, int origem, int destino, TIPOPESO peso)
{
    if (!grafo)
        return false;
    if ((origem < 0) || (origem >= grafo->numVertices))
        return false;
    if ((destino < 0) || (destino >= grafo->numVertices))
        return false;

    Aresta *novaAresta = (Aresta *)malloc(sizeof(Aresta));
    novaAresta->destino = destino;
    novaAresta->peso = peso;

    novaAresta->proxima = grafo->vertices[origem].listaAdj;

    grafo->vertices[origem].listaAdj = novaAresta;
    grafo->numArestas++;
}

// void imprimirGrafo(Grafo *grafo)
// {
//     for (int i = 0; i < grafo->numVertices; i++)
//     {
//         Aresta *atual = grafo->vertices[i].listaAdj;
//         printf("Vértice %d: ", i);
//         while (atual != NULL)
//         {
//             printf("(%d, %d) ", atual->destino, atual->peso);
//             atual = atual->proxima;
//         }
//         printf("\n");
//     }
// }

void imprimirGrafo(Grafo *grafo)
{
    for (int i = 0; i < grafo->numVertices; i++)
    {
        printf("v%d: ", i);
        Aresta *atual = grafo->vertices[i].listaAdj;
        while (atual != NULL)
        {
            printf("v%d(%d) ", atual->destino, atual->peso);
            atual = atual->proxima;
        }
        printf("\n");
    }
}

int calcularCaminho(Grafo *grafo, int origem, int destino)
{
    int *distancia = (int *)malloc(grafo->numVertices * sizeof(int));

    for (int i = 0; i < grafo->numVertices; i++)
    {
        distancia[i] = INT_MAX;
    }

    distancia[origem] = 0;

    for (int i = 0; i < grafo->numVertices - 1; i++)
    {
        for (int v = 0; v < grafo->numVertices; v++)
        {
            Aresta *atual = grafo->vertices[v].listaAdj;
            while (atual != NULL)
            {
                int u = v;
                int w = atual->destino;
                int peso = atual->peso;
                if (distancia[u] != INT_MAX && distancia[u] + peso < distancia[w])
                {
                    distancia[w] = distancia[u] + peso;
                }
                atual = atual->proxima;
            }
        }
    }

    int resultado = distancia[destino];
    free(distancia);
    return resultado;
}

void imprimirMatrizAdjacencia(Grafo *grafo)
{
    for (int i = 0; i < grafo->numVertices; i++)
    {
        for (int j = 0; j < grafo->numVertices; j++)
        {
            Aresta *atual = grafo->vertices[i].listaAdj;
            int peso = 0;

            while (atual != NULL)
            {
                if (atual->destino == j)
                {
                    peso = atual->peso;
                    break;
                }
                atual = atual->proxima;
            }

            printf("%d\t", peso);
        }
        printf("\n");
    }
}

int main()
{
    int numVertices = 5;

    printf("------------- GRAFO 1 -------------\n");


    Grafo *grafo = criarGrafo(numVertices);

    adicionarAresta(grafo, 0, 0, 1);
    adicionarAresta(grafo, 0, 2, 1);
    adicionarAresta(grafo, 1, 2, 1);
    adicionarAresta(grafo, 1, 3, 1);
    adicionarAresta(grafo, 1, 4, 1);
    adicionarAresta(grafo, 2, 3, 1);
    adicionarAresta(grafo, 3, 4, 1);

    printf("Lista de Adjacência:\n");
    imprimirGrafo(grafo);
    printf("\n");

    printf("Matriz de Adjacência:\n");
    imprimirMatrizAdjacencia(grafo);

    printf("------------- GRAFO 2 -------------\n");


    Grafo *grafo2 = criarGrafo(numVertices);

    adicionarAresta(grafo2, 0, 0, 2);
    adicionarAresta(grafo2, 0, 2, 7);
    adicionarAresta(grafo2, 2, 1, 5);
    adicionarAresta(grafo2, 2, 3, 10);
    adicionarAresta(grafo2, 3, 1, 9);
    adicionarAresta(grafo2, 3, 4, 1);
    adicionarAresta(grafo2, 4, 1, 12);

    printf("Lista de Adjacência:\n");
    imprimirGrafo(grafo2);
    printf("\n");

    printf("Matriz de Adjacência:\n");
    imprimirMatrizAdjacencia(grafo2);

    int origem = 0;
    int destino = 4;
    int valorTotal = calcularCaminho(grafo2, origem, destino);

    if (valorTotal != INT_MAX)
    {
        printf("\nO valor total do caminho de {%d, %d} é %d\n", origem, destino, valorTotal);
    }
    else
    {
        printf("\nNão há caminho de {%d, %d}\n", origem, destino);
    }

    return 0;
}
