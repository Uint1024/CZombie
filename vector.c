#include "vector.h"
#include <stdlib.h>
#include "entity.h"

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
    //printf("push\n");
    Entity* to_push = (Entity*)e;
    Zombie_Type type = to_push->zombie_type;



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
            printf("Vector_Push - Error after malloc and memset");
            return;
        }
        if (v->size == v->count)
        {
            v->size *= 2;
            v->data = realloc(v->data, sizeof(void*) * v->size);
        }

        if(v->data == NULL)
        {
            printf("Vector_Push - Error after realloc");
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

void Vector_Clear(Vector* v)
{
    for(int i = 0 ; i < v->count ; i++)
    {
        free(v->data[i]);
    }
    free(v->data);
    v->size = 0;
    v->count = 0;
    v->data = NULL;
}
void Vector_Delete(Vector *v, int index)
{

    Entity* to_delete = (Entity*)Vector_Get(v, index);
    Zombie_Type type = to_delete->zombie_type;

	if (index >= v->count)
    {
        printf("Index too high");
		return;
	}



    //int destination = index;

    if(index + 1 < v->count)
    {
        for(int index_to_copy = index + 1; index_to_copy < v->count ; index_to_copy++)
        {
            if(v->data[index_to_copy] != NULL)
            {
                v->data[index_to_copy - 1] = v->data[index_to_copy];
                //destination++;
            }
            else
            {
                printf("Error during vector delete : trying to move null pointer");
            }
        }

    }


    v->data[v->count-1] = NULL;
    free(to_delete);

	v->count--;


    if(v->count == 0 && v->size > 20)
    {
            v->size = 0;
            v->data = realloc(v->data, sizeof(void*) * v->size);
    }



}
