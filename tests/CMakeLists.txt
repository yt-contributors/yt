include(YtSubdirList)

YT_SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
FOREACH(subdir ${SUBDIRS})
    ADD_SUBDIRECTORY(${subdir})
ENDFOREACH()

#set ( modules_testing_src ${modules_testing_src}
#    "${CMAKE_CURRENT_SOURCE_DIR}/xxx_helper.cc"
#    CACHE INTERNAL ""
#)
list( REMOVE_DUPLICATES modules_testing_src )
