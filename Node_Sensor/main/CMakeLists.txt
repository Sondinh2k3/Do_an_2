# Register the main component
idf_component_register(SRCS "app_main.c"
                            "../common/dht11/dht11.c"
                            "../common/sx1278_ra02/ra02.c"
                    INCLUDE_DIRS "."
                    "../common/dht11"
                    "../common/sx1278_ra02")
