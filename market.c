#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "market.h"

typedef struct strNode *Node;
typedef struct strQueue *Queue;

struct strNode
{
	int articles;
	int clientNumber;
	struct strNode *next;
};

struct strQueue
{
	Node first, last;
	int size;
};

struct strMarket
{
	Queue *checkouts;
	int firstClient;
};

//-----------------------------------------------------------------
// FUNCIONES DE QUEUE

/**
 * Create a new queue data structure and allocate memory for it.
 * @return Pointer to the newly created queue.
 */
Queue queue_create()
{
	// Allocate memory for the queue structure and initialize it to zero.
	Queue q = (Queue)calloc(1, sizeof(struct strQueue));
	// Return the pointer to the newly created queue.
	return q;
}

/**
 * Check if the queue is empty.
 *
 * @param q The queue to check.
 * @return Returns true if the queue is empty, otherwise false.
 */
bool queue_isEmpty(Queue q)
{
	// If the queue is null, return true as there is nothing in the queue
	if (q == NULL)
		return TRUE;

	// If the size of the queue is 0, it means the queue is empty, hence return true.
	return q->size == 0;
}

/**
 * @brief Adds a new client to the queue. This function adds a new client to the end of the queue. If the queue is empty, the new client becomes the first and last client in the queue.
 * @param q Pointer to the queue structure.
 * @param clientNumber Integer representing the client number.
 * @param articleQtty Integer representing the quantity of articles the client wants to purchase.
 */
void queue_offer(Queue q, int clientNumber, int articleQtty)
{
	// Check if the queue is initialized
	if (q == NULL)
		return;

	// Allocate memory for the new client node
	Node newClient = (Node)calloc(1, sizeof(struct strNode));

	// Set the values for the new client node
	newClient->clientNumber = clientNumber;
	newClient->articles = articleQtty;

	newClient->next = NULL;

	// If the queue is empty, set the new client node as the first and last node in the queue
	if (queue_isEmpty(q))
		q->first = newClient;

	// If the queue is not empty, add the new client node to the end of the queue
	else
		q->last->next = newClient;

	// Increase the size of the queue
	q->last = newClient;
	q->size++;
}

/**
 * @brief Removes the first element from the queue. The function removes the first element from the queue and updates the first and last pointers accordingly. If the queue is empty or has not been initialized, the function returns immediately without doing anything.
 * @param q Pointer to the queue.
 */
void queue_poll(Queue q)
{
	// Check if the queue is NULL or empty
	if (q == NULL || q->size == 0)
		return;

	// Store a reference to the first node
	Node aux = q->first;

	// Update the first pointer to point to the second node
	q->first = q->first->next;
	q->size--;

	// If the queue is now empty, set the last pointer to NULL
	if (q->size == 0)
		q->last = NULL;

	// Free the memory occupied by the removed node
	free(aux);
}

/**
* @brief - Frees all the memory allocated by a queue structure and its nodes If the queue is empty or if it's NULL, this function returns without doing anything. This function iterates through all the nodes of the queue and frees each one of them. Finally, the memory allocated for the queue structure is freed. Note: Make sure to use this function when you're done using the queue structure.

@q: Queue structure to be destroyed

*/
void queue_destroy(Queue q)
{
	if (q == NULL)
		return;

	Node aux1, aux2;

	aux1 = q->first;

	// Iterates through all the nodes of the queue
	while (aux1 != NULL)
	{
		aux2 = aux1->next;
		free(aux1); // Frees each node
		aux1 = aux2;
	}

	free(q); // Frees the memory allocated for the queue structure
}

//-----------------------------------------------------------------
// FUNCIONES DE MARKET

/**
 * @brief Generates a random number of products between 1 and 75.
 * @return int The randomly generated number of products.
 */
int randomProducts()
{
	int num = (rand() % 75) + 1;
	return num;
}

/**
 *@brief randomToCheck - returns a random number within a specified range.If the value of 'max' is less than 15, the function returns a random number * between 1 and 'max'. If the value of 'max' is greater than or equal to 15, the function returns a random number between 5 and 15.
 *@param max: the maximum value that can be returned by the function.
 * @return int A random number within the specified range.
 */
int randomToCheck(int max)
{
	return (rand() % (max < 15 ? max : 11)) + (max < 15 ? 1 : 5);
}

/**
 * findBestCandidate - function that finds the best candidate to join in a market checkout queue
 *
 * @m: pointer to a Market structure
 *
 * Returns: an integer indicating the index of the best candidate to join in the checkout queue
 *
 * The function checks the state of each checkout queue in the market, and based on the conditions,
 * calculates the best candidate to join.
 * If all checkouts are closed, returns 0.
 * If all checkouts are open, returns the index of the checkout with the smallest size or,
 * if all are full, returns the index of the checkout with the lowest amount of products.
 * If some checkouts are closed and others are open, returns the index of the open checkout with
 * the smallest size or, if all are full, returns the first index of a closed checkout.
 * If all conditions are not met, returns -1.
 */
int findBestCandidate(Market m)
{
	int openQtty = 0;
	int fullQtty = 0;
	int lowestSize = -1;
	int indexLowestSize = 0;
	int lowestProductQtty = -1;
	int indexLowestProductQtty = 0;
	Node aux = NULL;

	// Count open and full checkouts
	for (int i = 0; i < 5; i++)
	{
		if (m->checkouts[i])
		{
			openQtty++;
			if (m->checkouts[i]->size >= 3)
				fullQtty++;
		}
	}

	// If all checkouts are closed
	if (openQtty == 0)
	{
		return 0;
	}

	// If all checkouts are open
	if (openQtty == 5)
	{
		// If all open checkouts are not full
		if (fullQtty < 5)
		{
			for (int i = 0; i < 5; i++)
			{
				if (m->checkouts[i]->size < lowestSize || lowestSize == -1)
				{
					lowestSize = m->checkouts[i]->size;
					indexLowestSize = i;
				}
			}
			return indexLowestSize;
		}
		// If all open checkouts are full
		else
		{
			for (int i = 0; i < 5; i++)
			{
				aux = m->checkouts[i]->first;
				int allQueueProducts = 0;
				while (aux)
				{
					allQueueProducts += aux->articles;
					aux = aux->next;
				}
				if (allQueueProducts < lowestProductQtty || lowestProductQtty == -1)
				{
					lowestProductQtty = allQueueProducts;
					indexLowestProductQtty = i;
				}
			}
			return indexLowestProductQtty;
		}
	}

	// If not all checkouts are open or closed
	if (openQtty > 0 && openQtty < 5)
	{
		// If all open checkouts are not full
		if (fullQtty < openQtty)
		{
			for (int i = 0; i < 5; i++)
			{
				if (m->checkouts[i])
				{
					if (m->checkouts[i]->size < lowestSize || lowestSize == -1)
					{
						lowestSize = m->checkouts[i]->size;
						indexLowestSize = i;
					}
				}
			}
			return indexLowestSize;
		}
		// If all open checkouts are full
		else
		{
			for (int i = 0; i < 5; i++)
			{
				if (!m->checkouts[i])
					return i;
			}
		}
	}
	return -1;
}

/**
 * market_print - Prints the information of the market
 * @m: Pointer to the market structure
 *
 * This function prints the information of the market in a table format, including
 * the status of each checkout and the articles in each checkout.
 *
 * Return: None
 */
void market_print(Market m)
{
	printf("CAJA\tSTATUS\tFIRST\tSECOND\tTHIRD\tFOURTH\n");
	for (int i = 0; i < 5; i++)
	{
		printf("C%d\t%d\t", i + 1, m->checkouts[i] != NULL);
		for (Node aux = m->checkouts[i] ? m->checkouts[i]->first : NULL; aux; aux = aux->next)
		{
			printf("%d\t", aux->articles);
		}
		printf("\n");
	}
	printf("\n");
}

/**
 * market_create - creates and initializes a new Market object.
 *
 * @returns: pointer to the newly created Market object.
 */
Market market_create()
{
	srand(time(0));
	Market m = (Market)calloc(1, sizeof(struct strMarket));
	m->checkouts = calloc(5, sizeof(Queue));
	m->firstClient = 1;
	return m;
}

/**
 * market_destroy - Deallocate memory of a Market object
 * @m: Pointer to the Market object
 *
 * Deallocate memory of a Market object, including its Queue objects and its checkouts.
 */
void market_destroy(Market m)
{
	for (int i = 0; i < 5; i++)
	{
		queue_destroy(m->checkouts[i]);
	}
	free(m);
}

/**

market_newClient - adds a new client to the market
@m: pointer to the market
The function finds the best candidate queue for the new client and
creates a new queue if the candidate is not found. The function then
adds the new client to the queue and increments the first client number.
The information about the new client and its queue is printed.
*/
void market_newClient(Market m)
{
	if (!m)
		return;

	// Encontrar mejor queue candidato
	int candidate = findBestCandidate(m);

	if (!m->checkouts[candidate])
	{
		m->checkouts[candidate] = queue_create();
		printf("Se abriO la caja %d\n", candidate + 1);
	}

	int articleQtty = randomProducts();
	queue_offer(m->checkouts[candidate], m->firstClient, articleQtty);
	printf("LlegO el cliente %d en la caja %d con %d artIculos\n", m->checkouts[candidate]->last->clientNumber, candidate + 1, articleQtty);
	m->firstClient++;
}

/**
 * market_checkoutCycle - performs a checkout cycle in a market.
 *
 * @m: a pointer to the Market structure
 *
 * This function performs a checkout cycle in a Market. It iterates through all the checkouts
 * and reduces the number of articles of the first customer in the queue of each checkout, if any.
 * If the customer's number of articles reaches 0, the customer is removed from the queue and a
 * message is displayed indicating that the customer left the market. If the queue is empty, the
 * checkout is destroyed and a message is displayed indicating that the checkout is closed.
 */
void market_checkoutCycle(Market m)
{
	int productsToCheck;
	for (int i = 0; i < 5; i++)
	{
		if (m->checkouts[i])
		{
			productsToCheck = randomToCheck(m->checkouts[i]->first->articles);
			m->checkouts[i]->first->articles -= productsToCheck;
			printf("Se cobraron %d artIculos del cliente %d en la caja %d\n", productsToCheck, m->checkouts[i]->first->clientNumber, i + 1);

			if (m->checkouts[i]->first->articles <= 0)
			{
				printf("Se atendiO al cliente %d y saliO del supermercado.\n", m->checkouts[i]->first->clientNumber);
				queue_poll(m->checkouts[i]);
			}
			if (m->checkouts[i]->size == 0)
			{
				queue_destroy(m->checkouts[i]);
				m->checkouts[i] = NULL;
				printf("Se cerrO la caja %d\n", i + 1);
			}
		}
	}
}
