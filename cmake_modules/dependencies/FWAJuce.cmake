fwa_fetch(
        juce
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG 6.0.4)

fwa_add_dependency(juce STATIC)

fwa_target_link(
        juce
        PRIVATE
        fwa_asio
        juce::juce_core
        juce::juce_events
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_dsp
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

fwa_target_set(
        juce
        POSITION_INDEPENDENT_CODE TRUE
        VISIBILITY_INLINES_HIDDEN TRUE
        C_VISIBILITY_PRESET hidden
        CXX_VISIBILITY_PRESET hidden)

fwa_target_definitions(
        juce
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_ASIO=1
        INTERFACE
        $<TARGET_PROPERTY:fwa_juce,COMPILE_DEFINITIONS>)

fwa_target_include(
        juce
        INTERFACE
        $<TARGET_PROPERTY:fwa_juce,INCLUDE_DIRECTORIES>)

fwa_target_precompile(
        juce
        INTERFACE
        <juce_core/juce_core.h>
        <juce_events/juce_events.h>
        <juce_audio_basics/juce_audio_basics.h>
        <juce_audio_devices/juce_audio_devices.h>
        <juce_audio_formats/juce_audio_formats.h>
        <juce_dsp/juce_dsp.h>)
