macro(add_resource RES_NAME RES_FILE RES_TARGET)
    set(RES_OBJECT_FILE ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${RES_TARGET}.dir/${RES_NAME}.o)
    set(RES_OBJCOPY_OPTS --input binary --output elf64-x86-64 --binary-architecture i386:x86-64 --rename-section .data=.rodata,CONTENTS,ALLOC,LOAD,READONLY,DATA)
    set(RES_RENAME_SYMBOLS --redefine-sym _binary_${RES_NAME}_start=resource_${RES_NAME}_start --redefine-sym _binary_${RES_NAME}_end=resource_${RES_NAME}_end  --redefine-sym _binary_${RES_NAME}_size=resource_${RES_NAME}_size)
    
    add_custom_command(
        TARGET ${RES_TARGET}
        PRE_BUILD
        COMMAND dd if=/dev/zero of=zero_pad bs=1 count=1 2> /dev/null
        COMMAND cat < ${CMAKE_CURRENT_SOURCE_DIR}/${RES_FILE}  < zero_pad > ${RES_NAME}
        COMMAND objcopy ${RES_OBJCOPY_OPTS} ${RES_RENAME_SYMBOLS} ${RES_NAME} ${RES_OBJECT_FILE}
        COMMAND rm zero_pad ${RES_NAME}
        BYPRODUCTS ${RES_OBJECT_FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    set(RESOURCES_OBJECTS ${RESOURCES_OBJECTS} ${RES_OBJECT_FILE})

    unset(RES_OBJECT_FILE)
    unset(RES_OBJCOPY_OPTS)
    unset(RES_RENAME_SYMBOLS)
endmacro(add_resource)
