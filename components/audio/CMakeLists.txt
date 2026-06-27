idf_component_register(

    SRCS
        "AudioEngine.cpp"
        "Microphone.cpp"
        "Speaker.cpp"
        "Recorder.cpp"
        "WavWriter.cpp"

    INCLUDE_DIRS
        "include"

    REQUIRES
        driver
        esp_driver_i2s
        esp_driver_gpio
        spiffs
)