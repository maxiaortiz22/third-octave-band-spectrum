//
// Created by alex on 21/03/2016.
//
#ifndef __usoundlib_params__
#define __usoundlib_params__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum io_device_type {
    IO_DEVICE_NULL = 0x0,
    IO_DEVICE_OPEN_SL_ES = 0x1,
    IO_DEVICE_LIBUSB = 0x2,
    IO_DEVICE_BLUETOOTH = 0x3,
    IO_DEVICE_OBOE = 0x4
} IoDevice;

typedef enum sample_rate {
    SR_44100 = 0xAC44,
    SR_48000 = 0xBB80,
    SR_96000 = 0x17700
} SampleRate;

typedef enum band_type {
    BAND_TYPE_7B = 0x7,
    BAND_TYPE_13B = 0xD
} BandType;

typedef enum channel_type {
    CENTER_CHANNEL = 0x0,
    MONO_CHANNEL = 0x1,
    STEREO_CHANNEL = 0x2,
    MIC_CHANNEL_MODE = 0x3,
    LEFT_CHANNEL = 0x4,
    RIGHT_CHANNEL = 0x5,
    MIC_LEFT_CHANNEL_MODE = 0x6,
    MIC_RIGHT_CHANNEL_MODE = 0x7

} ChannelType;

typedef enum tone_type {
    AUDIOMETRY_TONE = 0x1,
    REPEATED_TONE = 0x2,
    DEMO_TONE = 0x3,
    USER_REPEATED_TONE = 0x4,
    CONTINUOUS_TONE = 0x5,
    PULSE_TONE_HALF_SEC = 0x6,
    PULSE_TONE_ONE_SEC = 0x7,
    MASKING_STIMULUS = 0x8,
    WARBLE_TONE = 0x9,
    LINEARITY_TEST = 0xA
} ToneType;

typedef enum noise_type {
    PINK_NOISE = 0X1,
    WHITE_NOISE = 0X2,
    NARROW_BAND_PINK_NOISE = 0X3,
    NARROW_BAND_WHITE_NOISE = 0X4,
    NO_NOISE = 0x5,
    SPEECH_TEST_WHITE_NOISE = 0X6
}NoiseType;

typedef enum usound_smart_earphones {
    //VID = 0x040d,
    //PID = 0x3417
    VID = 0x08BB,
    PID = 0x29C6
} UsoundSmartEarphones;

typedef enum boolean {
    TRUE = 0x1,
    FALSE = 0x0
} Boolean;

typedef enum endianess {
    BIG_ENDIAN = 0x1,
    LITTLE_ENDIAN = 0x2
} Endianess;

typedef enum usound_result {
    USOUND_STATE_ALREADY_PLAYING =                     2,
    USOUND_STATE_ALREADY_STOPPED =                     1,
    USOUND_SUCCESS =                                   0,
    USOUND_ERROR_NULL_INSTANCE =                      -1,
    USOUND_ERROR_NO_ENGINE_SET =                      -2,
    USOUND_ERROR_NO_INPUT_DEVICE_SET =                -3,
    USOUND_ERROR_NO_OUTPUT_DEVICE_SET =               -4,
    USOUND_ERROR_NULL_CALLBACK =                      -5,
    USOUND_ERROR_IO_IS_NULL =                         -6,
    USOUND_ERROR_DEVICE_NOT_READY =                   -7,
    USOUND_ERROR_INITIALIZING_ENGINE =                -9,
    USOUND_ERROR_NULL_IO_DEVICE =                     -10,
    USOUND_ERROR_INCONSISTENT_IO_DEVICE =             -11,
    USOUND_ERROR_CREATING_THREAD =                    -12,
    USOUND_ERROR_ENGINE_IS_NULL =                     -13,
    USOUND_ERROR_ENGINE_UNKNOWN_PROCESSOR =           -14,
    USOUND_ERROR_EQUALIZER_IS_NULL =                  -15,
    USOUND_ERROR_EQUALIZER_SET_PARAMETER_ERROR =      -16,
    USOUND_ERROR_FREQUENCY_CUT_IS_NULL =              -17,
    USOUND_ERROR_FREQUENCY_CUT_SET_PARAMETER_ERROR =  -18,
    USOUND_ERROR_LIMITER_IS_NULL =                    -19,
    USOUND_ERROR_LIMITER_SET_PARAMETER_ERROR =        -20,
    USOUND_ERROR_COMPRESSOR_IS_NULL =                 -21,
    USOUND_ERROR_COMPRESSOR_SET_PARAMETER_ERROR =     -22,
    USOUND_ERROR_EXPANDER_IS_NULL =                   -23,
    USOUND_ERROR_EXPANDER_SET_PARAMETER_ERROR =       -24,
    USOUND_ERROR_NOISE_GATE_IS_NULL =                 -25,
    USOUND_ERROR_NOISE_GATE_SET_PARAMETER_ERROR =     -26,
    USOUND_ERROR_MS_MATRIX_IS_NULL =                  -27,
    USOUND_ERROR_MS_MATRIX_SET_PARAMETER_ERROR =      -28,
    USOUND_ERROR_TONE_GENERATOR_IS_NULL =             -29,
    USOUND_ERROR_TONE_GENERATOR_SET_PARAMETER_ERROR = -30,
    USOUND_ERROR_LIBUAC_INIT =                        -31,
    USOUND_ERROR_LIBUAC_NOT_INITIALIZED =             -32,
    USOUND_ERROR_LIBUAC_SETUP =                       -33,
    USOUND_ERROR_LIBUAC_SET_VOLUME =                  -34,
    USOUND_ERROR_LIBUAC_FAILED_SCHEDULING_PLAYBACK = -35,
    USOUND_ERROR_OBOE_STREAMS_WERE_NOT_OPEN = -37,
    USOUND_ERROR_OBOE_COULD_NOT_CLOSE_RECORDER_STREAM = -38,
    USOUND_ERROR_OBOE_COULD_NOT_CLOSE_PLAYER_STREAM = -39,
    USOUND_ERROR_OBOE_COULD_NOT_STOP_STREAM = -40,
    USOUND_ERROR_OBOE_COULD_NOT_STOP_FULL_DUPLEX_STREAMS = -41,
    USOUND_ERROR_OBOE_INVALID_STREAM = -42,
    USOUND_ERROR_OBOE_COULD_NOT_CLOSE_STREAM = -43,
    USOUND_ERROR_OBOE_RECORDER_PROCESSOR_CALLBACK_IS_NULL = -44,
    USOUND_ERROR_OBOE_PLAYER_PROCESSOR_CALLBACK_IS_NULL = -45,
    USOUND_ERROR_OBOE_GAIN_CALLBACK_IS_NULL = -46,
    USOUND_ERROR_OBOE_SET_PARAM_CALLBACK_IS_NULL = -47,
    USOUND_ERROR_OBOE_GET_PARAM_CALLBACK_IS_NULL = -48,
    USOUND_ERROR_OBOE_INPUT_DEVICE_ID_INCONSISTENT = -49,

    USOUND_WARNING_OBOE_FAILED_TO_SET_AUDIO_API_LOOP_WAS_ON = -200,
    USOUND_WARNING_OBOE_INCONSISTENT_DEVICE_ID_SET_REQUEST = -201,

    ErrorBase = -900, // AAUDIO_ERROR_BASE,
    ErrorDisconnected = -899, // AAUDIO_ERROR_DISCONNECTED,
    ErrorIllegalArgument = -898, // AAUDIO_ERROR_ILLEGAL_ARGUMENT,
    ErrorInternal = -896, // AAUDIO_ERROR_INTERNAL,
    ErrorInvalidState = -895, // AAUDIO_ERROR_INVALID_STATE,
    ErrorInvalidHandle = -892, // AAUDIO_ERROR_INVALID_HANDLE,
    ErrorUnimplemented = -890, // AAUDIO_ERROR_UNIMPLEMENTED,
    ErrorUnavailable = -889, // AAUDIO_ERROR_UNAVAILABLE,
    ErrorNoFreeHandles = -888, // AAUDIO_ERROR_NO_FREE_HANDLES,
    ErrorNoMemory = -887, // AAUDIO_ERROR_NO_MEMORY,
    ErrorNull = -886, // AAUDIO_ERROR_NULL,
    ErrorTimeout = -885, // AAUDIO_ERROR_TIMEOUT,
    ErrorWouldBlock = -884, // AAUDIO_ERROR_WOULD_BLOCK,
    ErrorInvalidFormat = -883, // AAUDIO_ERROR_INVALID_FORMAT,
    ErrorOutOfRange = -882, // AAUDIO_ERROR_OUT_OF_RANGE,
    ErrorNoService = -881, // AAUDIO_ERROR_NO_SERVICE,
    ErrorInvalidRate = -880, // AAUDIO_ERROR_INVALID_RATE,
} UsoundResult;

typedef void (* ProcessorCallback_t)(float *, int);
typedef UsoundResult (*SetGainCallback_t)(ChannelType channelType, int newVol);
typedef UsoundResult (*SetParamCallback_t)(int param, float value);
typedef float (*GetParamCallback_t)(int param);

inline char* read_usound_message(UsoundResult uMessage) {
    switch(uMessage) {
        case USOUND_SUCCESS:
            return (char *) "USOUND SUCCESS!";
        case USOUND_ERROR_NULL_INSTANCE:
            return (char *) "USOUND ERROR NULL INSTANCE";
        case USOUND_ERROR_NO_ENGINE_SET:
            return (char *) "USOUND ERROR NO ENGINE SET";
        case USOUND_ERROR_NO_INPUT_DEVICE_SET:
            return (char *) "USOUND ERROR NO INPUT DEVICE SET";
        case USOUND_ERROR_NO_OUTPUT_DEVICE_SET:
            return (char *) "USOUND ERROR NO OUTPUT DEVICE SET";
        case USOUND_ERROR_NULL_CALLBACK:
            return (char *) "USOUND ERROR NULL CALLBACK";
        case USOUND_ERROR_IO_IS_NULL:
            return (char *) "USOUND ERROR IO IS NULL";
        case USOUND_ERROR_DEVICE_NOT_READY:
            return (char *) "USOUND ERROR DEVICE NOT READY";
        default:
            return (char *) "USOUND ERROR UNKNOWN";
    }
}

#ifdef __cplusplus
};
#endif

#endif /* __usoundlib_params__ */
