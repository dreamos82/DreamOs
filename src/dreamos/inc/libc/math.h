/// @file   math.h

#pragma once

/// @brief The absolute value.
#define abs(a) (((a) < 0)?-(a):(a))
/// @brief The max of the two values.
#define max(a, b) (((a) > (b)) ? (a) : (b))
/// @brief The min of the two values.
#define min(a, b) (((a) < (b)) ? (a) : (b))
/// @brief The sign the the passed value.
#define sign(x) ((x < 0) ? -1 :((x > 0) ? 1 : 0))
