/// @file   queue.c

#include <queue.h>
#include <kheap.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <debug.h>

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
    dbg_print("Queue starts at    : %p [%ld]\n", queue->data, queue->data);
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
    kfree(queue->data);
    kfree(queue);
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
    if (element == NULL)
    {
        printf("Failed to get a valid position inside the queue.\n");
        return false;
    }
    dbg_print("Enqueue element at : %p [%ld]\n", element, element);
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
    if (element == NULL)
    {
        printf("Failed to get a valid position inside the queue.\n");
        return false;
    }
    dbg_print("Dequeue element at : %p [%ld]\n", element, element);
    kfree(element);
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
    if (element == NULL)
    {
        printf("Failed to get a valid position inside the queue.\n");
        return false;
    }
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
        if (element == NULL)
        {
            printf("Failed to get a valid position inside the queue.\n");
            return false;
        }
        kfree(element);
    }
    queue->size = 0;
    queue->head = 1;
    queue->tail = 0;
    return true;
}
