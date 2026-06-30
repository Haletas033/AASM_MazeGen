#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CEIL_DIV(A,B) ((A + B - 1) / B)

unsigned char grid_size = 0;
unsigned short *cell_data = NULL;

void wait_for_sentinel(void);
void draw_top_line(FILE* file, unsigned short position);
void draw_right_line(FILE* file, unsigned short position);

int main(void) {
	// Read the data
	char line[1024];

	// Remove any text before data
	wait_for_sentinel();
	printf("Generating...\n");

	wait_for_sentinel();
	printf("Receiving data...\n");

	fgets(line, sizeof(line), stdin);
	grid_size = (char)atoi(line);

	const unsigned short cell_chunk_count = CEIL_DIV(grid_size*grid_size,5);

	// Allocate enough space for cell data
	cell_data = malloc(cell_chunk_count * sizeof *cell_data);
	if (!cell_data) {
		return -1;
	}

	// Load output into cell data
	unsigned short i = 0;
	while (fgets(line, sizeof(line), stdin) != NULL) {
		// Remove \n
		line[strcspn(line, "\n")] = '\0';
		if (strcmp(line, "65535") == 0 || i >= cell_chunk_count) { break; }
		cell_data[i] = atoi(line);
		++i;
	}

	printf("Data received. Drawing maze\n");

	fflush(stdout);

	// Draw the maze
	FILE* maze_svg = fopen("maze.svg", "w");
	if (!maze_svg) { goto FILE_ERROR; }

	// Header
	fprintf(maze_svg, "<svg width=\"1000\" height=\"1000\" viewBox=\"-20 -20 1040 1040\">");

		// Main data
		int done = 0;
		for (i = 0; i < cell_chunk_count && !done; ++i) {
			// Read all cells in chunk

			const unsigned short cell_chunk = cell_data[i];
			for (unsigned char slot = 0; slot < 5; ++slot) {
				const unsigned short shifted = cell_chunk >> (13 - slot*3);
				const unsigned short cell = shifted & 0b111;
				if (!(cell >> 2 & 1)) { done = 1; break; } // Avoid reading extra cells
				if (!(cell >> 1 & 1)) { draw_top_line(maze_svg, i*5+slot); }
				if (!(cell & 1)) { draw_right_line(maze_svg, i*5+slot); }
			}
		}
	// Add bottom and left edge
	const unsigned short cell_size = 1000/grid_size;
	const unsigned int true_size = cell_size*grid_size; // Account for integer division error
	fprintf(maze_svg, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\"/>", 0, true_size, true_size-cell_size /* leave gap at bottom right*/, true_size); // Bottom
	fprintf(maze_svg, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\"/>", 0, 0, 0, true_size); // Left


	// Closing tag
	fprintf(maze_svg, "</svg>");
	fclose(maze_svg);

	// Cleanup
	free(cell_data);

	// Present the maze
#ifdef _WIN32
	system("start maze.svg");
#else
	system("xdg-open maze.svg");
#endif

	return 0;

FILE_ERROR:
	fprintf(stderr, "Failed to create maze.svg\n");
	free(cell_data);
	return -1;
}

void wait_for_sentinel(void) {
	char line[1024];
	while (fgets(line, sizeof(line), stdin) != NULL) {
		line[strcspn(line, "\n")] = '\0';
		if (strcmp(line, "65535") == 0) { break; }
	}
}

void draw_top_line(FILE* file, const unsigned short position) {
	if (position == 0) { return; } // Skip drawing top wall for position 0
	const unsigned short cell_size = 1000/grid_size;
	const unsigned short x_start = cell_size * (position%grid_size);
	const unsigned short x_end = cell_size * (position%grid_size+1);
	const unsigned short y = cell_size * (position/grid_size);

	fprintf(file, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\"/>", x_start, y, x_end, y);
}

void draw_right_line(FILE* file, const unsigned short position) {
	const unsigned short cell_size = 1000/grid_size;
	const unsigned short y_end = cell_size * (position/grid_size+1);
	const unsigned short y_start = cell_size * (position/grid_size);
	const unsigned short x = cell_size * (position%grid_size+1);

	fprintf(file, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\"/>", x, y_start, x, y_end);
}