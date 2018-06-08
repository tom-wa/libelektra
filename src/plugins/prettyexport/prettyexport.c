/**
 * @file
 *
 * @brief Source for prettyexport plugin
 *
 * @copyright BSD License (see LICENSE.md or https://www.libelektra.org)
 *
 */

#include "prettyexport.h"

#include <kdbhelper.h>


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
		KeySet * contract =
			ksNew (30, keyNew ("system/elektra/modules/prettyexport", KEY_VALUE, "prettyexport plugin waits for your orders", KEY_END),
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

int elektraPrettyexportSet (Plugin * handle ELEKTRA_UNUSED, KeySet * returned ELEKTRA_UNUSED, Key * parentKey ELEKTRA_UNUSED)
{
	// set all keys
	// this function is optional

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
