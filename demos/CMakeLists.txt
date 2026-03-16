function(add_game_demo target_name source_file)
    add_executable(${target_name} ${source_file})
    
    # By linking RoguelikeCardgameLibrary as PUBLIC/PRIVATE, 
    # the demo now inherits the /include paths automatically!
    target_link_libraries(${target_name} PRIVATE RoguelikeCardgameLibrary)

    # Note: Using ${CMAKE_BINARY_DIR} is safer for finding the assets folder from subdirs
    set_target_properties(${target_name} PROPERTIES 
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
endfunction()

add_subdirectory(cardrendererdemo)
add_subdirectory(cardregistrydemo)
add_subdirectory(handrendererdemo)