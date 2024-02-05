#pragma once

#include <stddef.h>

typedef struct {
	char *values;
	size_t len;
	size_t capacity;
} String;

String string_init(void);
void string_free(String *string);
void string_reverse(String *string);
void string_trim_left(String *string);
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