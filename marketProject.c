#include <stdio.h>
#include <stdlib.h>
#include "market.h"


int main(){

	Market ourMarket = market_create();

	int iterations;

	scanf("%d", &iterations);

	while(iterations != 0){
		for(int i=0; i<iterations; i++){
			market_newClient(ourMarket);
			market_print(ourMarket);
			market_checkoutCycle(ourMarket);
			market_print(ourMarket);
			market_checkoutCycle(ourMarket);
			market_print(ourMarket);
			printf("\n");
		}
		scanf("%d", &iterations);
	}

	market_destroy(ourMarket);
}