set(sources
  src/DiabloModule.cpp
  src/DiabloModule.h
)

set(scripts
  ${sources}
)

# Add the configuration file to the build
set(config
  conf/mod_diablo.conf
)

# Install configuration
install(FILES "${config}" DESTINATION "${AZEROTH_INSTALL_PREFIX}/etc/modules")

message("  -> Prepared: Diablo Mode Module")