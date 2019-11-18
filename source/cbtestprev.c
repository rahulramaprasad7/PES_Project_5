#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "circularBuffer.h"



int maini(void)
{

    circularBuf *txBuf = malloc(sizeof(circularBuf));
    if (txBuf == NULL)
        return 1;
    txBuf->charArray = NULL;

    txBuf->length = 8;
    txBuf->count = 0;
    txBuf->charArray = malloc(sizeof(txBuf->length * sizeof(char)));
    if (txBuf->charArray == NULL) {
        free(txBuf);
        txBuf = NULL;
        return 1;
    }

    txBuf->head = 0;
    txBuf->tail = 0;

    printf("%p %lu %lu %lu %lu\n", txBuf->charArray, txBuf->head, txBuf->tail, txBuf->length, txBuf->count);

    int i = 0;
    while (addElement(txBuf, i) != failure) {
        printf("Added\n");
        i++;
    }
    i = 0;

    printf("Buffer:\n");
    for (int i = 0; i < txBuf->length; ++i)
    {
        printf("%d: %d\n", i, txBuf->charArray[i]);
    }

    uint8_t c;
    while ((c = delElement(txBuf)) != 0xFE) {
        printf("Head: %lu Tail: %lu\n", txBuf->head, txBuf->tail);
        printf("Delete: %d\n", c);
    }

    i = 0;

    addElement(txBuf, 98);
    addElement(txBuf, 99);

    delAllElements(txBuf);

    for (int i = 0; i < txBuf->length; ++i)
    {
        txBuf->charArray[i] = 0;
    }
    emptyBuffer(txBuf);

    while (1) {

        uint8_t temp;
        scanf("%c", &temp);
        if (temp == '.') {
            printf(". detected, dumping all elements\n");

            delAllElements(txBuf);
            continue;
        }

        if (addElement(txBuf, temp) == failure) {

            printf("Buffer Full, realloc to %lu\n", txBuf->length * 2);
            txBuf->length *= 2;
            char *bufTemp = txBuf->charArray;

            txBuf->charArray = realloc(txBuf->charArray, sizeof(txBuf->length * sizeof(char)));

            if (txBuf->charArray == NULL) {
                printf("Realloc failed\nDumping all elements");
                txBuf->charArray = bufTemp;

                delAllElements(txBuf);
            }

            if (txBuf->tail == txBuf->head) {
                printf("Buffer Wrapped, moving elements\n");
                adjustElements(txBuf);
            }

            if (addElement(txBuf, temp) == failure)
                return 1;
        }
    }
    return 0;
}
