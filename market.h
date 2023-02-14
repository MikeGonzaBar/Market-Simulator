#ifndef MARKET_H_
#define MARKET_H_

typedef enum
{
  FALSE,
  TRUE
} bool;

// definiciones
typedef struct strMarket *Market;

// prototipos de Market
void market_print(Market m);
Market market_create();
void market_destroy(Market m);
void market_newClient(Market m);
void market_checkoutCycle(Market m);

#endif