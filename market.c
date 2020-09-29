#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "market.h"

typedef struct strNode* Node;
typedef struct strQueue* Queue;

struct strNode{
	int articles;
	int clientNumber;
	struct strNode * next;
};

struct strQueue{
	Node first, last;
	int size;
};

struct strMarket{
	Queue* checkouts;
	int firstClient;
};

//-----------------------------------------------------------------
//FUNCIONES DE QUEUE

Queue queue_create(){
	Queue q = (Queue) calloc(1, sizeof(struct strQueue));
	return q;
}

bool queue_isEmpty(Queue q){
	if(q == NULL)
		return TRUE;

	return q->size == 0;
}

void queue_offer(Queue q, int clientNumber, int articleQtty){
	if(q == NULL)
		return;

	//Reservar memoria para el nodo del cliente
	Node newClient = (Node) calloc(1, sizeof(struct strNode));

	newClient->clientNumber = clientNumber;
	newClient->articles = articleQtty;

	newClient->next = NULL;

	if(queue_isEmpty(q))
		q->first = newClient;
	else
		q->last->next = newClient;

	q->last = newClient;
	q->size++;
}

void queue_poll(Queue q){
	if(q == NULL || q->size == 0)
		return;

	Node aux = q->first;

	q->first = q->first->next;
	q->size--;

	if(q->size == 0)
		q->last = NULL;

	free(aux);
}

void queue_destroy(Queue q){
	if(q == NULL)
		return;

	Node aux1, aux2;

	aux1 = q->first;

	while(aux1 != NULL)
	{
		aux2 = aux1->next;
		free(aux1);
		aux1 = aux2;
	}

	free(q);
}


//-----------------------------------------------------------------
//FUNCIONES DE MARKET

int randomProducts(){
	int num = (rand() % 75) + 1; 
	return num;
}

int randomToCheck(int max){
	int num;
	if(max<15){
		num = (rand() % max) + 1; 	
	}else{
		num = (rand() % 11) + 5; 
	}
	return num;
}

int findBestCandidate(Market m){
	bool allClosed = FALSE;
	bool allOpen = FALSE;
	bool allOpenFull = FALSE;
	int lowestSize = -1;
	int indexLowestSize = 0;

	int openQtty = 0;
	int fullQtty = 0;

	//obtener cuántos están abiertos y cuántos abiertos estan llenos
	for(int i=0; i<5; i++){
		if(m->checkouts[i]){
			openQtty++;
			if(m->checkouts[i]->size >= 3)
				fullQtty++;
		}
	}

	//para facilitar checar las condiciones
	if(openQtty == 0)
		allClosed = TRUE;
	if(openQtty == 5)
		allOpen = TRUE;
	if(fullQtty == openQtty)
		allOpenFull = TRUE;

	//si todas las filas están cerradas
	if(allClosed){
		return 0;
	}

	if(allOpen){
		if(!allOpenFull){
			lowestSize = m->checkouts[0]->size;
			for(int i=0; i<5; i++){
				if(m->checkouts[i]->size < lowestSize){
					lowestSize = m->checkouts[i]->size;
					indexLowestSize = i;
				}
			}
			return indexLowestSize;
		}

		if(allOpenFull){
			int lowestProductQtty = -1;
			int indexLowestProductQtty = 0;

			Node aux = NULL;
			for(int i=0; i<5; i++){
				aux = m->checkouts[i]->first;
				int allQueueProducts = 0;
				while(aux){
					allQueueProducts += aux->articles;
					aux = aux->next;
				}
				if(lowestProductQtty == -1){
					lowestProductQtty = allQueueProducts;
					indexLowestProductQtty = i;
				}
				else if(allQueueProducts < lowestProductQtty){
					lowestProductQtty = allQueueProducts;
					indexLowestProductQtty = i;
				}
			}
			return indexLowestProductQtty;
		}
	}

	if(!allOpen && !allClosed){
		if(!allOpenFull){
			//sacar el menor índice
			for(int i=0; i<5; i++){
				if(m->checkouts[i]){
					if(lowestSize==-1){
						lowestSize = m->checkouts[i]->size;
						indexLowestSize = i;
					}
					if(m->checkouts[i]->size < lowestSize){
						lowestSize = m->checkouts[i]->size;
						indexLowestSize = i;
					}
				}
			}
			return indexLowestSize;
		}

		if(allOpenFull){
			for(int i=0; i<5; i++)
				if(!m->checkouts[i])
					return i;
		}
	}
	return -1;
}

void market_print(Market m){
	printf("CAJA\tSTATUS\tFIRST\tSECOND\tTHIRD\tFOURTH\n");
	for(int i=0; i<5; i++){
		bool status = FALSE;
		if(m->checkouts[i]){
			status = TRUE;

			printf("C%d\t%d\t", i+1, status);

			Node aux = m->checkouts[i]->first;
			while(aux){
				printf("%d\t", aux->articles);
				aux = aux->next;
			}
		}
		else{
			printf("C%d\t%d", i+1, status);
		}
		printf("\n");
	}
	printf("\n");
}

Market market_create(){
	srand(time(0));
	Market m = (Market) calloc(1, sizeof(struct strMarket));
	m->checkouts = calloc(5, sizeof(Queue));
	m->firstClient = 1;
	return m;
}

void market_destroy(Market m){
	for(int i=0; i<5; i++){
		queue_destroy(m->checkouts[i]);
	}
	free(m);
}

void market_newClient(Market m){
	if(!m)
		return;

	//Encontrar mejor queue candidato
	int candidate = findBestCandidate(m);

	if(!m->checkouts[candidate]){
		m->checkouts[candidate] = queue_create();
		printf("Se abriO la caja %d\n", candidate+1);
	}

	int articleQtty = randomProducts();
	queue_offer(m->checkouts[candidate], m->firstClient, articleQtty);
	printf("LlegO el cliente %d en la caja %d con %d artIculos\n", m->checkouts[candidate]->last->clientNumber, candidate+1, articleQtty);
	m->firstClient++;
}

void market_checkoutCycle(Market m){
	int productsToCheck;
	for(int i=0; i<5; i++){
		if(m->checkouts[i]){
			productsToCheck = randomToCheck(m->checkouts[i]->first->articles);
			m->checkouts[i]->first->articles -= productsToCheck;
			printf("Se cobraron %d artIculos del cliente %d en la caja %d\n", productsToCheck, m->checkouts[i]->first->clientNumber, i+1);

			if(m->checkouts[i]->first->articles <= 0){
				printf("Se atendiO al cliente %d y saliO del supermercado.\n", m->checkouts[i]->first->clientNumber);
				queue_poll(m->checkouts[i]);
			}
			if(m->checkouts[i]->size == 0){
				queue_destroy(m->checkouts[i]);
				m->checkouts[i] = NULL;
				printf("Se cerrO la caja %d\n", i+1);
			}
		}
	}
}

