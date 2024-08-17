#include <stdio.h>
#include <stdlib.h>
#define ROW 4
#define COL 5

int sum_row(int row[COL]) {
	int sum = 0;
	for (int i = 0; i < COL; i++) {
		sum += row[i];
	}
	return sum;
}

int main(int argc, char **argv) {
	int pad = 0;
	if (argc > 1) {
		pad = atoi(argv[1]);
	}
	int nums[ROW][COL];

	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			nums[i][j] = i + j;
		}
	}

	int sum = pad;
	for (int i = 0; i < ROW; i++) {
		sum += sum_row(nums[i]);
	}

	printf("2D Array Sum: %d\n", sum);

	return EXIT_SUCCESS;
}
