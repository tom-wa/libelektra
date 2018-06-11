/**
 * @file
 *
 * @brief helper function for printing rst tables
 *
 * @copyright BSD License (see LICENSE.md or https://www.libelektra.org)
 *
 */

#include "prettyexport.h"
#include "rsttable.h"

#include <kdb.h>
#include <kdbease.h> // elektraArrayValidateName, elektraKeyGetRelativeName
#include <kdbhelper.h>
#include <stdio.h>
#include <stdlib.h>

static ssize_t calcTableCellLength (Key * key)
{
	ssize_t len = keyGetValueSize (key);
	if (keyGetMeta (key, "pretty/bold"))
		len += 4;
	else if (keyGetMeta (key, "pretty/italics"))
		len += 2;
	else if (keyGetMeta (key, "pretty/mono"))
		len += 4;
	return len;
}

static ssize_t calcTableCellHeight (Key * key)
{
	const char * value = keyString (key);

	ssize_t numberOfLines = 1;
	while (*value != '\0')
	{
		if (*value == '\n') numberOfLines++;
		value++;
	}

	return numberOfLines;
}

void calcSizes (PrettyHeadNode * head, PrettyIndexType indexType, 
        ssize_t numRows, ssize_t rowHeights[numRows], 
        ssize_t numCols, ssize_t colLengths[numCols])
{
	int rowCount = 0;
	int colCount = 1;

	ksRewind (head->nodes);
	Key * cur;
	while ((cur = ksNext (head->nodes)) != NULL)
	{
		PrettyIndexNode * node = *(PrettyIndexNode **) keyValue (cur);
		ssize_t indexCellLength = 0;
		if (indexType == PRETTY_INDEX_NAME)
		{
			indexCellLength = elektraStrLen (keyBaseName (node->key));
		}
		else
		{
			indexCellLength = keyGetValueSize (node->key);
		}

		if (keyGetMeta (node->key, "pretty/bold"))
			indexCellLength += 4;
		else if (keyGetMeta (node->key, "pretty/italics"))
			indexCellLength += 2;
		else if (keyGetMeta (node->key, "pretty/mono"))
			indexCellLength += 4;


		if (colLengths[0] < indexCellLength) colLengths[0] = indexCellLength;
		ksRewind (node->ordered);
		Key * cur2;
		while ((cur2 = ksNext (node->ordered)) != NULL)
		{
			ssize_t cellLength = calcTableCellLength (cur2);
			if (colLengths[colCount] < cellLength) colLengths[colCount] = cellLength;

			ssize_t cellHeight = calcTableCellHeight (cur2);
			if (rowHeights[rowCount] < cellHeight) rowHeights[rowCount] = cellHeight;

			colCount++;

			// TODO check size of colCout < numCols
		}

		colCount = 1;
		rowCount++;

		// TODO check size rowCount < numRows
	}
}

ssize_t calcTableLength (ssize_t numCols, ssize_t colLengths[numCols])
{
	return numCols;
}

ssize_t calcTableHeight (ssize_t numRows, ssize_t rowHeights[numRows])
{
	ssize_t tableHeight = 0;
	for (int i = 0; i < numRows; i++)
	{
		tableHeight += rowHeights[i];
	}

	return tableHeight;
}

void callocTable (ssize_t tableLength, ssize_t tableHeight, TableCell * table[tableLength][tableHeight])
{
	for (int i = 0; i < tableLength; ++i)
	{
		for (int j = 0; j < tableHeight; ++j)
		{
			table[i][j] = elektraCalloc (sizeof (TableCell));
		}
	}
}

void freeTable (ssize_t tableLength, ssize_t tableHeight, TableCell * table[tableLength][tableHeight])
{
	for (int i = 0; i < tableLength; ++i)
	{
		for (int j = 0; j < tableHeight; ++j)
        {
            elektraFree (table[i][j]);
		}
	}

}

void fillTable(PrettyHeadNode * head, PrettyIndexType indexType, 
        ssize_t tableLength, ssize_t tableHeight, TableCell * table[tableLength][tableHeight])
{
	ssize_t row = 0;
	Key * cur;
	ksRewind (head->nodes);
	while ((cur = ksNext (head->nodes)) != NULL)
	{
        ssize_t col = 0;
		PrettyIndexNode * node = *(PrettyIndexNode **) keyValue (cur);
		if (indexType == PRETTY_INDEX_NAME)
		{
			(table[col][row])->value = keyBaseName (node->key);
		}
		else
		{
			(table[col][row])->value = keyString (node->key);
		}
        ++col;

		Key * cur2;
		ksRewind (node->ordered);
		while ((cur2 = ksNext (node->ordered)) != NULL)
		{
            ssize_t valueLength = keyGetValueSize (cur2);
            char value[valueLength];
            keyGetString (cur2, value, valueLength);

            ssize_t currentRowHeight = 0;
            for(const char * line = strtok(value, "\n"); line!=NULL; line = strtok(NULL, "\n"))
            {
			    char * lineAlloc = elektraStrNDup(line, elektraStrLen (line)); 
                fprintf (stderr, "DEBUG: table[%zd][%zd] = %s\n", col, row+currentRowHeight, lineAlloc);
			    (table[col][row + currentRowHeight])->value = lineAlloc;
                ++currentRowHeight;
            }

			++col;
		}
		++row;
	}
}

// will optimize this shit later
void printSeparatorLine (FILE * fh, const char c, ssize_t numCols, ssize_t colLengths[])
{
	for (ssize_t j = 0; j < numCols; ++j)
	{
		fputc ('+', fh);
		for (ssize_t j2 = 0; j2 < colLengths[j]; ++j2)
		{
			fputc (c, fh);
		}
	}
	fputs ("+\n", fh);
}

void printTable(FILE * fh, PrettyIndexNode * firstIndexNode, 
        ssize_t tableLength, ssize_t tableHeight, TableCell * table[tableLength][tableHeight], 
        ssize_t numCols, ssize_t colLengths[numCols], 
        ssize_t numRows, ssize_t rowHeights[numRows])
{
	printSeparatorLine (fh, '-', numCols, colLengths);
	ksRewind (firstIndexNode->ordered);
	fprintf (fh, "|%*s|", (int) colLengths[0], " ");
	int whatever = 1;
	Key * cur;
    while ((cur = ksNext (firstIndexNode->ordered)) != NULL)
	{
		fprintf (fh, "%-*s|", (int) colLengths[whatever], keyBaseName (cur));
		++whatever;
	}
	fputc ('\n', fh);
	printSeparatorLine (fh, '=', numCols, colLengths);

	ssize_t line = 0;
	for (ssize_t i = 0; i < numRows; ++i)
	{
		for (ssize_t i2 = 0; i2 < rowHeights[i]; ++i2)
		{
			fputc ('|', fh);
			for (ssize_t j = 0; j < numCols; ++j)
			{
				fprintf (fh, "%-*s|", (int) colLengths[j], (table[j][line])->value);
			}
			fputc ('\n', fh);
			++line;
		}
		printSeparatorLine (fh, '-', numCols, colLengths);
	}
}

