- infos = Information about the prettyexport plugin is in keys below
- infos/author = Author Name <elektra@libelektra.org>
- infos/licence = BSD
- infos/needs =
- infos/provides =
- infos/recommends =
- infos/placements = prerollback rollback postrollback getresolver pregetstorage getstorage postgetstorage setresolver presetstorage setstorage precommit commit postcommit
- infos/status = recommended productive maintained reviewed conformant compatible coverage specific unittest shelltest tested nodep libc configurable final preview memleak experimental difficult unfinished old nodoc concept orphan obsolete discouraged -1000000
- infos/metadata =
- infos/description = one-line description of prettyexport

## Introduction

## Examples

```
kdb mount /etc/passwd system/passwd passwd index=name
kdb mount passwdspec.ini spec/passwd ni
kdb export /passwd prettyexport -c pretty=table
```
## Limitations

None.
