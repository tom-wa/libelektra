/**
 * @file
 *
 * @brief Header for cpasswd plugin
 *
 * @copyright BSD License (see LICENSE.md or https://www.libelektra.org)
 *
 */

#ifndef ELEKTRA_PLUGIN_CPASSWD_H
#define ELEKTRA_PLUGIN_CPASSWD_H

#include <kdbplugin.h>


int elektraCpasswdGet (Plugin * handle, KeySet * ks, Key * parentKey);

Plugin * ELEKTRA_PLUGIN_EXPORT (cpasswd);

#endif
