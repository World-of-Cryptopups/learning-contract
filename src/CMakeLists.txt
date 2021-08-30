project(learning)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( learning learning learning.cpp )
target_include_directories( learning PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( learning ${CMAKE_SOURCE_DIR}/../ricardian )