#include <signal.h>
#include <stdio.h>
struct two_int { int a, b; } data;
void signal_handler(int signum){
	printf ("%d, %d\n", data.a, data.b);
	alarm (1);
}

void signal_handler1(int signum){
	printf ("received CTRL+C\n");
}

int main (void){
	static struct two_int zeros = { 0, 0 }, ones = { 1, 1 };
	signal (SIGALRM, signal_handler);
	signal (9, signal_handler1);
	data = zeros;
	alarm (1);
	while (1)
	{data = zeros; data = ones;}
}

