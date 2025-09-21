project(nyoomPlugin VERSION 0.0.1)

juce_add_plugin(nyoomPlugin
        VERSION 0.0.1

        COMPANY_NAME SonicShark

        PLUGIN_MANUFACTURER_CODE Shrk
        PLUGIN_CODE shNy

        FORMATS AU VST3 Standalone
        PRODUCT_NAME "nyoom")

target_sources(nyoomPlugin
        PRIVATE
        PluginEditor.cpp
        PluginProcessor.cpp
        Effect.cpp
        Delay.cpp
        Doppler.cpp
        Interpolation.cpp
)

target_link_libraries(nyoomPlugin
        PRIVATE
        juce::juce_audio_utils
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)