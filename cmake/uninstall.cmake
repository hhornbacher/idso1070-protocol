# uninstall target
# https://cmake.org/Wiki/CMake_FAQ#Can_I_do_.22make_uninstall.22_with_CMake.3F

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.uninstall_target.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/uninstall_target.cmake"
    IMMEDIATE @ONLY)
    
add_custom_target(uninstall
    ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/uninstall_target.cmake
)