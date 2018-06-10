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
    return (ssize_t) keyGetValueSize (key); 
}

static ssize_t calcTableCellHeight (Key * key)
{
    Key * dup = keyDup (key);
    const char * value = keyString (dup);

    ssize_t numberOfLines = 1;    
    while(*value != '\0')
    {
        if (*value == '\n') numberOfLines++;
        value++;
    }

    return numberOfLines;
}

void calcSizes(PrettyHeadNode * head, ssize_t rowHeights[], ssize_t numRows, ssize_t colLengths[], ssize_t numCols)
{
    int rowCount = 0;
    int colCount = 0;

    ksRewind (head->nodes);
    Key * cur;
    while ((cur = ksNext (head->nodes)) != NULL)
    {
		PrettyIndexNode * node = *(PrettyIndexNode **) keyValue (cur);
        
        ksRewind (node->ordered);
        Key * cur2;
        while ((cur2 = ksNext (node->ordered)) != NULL)
        {
            ssize_t cellLength = calcTableCellLength(cur2);
            if (colLengths[colCount] < cellLength) colLengths[colCount] = cellLength;
            
            ssize_t cellHeight = calcTableCellHeight(cur2);
            if (rowHeights[rowCount] < cellHeight) rowHeights[rowCount] = cellHeight;
        
            colCount++;

            //TODO check size of colCout < numCols
        }

        colCount = 0;
        rowCount++; 

        // TODO check size rowCount < numRows
    }
}

ssize_t calcTableLength(ssize_t colLengths[], ssize_t numCols)
{
    return numCols;
}

ssize_t calcTableHeight(ssize_t rowHeights[], ssize_t numRows)
{
    ssize_t tableHeight = 0;
    for (int i=0; i<numRows; i++)
    {
       tableHeight += rowHeights[i]; 
    }
    
    return tableHeight;
}

