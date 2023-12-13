file(GLOB FILES "*.json" "*.csv")
file(COPY ${FILES} DESTINATION ${CMAKE_BINARY_DIR}/)
