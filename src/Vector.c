/**
 * @file Vector.cpp
 */

#include "cgds/Vector.h"

//////////////////
// Vector logic //
//////////////////

void _vector_init(Vector* vector, size_t dataSize)
{
	vector->datas = NULL;
	vector->dataSize = dataSize;
	vector->size = 0;
	vector->capacity = 0;
}

Vector* _vector_new(size_t dataSize)
{
	Vector* vector = (Vector*) safe_malloc(sizeof (Vector));
	_vector_init(vector, dataSize);
	return vector;
}

Vector* vector_copy(Vector* vector)
{
	Vector* vectorCopy = _vector_new(vector->dataSize);
	vectorCopy->size = vector->size;
	vectorCopy->capacity = vector->capacity;
	vectorCopy->datas = (void**) safe_malloc(vector->capacity * sizeof (void*));
	for (UInt i = 0; i < vector->size; i++)
	{
		vectorCopy->datas[i] = safe_malloc(vector->dataSize);
		memcpy(vectorCopy->datas[i], vector->datas[i], vector->dataSize);
	}
	return vectorCopy;
}

Bool vector_empty(Vector* vector)
{
	return (vector->size == 0);
}

UInt vector_size(Vector* vector)
{
	return vector->size;
}

void _vector_realloc(Vector* vector, UInt newCapacity)
{
	void** rellocatedDatas = (void**) safe_malloc(newCapacity * sizeof (void*));
	for (UInt i = 0; i < vector->size; i++)
	{
		rellocatedDatas[i] = (void*) safe_malloc(vector->dataSize);
		memcpy(rellocatedDatas[i], vector->datas[i], vector->dataSize);
		safe_free(vector->datas[i]);
	}
	safe_free(vector->datas);
	vector->datas = rellocatedDatas;
	vector->capacity = newCapacity;
}

void _vector_push(Vector* vector, void* data)
{
	void* pData = safe_malloc(vector->dataSize);
	memcpy(pData, data, vector->dataSize);
	if (vector->size >= vector->capacity)
	{
		UInt increasedCapacity = vector->capacity > 0 ? 2 * vector->capacity : 1;
		_vector_realloc(vector, increasedCapacity);
	}
	vector->datas[vector->size] = pData;
	vector->size++;
}

void vector_pop(Vector* vector)
{
	safe_free(vector->datas[vector_size(vector) - 1]);
	vector->size--;
	if (vector_size(vector) <= (vector->capacity >> 1))
		_vector_realloc(vector, vector->capacity >> 1);
}

void* _vector_get(Vector* vector, UInt index)
{
	return vector->datas[index];
}

void _vector_set(Vector* vector, UInt index, void* data)
{
	memcpy(vector->datas[index], data, vector->dataSize);
}

void vector_clear(Vector* vector)
{
	for (UInt i = 0; i < vector->size; i++)
		safe_free(vector->datas[i]);
	safe_free(vector->datas);
	_vector_init(vector, vector->dataSize);
}

void vector_destroy(Vector* vector)
{
	vector_clear(vector);
	safe_free(vector);
}

////////////////////
// Iterator logic //
////////////////////

VectorIterator* vector_get_iterator(Vector* vector)
{
	VectorIterator* vectorI = (VectorIterator*) safe_malloc(sizeof (VectorIterator));
	vectorI->vector = vector;
	vectorI_reset_begin(vectorI);
	return vectorI;
}

void vectorI_reset_begin(VectorIterator* vectorI)
{
	vectorI->current = vectorI->vector->datas;
}

void vectorI_reset_end(VectorIterator* vectorI)
{
	vectorI->current = vectorI->vector->datas + vectorI->vector->size - 1;
}

Bool vectorI_has_data(VectorIterator* vectorI)
{
	return (vectorI->current >= vectorI->vector->datas &&
		vectorI->current < vectorI->vector->datas + vectorI->vector->size);
}

void* _vectorI_get(VectorIterator* vectorI)
{
	return *(vectorI->current);
}

void _vectorI_set(VectorIterator* vectorI, void* data)
{
	*(vectorI->current) = data;
}

void vectorI_move_next(VectorIterator* vectorI)
{
	vectorI->current++;
}

void vectorI_move_prev(VectorIterator* vectorI)
{
	vectorI->current--;
}

void vectorI_destroy(VectorIterator* vectorI)
{
	safe_free(vectorI);
}
