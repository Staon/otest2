# a function which adds otest2 sources to a specified target
#
# Usage: target_otest2_sources(<target> [DOMAIN <domain>] sources...)
function(TARGET_OTEST2_SOURCES target)
  set(one_value_args DOMAIN)
  cmake_parse_arguments(OTEST2 "" "${one_value_args}" "" ${ARGN})
  
  if(OTEST2_DOMAIN)
    set(domain_arg "-d${OTEST2_DOMAIN}")
  else()
    set(domain_arg "")
  endif()

  foreach(src IN LISTS OTEST2_UNPARSED_ARGUMENTS)
    set(gensrc "${CMAKE_CURRENT_SOURCE_DIR}/${src}.cpp")
    set(includes "$<TARGET_PROPERTY:${target},INCLUDE_DIRECTORIES>")
    set(cdefs "$<TARGET_PROPERTY:${target},COMPILE_DEFINITIONS>")
    add_custom_command(
        OUTPUT ${gensrc}
        COMMAND otest2
            -s ${CMAKE_CURRENT_SOURCE_DIR}/${src}
            -o ${gensrc} 
            ${domain_arg}
            --
            "$<$<BOOL:${includes}>:-I$<JOIN:${includes},;-I>>"
            "$<$<BOOL:${cdefs}>:-D$<JOIN:${cdefs},;-D>>"
            "$<JOIN:$<TARGET_PROPERTY:${target},COMPILE_OPTIONS>,;>"
        DEPENDS ${src} otest2
        COMMAND_EXPAND_LISTS
    )
    target_sources(${target} PRIVATE ${gensrc})
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${gensrc})
  endforeach()
endfunction()

# -- name of the default template of the main.cpp
set(OTEST2_MAIN_TEMPLATE "${CMAKE_CURRENT_LIST_DIR}/template-main.cpp")

# generate default test main file
#
# Usage target_otest2_main(<target> [FILE genfile.cpp])
function(TARGET_OTEST2_MAIN target)
  set(one_value_args FILE)
  cmake_parse_arguments(OTEST2 "" "${one_value_args}" "" ${ARGN})
  
  if(OTEST2_FILE)
    set(main_file "${CMAKE_CURRENT_SOURCE_DIR}/${OTEST2_FILE}")
  else()
    set(main_file "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
  endif()

  add_custom_command(
      OUTPUT ${main_file}
      COMMAND cat ${OTEST2_MAIN_TEMPLATE} > ${main_file}
  )
  target_sources(${target} PRIVATE ${main_file})
  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${main_file})
endfunction()
