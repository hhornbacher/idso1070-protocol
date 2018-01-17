add_library(device Channel.cxx Trigger.cxx IDSO1070.cxx EEROMData.cxx)
target_link_libraries (device)