/// @file   queue.h

#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct queue_node_t
{
    void * data;
    struct queue_node_t * next;
} queue_node_t;

typedef struct queue_t
{
    queue_node_t * front;
    queue_node_t * back;
    size_t data_size;
} * queue_t;

/// @brief Creates a queue.
/// @param data_size The size of the stored elements.
/// @return The created queue.
queue_t queue_create(size_t data_size);

/// @brief Destroys the queue.
/// @param queue The queue to be destroyed.
/// @return If the queue is destroyed.
bool_t queue_destroy(queue_t queue);

/// @brief Returns if the queue is empty.
bool_t queue_is_empty(queue_t queue);

/// @brief Allows to add data to the queue.
/// @param queue The queue.
/// @param data  The data to add.
/// @return If the data has been pushed inside the queue.
bool_t queue_enqueue(queue_t queue, void * data);

/// @brief Removes the first element of the queue.
/// @param queue The queue.
/// @return If the data has been removed.
bool_t queue_dequeue(queue_t queue);

/// @brief Returns the first element of the queue.
/// @param queue    The queue.
/// @param data     The data.
/// @return If the data has been correctly retrieved.
bool_t queue_front(queue_t queue, void * data);

/// @brief Returns the last element of the queue.
/// @param queue    The queue.
/// @param data     The data.
/// @return If the data has been correctly retrieved.
bool_t queue_back(queue_t queue, void * data);

/// @brief Returns the first element of the queue and removes it.
/// @param queue    The queue.
/// @param data     The data.
/// @return If the data has been correctly retrieved.
bool_t queue_front_and_dequeue(queue_t queue, void * data);

/// @brief Deletes all the elements inside the queue.
/// @param queue The queue.
/// @return If the queue has been cleared.
bool_t queue_clear(queue_t queue);
