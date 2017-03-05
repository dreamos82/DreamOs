/// @file   compiler.h

#pragma once

#define WRITE_ONCE(var, val) (*((volatile typeof(val) *)(&(var))) = (val))

#define READ_ONCE(var) (*((volatile typeof(var) *)(&(var))))