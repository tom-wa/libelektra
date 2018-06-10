/**
 * @file
 *
 * @brief Header for prettyexport plugin
 *
 * @copyright BSD License (see LICENSE.md or https://www.libelektra.org)
 *
 */

#ifndef ELEKTRA_PLUGIN_PRETTYEXPORT_H
#define ELEKTRA_PLUGIN_PRETTYEXPORT_H

#include <kdbplugin.h>


int elektraPrettyexportOpen (Plugin * handle, Key * errorKey);
int elektraPrettyexportClose (Plugin * handle, Key * errorKey);
int elektraPrettyexportGet (Plugin * handle, KeySet * ks, Key * parentKey);
int elektraPrettyexportSet (Plugin * handle, KeySet * ks, Key * parentKey);
int elektraPrettyexportError (Plugin * handle, KeySet * ks, Key * parentKey);
int elektraPrettyexportCheckConfig (Key * errorKey, KeySet * conf);

Plugin * ELEKTRA_PLUGIN_EXPORT (prettyexport);

enum _PrettyType {
	PRETTY_TYPE_INVALID,
	PRETTY_TYPE_LIST,
	PRETTY_TYPE_TABLE,
};
enum _PrettyIndexType {
	PRETTY_INDEX_NAME,
	PRETTY_INDEX_VALUE,
};
enum _PrettyStyle {
	PRETTY_STYLE_NORMAL = 0 << 0,
	PRETTY_STYLE_BOLD = 1 << 0,
	PRETTY_STYLE_ITALICS = 1 << 1,
	PRETTY_STYLE_MONO = 1 << 2,
};

typedef enum _PrettyType PrettyType;
typedef enum _PrettyIndexType PrettyIndexType;
typedef enum _PrettyStyle PrettyStyle;

struct _PrettyHeadNode
{
	PrettyType prettyType;
	PrettyIndexType indexType;
	Key * key;
	KeySet * nodes;
};

struct _PrettyIndexNode
{
	Key * key;
	ssize_t requiredLength;
	KeySet * ordered;
	KeySet * unordered;
};

struct _TableCell
{
	PrettyStyle style;
	const char * value;
};

typedef struct _PrettyIndexNode PrettyIndexNode;
typedef struct _PrettyHeadNode PrettyHeadNode;
typedef struct _TableCell TableCell;

#endif
