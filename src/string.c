#include <ctype.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>

#include "string.h"

String string_init() {
	char *values = malloc(sizeof(char));

	return (String){.values = values, .len = 0, .capacity = 1};
}

void string_free(String *string) {
	free(string->values);

	string->len = 0;
	string->capacity = 1;
}

void string_reverse(String *string) {
	int start = 0;
	int end = string->len - 1;

	while (start < end) {
		char temp = string->values[start];

		string->values[start] = string->values[end];
		string->values[end] = temp;

		start++;
		end--;
	}
}

void string_trim_left(String *string) {
	string_reverse(string);

	while (string->len > 0) {
		if (!isspace(string->values[string->len - 1])) {
			break;
		}

		string_pop(string);
	}

	string_reverse(string);
}

bool string_equal_cstr(String *string, const char *cstr) {
	if (string->len != strlen(cstr))
		return false;

	for (size_t i = 0; i < string->len; i++) {
		if (string->values[i] != cstr[i])
			return false;
	}

	return true;
}

void string_push(String *string, char ch) {
	string->values[string->len++] = ch;

	if (string->len == string->capacity) {
		string->capacity *= 2;

		string->values =
			realloc(string->values, sizeof(char) * string->capacity);
	}
}

void string_pop(String *string) {
	if (string->len > 0) {
		string->len--;
	}
}

String string_pop_word(String *input) {
	String word = string_init();

	string_reverse(input);

	while (true) {
		if (input->len == 0) {
			break;
		}

		if (input->values[input->len - 1] == ' ') {
			string_pop(input);

			break;
		}

		string_push(&word, input->values[input->len - 1]);

		string_pop(input);
	}

	string_reverse(input);

	return word;
}

Strings string_tokenize_words(String *input) {
	Strings words = strings_init();

	while (true) {
		String word = string_pop_word(input);

		if (word.len == 0) {
			break;
		}

		string_push(&word, '\0');

		strings_push(&words, word);
	}

	return words;
}

Strings strings_init(void) {
	String *values = malloc(sizeof(String));

	return (Strings){.values = values, .len = 0, .capacity = 1};
}

void strings_free(Strings *strings) {
	for (size_t i = 0; i < strings->len; i++) {
		string_free(&strings->values[i]);
	}

	free(strings->values);

	strings->len = 0;
	strings->capacity = 1;
}

char **strings_to_raw(Strings *strings) {
	char **raw_strings = malloc(sizeof(char *) * strings->len);

	for (size_t i = 0; i < strings->len; i++) {
		raw_strings[i] = strings->values[i].values;
	}

	return raw_strings;
}

void strings_push(Strings *strings, String string) {
	strings->values[strings->len++] = string;

	if (strings->len == strings->capacity) {
		strings->capacity *= 2;

		strings->values =
			realloc(strings->values, sizeof(char) * strings->capacity);
	}
}

void strings_pop(Strings *strings) {
	if (strings->len > 0) {
		strings->len--;
	}
}
