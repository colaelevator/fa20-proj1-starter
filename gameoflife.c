/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	Color* newColor = (Color*)malloc(sizeof(Color));
	int rows = image->rows;
	int cols = image->cols;
	int activeNeighbours = 0;

	int neighbours[8][2] = {
		{(row - 1 + rows) % rows, (col - 1 + cols) % cols},
		{(row - 1 + rows) % rows, col},
		{(row - 1 + rows) % rows, (col + 1) % cols},
		{row, (col - 1 + cols) % cols},
		{row, (col + 1) % cols},
		{(row + 1) % rows, (col - 1 + cols) % cols},
		{(row + 1) % rows, col},
		{(row + 1) % rows, (col + 1) % cols}
	};

	for (int i = 0; i < 8; i++)
	{
		int neighbour_row = neighbours[i][0];
		int neighbour_col = neighbours[i][1];

		if (image->image[neighbour_row][neighbour_col].R == 255)
		{
			activeNeighbours++;
		}
	}

	int current_state = (image->image[row][col].R == 255) ? 1 : 0;
	int new_state = (rule >> (current_state * 9 + activeNeighbours)) & 1;

	if (new_state == 1)
	{
		newColor->R = newColor->B = newColor->G = 255;
	} else
	{
		newColor->R = newColor->G = newColor->B = 0;
	}

	return newColor;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image *newImage = (Image*)malloc(sizeof(Image));
	newImage->rows = image->rows;
	newImage->cols = image->cols;
	newImage->image = (Color**)malloc(sizeof(Color*) * newImage->rows);
	for (int i = 0; i < newImage->rows; i++)
	{
		newImage->image[i] = (Color*)malloc(sizeof(Color) * newImage->cols);
	}

	for (int i = 0; i < newImage->rows; i++)
	{
		for (int j = 0; j < newImage->cols; j++)
		{
			Color *ij = evaluateOneCell(image, i, j, rule);
			newImage->image[i][j] = *ij;
			free(ij);
		}
	}
	return newImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if (argc <= 2)
	{
		printf("usage: ./gameOfLife filename rule\nfilename is an ASCII PPM file (type P3) with maximum value 255.\nrule is a hex number beginning with 0x; Life is 0x1808.");
		return -1;
	}

	char *filename = argv[1];
	char *rule = argv[2];

	char *endptr;
	uint32_t rule_value = strtol(rule, &endptr, 16);
	if (*endptr != '\0' || rule_value < 0x00000 || rule_value > 0x3FFFF)
	{
		return -1;
	}
	Image* image = readData(filename);
	Image *newImage = life(image, rule_value);
	writeData(newImage);
	freeImage(image);
	freeImage(newImage);
	return 0;
}