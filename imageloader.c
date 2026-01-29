/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	FILE *fp = fopen(filename, "rb");
	if(!fp) {
		printf("Could not open file %s\n", filename);
	}
	char buffer[256];
	fscanf(fp, "%s", buffer);
	uint32_t rows;
	uint32_t cols;
	fscanf(fp, "%u %u", &rows, &cols);
	uint32_t range;
	fscanf(fp, "%u", &range);
	Image *image = (Image*)malloc(sizeof(Image));
	if (!image) {
		printf("Could not allocate image\n");
		return NULL;
	}
	image->rows = rows;
	image->cols = cols;
	image->image = (Color**)malloc(sizeof(Color*)*rows);
	for (int i = 0; i < rows; i++)
	{
		image->image[i] = (Color*)malloc(sizeof(Color)*cols);
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			fscanf(fp, "%hhu %hhu %hhu", &image->image[i][j].R, &image->image[i][j].G, &image->image[i][j].B);
		}
	}
	fclose(fp);
	return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	printf("P3\n");
	printf("%u %u\n", image->rows, image->cols);
	printf("255\n");
	for (int i = 0; i < image->rows; i++)
	{
		for (int j = 0; j < image->cols; j++)
		{
			printf("%3d %3d %3d",
				image->image[i][j].R,
				image->image[i][j].G,
				image->image[i][j].B
				);
			if (j < image->cols - 1)
			{
				printf("   ");
			}
		}
		printf("\n");
	}
}

//Frees an image
void freeImage(Image *image)
{
	for (int i = 0; i < image->rows; i++)
	{
		free(image->image[i]);
	}

	free(image->image);

	free(image);
}