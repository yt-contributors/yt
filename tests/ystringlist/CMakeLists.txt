FILE(GLOB all_sources "*.cc")
set ( modules_testing_src ${modules_testing_src}
    ${all_sources}
    CACHE INTERNAL ""
)
list( REMOVE_DUPLICATES modules_testing_src )

