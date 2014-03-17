macro(ADD_APP_ICON appsources filename)

# MacOS icon specific stuff
if(APPLE)
    message(STATUS "AddAppIcon macro, MacOS version")

    SET(ICON_source_file ${CMAKE_CURRENT_SOURCE_DIR}/${filename}_512.png)
    # Extract filename without extension
    get_filename_component(ICON_NAME ${filename} NAME_WE)
    # find icon conversion programs
    find_program(SIPS_EXECUTABLE NAMES sips) # for PNG -> TIFF conversion
    find_program(TIFF2ICNS_EXECUTABLE NAMES tiff2icns) # for TIFF -> ICNS
    if(SIPS_EXECUTABLE AND TIFF2ICNS_EXECUTABLE)
        message(STATUS "The sips and tiff2icns utilities are available. Application will have an application icon!")
        SET(ICON_outfile ${CMAKE_CURRENT_BINARY_DIR}/${ICON_NAME})
        # Command to convert PNG to ICNS
        add_custom_command(OUTPUT ${ICON_outfile}.icns ${ICON_outfile}.tiff
                           COMMAND ${SIPS_EXECUTABLE} -s format tiff ${ICON_source_file} --out ${ICON_outfile}.tiff
                           COMMAND ${TIFF2ICNS_EXECUTABLE} ${ICON_outfile}.tiff ${ICON_outfile}.icns
                           DEPENDS ${ICON_source_file}
                           )
        #
        set(MACOSX_BUNDLE_ICON_FILE ${ICON_NAME}.icns)
        #
        list(APPEND ${appsources} ${ICON_outfile}.icns)
        #
        set_source_files_properties(${ICON_outfile}.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

    else(SIPS_EXECUTABLE AND TIFF2ICNS_EXECUTABLE)
        message(STATUS "Unable to find the sips and tiff2icns utilities. Application will not have an application icon!")
    endif(SIPS_EXECUTABLE AND TIFF2ICNS_EXECUTABLE)

else(APPLE)
    message(STATUS "AddAppIcon only supports Apple MacOS. Application will not have an application icon!")

endif(APPLE)

endmacro(ADD_APP_ICON)
