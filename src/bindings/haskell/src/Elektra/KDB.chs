module Elektra.KDB (kdbOpen, kdbClose, kdbGet, kdbSet) where

{#import Elektra.Key#}
{#import Elektra.KeySet#}

#include <kdb.h>

{#pointer *KDB foreign newtype #}

-- ***
-- KDB METHODS
-- ***

{#fun unsafe kdbOpen {`Key'} -> `KDB' #}
{#fun unsafe kdbClose {`KDB', `Key'} -> `Int' #}
{#fun unsafe kdbGet {`KDB', `KeySet', `Key'} -> `Int' #}
{#fun unsafe kdbSet {`KDB', `KeySet', `Key'} -> `Int' #}
