#include <stdint.h>

// https://robey.lag.net/2010/01/23/tiny-monospace-font.html
const uint8_t font_4x6[] = {0, 0, 0, 0, 0, 0, 64, 64, 64, 0, 64, 0, 160, 160, 0, 0, 0, 0, 160, 224, 160, 224, 160, 0, 96, 192, 96, 192, 64, 0, 128, 32, 64, 128, 32, 0, 192, 192, 224, 160, 96, 0, 64, 64, 0, 0, 0, 0, 32, 64, 64, 64, 32, 0, 128, 64, 64, 64, 128, 0, 160, 64, 160, 0, 0, 0, 0, 64, 224, 64, 0, 0, 0, 0, 0, 64, 128, 0, 0, 0, 224, 0, 0, 0, 0, 0, 0, 0, 64, 0, 32, 32, 64, 128, 128, 0, 96, 160, 160, 160, 192, 0, 64, 192, 64, 64, 64, 0, 192, 32, 64, 128, 224, 0, 192, 32, 64, 32, 192, 0, 160, 160, 224, 32, 32, 0, 224, 128, 192, 32, 192, 0, 96, 128, 224, 160, 224, 0, 224, 32, 64, 128, 128, 0, 224, 160, 224, 160, 224, 0, 224, 160, 224, 32, 192, 0, 0, 64, 0, 64, 0, 0, 0, 64, 0, 64, 128, 0, 32, 64, 128, 64, 32, 0, 0, 224, 0, 224, 0, 0, 128, 64, 32, 64, 128, 0, 224, 32, 64, 0, 64, 0, 64, 160, 224, 128, 96, 0, 64, 160, 224, 160, 160, 0, 192, 160, 192, 160, 192, 0, 96, 128, 128, 128, 96, 0, 192, 160, 160, 160, 192, 0, 224, 128, 224, 128, 224, 0, 224, 128, 224, 128, 128, 0, 96, 128, 224, 160, 96, 0, 160, 160, 224, 160, 160, 0, 224, 64, 64, 64, 224, 0, 32, 32, 32, 160, 64, 0, 160, 160, 192, 160, 160, 0, 128, 128, 128, 128, 224, 0, 160, 224, 224, 160, 160, 0, 160, 224, 224, 224, 160, 0, 64, 160, 160, 160, 64, 0, 192, 160, 192, 128, 128, 0, 64, 160, 160, 224, 96, 0, 192, 160, 224, 192, 160, 0, 96, 128, 64, 32, 192, 0, 224, 64, 64, 64, 64, 0, 160, 160, 160, 160, 96, 0, 160, 160, 160, 64, 64, 0, 160, 160, 224, 224, 160, 0, 160, 160, 64, 160, 160, 0, 160, 160, 64, 64, 64, 0, 224, 32, 64, 128, 224, 0, 224, 128, 128, 128, 224, 0, 0, 128, 64, 32, 0, 0, 224, 32, 32, 32, 224, 0, 64, 160, 0, 0, 0, 0, 0, 0, 0, 0, 224, 0, 128, 64, 0, 0, 0, 0, 0, 192, 96, 160, 224, 0, 128, 192, 160, 160, 192, 0, 0, 96, 128, 128, 96, 0, 32, 96, 160, 160, 96, 0, 0, 96, 160, 192, 96, 0, 32, 64, 224, 64, 64, 0, 0, 96, 160, 224, 32, 64, 128, 192, 160, 160, 160, 0, 64, 0, 64, 64, 64, 0, 32, 0, 32, 32, 160, 64, 128, 160, 192, 192, 160, 0, 192, 64, 64, 64, 224, 0, 0, 224, 224, 224, 160, 0, 0, 192, 160, 160, 160, 0, 0, 64, 160, 160, 64, 0, 0, 192, 160, 160, 192, 128, 0, 96, 160, 160, 96, 32, 0, 96, 128, 128, 128, 0, 0, 96, 192, 96, 192, 0, 64, 224, 64, 64, 96, 0, 0, 160, 160, 160, 96, 0, 0, 160, 160, 224, 64, 0, 0, 160, 224, 224, 224, 0, 0, 160, 64, 64, 160, 0, 0, 160, 160, 96, 32, 64, 0, 224, 96, 192, 224, 0, 96, 64, 128, 64, 96, 0, 64, 64, 0, 64, 64, 0, 192, 64, 32, 64, 192, 0, 96, 192, 0, 0, 0, 0, 224, 224, 224, 224, 224, 0};

const uint16_t mimi_art[] = {0, 0, 6192, 15480, 15480, 7920, 8176, 4112, 10280, 9096, 28684, 28684, 8200, 4144, 4080, 7184, 0, 0, 0, 0, 0, 0, 0, 4064, 6096, 7280, 4080, 4080, 8176, 4032, 0, 0, 0, 0, 6192, 15480, 15480, 7920, 8176, 4112, 10280, 9096, 12302, 12302, 4100, 2056, 4080, 2104, 0, 0, 0, 0, 0, 0, 0, 4064, 6096, 7280, 4080, 4080, 4088, 2032, 0, 0, 0, 14392, 31868, 32508, 8176, 4112, 11368, 59438, 41866, 17284, 9096, 8456, 8200, 4112, 4064, 7280, 0, 0, 0, 0, 0, 4064, 5008, 6096, 23668, 15480, 7280, 7920, 8176, 4064, 0, 0, 0, 14392, 31868, 32508, 8176, 4112, 11368, 59438, 41866, 17284, 9096, 8456, 8200, 4112, 4064, 7280, 0, 0, 0, 0, 0, 4064, 5008, 6096, 23668, 15480, 7280, 7920, 8176, 4064, 0, 0, 0, 0, 6192, 15480, 15480, 7920, 8176, 4112, 8200, 8200, 24588, 24588, 8584, 4688, 3696, 7608, 0, 0, 0, 0, 0, 0, 0, 4064, 8176, 8176, 8176, 8176, 7792, 3488, 384, 0, 0, 14392, 31868, 32508, 8176, 4112, 10280, 9288, 27564, 33666, 24588, 8200, 4112, 4064, 3168, 6192, 0, 0, 0, 0, 0, 4064, 6096, 7088, 5200, 31868, 8176, 8176, 4064, 0, 0, 0, 0, 0, 0, 0, 15480, 32508, 32764, 4112, 11368, 8456, 8200, 10920, 9288, 4112, 4064, 7280, 0, 0, 0, 0, 0, 0, 0, 4064, 5008, 7920, 8176, 5456, 7088, 4064, 0, 0, 6192, 15480, 15480, 7920, 8176, 4112, 10280, 58446, 41866, 17284, 9096, 8200, 4112, 4064, 3168, 6192, 0, 0, 0, 0, 0, 4064, 6096, 7088, 23668, 15480, 7280, 8176, 4064, 0, 0, 0, 0, 0, 0, 0, 15480, 32508, 32764, 4112, 11368, 8456, 8200, 10920, 9288, 4112, 4064, 7280, 0, 0, 0, 0, 0, 0, 0, 4064, 5008, 7920, 8176, 5456, 7088, 4064, 0, 0, 0, 0, 0, 0, 15480, 32508, 32764, 4112, 11368, 8456, 8200, 10920, 9288, 4112, 4064, 7280, 0, 0, 0, 0, 0, 0, 0, 4064, 5008, 7920, 8176, 5456, 7088, 4064, 0, 0, 0, 0, 0, 0, 15480, 32508, 32764, 4112, 11368, 8456, 8200, 10920, 9288, 4112, 4064, 7280, 0, 0, 0, 0, 0, 0, 0, 4064, 5008, 7920, 8176, 5456, 7088, 4064, 0, 0, 0, 0, 0, 0, 15480, 32508, 32764, 4112, 11368, 8456, 8200, 10920, 9288, 4112, 4064, 7280, 0, 0, 0, 0, 0, 0, 0, 4064, 5008, 7920, 8176, 5456, 7088, 4064, 0, 0, 0, 0, 920, 1980, 1916, 4088, 4080, 4112, 8328, 15368, 4268, 4170, 4106, 2076, 2040, 3612, 0, 0, 0, 0, 0, 0, 0, 4064, 8048, 1008, 3920, 4020, 4084, 2016, 0, 0, 0, 0, 896, 1820, 4094, 4223, 12051, 12200, 12168, 9992, 4264, 4264, 4172, 2068, 8184, 14448, 0, 0, 0, 0, 0, 3968, 4320, 4176, 4208, 6384, 3920, 3920, 4016, 2024, 0, 0, 0, 0, 0, 1912, 3836, 8188, 8160, 8224, 17168, 30736, 8528, 8528, 8344, 4136, 4080, 7224, 0, 0, 0, 0, 0, 0, 0, 8128, 15584, 2016, 7840, 7840, 8032, 4048, 0, 0, 0, 0, 0, 920, 1980, 1916, 4088, 4080, 4112, 8584, 15368, 4264, 4392, 21708, 30228, 32760, 0, 0, 0, 0, 0, 0, 0, 0, 4064, 7792, 1008, 3920, 3792, 2864, 2536, 0, 0, 0, 224, 992, 1948, 4094, 4159, 8403, 12104, 12040, 9736, 4264, 4264, 21580, 30228, 32760, 0, 0, 0, 0, 0, 0, 4032, 7968, 4272, 4336, 6640, 3920, 3920, 2992, 2536, 0, 0, 0, 1840, 3960, 3832, 8176, 8160, 8224, 16656, 30736, 14552, 24628, 24788, 4152, 4064, 1728, 0, 0, 0, 0, 0, 0, 0, 8128, 16096, 2016, 1824, 8136, 7976, 4032, 0, 0, 0, 896, 1976, 3964, 4092, 8184, 8224, 16656, 30736, 8976, 9240, 8980, 4148, 24568, 30776, 8, 0, 0, 0, 0, 0, 0, 8128, 16096, 2016, 7392, 7136, 7400, 4040, 0, 0, 0, 0, 0, 0, 0, 48, 74, 12798, 31246, 31748, 31748, 15444, 52567, 64806, 64516, 15496, 1008, 0, 0, 0, 0, 0, 48, 0, 496, 1016, 1016, 936, 680, 728, 1016, 880, 0, 0, 0, 12312, 30780, 31868, 16376, 8176, 4112, 59404, 33060, 24840, 8456, 8248, 4152, 16368, 28672, 0, 0, 0, 0, 0, 0, 0, 4064, 6128, 32472, 7920, 7920, 8128, 4032, 0, 0, 0, 0, 2080, 7280, 7280, 8176, 8176, 4112, 10280, 8200, 14648, 8456, 20500, 28700, 4064, 1728, 0, 0, 0, 0, 0, 0, 0, 4064, 6096, 8176, 1728, 7920, 12264, 4064, 0, 0, 0, 0, 48, 14456, 31864, 32496, 8176, 4240, 11368, 8200, 24844, 25228, 8200, 4112, 4080, 7224, 0, 0, 0, 0, 0, 0, 0, 3936, 5008, 8176, 7920, 7536, 8176, 4064, 0, 0, 0, 0, 6192, 15480, 15480, 7920, 8176, 5200, 10920, 9288, 12558, 24590, 4100, 2056, 4080, 2104, 0, 0, 0, 0, 0, 0, 0, 2976, 5456, 7088, 3824, 8176, 4088, 2032, 0, 0, 0, 6192, 15480, 15480, 7920, 8176, 4752, 11368, 8456, 29404, 28700, 8200, 8200, 4112, 8160, 4208, 0, 0, 0, 0, 0, 0, 3424, 5008, 7920, 3360, 4064, 8176, 8176, 4064, 0, 0, 0, 0, 48, 14456, 31864, 32496, 8176, 4112, 10280, 9096, 9540, 6436, 4100, 2056, 2032, 3680, 0, 0, 0, 0, 0, 0, 0, 4064, 6096, 7280, 6840, 1752, 4088, 2032, 0, 0, 896, 2012, 4094, 4094, 12310, 8456, 4296, 8200, 12428, 4170, 4138, 4332, 2064, 2016, 448, 192, 0, 0, 0, 0, 4064, 7920, 3888, 8176, 3952, 4020, 4052, 3856, 2016, 0, 0, 0, 0, 896, 7104, 15840, 7920, 4080, 4080, 4240, 8456, 14344, 22024, 63496, 17940, 24628, 57304, 14336, 0, 0, 0, 0, 0, 0, 0, 3936, 7920, 2032, 2544, 2032, 14824, 8136, 0, 0, 0, 0, 1840, 3960, 3832, 8176, 8160, 8224, 16662, 30777, 8541, 8339, 8209, 4159, 4080, 7224, 0, 0, 0, 0, 0, 0, 0, 8128, 16096, 1990, 7842, 8044, 8174, 4032, 0, 0, 0, 0, 6192, 15480, 15480, 7920, 8176, 4112, 8200, 14296, 26668, 27756, 11176, 6192, 4080, 7224, 0, 0, 0, 0, 0, 0, 0, 4064, 8176, 2080, 6096, 5008, 5200, 1984, 0, 0, 0, 0, 896, 1820, 4094, 4223, 12051, 12200, 12168, 9992, 4264, 4264, 4172, 2068, 8184, 14448, 0, 0, 0, 0, 0, 3968, 4320, 4176, 4208, 6384, 3920, 3920, 4016, 2024, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

const uint16_t terrier_art[] = {1536, 2944, 6496, 8208, 10384, 18568, 17416, 19272, 8296, 14372, 8212, 20564, 28722, 37665, 35505, 32510, 0, 1024, 1664, 8160, 5984, 14192, 15344, 13488, 8080, 2008, 8168, 12200, 4044, 27870, 29774, 0, 0, 1536, 2944, 6496, 8208, 10384, 18568, 17416, 19272, 8296, 14372, 8212, 20562, 45105, 42897, 30846, 0, 0, 1024, 1664, 8160, 5984, 14192, 15344, 13488, 8080, 2008, 8168, 12204, 20430, 22638, 0, 1536, 2944, 6496, 8208, 12432, 26952, 17416, 53016, 44844, 38476, 16404, 12324, 20514, 37665, 35505, 32510, 0, 1024, 1664, 8160, 3936, 5808, 15344, 12512, 20688, 27056, 16360, 4056, 12252, 27870, 29774, 0, 384, 1760, 2640, 4104, 5160, 10836, 8324, 9156, 21450, 18834, 8196, 12300, 20490, 37769, 35409, 32382, 0, 256, 1440, 4080, 3024, 5544, 8056, 7224, 11316, 13932, 8184, 4080, 12276, 27766, 29742, 0, 1536, 2944, 6496, 8208, 8208, 16392, 16392, 16392, 8776, 14916, 8900, 16932, 17506, 34721, 35473, 32510, 0, 1024, 1664, 8160, 8160, 16368, 16368, 16368, 7600, 1464, 7480, 15832, 15260, 30814, 29806, 0, 0, 0, 0, 1536, 2944, 6496, 8208, 10512, 22920, 17416, 19016, 8264, 14374, 20529, 41097, 65535, 0, 0, 0, 0, 1024, 1664, 8160, 5856, 9840, 15344, 13744, 8112, 2008, 12238, 24438, 0, 0, 0, 0, 0, 3072, 5888, 12992, 16416, 21024, 45840, 34832, 38024, 16454, 28721, 41097, 65535, 0, 0, 0, 0, 0, 2048, 3328, 16320, 11712, 19680, 30688, 27504, 16312, 4046, 24438, 0, 0, 1536, 2944, 6496, 8208, 12432, 22920, 17416, 20296, 9832, 14372, 8212, 20562, 45105, 42897, 30846, 0, 0, 1024, 1664, 8160, 3936, 9840, 15344, 12464, 6544, 2008, 8168, 12204, 20430, 22638, 0, 1536, 2944, 6496, 8208, 12432, 22920, 17416, 19272, 8312, 14340, 16436, 20516, 28730, 37665, 35505, 32510, 0, 1024, 1664, 8160, 3936, 9840, 15344, 13488, 8064, 2040, 16328, 12248, 4036, 27870, 29774, 0, 0, 0, 0, 0, 0, 0, 3072, 5888, 12992, 16416, 16444, 45850, 34887, 54689, 41521, 32767, 0, 0, 0, 0, 0, 0, 0, 2048, 3328, 16320, 16320, 19684, 30648, 10846, 24014, 0, 0, 0, 0, 0, 0, 3072, 5888, 12992, 16416, 16432, 45852, 34898, 37966, 24994, 37425, 32767, 0, 0, 0, 0, 0, 0, 2048, 3328, 16320, 16320, 19680, 30636, 27568, 7772, 28110, 0, 1536, 2944, 6496, 8208, 8208, 22920, 16904, 17736, 8296, 14372, 8212, 20564, 28722, 37665, 35505, 32510, 0, 1024, 1664, 8160, 8160, 9840, 15856, 15024, 8080, 2008, 8168, 12200, 4044, 27870, 29774, 0, 0, 1536, 2944, 6496, 8208, 12432, 26952, 17416, 19272, 8296, 14372, 8212, 20562, 45105, 42897, 30846, 0, 0, 1024, 1664, 8160, 3936, 5808, 15344, 13488, 8080, 2008, 8168, 12204, 20430, 22638, 0, 1536, 2944, 6496, 8208, 10384, 18568, 17416, 20296, 9832, 13860, 8212, 16458, 28730, 37665, 38065, 31982, 0, 1024, 1664, 8160, 5984, 14192, 15344, 12464, 6544, 2520, 8168, 16308, 4036, 27870, 26702, 0, 0, 1536, 2944, 6496, 8208, 12432, 22920, 17416, 19272, 8296, 14372, 8212, 20562, 45105, 42897, 30846, 0, 0, 1024, 1664, 8160, 3936, 9840, 15344, 13488, 8080, 2008, 8168, 12204, 20430, 22638, 0, 0, 3072, 6016, 4704, 8208, 8720, 16904, 20488, 27720, 8296, 14372, 12612, 23110, 41409, 36929, 32766, 0, 0, 2048, 3456, 8160, 7648, 15856, 12272, 5040, 8080, 2008, 3768, 9656, 24126, 28606, 0, 0, 3072, 6016, 4704, 8208, 8720, 17672, 20488, 31816, 14440, 14372, 12612, 22854, 41153, 36929, 32766, 0, 0, 2048, 3456, 8160, 7648, 15088, 12272, 944, 1936, 2008, 3768, 9912, 24382, 28606, 0, 3072, 6016, 4704, 8208, 8720, 16904, 20488, 27720, 8296, 14372, 12612, 21060, 20932, 21314, 19106, 32510, 0, 2048, 3456, 8160, 7648, 15856, 12272, 5040, 8080, 2008, 3768, 11704, 11832, 11452, 13404, 0, 0, 3072, 6016, 4704, 8208, 8720, 16904, 20488, 27724, 15140, 13346, 21282, 36897, 41761, 38577, 31854, 0, 0, 2048, 3456, 8160, 7648, 15856, 12272, 5040, 1240, 3036, 11484, 28638, 23774, 26702, 0, 0, 0, 0, 0, 0, 0, 3072, 5888, 12992, 16416, 16444, 45850, 34887, 54689, 41521, 32767, 0, 0, 0, 0, 0, 0, 0, 2048, 3328, 16320, 16320, 19684, 30648, 10846, 24014, 0, 1536, 2944, 6496, 8208, 10384, 18568, 16904, 16968, 8808, 10340, 9508, 24036, 20514, 37665, 35505, 32510, 0, 1024, 1664, 8160, 5984, 14192, 15856, 15792, 7568, 6040, 6872, 8728, 12252, 27870, 29774, 0, 0, 1536, 2944, 6496, 8208, 12432, 26952, 17416, 19256, 28744, 18444, 8212, 20530, 45073, 42897, 30846, 0, 0, 1024, 1664, 8160, 3936, 5808, 15344, 13504, 4016, 14320, 8168, 12236, 20462, 22638, 0, 0, 1536, 2944, 6496, 8208, 8720, 22920, 17416, 19272, 8296, 14372, 8212, 20562, 45105, 42897, 30846, 0, 0, 1024, 1664, 8160, 7648, 9840, 15344, 13488, 8080, 2008, 8168, 12204, 20430, 22638, 0, 1536, 2944, 6496, 8208, 10384, 21832, 18568, 16968, 8296, 14356, 12340, 24676, 20514, 45857, 35505, 32510, 0, 1024, 1664, 8160, 5984, 10928, 14192, 15792, 8080, 2024, 4040, 8088, 12252, 19678, 29774, 0, 1536, 2944, 6496, 8208, 14736, 18568, 17416, 19272, 8296, 14372, 8212, 20564, 28722, 37665, 35505, 32510, 0, 1024, 1664, 8160, 1632, 14192, 15344, 13488, 8080, 2008, 8168, 12200, 4044, 27870, 29774, 0, 1536, 2944, 6496, 8208, 10384, 18568, 17416, 20360, 10824, 13092, 9636, 20516, 20514, 37665, 35505, 32510, 0, 1024, 1664, 8160, 5984, 14192, 15344, 12400, 5552, 3288, 6744, 12248, 12252, 27870, 29774, 0, 3072, 6016, 4704, 8208, 29200, 16776, 8200, 16456, 28776, 6180, 12436, 20580, 20548, 21314, 19106, 32510, 0, 2048, 3456, 8160, 3552, 15984, 8176, 16304, 3984, 2008, 3944, 12184, 12216, 11452, 13404, 0, 1536, 3008, 2352, 4104, 4232, 8452, 10244, 13858, 8290, 14370, 13124, 64580, 25540, 24708, 18312, 31992, 0, 1024, 1728, 4080, 3952, 7928, 6136, 2524, 8092, 2012, 3256, 952, 7224, 8056, 14448, 0, 3072, 6016, 4704, 8208, 8720, 16904, 20488, 27726, 8265, 14405, 12615, 20805, 20679, 21314, 19106, 32510, 0, 2048, 3456, 8160, 7648, 15856, 12272, 5040, 8118, 1978, 3768, 11962, 12088, 11452, 13404, 0, 1536, 2944, 6496, 8208, 8208, 16392, 16392, 16392, 9192, 15380, 9780, 17876, 17426, 34801, 35473, 32510, 0, 1024, 1664, 8160, 8160, 16368, 16368, 16368, 7184, 1000, 6600, 14888, 15340, 30734, 29806, 0, 1536, 2944, 6496, 8208, 12432, 22920, 50204, 53028, 25928, 12828, 8212, 20532, 20530, 37665, 35505, 32510, 0, 1024, 1664, 8160, 3936, 9840, 15328, 12504, 6832, 3552, 8168, 12232, 12236, 27870, 29774, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

const uint16_t lop_art[] = {3072, 15040, 10592, 8208, 10384, 18568, 17416, 19272, 8296, 14372, 8212, 20564, 28722, 37665, 35505, 32510, 0, 1024, 5760, 8160, 5984, 14192, 15344, 13488, 8080, 2008, 8168, 12200, 4044, 27870, 29774, 0, 0, 3072, 15040, 10592, 8208, 10384, 18568, 17416, 19272, 8296, 14372, 8212, 20562, 45105, 42897, 30846, 0, 0, 1024, 5760, 8160, 5984, 14192, 15344, 13488, 8080, 2008, 8168, 12204, 20430, 22638, 0, 3072, 15040, 10592, 8208, 12432, 26952, 17416, 53016, 44844, 38476, 16404, 12324, 20514, 37665, 35505, 32510, 0, 1024, 5760, 8160, 3936, 5808, 15344, 12512, 20688, 27056, 16360, 4056, 12252, 27870, 29774, 0, 384, 2768, 5736, 4104, 5160, 10836, 8324, 9156, 21450, 18834, 8196, 12300, 20490, 37769, 35409, 32382, 0, 256, 2448, 4080, 3024, 5544, 8056, 7224, 11316, 13932, 8184, 4080, 12276, 27766, 29742, 0, 1664, 6976, 10528, 8208, 8208, 16392, 16392, 16392, 8776, 14916, 8900, 16932, 17506, 34721, 35473, 32510, 0, 1152, 5824, 8160, 8160, 16368, 16368, 16368, 7600, 1464, 7480, 15832, 15260, 30814, 29806, 0, 0, 0, 0, 3072, 15040, 10592, 8208, 10512, 22920, 17416, 19016, 8264, 14374, 20529, 41097, 65535, 0, 0, 0, 0, 1024, 5760, 8160, 5856, 9840, 15344, 13744, 8112, 2008, 12238, 24438, 0, 0, 0, 0, 0, 6144, 29952, 21184, 16416, 21024, 45840, 34832, 38024, 16454, 28721, 41097, 65535, 0, 0, 0, 0, 0, 2048, 11520, 16320, 11712, 19680, 30688, 27504, 16312, 4046, 24438, 0, 0, 3072, 15040, 10592, 8208, 12432, 22920, 17416, 20296, 9832, 14372, 8212, 20562, 45105, 42897, 30846, 0, 0, 1024, 5760, 8160, 3936, 9840, 15344, 12464, 6544, 2008, 8168, 12204, 20430, 22638, 0, 3072, 15040, 10592, 8208, 12432, 22920, 17416, 19272, 8312, 14340, 16436, 20516, 28730, 37665, 35505, 32510, 0, 1024, 5760, 8160, 3936, 9840, 15344, 13488, 8064, 2040, 16328, 12248, 4036, 27870, 29774, 0, 0, 0, 0, 0, 0, 0, 6144, 30080, 21184, 16416, 16444, 45850, 34887, 54689, 41521, 32767, 0, 0, 0, 0, 0, 0, 0, 2048, 11520, 16320, 16320, 19684, 30648, 10846, 24014, 0, 0, 0, 0, 0, 0, 6144, 30080, 21184, 16416, 16432, 45852, 34898, 37966, 24994, 37425, 32767, 0, 0, 0, 0, 0, 0, 2048, 11520, 16320, 16320, 19680, 30636, 27568, 7772, 28110, 0, 3072, 15040, 10592, 8208, 8208, 22920, 16904, 17736, 8296, 14372, 8212, 20564, 28722, 37665, 35505, 32510, 0, 1024, 5760, 8160, 8160, 9840, 15856, 15024, 8080, 2008, 8168, 12200, 4044, 27870, 29774, 0, 0, 3072, 15040, 10592, 8208, 12432, 26952, 17416, 19272, 8296, 14372, 8212, 20562, 45105, 42897, 30846, 0, 0, 1024, 5760, 8160, 3936, 5808, 15344, 13488, 8080, 2008, 8168, 12204, 20430, 22638, 0, 3072, 15040, 10592, 8208, 10384, 18568, 17416, 20296, 9832, 13860, 8212, 16458, 28730, 37665, 38065, 31982, 0, 1024, 5760, 8160, 5984, 14192, 15344, 12464, 6544, 2520, 8168, 16308, 4036, 27870, 26702, 0, 0, 3072, 15040, 10592, 8208, 12432, 22920, 17416, 19272, 8296, 14372, 8212, 20562, 45105, 42897, 30846, 0, 0, 1024, 5760, 8160, 3936, 9840, 15344, 13488, 8080, 2008, 8168, 12204, 20430, 22638, 0, 0, 3072, 14080, 21216, 8336, 8720, 16904, 20488, 27720, 8296, 14372, 12612, 23110, 41409, 36929, 32766, 0, 0, 2048, 11520, 8032, 7648, 15856, 12272, 5040, 8080, 2008, 3768, 9656, 24126, 28606, 0, 0, 3072, 14208, 21216, 8336, 8720, 17672, 20488, 31816, 14440, 14372, 12612, 22854, 41153, 36929, 32766, 0, 0, 2048, 11520, 8032, 7648, 15088, 12272, 944, 1936, 2008, 3768, 9912, 24382, 28606, 0, 3072, 14208, 21216, 8336, 8720, 16904, 20488, 27720, 8296, 14372, 12612, 21060, 20932, 21314, 19106, 32510, 0, 2048, 11520, 8032, 7648, 15856, 12272, 5040, 8080, 2008, 3768, 11704, 11832, 11452, 13404, 0, 0, 3072, 14208, 21216, 8336, 8720, 16904, 20488, 27724, 15140, 13346, 21282, 36897, 41761, 38577, 31854, 0, 0, 2048, 11520, 8032, 7648, 15856, 12272, 5040, 1240, 3036, 11484, 28638, 23774, 26702, 0, 0, 0, 0, 0, 0, 0, 6144, 30080, 21184, 16416, 16444, 45850, 34887, 54689, 41521, 32767, 0, 0, 0, 0, 0, 0, 0, 2048, 11520, 16320, 16320, 19684, 30648, 10846, 24014, 0, 3072, 15040, 10592, 8208, 10384, 18568, 16904, 16968, 8808, 10340, 9508, 24036, 20514, 37665, 35505, 32510, 0, 1024, 5760, 8160, 5984, 14192, 15856, 15792, 7568, 6040, 6872, 8728, 12252, 27870, 29774, 0, 0, 3072, 15040, 10592, 8208, 12432, 26952, 17416, 19256, 28744, 18444, 8212, 20530, 45073, 42897, 30846, 0, 0, 1024, 5760, 8160, 3936, 5808, 15344, 13504, 4016, 14320, 8168, 12236, 20462, 22638, 0, 0, 3072, 15040, 10592, 8208, 8720, 22920, 17416, 19272, 8296, 14372, 8212, 20562, 45105, 42897, 30846, 0, 0, 1024, 5760, 8160, 7648, 9840, 15344, 13488, 8080, 2008, 8168, 12204, 20430, 22638, 0, 3072, 15040, 10592, 8208, 10384, 21832, 18568, 16968, 8296, 14356, 12340, 24676, 20514, 45857, 35505, 32510, 0, 1024, 5760, 8160, 5984, 10928, 14192, 15792, 8080, 2024, 4040, 8088, 12252, 19678, 29774, 0, 3072, 15040, 10592, 8208, 14736, 18568, 17416, 19272, 8296, 14372, 8212, 20564, 28722, 37665, 35505, 32510, 0, 1024, 5760, 8160, 1632, 14192, 15344, 13488, 8080, 2008, 8168, 12200, 4044, 27870, 29774, 0, 3072, 15040, 10592, 8208, 10384, 18568, 17416, 20360, 10824, 13092, 9636, 20516, 20514, 37665, 35505, 32510, 0, 1024, 5760, 8160, 5984, 14192, 15344, 12400, 5552, 3288, 6744, 12248, 12252, 27870, 29774, 0, 3072, 14208, 21216, 8336, 29200, 16776, 8200, 16456, 28776, 6180, 12436, 20580, 20548, 21314, 19106, 32510, 0, 2048, 11520, 8032, 3552, 15984, 8176, 16304, 3984, 2008, 3944, 12184, 12216, 11452, 13404, 0, 1536, 7104, 10608, 4168, 4232, 8452, 10244, 13858, 8290, 14370, 13124, 64580, 25540, 24708, 18312, 31992, 0, 1024, 5760, 4016, 3952, 7928, 6136, 2524, 8092, 2012, 3256, 952, 7224, 8056, 14448, 0, 3072, 14208, 21216, 8336, 8720, 16904, 20488, 27726, 8265, 14405, 12615, 20805, 20679, 21314, 19106, 32510, 0, 2048, 11520, 8032, 7648, 15856, 12272, 5040, 8118, 1978, 3768, 11962, 12088, 11452, 13404, 0, 1664, 6976, 10528, 8208, 8208, 16392, 16392, 16392, 9192, 15380, 9780, 17876, 17426, 34801, 35473, 32510, 0, 1152, 5824, 8160, 8160, 16368, 16368, 16368, 7184, 1000, 6600, 14888, 15340, 30734, 29806, 0, 3072, 15040, 10592, 8208, 12432, 22920, 50204, 53028, 25928, 12828, 8212, 20532, 20530, 37665, 35505, 32510, 0, 1024, 5760, 8160, 3936, 9840, 15328, 12504, 6832, 3552, 8168, 12232, 12236, 27870, 29774, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

const uint16_t pyonko_art[] = {0, 0, 12312, 10280, 10184, 10344, 5304, 9360, 8208, 9744, 4112, 5400, 2600, 2096, 1344, 640, 0, 0, 0, 4112, 6192, 6032, 2880, 7008, 8160, 6624, 4064, 2784, 1488, 1984, 640, 0, 0, 12312, 10280, 10184, 10344, 5304, 9360, 8208, 10128, 4880, 4112, 5456, 2224, 2088, 1368, 640, 0, 0, 4112, 6192, 6032, 2880, 7008, 8160, 6240, 3296, 4064, 2720, 1856, 2000, 640, 0, 0, 12300, 10260, 10212, 10836, 12876, 4104, 6120, 5064, 12684, 10260, 4104, 2064, 2064, 6120, 6168, 0, 0, 4104, 6168, 5544, 3504, 4080, 2064, 3120, 3696, 6120, 4080, 2016, 2016, 2064, 0, 0, 12300, 10260, 10212, 10836, 12876, 4104, 6120, 5064, 12684, 10260, 4104, 2064, 2064, 6120, 6168, 0, 0, 4104, 6168, 5544, 3504, 4080, 2064, 3120, 3696, 6120, 4080, 2016, 2016, 2064, 0, 0, 0, 7196, 9764, 10212, 5128, 11272, 8196, 8196, 8196, 4196, 2212, 2212, 2116, 1512, 528, 0, 0, 0, 6168, 6168, 3056, 5104, 8184, 8184, 8184, 3992, 1880, 1880, 1976, 528, 0, 0, 0, 12312, 10280, 10184, 10344, 5432, 10384, 8208, 9744, 4112, 5432, 2600, 2096, 1344, 640, 0, 0, 0, 4112, 6192, 6032, 2752, 5984, 8160, 6624, 4064, 2752, 1488, 1984, 640, 0, 0, 0, 12312, 10280, 10184, 10344, 4152, 14800, 8208, 9232, 4112, 5400, 2600, 2096, 1344, 640, 0, 0, 0, 4112, 6192, 6032, 4032, 1568, 8160, 7136, 4064, 2784, 1488, 1984, 640, 0, 0, 12300, 10260, 10212, 11316, 14940, 4104, 5064, 5064, 13260, 11220, 4104, 2064, 2064, 6120, 6168, 0, 0, 4104, 6168, 5064, 1440, 4080, 3120, 3120, 3120, 5160, 4080, 2016, 2016, 2064, 0, 0, 0, 12312, 10280, 10184, 10344, 4152, 14800, 8208, 9232, 4112, 5400, 2600, 2096, 1344, 640, 0, 0, 0, 4112, 6192, 6032, 4032, 1568, 8160, 7136, 4064, 2784, 1488, 1984, 640, 0, 0, 0, 12312, 10280, 10184, 10344, 4152, 14800, 8208, 9232, 4112, 5400, 2600, 2096, 1344, 640, 0, 0, 0, 4112, 6192, 6032, 4032, 1568, 8160, 7136, 4064, 2784, 1488, 1984, 640, 0, 0, 0, 12312, 10280, 10184, 10344, 4152, 14800, 8208, 9232, 4112, 5400, 2600, 2096, 1344, 640, 0, 0, 0, 4112, 6192, 6032, 4032, 1568, 8160, 7136, 4064, 2784, 1488, 1984, 640, 0, 0, 0, 12312, 10280, 10184, 10344, 4152, 14800, 8208, 9232, 4112, 5400, 2600, 2096, 1344, 640, 0, 0, 0, 4112, 6192, 6032, 4032, 1568, 8160, 7136, 4064, 2784, 1488, 1984, 640, 0, 0, 0, 248, 264, 1992, 2504, 5360, 9232, 8208, 14352, 4112, 2712, 2344, 1072, 1344, 640, 0, 0, 0, 240, 48, 1584, 2816, 7136, 8160, 2016, 4064, 1376, 1744, 960, 640, 0, 0, 248, 264, 1992, 2504, 5360, 9232, 8208, 15376, 7376, 7512, 7336, 2096, 1056, 1344, 640, 0, 0, 240, 48, 1584, 2816, 7136, 8160, 992, 800, 672, 848, 1984, 960, 640, 0, 0, 0, 12312, 10280, 10184, 10344, 4152, 14736, 9232, 10768, 4112, 5456, 2960, 2064, 1440, 576, 0, 0, 0, 4112, 6192, 6032, 4032, 1632, 7136, 5600, 4064, 2720, 1120, 2016, 576, 0, 0, 0, 0, 124, 132, 900, 1220, 2680, 4616, 4104, 7176, 2056, 1356, 1172, 2072, 2016, 0, 0, 0, 0, 120, 120, 824, 1408, 3568, 4080, 1008, 2032, 688, 872, 2016, 0, 0, 0, 0, 780, 1300, 2020, 2100, 3292, 4104, 6024, 6032, 2064, 2236, 2132, 4136, 4032, 0, 0, 0, 0, 520, 24, 1992, 800, 4080, 2160, 2144, 2016, 1856, 1960, 4032, 0, 0, 0, 124, 132, 900, 1220, 2680, 4616, 4104, 7176, 2056, 1356, 1172, 1048, 672, 320, 0, 0, 0, 120, 120, 824, 1408, 3568, 4080, 1008, 2032, 688, 872, 992, 320, 0, 248, 264, 1800, 2440, 4848, 9232, 8208, 14544, 4432, 2192, 2088, 1080, 1056, 672, 320, 0, 0, 240, 240, 1648, 3328, 7136, 8160, 1824, 3744, 1888, 2000, 960, 960, 320, 0, 0, 0, 0, 0, 0, 0, 384, 61784, 36840, 34832, 34952, 39244, 28930, 16396, 18448, 11024, 16352, 0, 0, 0, 0, 0, 0, 128, 28688, 30688, 30576, 26288, 3836, 16368, 14304, 5344, 0, 0, 6156, 5140, 5092, 5172, 2652, 4680, 4360, 4360, 4360, 2056, 2700, 1300, 1048, 672, 320, 0, 0, 2056, 3096, 3016, 1440, 3504, 3824, 3824, 3824, 2032, 1392, 744, 992, 320, 0, 0, 0, 6156, 5140, 5092, 5172, 3292, 4104, 6248, 4872, 2056, 2700, 1300, 1048, 672, 320, 0, 0, 0, 2056, 3096, 3016, 800, 4080, 1936, 3312, 2032, 1392, 744, 992, 320, 0, 0, 0, 6156, 5140, 5092, 5172, 2332, 7368, 4104, 4872, 2056, 2700, 1300, 1048, 672, 320, 0, 0, 0, 2056, 3096, 3016, 1760, 816, 4080, 3312, 2032, 1392, 744, 992, 320, 0, 0, 0, 12300, 10260, 10212, 10324, 13484, 6728, 5128, 4488, 2068, 4120, 3088, 2064, 1488, 544, 0, 0, 0, 4104, 6168, 6056, 2896, 1456, 3056, 3696, 2024, 4064, 992, 2016, 544, 0, 0, 6156, 5140, 5092, 5172, 2332, 7368, 4104, 4168, 5000, 2056, 2700, 1300, 1048, 672, 320, 0, 0, 2056, 3096, 3016, 1760, 816, 4080, 4016, 3184, 2032, 1392, 744, 992, 320, 0, 0, 0, 6156, 5140, 6116, 2100, 5276, 5256, 4104, 5896, 2696, 2572, 1044, 1048, 672, 320, 0, 0, 0, 2056, 2072, 1992, 2912, 2928, 4080, 2288, 1392, 1520, 1000, 992, 320, 0, 0, 412, 674, 994, 1074, 14620, 8456, 4296, 8200, 12296, 2056, 2700, 1300, 1048, 672, 320, 0, 0, 284, 28, 972, 1760, 7920, 3888, 8176, 4080, 2032, 1392, 744, 992, 320, 0, 0, 398, 658, 994, 3122, 4542, 7176, 4616, 2056, 14352, 21024, 64576, 16496, 17552, 48096, 52224, 0, 0, 268, 28, 972, 3648, 1008, 3568, 2032, 2016, 11712, 896, 16256, 15200, 17408, 0, 0, 124, 132, 900, 1220, 2680, 4622, 4113, 7197, 2163, 1209, 1118, 1048, 528, 672, 320, 0, 0, 120, 120, 824, 1408, 3568, 4078, 994, 1932, 838, 928, 992, 480, 320, 0, 0, 12294, 10250, 10226, 8194, 12294, 5092, 5140, 5684, 5588, 3096, 3048, 2376, 1168, 1488, 544, 0, 0, 4100, 6156, 8188, 4088, 3096, 3048, 2504, 2600, 992, 1040, 1712, 864, 544, 0, 0, 248, 264, 1992, 2504, 5360, 11280, 8208, 15376, 7376, 7512, 7336, 2096, 1056, 1344, 640, 0, 0, 240, 48, 1584, 2816, 5088, 8160, 992, 800, 672, 848, 1984, 960, 640, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

const uint16_t heart10_art[] = {12672, 19008, 33824, 32800, 32800, 16448, 8320, 4352, 2560, 1024};
const uint8_t heart7_art[] = {108, 146, 130, 130, 68, 40, 16};

const uint8_t smiley[8] = {
		0b00111100,
		0b01000010,
		0b10100101,
		0b10000001,
		0b10100101,
		0b10011001,
		0b01000010,
		0b00111100
};

// ----------------------------------------------------------------------------
#include "vpet.h"

const uint16_t *character_spritesheets[] = {
	mimi_art, pyonko_art, terrier_art, lop_art,
};

void vpet_draw_pet_crop(int x, int y, int hflip, enum character_id character, enum character_frame frame, int rows) {
	vpet_sprite_mask_16(x, y, hflip, rows, character_spritesheets[character] + frame*32, character_spritesheets[character] + frame*32+16);
}

void vpet_draw_pet(int x, int y, int hflip, enum character_id character, enum character_frame frame) {
	vpet_sprite_mask_16(x, y, hflip, 16, character_spritesheets[character] + frame*32, character_spritesheets[character] + frame*32+16);
} 
