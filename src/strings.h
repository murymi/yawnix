#pragma once
#include <ctype.h>
#include <stdint.h>
#include <math.h>

void int_to_string(char *buf, long value);
void uint_to_string(char *buf, unsigned long value);
void int_to_string_hex(char *buf, long value);
void uint_to_string_hex(char *buf, unsigned long value);

