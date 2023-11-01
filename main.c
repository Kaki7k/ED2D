//Thiago Henrique Rodrigues Arakaki 2261286
//Gabriel de Souza Muniz 2475430

#include <stdio.h>
#include <stdlib.h>

#define MAX 3
#define MIN 2

struct NoArvoreB {
  int item[MAX + 1], contador;
  struct NoArvoreB *ligacoes[MAX + 1];
};

struct NoArvoreB *raiz;

// Criação de um nó
struct NoArvoreB *criarNo(int item, struct NoArvoreB *filho) {
  struct NoArvoreB *novoNo;
  novoNo = (struct NoArvoreB *)malloc(sizeof(struct NoArvoreB));
  novoNo->item[1] = item;
  novoNo->contador = 1;
  novoNo->ligacoes[0] = raiz;
  novoNo->ligacoes[1] = filho;
  return novoNo;
}

// Adiciona valor ao nó
void adicionarValorNo(int item, int pos, struct NoArvoreB *no,
          struct NoArvoreB *filho) {
  int j = no->contador;
  while (j > pos) {
    no->item[j + 1] = no->item[j];
    no->ligacoes[j + 1] = no->ligacoes[j];
    j--;
  }
  no->item[j + 1] = item;
  no->ligacoes[j + 1] = filho;
  no->contador++;
}

// Dividir o nó
void dividirNo(int item, int *pval, int pos, struct NoArvoreB *no,
         struct NoArvoreB *filho, struct NoArvoreB **novoNo) {
  int mediana, j;

  if (pos > MIN)
    mediana = MIN + 1;
  else
    mediana = MIN;

  *novoNo = (struct NoArvoreB *)malloc(sizeof(struct NoArvoreB));
  j = mediana + 1;
  while (j <= MAX) {
    (*novoNo)->item[j - mediana] = no->item[j];
    (*novoNo)->ligacoes[j - mediana] = no->ligacoes[j];
    j++;
  }
  no->contador = mediana;
  (*novoNo)->contador = MAX - mediana;

  if (pos <= MIN) {
    adicionarValorNo(item, pos, no, filho);
  } else {
    adicionarValorNo(item, pos - mediana, *novoNo, filho);
  }
  *pval = no->item[no->contador];
  (*novoNo)->ligacoes[0] = no->ligacoes[no->contador];
  no->contador--;
}

// Definir o valor no nó
int definirValorNo(int item, int *pval,
           struct NoArvoreB *no, struct NoArvoreB **filho) {
  int pos;
  if (!no) {
    *pval = item;
    *filho = NULL;
    return 1;
  }

  if (item < no->item[1]) {
    pos = 0;
  } else {
    for (pos = no->contador;
       (item < no->item[pos] && pos > 1); pos--)
      ;
    if (item == no->item[pos]) {
      printf("Duplicados não são permitidos\n");
      return 0;
    }
  }
  if (definirValorNo(item, pval, no->ligacoes[pos], filho)) {
    if (no->contador < MAX) {
      adicionarValorNo(*pval, pos, no, *filho);
    } else {
      dividirNo(*pval, pval, pos, no, *filho, filho);
      return 1;
    }
  }
  return 0;
}

// Operação de inserção
void insercao(int item) {
  int flag, i;
  struct NoArvoreB *filho;

  flag = definirValorNo(item, &i, raiz, &filho);
  if (flag)
    raiz = criarNo(i, filho);
}

// Copiar o sucessor
void copiarSucessor(struct NoArvoreB *meuNo, int pos) {
  struct NoArvoreB *dummy;
  dummy = meuNo->ligacoes[pos];

  for (; dummy->ligacoes[0] != NULL;)
    dummy = dummy->ligacoes[0];
  meuNo->item[pos] = dummy->item[1];
}

// Remover o valor
void removerValor(struct NoArvoreB *meuNo, int pos) {
  int i = pos + 1;
  while (i <= meuNo->contador) {
    meuNo->item[i - 1] = meuNo->item[i];
    meuNo->ligacoes[i - 1] = meuNo->ligacoes[i];
    i++;
  }
  meuNo->contador--;
}

// Fazer o deslocamento para a direita
void deslocamentoDireita(struct NoArvoreB *meuNo, int pos) {
  struct NoArvoreB *x = meuNo->ligacoes[pos];
  int j = x->contador;

  while (j > 0) {
    x->item[j + 1] = x->item[j];
    x->ligacoes[j + 1] = x->ligacoes[j];
  }
  x->item[1] = meuNo->item[pos];
  x->ligacoes[1] = x->ligacoes[0];
  x->contador++;

  x = meuNo->ligacoes[pos - 1];
  meuNo->item[pos] = x->item[x->contador];
  meuNo->ligacoes[pos] = x->ligacoes[x->contador];
  x->contador--;
  return;
}

// Fazer o deslocamento para a esquerda
void deslocamentoEsquerda(struct NoArvoreB *meuNo, int pos) {
  int j = 1;
  struct NoArvoreB *x = meuNo->ligacoes[pos - 1];

  x->contador++;
  x->item[x->contador] = meuNo->item[pos];
  x->ligacoes[x->contador] = meuNo->ligacoes[pos]->ligacoes[0];

  x = meuNo->ligacoes[pos];
  meuNo->item[pos] = x->item[1];
  x->ligacoes[0] = x->ligacoes[1];
  x->contador--;

  while (j <= x->contador) {
    x->item[j] = x->item[j + 1];
    x->ligacoes[j] = x->ligacoes[j + 1];
    j++;
  }
  return;
}

// Fundir os nós
void fundirNos(struct NoArvoreB *meuNo, int pos) {
  int j = 1;
  struct NoArvoreB *x1 = meuNo->ligacoes[pos], *x2 = meuNo->ligacoes[pos - 1];

  x2->contador++;
  x2->item[x2->contador] = meuNo->item[pos];
  x2->ligacoes[x2->contador] = meuNo->ligacoes[0];

  while (j <= x1->contador) {
    x2->contador++;
    x2->item[x2->contador] = x1->item[j];
    x2->ligacoes[x2->contador] = x1->ligacoes[j];
    j++;
  }

  j = pos;
  while (j < meuNo->contador) {
    meuNo->item[j] = meuNo->item[j + 1];
    meuNo->ligacoes[j] = meuNo->ligacoes[j + 1];
    j++;
  }
  meuNo->contador--;
  free(x1);
}

// Ajustar o nó
void ajustarNo(struct NoArvoreB *meuNo, int pos) {
  if (!pos) {
    if (meuNo->ligacoes[1]->contador > MIN) {
      deslocamentoEsquerda(meuNo, 1);
    } else {
      fundirNos(meuNo, 1);
    }
  } else {
    if (meuNo->contador != pos) {
      if (meuNo->ligacoes[pos - 1]->contador > MIN) {
        deslocamentoDireita(meuNo, pos);
      } else {
        if (meuNo->ligacoes[pos + 1]->contador > MIN) {
          deslocamentoEsquerda(meuNo, pos + 1);
        } else {
          fundirNos(meuNo, pos);
        }
      }
    } else {
      if (meuNo->ligacoes[pos - 1]->contador > MIN)
        deslocamentoDireita(meuNo, pos);
      else
        fundirNos(meuNo, pos);
    }
  }
}

// Excluir um valor do nó
int excluirValorDoNo(int item, struct NoArvoreB *meuNo) {
  int pos, flag = 0;
  if (meuNo) {
    if (item < meuNo->item[1]) {
      pos = 0;
      flag = 0;
    } else {
      for (pos = meuNo->contador; (item < meuNo->item[pos] && pos > 1); pos--);
      if (item == meuNo->item[pos]) {
        flag = 1;
      } else {
        flag = 0;
      }
    }
    if (flag) {
      if (meuNo->ligacoes[pos - 1]) {
        copiarSucessor(meuNo, pos);
        flag = excluirValorDoNo(meuNo->item[pos], meuNo->ligacoes[pos]);
        if (flag == 0) {
          printf("O dado informado não está presente na Árvore B\n");
        }
      } else {
        removerValor(meuNo, pos);
      }
    } else {
      flag = excluirValorDoNo(item, meuNo->ligacoes[pos]);
    }
    if (meuNo->ligacoes[pos]) {
      if (meuNo->ligacoes[pos]->contador < MIN)
        ajustarNo(meuNo, pos);
    }
  }
  return flag;
}

// Operação de exclusão
void excluir(int item, struct NoArvoreB *meuNo) {
  struct NoArvoreB *tmp;
  if (!excluirValorDoNo(item, meuNo)) {
    printf("Não presente\n");
    return;
  } else {
    if (meuNo->contador == 0) {
      tmp = meuNo;
      meuNo = meuNo->ligacoes[0];
      free(tmp);
    }
  }
  raiz = meuNo;
  return;
}

void busca(int item, int *pos, struct NoArvoreB *meuNo) {
  if (!meuNo) {
    return;
  }

  if (item < meuNo->item[1]) {
    *pos = 0;
  } else {
    for (*pos = meuNo->contador; (item < meuNo->item[*pos] && *pos > 1); (*pos)--);
    if (item == meuNo->item[*pos]) {
      printf("%d presente na Árvore B", item);
      return;
    }
  }
  busca(item, pos, meuNo->ligacoes[*pos]);
  return;
}

void percorrer(struct NoArvoreB *meuNo) {
  int i;
  if (meuNo) {
    for (i = 0; i < meuNo->contador; i++) {
      percorrer(meuNo->ligacoes[i]);
      printf("%d ", meuNo->item[i + 1]);
    }
    percorrer(meuNo->ligacoes[i]);
  }
}

int main() {
  int item, ch;

  insercao(1);
  insercao(2);
  insercao(3);
  insercao(10);
  insercao(20);
  insercao(30);
  insercao(150);
  insercao(170);
  insercao(190);
  insercao(15);

  percorrer(raiz);

  excluir(20, raiz);
  printf("\n");
  percorrer(raiz);
}
