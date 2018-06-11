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

static void test_basicKS (void)
{
	Key * parentKey = keyNew ("user/tests/prettyexport", KEY_VALUE, elektraFilename (), KEY_META, "description",
				  "this is the mounted passwd file", KEY_META, "pretty", "list", KEY_META, "pretty/index", "name", KEY_END);
	KeySet * conf = ksNew (0, KS_END);
	PLUGIN_OPEN ("prettyexport");
	KeySet * ks =
		ksNew (30, keyNew ("user/tests/prettyexport/thomas", KEY_META, "pretty/index", "", KEY_END),
		       keyNew ("user/tests/prettyexport/thomas/gecos", KEY_VALUE, "Thomas Waser", KEY_META, "pretty/field", "#1", KEY_END),
		       keyNew ("user/tests/prettyexport/thomas/uid", KEY_VALUE, "1000", KEY_META, "pretty/field", "#2", KEY_END),
		       keyNew ("user/tests/prettyexport/thomas/gid", KEY_VALUE, "1000", KEY_META, "pretty/field", "#3", KEY_END),
		       keyNew ("user/tests/prettyexport/thomas/shell", KEY_VALUE, "/usr/bin/zsh", KEY_META, "pretty/field", "#4", KEY_END),
		       keyNew ("user/tests/prettyexport/thomas/home", KEY_VALUE, "/home/thomas", KEY_META, "pretty/field", "#5", KEY_END),
		       keyNew ("user/tests/prettyexport/thomas/passwd", KEY_VALUE, "x", KEY_END),
		       keyNew ("user/tests/prettyexport/tgi", KEY_META, "pretty/index", "", KEY_END),
		       keyNew ("user/tests/prettyexport/tgi/gecos", KEY_VALUE, "TGI Staff", KEY_META, "pretty/field", "#1", KEY_END),
		       keyNew ("user/tests/prettyexport/tgi/uid", KEY_VALUE, "1003", KEY_META, "pretty/field", "#2", KEY_END),
		       keyNew ("user/tests/prettyexport/tgi/gid", KEY_VALUE, "1003", KEY_META, "pretty/field", "#3", KEY_END),
		       keyNew ("user/tests/prettyexport/tgi/shell", KEY_VALUE, "/bin/bash", KEY_META, "pretty/field", "#4", KEY_END),
		       keyNew ("user/tests/prettyexport/tgi/home", KEY_VALUE, "/home/tgi", KEY_META, "pretty/field", "#5", KEY_END),
		       keyNew ("user/tests/prettyexport/tgi/passwd", KEY_VALUE, "x", KEY_END), KS_END);
	succeed_if (plugin->kdbSet (plugin, ks, parentKey) >= 0, "failed!");

	succeed_if (compare_line_files (srcdir_file ("prettyexport/basic.rst"), keyString (parentKey)), "files do not match as expected");
	ksDel (ks);
	keyDel (parentKey);
	PLUGIN_CLOSE ();
}

static void test_basicKSTable (void)
{
	Key * parentKey =
		keyNew ("user/tests/prettyexport", KEY_VALUE, elektraFilename (), KEY_META, "description",
			"this is the mounted passwd file", KEY_META, "pretty", "table", KEY_META, "pretty/index", "name", KEY_END);
	KeySet * conf = ksNew (0, KS_END);
	PLUGIN_OPEN ("prettyexport");
	KeySet * ks = ksNew (
		30, keyNew ("user/tests/prettyexport/thomas", KEY_META, "pretty/index", "", KEY_END),
		keyNew ("user/tests/prettyexport/thomas/gecos", KEY_VALUE, "Thomas\nWaser", KEY_META, "pretty/field", "#1", KEY_END),
		keyNew ("user/tests/prettyexport/thomas/uid", KEY_VALUE, "1000", KEY_META, "pretty/field", "#2", KEY_END),
		keyNew ("user/tests/prettyexport/thomas/gid", KEY_VALUE, "1000", KEY_META, "pretty/field", "#3", KEY_END),
		keyNew ("user/tests/prettyexport/thomas/shell", KEY_VALUE, "/usr\n/bin\n/zsh", KEY_META, "pretty/field", "#4", KEY_END),
		keyNew ("user/tests/prettyexport/thomas/home", KEY_VALUE, "/home\n/thomas", KEY_META, "pretty/field", "#5", KEY_END),
		keyNew ("user/tests/prettyexport/thomas/passwd", KEY_VALUE, "x", KEY_END),
		keyNew ("user/tests/prettyexport/tgi", KEY_META, "pretty/index", "", KEY_END),
		keyNew ("user/tests/prettyexport/tgi/gecos", KEY_VALUE, "TGI\nStaff", KEY_META, "pretty/field", "#1", KEY_END),
		keyNew ("user/tests/prettyexport/tgi/uid", KEY_VALUE, "1003", KEY_META, "pretty/field", "#2", KEY_END),
		keyNew ("user/tests/prettyexport/tgi/gid", KEY_VALUE, "1003", KEY_META, "pretty/field", "#3", KEY_END),
		keyNew ("user/tests/prettyexport/tgi/shell", KEY_VALUE, "/bin\n/bash", KEY_META, "pretty/field", "#4", KEY_END),
		keyNew ("user/tests/prettyexport/tgi/home", KEY_VALUE, "/home\n/tgi", KEY_META, "pretty/field", "#5", KEY_END),
		keyNew ("user/tests/prettyexport/tgi/passwd", KEY_VALUE, "x", KEY_END), KS_END);
	succeed_if (plugin->kdbSet (plugin, ks, parentKey) >= 0, "failed!");

	printf ("%s\n", keyString (parentKey));
	succeed_if (compare_line_files (srcdir_file ("prettyexport/basicTable.rst"), keyString (parentKey)),
		    "files do not match as	expected");
	ksDel (ks);
	keyDel (parentKey);
	PLUGIN_CLOSE ();
}

int main (int argc, char ** argv)
{
	printf ("PRETTYEXPORT     TESTS\n");
	printf ("==================\n\n");

	init (argc, argv);

	test_basicKS ();

	test_basicKSTable ();

	print_result ("testmod_prettyexport");

	return nbError;
}
