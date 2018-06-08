/**
 * @file
 *
 * @brief Source for prettyexport plugin
 *
 * @copyright BSD License (see LICENSE.md or https://www.libelektra.org)
 *
 */

#include "prettyexport.h"

#include <kdbease.h> // elektraArrayValidateName, elektraKeyGetRelativeName
#include <kdbhelper.h>
#include <stdio.h>
#include <stdlib.h>

struct _PrettyHeadNode
{
	Key * key;
	KeySet * nodes;
};

struct _PrettyIndexNode
{
	Key * key;
	ssize_t requiredLenght;
	KeySet * ordered;
	KeySet * unordered;
};

typedef struct _PrettyIndexNode PrettyIndexNode;
typedef struct _PrettyHeadNode PrettyHeadNode;

int elektraPrettyexportOpen (Plugin * handle ELEKTRA_UNUSED, Key * errorKey ELEKTRA_UNUSED)
{
	// plugin initialization logic
	// this function is optional

	return ELEKTRA_PLUGIN_STATUS_SUCCESS;
}

int elektraPrettyexportClose (Plugin * handle ELEKTRA_UNUSED, Key * errorKey ELEKTRA_UNUSED)
{
	// free all plugin resources and shut it down
	// this function is optional

	return ELEKTRA_PLUGIN_STATUS_SUCCESS;
}

int elektraPrettyexportGet (Plugin * handle ELEKTRA_UNUSED, KeySet * returned, Key * parentKey)
{
	if (!elektraStrCmp (keyName (parentKey), "system/elektra/modules/prettyexport"))
	{
		KeySet * contract = ksNew (
			30, keyNew ("system/elektra/modules/prettyexport", KEY_VALUE, "prettyexport plugin waits for your orders", KEY_END),
			keyNew ("system/elektra/modules/prettyexport/exports", KEY_END),
			keyNew ("system/elektra/modules/prettyexport/exports/open", KEY_FUNC, elektraPrettyexportOpen, KEY_END),
			keyNew ("system/elektra/modules/prettyexport/exports/close", KEY_FUNC, elektraPrettyexportClose, KEY_END),
			keyNew ("system/elektra/modules/prettyexport/exports/get", KEY_FUNC, elektraPrettyexportGet, KEY_END),
			keyNew ("system/elektra/modules/prettyexport/exports/set", KEY_FUNC, elektraPrettyexportSet, KEY_END),
			keyNew ("system/elektra/modules/prettyexport/exports/error", KEY_FUNC, elektraPrettyexportError, KEY_END),
			keyNew ("system/elektra/modules/prettyexport/exports/checkconf", KEY_FUNC, elektraPrettyexportCheckConfig, KEY_END),
#include ELEKTRA_README (prettyexport)
			keyNew ("system/elektra/modules/prettyexport/infos/version", KEY_VALUE, PLUGINVERSION, KEY_END), KS_END);
		ksAppend (returned, contract);
		ksDel (contract);

		return ELEKTRA_PLUGIN_STATUS_SUCCESS;
	}
	// get all keys

	return ELEKTRA_PLUGIN_STATUS_NO_UPDATE;
}

static Key * keyToFieldIndex (const Key * key, const Key * parent)
{
	if (!key) return NULL;
	const Key * fieldIndexMeta = keyGetMeta (key, "pretty/field");
	if (!fieldIndexMeta) return NULL;
	Key * newKey = keyNew (keyName (parent), KEY_VALUE, keyValue (key));
	keyAddName (newKey, keyString (fieldIndexMeta));
	if (elektraArrayValidateName (newKey) != 1)
	{
		keyDel (newKey);
		return NULL;
	}
	keyAddName (newKey, elektraKeyGetRelativeName (key, parent));
	return newKey;
}

static inline unsigned short isIndexNode (Key * key)
{
	if (!key) return 0;
	if (keyGetMeta (key, "pretty/index"))
		return 1;
	else
		return 0;
}

static void prettyCleanUp (PrettyHeadNode * head)
{
	ksRewind (head->nodes);
	Key * cur;
	while ((cur = ksNext (head->nodes)) != NULL)
	{
		PrettyIndexNode * node = *(PrettyIndexNode **) keyValue (cur);
		ksDel (node->ordered);
		ksDel (node->unordered);
		keyDel (node->key);
		elektraFree (node);
	}
	ksDel (head->nodes);
	keyDel (head->key);
}

static int addNodes (const Key * key, KeySet * workingSet)
{
	PrettyIndexNode * node = *(PrettyIndexNode **) keyValue (key);
	node->ordered = ksNew (0, KS_END);
	node->unordered = ksNew (0, KS_END);
	KeySet * cutKS = ksCut (workingSet, node->key);
	Key * cur;
	while ((cur = ksNext (cutKS)) != NULL)
	{
		Key * newKey = keyToFieldIndex (cur, node->key);
		if (!newKey)
		{
			ksAppendKey (node->unordered, keyDup (cur));
			if ((ssize_t) (keyGetValueSize (cur) + elektraStrLen (keyBaseName (cur))) > node->requiredLenght)
				node->requiredLenght = (ssize_t) (keyGetValueSize (cur) + elektraStrLen (keyBaseName (cur)));
		}
		else
		{
			ksAppendKey (node->ordered, newKey);
			if ((ssize_t) (keyGetValueSize (newKey) + elektraStrLen (keyBaseName (newKey))) > node->requiredLenght)
				node->requiredLenght = (ssize_t) (keyGetValueSize (newKey) + elektraStrLen (keyBaseName (newKey)));
		}
	}
	ksDel (cutKS);
	return 0;
}

static void printTree (PrettyHeadNode * head)
{
	fprintf (stderr, "DEBUG: pretting tree\n");
	ksRewind (head->nodes);
	Key * cur;
	while ((cur = ksNext (head->nodes)) != NULL)
	{
		PrettyIndexNode * node = *(PrettyIndexNode **) keyValue (cur);
		fprintf (stderr, "DEBUG: INDEX: %s\t required Lenght: %zu\n", keyName (node->key), node->requiredLenght);
		Key * cur2;
		ksRewind (node->ordered);
		while ((cur2 = ksNext (node->ordered)) != NULL)
		{
			fprintf (stderr, "DEBUG: NODE: \t%s:(%s)\n", keyName (cur2), keyString (cur2));
		}
		ksRewind (node->unordered);
		while ((cur2 = ksNext (node->unordered)) != NULL)
		{
			fprintf (stderr, "DEBUG: NODE: \t%s:(%s)\n", keyName (cur2), keyString (cur2));
		}
	}
}

int elektraPrettyexportSet (Plugin * handle ELEKTRA_UNUSED, KeySet * returned, Key * parentKey)
{
	// set all keys
	// this function is optional

	const Key * prettyType = keyGetMeta (parentKey, "pretty");
	if (!prettyType) return 0;

	PrettyHeadNode * head = elektraCalloc (sizeof (PrettyHeadNode));
	head->key = keyDup (parentKey);
	head->nodes = ksNew (ksGetSize (returned), KS_END);

	KeySet * workingSet = ksDup (returned);
	ksRewind (workingSet);
	Key * cur;

	while ((cur = ksNext (workingSet)) != NULL)
	{
		if (!isIndexNode (cur)) continue;
		fprintf (stderr, "DEBUG: found index node %s\n", keyName (cur));
		PrettyIndexNode * node = elektraCalloc (sizeof (PrettyIndexNode));
		node->key = keyDup (cur);
		Key * nodeKey = keyNew ("/", KEY_BINARY, KEY_SIZE, sizeof (PrettyIndexNode), KEY_VALUE, &node, KEY_END);
		keyAddName (nodeKey, elektraKeyGetRelativeName (cur, head->key));
		ksAppendKey (head->nodes, nodeKey);
	}

	ksRewind (head->nodes);
	while ((cur = ksNext (head->nodes)) != NULL)
	{
		addNodes (cur, workingSet);
	}

	printTree (head);

	prettyCleanUp (head);
	elektraFree (head);
	ksDel (workingSet);

	return ELEKTRA_PLUGIN_STATUS_NO_UPDATE;
}

int elektraPrettyexportError (Plugin * handle ELEKTRA_UNUSED, KeySet * returned ELEKTRA_UNUSED, Key * parentKey ELEKTRA_UNUSED)
{
	// handle errors (commit failed)
	// this function is optional

	return ELEKTRA_PLUGIN_STATUS_SUCCESS;
}

int elektraPrettyexportCheckConfig (Key * errorKey ELEKTRA_UNUSED, KeySet * conf ELEKTRA_UNUSED)
{
	// validate plugin configuration
	// this function is optional

	return ELEKTRA_PLUGIN_STATUS_NO_UPDATE;
}

Plugin * ELEKTRA_PLUGIN_EXPORT (prettyexport)
{
	// clang-format off
    return elektraPluginExport ("prettyexport",
            ELEKTRA_PLUGIN_OPEN,	&elektraPrettyexportOpen,
            ELEKTRA_PLUGIN_CLOSE,	&elektraPrettyexportClose,
            ELEKTRA_PLUGIN_GET,	&elektraPrettyexportGet,
            ELEKTRA_PLUGIN_SET,	&elektraPrettyexportSet,
            ELEKTRA_PLUGIN_ERROR,	&elektraPrettyexportError,
            ELEKTRA_PLUGIN_END);
}
