#include <stdint.h>
#include <stdio.h>

// FORTH Main
enum forth_data_type{
	Integer,
	Float,
	String,
};


struct forth_data {
	enum forth_data_type type;
	union {
		int integer;
		float floating;
		char* string;
	} data;
};

int top_stack = 0;
struct forth_data data_stack[100];


// stack base operations

void push_integer(int value) {
	data_stack[top_stack] = (struct forth_data){ .data.integer = value, .type = Integer };
	top_stack++;
	puts("push_integer");
}

void push_string(char* string) {
	data_stack[top_stack] = (struct forth_data){ .data.string = string, .type = String };
	top_stack++;
	puts("push_string");
}

void push_float(float value) {
	data_stack[top_stack] = (struct forth_data){ .data.floating = value, .type = Float };
	top_stack++;
	puts("push_float");
}

void push_data(struct forth_data value) {
	data_stack[top_stack] = value;
	top_stack++;
	puts("push_data");
}

void drop(void) {
	top_stack--;
	if (top_stack < 0) {
		puts("runtime error: stack size < 0 :(");
	}
}

struct forth_data drop_ret(void) {
	top_stack--;
	if (top_stack < 0) {
		puts("runtime error: stack size < 0 :(");
	}
	return data_stack[top_stack];
}

void dup(void) {
	struct forth_data to_copy = drop_ret();
	push_data(to_copy);
	push_data(to_copy);
	puts("dup void");
}

void print(void) {
	struct forth_data to_copy = drop_ret();
	if (to_copy.type == Integer) {
		printf("%d", to_copy.data.integer);
	}

	if (to_copy.type == Float) {
		printf("%f", to_copy.data.floating);
	}

	if (to_copy.type == String) {
		printf("%s", to_copy.data.string);
	}
}

void swap(void) {
}

void mult(void) {
	struct forth_data first = drop_ret();
	struct forth_data second = drop_ret();

	if (first.type == String || second.type == String) {
		puts("runtime error: string unsupport * operator");
		return;
	}

	struct forth_data result = (struct forth_data){ .type = Integer };

	if (first.type == Float || second.type == Float) {
		result.type = Float;
	}

	if (first.type == Float && second.type == Float) {
		result.data.floating = first.data.floating * second.data.floating;

	} else if (first.type == Float && second.type == Integer) {
		result.data.floating = first.data.floating * second.data.integer;

	} else if (first.type == Integer && second.type == Float) {
		result.data.floating = first.data.integer * second.data.floating;

	} else if (first.type == Integer && second.type == Integer) {
		result.data.floating = first.data.integer * second.data.integer;
	}

	push_data(result);
}