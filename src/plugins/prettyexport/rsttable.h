/**
 * @file
 *
 * @brief Header for rst table helper functions
 *
 * @copyright BSD License (see LICENSE.md or https://www.libelektra.org)
 *
 */
#include "prettyexport.h"
#include <stdio.h>

enum _PrettyStyle {
	PRETTY_STYLE_NORMAL = 0,
	PRETTY_STYLE_BOLD,
	PRETTY_STYLE_ITALICS,
	PRETTY_STYLE_MONO,
};
typedef enum _PrettyStyle PrettyStyle;

struct _TableCell
{
	PrettyStyle style;
	char * value;
};
typedef struct _TableCell TableCell;

void calcSizes (PrettyHeadNode * head, PrettyIndexType indexType, ssize_t numRows, ssize_t rowHeights[numRows], ssize_t numCols,
		ssize_t colLengths[numCols]);

ssize_t calcTableHeight (ssize_t numRows, ssize_t rowHeights[numRows]);

void callocTable (ssize_t tableLength, ssize_t tableHeight, TableCell * table[tableLength][tableHeight]);
void freeTable (ssize_t tableLength, ssize_t tableHeight, TableCell * table[tableLength][tableHeight]);

void fillTable (PrettyHeadNode * head, PrettyIndexType indexType, ssize_t tableLength, ssize_t tableHeight,
		TableCell * table[tableLength][tableHeight], ssize_t numRows, ssize_t rowHeights[numRows]);
void printTable (FILE * fh, PrettyIndexNode * firstIndexNode, ssize_t tableLength, ssize_t tableHeight,
		 TableCell * table[tableLength][tableHeight], ssize_t numCols, ssize_t colLengths[numCols], ssize_t numRows,
		 ssize_t rowHeights[numRows]);
