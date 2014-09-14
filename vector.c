#include "vector.h"
#include <stdlib.h>

Vector Vector_Create()
{
    Vector v;
    v.data = NULL;
    v.count = 0;
    v.size = 0;

    return v;
}


int Vector_Count(Vector *v)
{
	return v->count;
}

void Vector_Push(Vector *v, void *e)
{
    if(e != NULL)
    {
        if (v->size == 0)
        {
            v->size = 20;
            v->data = malloc(sizeof(void*) * v->size);
            memset(v->data, NULL, sizeof(void*) * v->size);
        }

        if(v->data == NULL)
        {
            printf("Vector_Push - Error after malloc and memset of vector");
            return;
        }
        if (v->size == v->count)
        {
            v->size *= 2;
            v->data = realloc(v->data, sizeof(void*) * v->size);
        }

        if(v->data == NULL)
        {
            printf("Vector_Push - Error after realloc of vector");
            return;
        }

        v->data[v->count] = e;
        v->count++;
    }
}


void* Vector_Get(Vector *v, int index)
{
	if (index >= v->count)
    {
        printf("Index too high");
		return;
	}

	return v->data[index];
}

void Vector_Delete(Vector *v, int index)
{
	if (index >= v->count)
    {
        printf("Index too high");
		return;
	}

    int destination = index;

    for(int index_to_copy = index + 1; index_to_copy < v->count ; index_to_copy++)
    {
        if(v->data[index_to_copy] != NULL)
        {
            v->data[destination] = v->data[index_to_copy];
            destination++;
        }
    }

    printf("deleting, size = %d\n", v->size);

	v->data[v->count] = NULL;
	v->count--;
}