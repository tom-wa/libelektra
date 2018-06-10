include (LibAddMacros)

add_plugin (prettyexport SOURCES prettyexport.h prettyexport.c rsttable.h rsttable.c 
    LINK_ELEKTRA elektra-ease
    ADD_TEST TEST_README
    INSTALL_TEST_DATA)

