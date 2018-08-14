/**
 * @file
 *
 * @brief Source for cpasswd plugin
 *
 * @copyright BSD License (see LICENSE.md or https://www.libelektra.org)
 *
 */

#include "cpasswd.h"

#include <kdberrors.h>
#include <kdbhelper.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>

#define ID_MAX_CHARACTERS 11
#define PWD_DELIMITER ":\r\n"

static unsigned atou(char **s)
{
	unsigned x;
	for (x=0; **s-'0'<10U; ++*s) x=10*x+(**s-'0');
	return x;
}

static struct passwd * strToPasswd (char * line)
{
	struct passwd * pwd = elektraMalloc (sizeof (struct passwd));
    char * s = line;
    pwd->pw_name = s++;
    if(!(s = strchr(s, ':'))) return NULL;
    *s++ = 0; pwd->pw_passwd = s;
    if(!(s = strchr(s, ':'))) return NULL;
    *s++ = 0; pwd->pw_uid = atou(&s);
    if(!(s = strchr(s, ':'))) return NULL;
    *s++ = 0; pwd->pw_gid = atou(&s);
    if(!(s = strchr(s, ':'))) return NULL;
    *s++ = 0; pwd->pw_gecos = s;
    if(!(s = strchr(s, ':'))) return NULL;
    *s++ = 0; pwd->pw_dir = s;
    if(!(s = strchr(s, ':'))) return NULL;
    *s++ = 0; pwd->pw_shell = s;
    return pwd;
}

static KeySet * pwentToKS (struct passwd * pwd, Key * parentKey)
{
	KeySet * ks = ksNew (0, KS_END);
	Key * append = keyNew (keyName (parentKey), KEY_END);
	char id[ID_MAX_CHARACTERS];
	snprintf (id, sizeof (id), "%u", pwd->pw_uid);
	keyAddBaseName (append, id);
	keySetBinary (append, 0, 0);
	ksAppendKey (ks, keyDup (append));
	keyAddBaseName (append, "name");
	keySetString (append, pwd->pw_name);
	ksAppendKey (ks, keyDup (append));
	keySetString (append, 0);
	keySetBaseName (append, "shell");
	keySetString (append, pwd->pw_shell);
	ksAppendKey (ks, keyDup (append));
	keySetString (append, 0);
	keySetBaseName (append, "home");
	keySetString (append, pwd->pw_dir);
	ksAppendKey (ks, keyDup (append));
	keySetString (append, 0);
	keySetBaseName (append, "gid");
	snprintf (id, sizeof (id), "%u", pwd->pw_gid);
	keySetString (append, id);
	ksAppendKey (ks, keyDup (append));
	keySetString (append, 0);
	keySetBaseName (append, "passwd");
	keySetString (append, pwd->pw_passwd);
	ksAppendKey (ks, keyDup (append));
	keySetString (append, 0);
	keySetBaseName (append, "gecos");
	keySetString (append, pwd->pw_gecos);
	ksAppendKey (ks, keyDup (append));
	keyDel (append);
	return ks;
}

int elektraCpasswdGet (Plugin * handle ELEKTRA_UNUSED, KeySet * returned, Key * parentKey)
{
	if (!elektraStrCmp (keyName (parentKey), "system/elektra/modules/cpasswd"))
	{
		KeySet * contract =
			ksNew (30, keyNew ("system/elektra/modules/cpasswd", KEY_VALUE, "cpasswd plugin waits for your orders", KEY_END),
			       keyNew ("system/elektra/modules/cpasswd/exports", KEY_END),
			       keyNew ("system/elektra/modules/cpasswd/exports/get", KEY_FUNC, elektraCpasswdGet, KEY_END),
#include ELEKTRA_README (cpasswd)
			       keyNew ("system/elektra/modules/cpasswd/infos/version", KEY_VALUE, PLUGINVERSION, KEY_END), KS_END);
		ksAppend (returned, contract);
		ksDel (contract);
		return ELEKTRA_PLUGIN_STATUS_SUCCESS;
    }
        FILE *f = fopen("/etc/passwd", "r");
        char *line = NULL;
        size_t len = 0;
        struct passwd *pwd;
        while(getline(&line, &len, f) != -1)
        {
            pwd = strToPasswd(line);
            if(!pwd)
            {
                ELEKTRA_SET_ERRORF (110, parentKey, "Failed to parse line '%s' of passwd file\n", line);
                return -1;
            }
            KeySet * ks = pwentToKS(pwd, parentKey);
            ksAppend(returned, ks);
            ksDel(ks);
            elektraFree(pwd);
        }
        if(line)
        {
            elektraFree(line);
        }
        fclose(f);
	
	return ELEKTRA_PLUGIN_STATUS_NO_UPDATE;
}

Plugin * ELEKTRA_PLUGIN_EXPORT (cpasswd)
{
	// clang-format off
	return elektraPluginExport ("cpasswd",
		ELEKTRA_PLUGIN_GET,	&elektraCpasswdGet,
		ELEKTRA_PLUGIN_END);
}
