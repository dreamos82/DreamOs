/// @file   shell_history.h
/// @date   apr 01 2017

#pragma once

#include "shell.h"

/// @brief Initialize the init.
void history_init(void);

/// @brief Push the command inside the history.
void history_push(char * command);

/// @brief Give the key allows to navigate through the history.
void history_start(const int key);

/// @brief Prints the history.
void history_print(int argc, char ** argv);
