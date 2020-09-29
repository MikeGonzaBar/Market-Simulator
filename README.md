# Market-Simulator
This a simulation of how a checkout system would work

The objective of this project is to emulate a checkout system in the supermarket. The maximum number of checkouts available is 5 and these will be opened and closed as the demand of customers who are going to pay for their items increases or decreases.
The program will distribute the clients in the different checkouts in an equitable way; a new checkout will be open if all open boxes have 3 customers in queue.
Each customer will have a random number of items that can range from 1 to 75.
The program will be controlled by an endless cycle; This cycle will determine if a customer is served or if a new one is formed.
1 cycle of training should be rotated by 2 cycles of attention. In each service cycle, the cashier may dispatch from 5 to 15 items (random), unless the items to be dispatched from the customer served are less than those that would be dispatched; in which case only the total items pending from the customer are dispatched.
In each dispatch cycle, customers are served in parallel from all open boxes.
