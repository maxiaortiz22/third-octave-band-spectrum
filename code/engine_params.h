#ifndef __uSound__engine_params__
#define __uSound__engine_params__

typedef enum enum_mask {
	MASK_PROCESSOR 	= 0xF000,
	MASK_SPECIAL	= 0x0F00,
	MASK_BAND 		= 0x00F0,
	MASK_PARAM 		= 0x000F
} EnumMask;

typedef enum engine_param {
	ENGINE 		= 0x1000,

	EN_BYPASS 	= 0x0,

	EN_CHANNELS 		= 0x1,
	EN_SAMPLE_RATE 		= 0x2,
	EN_BUFFER_SIZE 		= 0x3,
    EN_PROCESS_MODE     = 0x4,
    EN_BANDS            = 0x5
} EngineParam;

typedef enum processor {
    EQUALIZER       = 0x2000,
    FREQUENCY_CUT   = 0x3000,
    LIMITER 		= 0x4000,
    COMPRESSOR 		= 0x5000,
	EXPANDER 		= 0x6000,
	WAV_READER		= 0x7000,
    NOISE_GATE 		= 0x8000,
    SPECTRUM		= 0x9000,
    VUMETER          = 0xA000,
	TONE_GENERATOR  = 0xB000,
    SECOND_TONE_GENERATOR = 0xC000,
    MS_MATRIX       = 0xD000,
    NOISE_GENERATOR      = 0xE000,
} Processor;

typedef enum equalizer_param {
	EQ_BAND_1 = 0x10,
	EQ_BAND_2 = 0x20,
	EQ_BAND_3 = 0x30,
	EQ_BAND_4 = 0x40,
	EQ_BAND_5 = 0x50,
	EQ_BAND_6 = 0x60,
	EQ_BAND_7 = 0x70,
	EQ_BAND_8 = 0x80,
	EQ_BAND_9 = 0x90,
	EQ_BAND_10 = 0xA0,
	EQ_BAND_11 = 0xB0,
	EQ_BAND_12 = 0xC0,
	EQ_BAND_13 = 0xD0,

	EQ_BYPASS = 0x0,

	EQ_IN_GAIN 			= 0x1,
	EQ_OUT_GAIN 		= 0x2,

	EQ_FILTER_FREQ 		= 0x1,
	EQ_LEFT_GAIN 		= 0x2,
	EQ_RIGHT_GAIN 		= 0x3,
	EQ_FILTER_Q_PARAM 	= 0x4,
	EQ_FILTER_TYPE 		= 0x5,
	EQ_FILTER_ENABLED 	= 0x6,
	EQ_NUM_BANDS		= 0x7,
	EQ_BOOST_VOLUME		= 0x8,
	EQ_LEFT_CHANNEL_GAIN    = 0x9,
	EQ_RIGHT_CHANNEL_GAIN	 = 0xA,

	EQ_SMOOTH_CHANGE	= 0x100,

	EQ_MAX_TUNNING		= 0x200,
	EQ_MIN_TUNNING		= 0x300
} EqualizerParam;

typedef enum frequency_cut_param {
	FC_LOWPASS 			= 0x10,
	FC_HIGHPASS 		= 0x20,

	FC_BYPASS 			= 0x0,

	FC_FILTER_FREQ 		= 0x1,
	FC_LEFT_GAIN 		= 0x2,
	FC_RIGHT_GAIN 		= 0x3,
	FC_FILTER_Q_PARAM 	= 0x4,
	FC_FILTER_TYPE 		= 0x5,
	FC_FILTER_ENABLED 	= 0x6,
	FC_CHANNEL_TYPE 	= 0x7,
	FC_LP_BYPASS 			= 0x8,
} FrequencyCutParam;

typedef enum compressor_param {
	CO_BYPASS 		= 0x0,

	CO_ATTACK 		= 0x1,
	CO_RELEASE 		= 0x2,
	CO_THRESHOLD 	= 0x3,
	CO_RATIO 		= 0x4
} CompressorParam;

typedef enum expander_param {
	EX_BYPASS 		= 0x0,

	EX_ATTACK 		= 0x1,
	EX_RELEASE 		= 0x2,
	EX_THRESHOLD 	= 0x3,
	EX_RATIO 		= 0x4
} ExpanderParam;

typedef enum limiter_param {
	LI_BYPASS 		= 0x0,

	LI_ATTACK 		= 0x1,
	LI_RELEASE 		= 0x2,
	LI_THRESHOLD 	= 0x3,
	LI_RATIO 		= 0x4
} LimiterParam;

typedef enum noise_gate_param {
	NG_BYPASS 		= 0x0,

	NG_ATTACK 		= 0x1,
	NG_DECAY 		= 0x2,
	NG_THRESHOLD 	= 0x3
} NoiseGateParam;

typedef enum spectrum_param {
	SP_LEFT_CHANNEL         = 0x10,
	SP_RIGHT_CHANNEL        = 0x20,
	SP_BYPASS 				= 0x30,
	SP_TYPE_CHANGE			= 0x40,
	SP_WINDOW				= 0x50,

	SP_RMS_PEAK				= 0x90,
	//SP_SPECTRUM_MAX			= 0x2,
	//SP_AVERAGES_MAX			= 0x3,
	//SP_SPECTRUM_MAX_INDEX	= 0x4,
	//SP_AVERAGES_MAX_INDEX	= 0x5,
	SP_SPECTRUM_MAX_FREQ	= 0x60,
	SP_AVERAGES_MAX_FREQ	= 0x70,
	SP_FREQ_STEP_SIZE       = 0x80
} SpectrumParam;

typedef enum wav_reader_param {
	WR_BYPASS			= 0x0,

} WavReaderParam;

typedef enum noise_generator_param {
	NSG_BYPASS			= 0x0,
	NSG_CHANNEL 		= 0X1,
	NSG_GAIN			= 0X2,
	NSG_NOISE_TYPE		= 0X3,
	NSG_FREQ_BAND		= 0x4,
	NSG_STIMULUS_MODE   = 0x5

} NoiseGeneratorParam;

typedef enum vumeter_param {
	VU_BYPASS           = 0x1,
	VU_MIC_CONSTANT     = 0X2,
	VU_MAX_PEAK_LEVEL  	= 0x3,
	VU_AVG_LEVEL		= 0x4,
	VU_INSTANTANEOUS_LEVEL		= 0x5,
	VU_INTEGRATION_TIME = 0x6,
	VU_MIN_PEAK_LEVEL  	= 0x7


} VumeterParam;

typedef enum multi_band_compressor_param {
    CROSSOVER1 = 0x10,
    CROSSOVER2 = 0x20,
    CROSSOVER3 = 0x30,
    EXPANDER1 = 0x40,
    EXPANDER2 = 0x50,
    EXPANDER3 = 0x60,
    COMPRESSOR1 = 0x70,
    COMPRESSOR2 = 0x80,
    COMPRESSOR3 = 0x90,
    MBC_LOWPASS = 0xA0,
    MBC_HIGHPASS = 0xB0,
    
    
    MBC_BYPASS = 0x0,
    
    MBC_FILTER_FREQ 	= 0x1,
    MBC_LEFT_GAIN 		= 0x2,
    MBC_RIGHT_GAIN 		= 0x3,
    MBC_FILTER_Q_PARAM 	= 0x4,
    MBC_FILTER_TYPE 	= 0x5,
    MBC_FILTER_ENABLED 	= 0x6,
    MBC_ATTACK 		= 0x7,
    MBC_RELEASE 	= 0x8,
    MBC_THRESHOLD 	= 0x9,
    MBC_RATIO 		= 0xA
    
    
} MultiBandCompressorParam;

typedef enum tone_gen_param {
	TG_BYPASS	= 0x0,

	TG_FREQ		= 0x1,
	TG_GAIN		= 0x2,
	TG_PAN		= 0x3,
	TG_INTERVAL		= 0x4,
	TG_INIT_GAIN		= 0x5,
	TG_FINAL_GAIN		= 0x6,
	TG_GAIN_CHANGE		= 0x7,
	TG_GAIN_THRESHOLD   = 0x8,
	TG_TONE_TYPE		= 0x9,
	TG_PULSE_INTERVAL	= 0xA,
	TG_INTERCOM_VOLUME = 0xB

} ToneGenParam;

typedef enum ms_matrix_param {
	MS_BYPASS	= 0x0,
	MS_SIDE_FACTOR	= 0x1,
	MS_MID_FACTOR = 0x2,

} MsMatrixParam;
typedef enum tone_pan_type {
	PT_LEFT		= -1,
	PT_RIGHT	= 1
} TonePanType;

typedef enum spectrum_change_type {
    TC_NONE				= 0,
    TC_AUDIOMETRY		= 1,
	TC_AUDIOM_SPEC		= 2
} SpectrumChangeType;

typedef enum fitting_method {
    FM_TEST        = 0,
    FM_NAL_R_V1    = 1,
	FM_NAL_R_V2    = 2,
	FM_NAL_R_V3    = 3,
	FM_BERGER      = 4,
	FM_POGO_II     = 5,
	FM_USOUND      = 6
} FittingMethod;

typedef enum filter_type {
	FT_NONE				= 0,
	FT_LOWPASS_ORDER1	= 1,
	FT_LOWPASS_ORDER2	= 2,
	FT_LOWPASS_ORDER3	= 3,
	FT_LOWPASS_ORDER4	= 4,
	FT_HIGHPASS_ORDER1	= 5,
	FT_HIGHPASS_ORDER2	= 6,
	FT_HIGHPASS_ORDER3	= 7,
	FT_HIGHPASS_ORDER4	= 8,
	FT_LOWSHELF			= 9,
	FT_HIGHSHELF		= 10,
	FT_PEAK				= 11,
	FT_NOTCH			= 12
} FilterType;

typedef enum spectrum_window_type {
	WT_NONE = 0
} SpectrumWindowType;

typedef enum spectrum_scan_type {
	TS_RMS	= 0,
	TS_PEAK	= 1
} SpectrumScanType;

typedef enum engine_process_mode {
	EPM_LEGACY		= 1,
	EPM_MULTIBAND	= 2
} EngineProcessMode;

typedef enum engine_type {
	NO_ENGINE = 0x0,
	UTEST_ENGINE = 0x1,
	DEMO_TONE_ENGINE = 0x2,
	UAWAY_ENGINE = 0x3,
	TONE_GENERATOR_ENGINE = 0x4,
	VUMETER_ENGINE = 0x5
} EngineType;

#endif
