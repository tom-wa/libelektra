/**
 * @file
 *
 * @brief Tests for prettyexport plugin
 *
 * @copyright BSD License (see LICENSE.md or https://www.libelektra.org)
 *
 */

#include <stdlib.h>
#include <string.h>

#include <kdbconfig.h>

#include <tests_plugin.h>

static void test_basics (void)
{
	printf ("test basics\n");

	Key * parentKey = keyNew ("user/tests/prettyexport", KEY_END);
	KeySet * conf = ksNew (0, KS_END);
	PLUGIN_OPEN ("prettyexport");

	KeySet * ks = ksNew (0, KS_END);

	succeed_if (plugin->kdbOpen (plugin, parentKey) == ELEKTRA_PLUGIN_STATUS_SUCCESS, "call to kdbOpen was not successful");

	succeed_if (plugin->kdbGet (plugin, ks, parentKey) == ELEKTRA_PLUGIN_STATUS_NO_UPDATE, "call to kdbGet was not successful");

	succeed_if (plugin->kdbSet (plugin, ks, parentKey) == ELEKTRA_PLUGIN_STATUS_NO_UPDATE, "call to kdbSet was not successful");

	succeed_if (plugin->kdbError (plugin, ks, parentKey) == ELEKTRA_PLUGIN_STATUS_SUCCESS, "call to kdbError was not successful");

	succeed_if (plugin->kdbClose (plugin, parentKey) == ELEKTRA_PLUGIN_STATUS_SUCCESS, "call to kdbClose was not successful");

	keyDel (parentKey);
	ksDel (ks);
	PLUGIN_CLOSE ();
}

static void test_basicKS (void)
{
	Key * parentKey = keyNew ("user/tests/prettyexport", KEY_META, "description", "this is the mounted passwd file", KEY_META, "pretty",
				  "list", KEY_END);
	KeySet * conf = ksNew (0, KS_END);
	PLUGIN_OPEN ("prettyexport");
	KeySet * ks =
		ksNew (30, keyNew ("user/tests/prettyexport/thomas", KEY_META, "pretty/index", "name", KEY_META, "pretty/type", "list", KEY_END),
		       keyNew ("user/tests/prettyexport/thomas/gecos", KEY_VALUE, "Thomas Waser", KEY_META, "pretty/field", "#1", KEY_END),
		       keyNew ("user/tests/prettyexport/thomas/uid", KEY_VALUE, "1000", KEY_META, "pretty/field", "#2", KEY_END),
		       keyNew ("user/tests/prettyexport/thomas/gid", KEY_VALUE, "1000", KEY_META, "pretty/field", "#3", KEY_END),
		       keyNew ("user/tests/prettyexport/thomas/shell", KEY_VALUE, "/usr/bin/zsh", KEY_META, "pretty/field", "#4", KEY_END),
		       keyNew ("user/tests/prettyexport/thomas/home", KEY_VALUE, "/home/thomas", KEY_META, "pretty/field", "#5", KEY_END),
		       keyNew ("user/tests/prettyexport/thomas/passwd", KEY_VALUE, "x", KEY_END),
		       keyNew ("user/tests/prettyexport/tgi", KEY_META, "pretty/index", "name", KEY_META, "pretty/type", "table", KEY_END),
		       keyNew ("user/tests/prettyexport/tgi/gecos", KEY_VALUE, "TGI Staff", KEY_META, "pretty/field", "#1", KEY_END),
		       keyNew ("user/tests/prettyexport/tgi/uid", KEY_VALUE, "1003", KEY_META, "pretty/field", "#2", KEY_END),
		       keyNew ("user/tests/prettyexport/tgi/gid", KEY_VALUE, "1003", KEY_META, "pretty/field", "#3", KEY_END),
		       keyNew ("user/tests/prettyexport/tgi/shell", KEY_VALUE, "/bin/bash", KEY_META, "pretty/field", "#4", KEY_END),
		       keyNew ("user/tests/prettyexport/tgi/home", KEY_VALUE, "/home/tgi", KEY_META, "pretty/field", "#5", KEY_END),
		       keyNew ("user/tests/prettyexport/tgi/passwd", KEY_VALUE, "x", KEY_END), KS_END);
	succeed_if (plugin->kdbSet (plugin, ks, parentKey) >= 0, "failed!");
	ksDel (ks);
	keyDel (parentKey);
	PLUGIN_CLOSE ();
}


int main (int argc, char ** argv)
{
	printf ("PRETTYEXPORT     TESTS\n");
	printf ("==================\n\n");

	init (argc, argv);

	test_basics ();


	test_basicKS ();

	print_result ("testmod_prettyexport");

	return nbError;
}
