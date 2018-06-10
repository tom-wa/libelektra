/**
 * @file
 *
 * @brief helper function for printing rst tables
 *
 * @copyright BSD License (see LICENSE.md or https://www.libelektra.org)
 *
 */

#include "prettyexport.h"

#include <kdb.h>
#include <kdbease.h> // elektraArrayValidateName, elektraKeyGetRelativeName
#include <kdbhelper.h>
#include <stdio.h>
#include <stdlib.h>

static ssize_t calcTableCellLength (Key * key)
{
	ssize_t len = keyGetValueSize (key);
	if (keyGetMeta (key, "pretty/bold")) len += 4;
	if (keyGetMeta (key, "pretty/italics")) len += 2;
	if (keyGetMeta (key, "pretty/mono")) len += 4;
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

void calcSizes (PrettyHeadNode * head, PrettyIndexType indexType, ssize_t rowHeights[], ssize_t numRows, ssize_t colLengths[],
		ssize_t numCols)
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

		if (keyGetMeta (node->key, "pretty/bold")) indexCellLength += 4;
		if (keyGetMeta (node->key, "pretty/italics")) indexCellLength += 2;
		if (keyGetMeta (node->key, "pretty/mono")) indexCellLength += 4;


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

ssize_t calcTableLength (ssize_t colLengths[], ssize_t numCols)
{
	return numCols;
}

ssize_t calcTableHeight (ssize_t rowHeights[], ssize_t numRows)
{
	ssize_t tableHeight = 0;
	for (int i = 0; i < numRows; i++)
	{
		tableHeight += rowHeights[i];
	}

	return tableHeight;
}
