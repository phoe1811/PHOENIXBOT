idf_component_register(
    SRCS
        "audio.cpp"
        "Microphone.cpp"
        "Speaker.cpp"

    INCLUDE_DIRS
        "include"

    REQUIRES
        esp_driver_i2s
        driver
)