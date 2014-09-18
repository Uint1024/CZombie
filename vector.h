#ifndef VECTOR_H
#define VECTOR_H

typedef struct Vector
{
    void**  data;
    int     size;
    int     count;
}Vector;

Vector Vector_Create();
int Vector_Count(Vector *v);
void Vector_Push(Vector *v, void *e);
void* Vector_Get(Vector *v, int index);
void Vector_Delete(Vector *v, int index);
void Vector_Clear(Vector* v);
#endif // VECTOR_H
