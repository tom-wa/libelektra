include (LibAddMacros)

add_plugin (prettyexport SOURCES prettyexport.h prettyexport.c 
    LINK_ELEKTRA elektra-ease
    ADD_TEST TEST_README)
