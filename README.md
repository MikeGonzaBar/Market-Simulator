# Market-Simulator

Co-developed with Pedro García Romero

This is a simulation of a market where clients can purchase articles from multiple checkouts. This simulation uses a linked list data structure (queue) to keep track of the clients in line and the number of articles they want to purchase.

## Usage

The simulation uses the following functions:

queue_create: Creates a new queue data structure and allocates memory for it.
queue_isEmpty: Check if the queue is empty.
queue_offer: Adds a new client to the end of the queue.
queue_poll: Removes the first element from the queue.
queue_destroy: Frees all the memory allocated by the queue structure and its nodes.
randomProducts: Generates a random number of products between 1 and 75.
market_create: Creates a new market simulation data structure and allocates memory for it.
market_simulate: Simulates the market, processing clients and checkouts.
market_destroy: Frees all the memory allocated by the market simulation data structure.
