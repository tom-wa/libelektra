/**
 * @file
 *
 * @brief Header for rst table helper functions
 *
 * @copyright BSD License (see LICENSE.md or https://www.libelektra.org)
 *
 */
#include "prettyexport.h"

void calcSizes (PrettyHeadNode * head, ssize_t rowHeights[], ssize_t numRows, ssize_t colLengths[], ssize_t numCols);

ssize_t calcTableLength (ssize_t colLengths[], ssize_t numCols);

ssize_t calcTableHeight (ssize_t rowHeights[], ssize_t numRows);
