/// @file   queue.c

#include <queue.h>
#include <kheap.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

queue_t queue_create(size_t data_size, size_t capacity)
{
    queue_t queue;
    queue = kmalloc(sizeof(struct queue_t));
    if (queue == NULL)
    {
        printf("Cannot create the queue.\n");
        return NULL;
    }
    queue->data_size = data_size;
    queue->capacity = capacity;
    queue->data = calloc(capacity, data_size);
    if (queue->data == NULL)
    {
        printf("Cannot initialize the queue data.\n");
        return NULL;
    }
    queue->head = 1;
    queue->tail = 0;
    queue->size = 0;
    return queue;
}

bool_t queue_destroy(queue_t queue)
{
    if (queue == NULL)
    {
        printf("Queue is NULL.\n");
        return false;
    }
    queue_clear(queue);
    free(queue->data);
    free(queue);
    return true;
}

size_t queue_next(queue_t queue, size_t index)
{
    if (++index == queue->capacity)
    {
        return 0;
    }
    return index;
}

bool_t queue_is_empty(queue_t queue)
{
    if (queue == NULL)
    {
        printf("Queue is NULL.\n");
        return false;
    }
    return (bool_t) (queue->size == 0);
}

bool_t queue_is_full(queue_t queue)
{
    if (queue == NULL)
    {
        printf("Queue is NULL.\n");
        return false;
    }
    return (bool_t) (queue->size == queue->capacity);
}

bool_t queue_enqueue(queue_t queue, const void * data)
{
    if (queue == NULL)
    {
        printf("Queue is NULL.\n");
        return false;
    }
    if (queue_is_full(queue))
    {
        printf("The queue is full.\n");
        return false;
    }
    queue->tail = queue_next(queue, queue->tail);
    void * element = ((char *) queue->data) +
                     queue->tail * queue->data_size;
    memcpy(element, data, queue->data_size);
    queue->size++;
    return true;
}

bool_t queue_dequeue(queue_t queue)
{
    if (queue_is_empty(queue))
    {
        printf("The queue is empty.\n");
        return false;
    }
    void * element = (char *) queue->data +
                     queue->head * queue->data_size;
    free(element);
    queue->head = queue_next(queue, queue->head);
    queue->size--;
    return true;
}

bool_t queue_front(queue_t queue, void * data)
{
    if (queue == NULL)
    {
        printf("Queue is NULL.\n");
        return false;
    }
    void * element = (char *) queue->data + queue->head * queue->data_size;
    memcpy(data, element, queue->data_size);
    return true;
}

bool_t queue_clear(queue_t queue)
{
    if (queue == NULL)
    {
        printf("Queue is NULL.\n");
        return false;
    }
    if (queue->size == 0)
    {
        return true;
    }
    size_t it = 0;
    for (; it < queue->size; ++it)
    {
        void * element = (char *) queue->data + it * queue->data_size;
        free(element);
    }
    queue->size = 0;
    queue->head = 1;
    queue->tail = 0;
    return true;
}
