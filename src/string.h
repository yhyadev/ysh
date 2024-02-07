#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
	char *values;
	size_t len;
	size_t capacity;
} String;

String string_init(void);
void string_free(String *string);
void string_reverse(String *string);
void string_trim_left(String *string);
bool string_read_line(String *string, FILE *file);
bool string_equal_cstr(String *string, const char *cstr);
bool string_contains(String *string, char ch);
bool string_is_dir(String *string);
bool string_is_path(String *string);
void string_push(String *string, char ch);
void string_pop(String *string);
String string_pop_word(String *input);

typedef struct {
	String *values;
	size_t len;
	size_t capacity;
} Strings;

Strings strings_init(void);
void strings_free(Strings *strings);
char **strings_to_raw(Strings *strings);
void strings_push(Strings *strings, String string);
void strings_pop(Strings *strings);

Strings string_tokenize_words(String *input);
