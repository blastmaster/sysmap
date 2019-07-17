# find lscpu

find_program(LSCPU_CMD lscpu)

if (NOT LSCPU_CMD)
    message("lscpu command not found, no CPU information through lscpu.")
else()
    message("lscpu: ${LSCPU_CMD}")
endif()
