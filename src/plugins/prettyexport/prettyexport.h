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

#endif
