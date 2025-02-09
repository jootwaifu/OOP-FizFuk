#include <string.h>
#include <assert.h>
#include <math.h>

#include "darray.h"

typedef struct {
    int array[8];
    float d_variable;
} Value;

int main(int argc, char* argv[])
{
    //Создаем динамический массив с элементами типа Value;
    void* darray = darray_create(sizeof(Value));

    assert(0 == darray_count(darray));
    assert(darray_stop(darray) == darray_first(darray));

    //Создаем объект для динамического массива
    Value value = { {1, 2, 3, 4, 5, 6, 7, 8}, 0.f };

    //Добавляем новый элемент в динамический массив
    Value* insertedValue = (Value*)darray_add(darray);

    //Инициализируем добавленный элемент
    *insertedValue = value;

    Value* item = (Value*)darray_item(darray, 0);

    for (size_t i = 0; 8 > i; ++i) {
        assert(item->array[i] == value.array[i]);
    }

    assert(fabsf(item->d_variable - value.d_variable) < 1e-10f);
    assert(NULL == darray_item(darray, 1));

    assert(darray_next(darray, darray_first(darray)) == darray_stop(darray));

    darray_destroy(darray, NULL);

    return 0;
}