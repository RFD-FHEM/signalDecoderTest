/*
*   Example Sketch for testing the signalDecoder lib and some sublibs
*   Copyright (C) 2014  .Butzek
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   This Sketch will only run complete on a Arduino Mega or in a Simulator which has enouth ram
*/


#define PROGNAME               "signalDecoderTest"
#define PROGVERS               "0.1"

#define BAUDRATE               115200
#include <signalDecoder.h>
#include <bitstore.h>
//Decoder
//patternDetector ooDetect;
SignalDetectorClass  ooDecode;
//ManchesterpatternDecoder  mcDecoder(&ooDecode);

// OSV2 Data hex : DADC539E18277055        //-1116, 840, -1104, 848, -1112, 352, -628, 836, -1124, 828, -644, -1112, 840, -1124, 832, -1116, -624, 840, -1120, 832, -1120, 836, -636, -1124, 832, -1116, -628, 840, -624, 352, -1124, 832, -1120, 836, -1116, 840, -1108, 844, -1104, 852, -1104, 364, -608, 856, -1100, 852, -1108, 848, -624, 352, -1092, 376, -604, 860, -624, 356, -1108, 356, -608, 860, -624, 352, -1092, 376, -604, 860, -604, 372, -1096, 368, -604, 864, -608, 368, -1104, 852, -1092, 372, -612, 852, -608, 368, -1092, 376, -612, 852, -3568,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          | Detector ends here
int sample_OSV2_data[] = {  863, -1080, 872, -1080, 876, -1088, 864, -1088, 868, -1092, 856, -1096, 860, -1084, 872, -1084, 868, -1100, 856, -1084, 868, -1096, 860, -1084, 868, -1080, 872, -1092, 864, -1088, 864, -1092, 864, -608, 368, -1088, 380, -592, 872, -600, 376, -1080, 388, -600, 860, -1092, 864, -600, 376, -1092, 372, -604, 864, -1088, 868, -600, 372, -1092, 864, -1084, 380, -600, 868, -1084, 868, -1088, 868, -596, 380, -1092, 376, -596, 868, -1088, 868, -1088, 864, -1092, 860, -620, 356, -1088, 868, -1088, 380, -604, 860, -608, 368, -1104, 360, -600, 868, -596, 384, -1084, 864, -1100, 364, -608, 860, -1092, 860, -1096, 860, -1096, 856, -600, 380, -1088, 860, -1092, 376, -616, 852, -612, 364, -1084, 872, -1084, 868, -1088, 376, -624, 844, -1088, 868, -596, 376, -1088, 868, -1084, 868, -1084, 384, -608, 856, -1096, 856, -1092, 864, -604, 372, -1088, 868, -1076, 392, -596, 864, -592, 388, -1084, 868, -1092, 860, -1092, 864, -1096, 860, -1092, 860, -1096, 368, -596, 868, -1088, 868, -1084, 868, -608, 372, -1092, 372, -600, 868, -604, 372, -1088, 376, -600, 864, -612, 368, -1088, 376, -608, 856, -600, 376, -1088, 380, -604, 864, -604, 372, -1080, 872, -1096, 372, -596, 868, -600, 376, -1084, 380, -604, 861 };

const uint8_t rand_data_size = 30;

int random_data[rand_data_size];  // Array for some Random Data

int sample_onoff_data[] =
// Logilink:
//{ 512, -9200, 576, -1920, 516, -3880, 512, -1924, 512, -3880, 516, -1936, 512, -3880, 512, -3880, 516, -3876, 516, -3880, 512, -1924, 512, -1920, 528, -3880, 516, -3876, 516, -3880, 512, -1924, 512, -3880, 512, -1936, 512, -1928, 508, -1924, 524, -1924, 508, -3884, 512, -3880, 512, -3880, 512, -1940, 512, -1920, 512, -1924, 524, -3880, 512, -3880, 516, -1920, 512, -3880, 512, -1936, 512, -1924, 512, -1936, 512, -1924, 508, -3880, 512, -1940, 508 };//, -9184, 576, -1936, 512, -3876, 516, -1920, 512, -3880, 528, -1920, 516, -3876, 512, -3880, 512, -3880, 528, -3876, 516, -1920, 512, -1924, 524, -3880, 512, -3880, 512, -3880, 512, -1920, 528, -3880, 512, -1920, 512, -1940, 508, -1924, 508, -1940, 508, -3884, 508, -3880, 512, -3880, 512, -1936, 512, -1924, 508, -1940, 508, -3880, 512, -3880, 512, -1936, 512, -3880, 512, -1920, 512, -1936, 512, -1924, 508, -1940, 508, -3880};
// ITTX Data:
//{-1012, -3556, -2364, 152, -108, -1448, -5604, -3944, 188, -3304, -768, -1212, -2156, -568, -636, -460, -704, 100, -496, -1324, -1052, 112, -5852, -1484, -716, -300, -2692, -380, 1264, -1012, 1260, -1004, 1256, -1016, 1240, -1032, 468, -1020, 1252, -1020, 488, -1024, 1232, -1032, 1236, -1020, 1236, -1036, 1236, -1036, 1228, -1040, 460, -1028, 1240, -1036, 1232, -1032, 1228, -1040, 1232, -1044, 1220, -1040, 460, -1032, 472, -1036, 1224, -1040, 464, -1044, 1224, -1036, 468, -1048, 1212, -1040, 1224, -1032, 476, -1036, 1232, -1024, 1236, -1044, 456, -1052, 460, -1036, 1228, -1044, 1220, -1040, 460, -1036, 1240, -1024, 468, -1032, 1248, -1024, 1232, -1028, 476, -1028, 1232, -1032, 476, -1036, 1224, -1048, 1216, -1040, 468, 464, -843};
//Einhell
{ 0, -364, -480, -312, -312, 348, 428, -548, -444, -308, -624, -596, -500, -540, -328, -412, 352, -308, -708, 428, 408, 440, -352, -704, 440, -300, -352, -492, 384, -384, 304, 400, 312, -644, -372, -688, 352, -668, -604, -352, -324, 304, -444, -532, 308, -384, 384, 308, 428, 300, -524, -392, -420, -404, 356, -432, -364, -520, 356, 324, 328, -504, -340, 308, -440, -308, -544, -348, -400, -500, 384, -356, 320, 316, -528, -440, -372, -452, 348, -500, 564, -300, -488, -312, -384, -516, -380, -368, -360, 576, -524, -444, -444, -312, 324, -368, -444, -588, -432, -428, -324, -416, -324, 324, -536, -504, -324, -300, 332, -320, -664, -368, -308, -536, 336, 376, -320, 336, -316, -308, -404, -328, -340, -304, -364, -332, 324, -528, -392, -304, 312, -376, -368, 376, 424, -340, -528, -332, -388, -304, 396, 484, -344, 356, -320, 328, -316, 488, -508, -328, -304, -580, -392, -504, -384, -556, 304, -424, -592, -388, 352, 328, -352, 364, 360, -364, -324, -396, -432, 304, 448, 476, 332, -340, -404, 336, -492, -392, -300, -688, 400, 572, 324, -388, 356, -316, 556, -348, -368, -364, -344, 320, -300, 300, -380, 568, -344, -360, -352, 360, 332, -544, -324, -368, -352, 428, -376, -340, -304, -400, -400, 472, -304, -368, -304, -320, -908, -428, 308, 500, -332, -524, 376, 364, -368, -360, -324, -324, -320, 416, -340, 404, -456, 348, 344, -412, -352, 412, -616, -384, -340, 572, -448, -376, -736, -304, -316, -356, 340, 304, 412, -316, -468, -372, -380, -304, -324, -356, -332, -368, -308, -320, -392, 376, -372, 392, -308, -380, -472, -360, -300, 352, -300, -384, -572, -364, 364, 348, -308, 412, -380, -316, 316, -352, 408, 364, -412, -356, -320, -424, 368, 340, -628, -408, -548, -452, 376, 316, -344, -388, 304, -392, 512, 1276, -340, 1236, -356, 1220, -376, 1200, -1188, 444, -356, 1192, -396, 1184, -1208, 440, -352, 1164, -1228, 408, -1196, 396, -1200, 396, -1204, 392, -1208, 384, -420, 1112, -1296, 352, -1252, 320, -1288, 308, -500, 1044, -1340, -1300, 300, -1292, 304, -1288, 312, -488, 1064, -512, 1068, -516, 1072, -512, 1076, -512, 1076, -504, 1080, -508, 1080, -500, 1088, -1300, 332, -20556, 392, -408, 1164, -436, 1136, -464, 1124, -460, 1124, -1264, 372, -428, 1120, -460, 1128, -1260, 392, -400, 1128, -1260, 372, -1224, 376, -1220, 376, -1220, 380, -1224, 368, -420, 1132, -1256, 396, -1196, 380, -1216, 380, -416, 1136, -1252, 384, -1216, 380, -1216, 380, -1212, 388, -416, 1132, -444, 1136, -448, 1140, -448, 1136, -452, 1136, -448, 1140, -448, 1136, -452, 1136, -1248, 384, -20528, 412, -388, 1184, -412, 1164, -428, 1160, -428, 1156, -1232, 404, -396, 1152, -436, 1152, -1232, 416, -376, 1152, -1232, 404, -1196, 400, -1200, 400, -1192, 404, -1196, 400, -396, 1156, -1232, 416, -1176, 400, -1196, 404, -392, 1156, -1232, 404, -1196, 400, -1196, 400, -1196, 404, -392, 1156, -424, 1156, -432, 1156, -432, 1152, -436, 1152, -432, 1156, -428, 1156, -436, 1152, -1232, 400, -20520, 420, -380, 1192, -404, 1172, -416, 1172, -420, 1164, -1224, 408, -388, 1164, -416, 1168, -1224, 428, -364, 1164, -1220, 412, -1188, 412, -1188, 408, -1184, 416, -1184, 412, -384, 1164, -1224, 428, -1164, 412, -1192, 404, -388, 1164, -1224, 408, -1188, 408, -1188, 412, -1184, 412, -384, 1164, -424, 1160, -420, 1164, -424, 1164, -420, 1164, -424, 1164, -420, 1164, -424, 1164, -1224, 408, -20512, 428, -376, 1196, -396, 1180, -416, 1172, -412, 1176, -1212, 420, -376, 1172, -416, 1172, -1216, 432, -360, 1168, -1220, 416, -1180, 416, -1188, 412, -1180, 416, -1188, 408, -380, 1168, -1220, 432, -1160, 416, -1180, 420, -384, 1164, -1224, 408, -1184, 416, -1184, 412, -1184, 412, -384, 1168, -416, 1164, -424, 1164, -416, 1168, -420, 1164, -424, 1164, -420, 1168, -420, 1168, -1220, 412, -20512, 428, -372, 1200, -392, 1184, -412, 1176, -408, 1176, -1212, 424, -372, 1176, -412, 1176, -1212, 436, -356, 1172, -1216, 420, -1180, 416, -1180, 416, -1184, 416, -1176, 420, -376, 1172, -1220, 432, -1160, 416, -1180, 416, -380, 1172, -1216, 416, -1184, 416, -1180, 416, -1180, 416, -384, 1168, -412, 1168, -420, 1164, -424, 1164, -420, 1168, -416, 1168, -424, 1164, -420, 1164, -1228, 408, -20508, 432, -372, 1200, -396, 1180, -412, 1176, -404, 1180, -1208, 428, -372, 1176, -416, 1168, -1212, 440, -356, 1172, -1220, 416, -1176, 420, -1176, 424, -1176, 420, -1180, 416, -376, 1172, -1216, 436, -1164, 412, -1184, 412, -380, 1172, -1212, 420, -1184, 416, -1180, 416, -1184, 412, -384, 1168, -412, 1168, -416, 1172, -416, 1168, -420, 1168, -416, 1168, -420, 1168, -420, 1168, -1220, 412, -20512, 432, -368, 1204, -392, 1184, -408, 1176, -408, 1180, -1212, 420, -376, 1176, -408, 1176, -1216, 436, -356, 1172, -1212, 424, -1176, 420, -1176, 420, -1180, 420, -1176, 420, -376, 1172, -1216, 436, -1156, 420, -1180, 416, -380, 1172, -1216, 416, -1180, 420, -1176, 420, -1180, 416, -380, 1172, -412, 1168, -416, 1172, -412, 1172, -420, 1168, -420, 1164, -424, 1164, -424, 1160, -1224, 412, -20516, 424, -372, 1200, -396, 1184, -408, 1176, -408, 1180, -1208, 424, -376, 1172, -416, 1172, -1216, 436, -352, 1176, -1212, 420, -1176, 424, -1176, 420, -1176, 420, -1180, 416, -376, 1176, -1216, 436, -1156, 420, -1176, 420, -380, 1172, -1216, 416, -1180, 416, -1180, 420, -1180, 416, -380, 1168, -416, 1168, -416, 1168, -416, 1172, -416, 1172, -416, 1168, -420, 1168, -420, 1164, -1220, 416, -20512, 428, -372, 1200, -396, 1184, -408, 1176, -408, 1180, -1212, 420, -372, 1180, -408, 1176, -1216, 436, -356, 1172, -1212, 424, -1176, 420, -1176, 420, -1180, 420, -1176, 420, -376, 1172, -1220, 432, -1156, 420, -1180, 416, -380, 1172, -1216, 416, -1180, 420, -1180, 416, -1180, 416, -380, 1172, -412, 1168, -420, 1168, -420, 1164, -416, 1172, -420, 1168, -420, 1164, -420, 1168, -1220, 412, -20512, 432, -368, 1204, -392, 1184, -404, 1184, -408, 1176, -1212, 424, -372, 1176, -408, 1180, -1208, 440, -356, 1172, -1216, 420, -1176, 420, -1176, 424, -1176, 420, -1180, 416, -376, 1176, -1212, 436, -1160, 416, -1180, 420, -380, 1168, -1224, 412, -1180, 416, -1180, 420, -1180, 416, -376, 1172, -412, 1172, -424, 1160, -420, 1168, -416, 1172, -416, 1168, -424, 1164, -420, 1168, -1220, 412, -20512, 432, -372, 1200, -392, 1184, -404, 1184, -408, 1176, -1212, 424, -372, 1176, -408, 1180, -1216, 432, -352, 1176, -1216, 420, -1184, 416, -1176, 420, -1176, 420, -1180, 416, -376, 1176, -1212, 440, -1156, 420, -1176, 420, -380, 1172, -1212, 420, -1180, 416, -1180, 420, -1176, 420, -380, 1172, -412, 1168, -416, 1172, -416, 1168, -416, 1172, -416, 1168, -420, 1168, -420, 1168, -1220, 412, -20512, 432, -372, 1200, -392, 1184, -408, 1180, -408, 1176, -1212, 424, -376, 1172, -408, 1180, -1212, 436, -356, 1172, -1216, 420, -1176, 420, -1180, 420, -1176, 420, -1180, 416, -376, 1176, -1212, 440, -1160, 416, -1180, 416, -376, -1216, 416, -1180, 420, -1176, 420, -1188, 408, -380, 1172, -408, 1172, -416, 1172, -416, 1168, -420, 1168, -416, 1172, -420, 1164, -420, 1168, -1220, 412, -20512, 432, -372, 1200, -392, 1184, -408, 1180, -408, 1180, -1208, 424, -372, 1180, -408, 1176, -1212, 440, -352, 1176, -1216, 420, -1176, 420, -1180, 416, -1176, 424, -1172, 424, -380, 1168, -1216, 436, -1156, 420, -1180, 416, -376, 1176, -1220, 416, -1176, 420, -1180, 416, -1184, 416, -380, 1168, -408, 1176, -416, 1168, -420, 1168, -416, 1168, -420, 1168, -420, 1168, -416, 1168, -1220, 416, -20512, 432, -368, 1204, -392, 1184, -408, 1180, -404, 1180, -1216, 420, -372, 1176, -408, 1180, -1212, 440, -352, 1176, -1212, 420, -1176, 424, -1176, 420, -1176, 424, -1176, 420, -376, 1172, -1216, 436, -1156, 420, -1180, 416, -384, 1168, -1212, 420, -1180, 420, -1180, 416, -1180, 416, -384, 1168, -412, 1172, -412, 1172, -420, 1168, -416, 1168, -420, 1168, -420, 1168, -416, 1168, -1224, 412, -20512, 432, -368, 1204, -392, 1184, -408, 1180, -408, 1176, -1216, 420, -372, 1176, -412, 1176, -1212, 436, -356, 1176, -1212, 420, -1176, 424, -1176, 420, -1176, 424, -1176, 420, -376, 1172, -1216, 436, -1156, 420, -1180, 416, -380, 1172, -1220, 416, -1176, 420, -1180, 416, -1184, 416, -376, 1172, -412, 1172, -412, 1172, -416, 1172, -416, 1172, -420, 1164, -420, 1168, -420, 1168, -1220, 412, -20516, 428, -372, 1200, -396, 1184, -404, 1180, -412, 1176, -1212, 420, -372, 1180, -408, 1180, -1208, 440, -360, 1168, -1212, 424, -1176, 420, -1176, 424, -1180, 416, -1180, 416, -376, 1176, -1216, 436, -1160, 416, -1176, 420, -376, 1172, -1216, 420, -1180, 420, -1180, 416, -1180, 416, -380, 1172, -412, 1168, -420, 1168, -420, 1168, -416, 1168, -420, 1168, -420, 1168, -420, 1164, -1224, 412, -20512, 432, -368, 1204, -392, 1184, -408, 1180, -408, 1180, -1208, 424, -372, 1180, -408, 1176, -1212, 440, -352, 1176, -1216, 420, -1176, 420, -1176, 420, -1180, 420, -1176, 420, -376, 1176, -1212, 436, -1160, 416, -1180, 420, -380, 1168, -1220, 416, -1180, 420, -1176, 420, -1180, 416, -376, 1176, -408, 1172, -416, 1172, -420, 1164, -420, 1168, -420, 1168, -416, 1168, -420, 1168, -1220, 416, -20512, 432, -368, 1204, -392, 1184, -408, 1180, -408, 1180, -1208, 424, -376, 1170 };

// AS Data
//SB:10110011
//B1:00000110  =0x06
//B2:10000000  =0x80
//B3:11001000  =0xc8
//B4:10000000  =0x80

/* ITv1
MU; P0 = -5802; P1 = 579; P2 = -171; P3 = 192; P4 = -558; D = 0121212121212121212123434341234123412341234123434301212121212121212121234343412341234123412341234343012121212121212121212343434123412341234123412343430121212121212121212123434341234123412341234123434301212121212121212121234343412341234123412341234343012; CP = 3; O; 
*/

//                                        | Start                                                                                                                                                                                                                                                                                                                                                                                         |Ends
int sample_AS_data[] = { -764, -524, -536, 1776, -1508, 956, -684, 1712, -784, 844, -1568, 924, -720, 1712, -808, 828, -812, 816, -820, 812, -824, 808, -1608, 880, -756, 1652, -1620, 1656, -832, 796, -844, 788, -844, 784, -844, 792, -844, 788, -844, 788, -1632, 864, -772, 1636, -856, 780, -1628, 1636, -856, 772, -860, 776, -1636, 1632, -864, 772, -860, 772, -864, 772, -860, 768, -860, 776, -860, 776, -860, 772, -780, 776, -3908, 1640, -1628, 864, -768, 1640, -852, 784, -1624, 864, -772, 1676, -848, 788, -844, 792, -844, 784, -848, 788, -1620, 868, -772, 1636, -1632, 1644, -848, 780, -852, 784, -852, 780, -848, 784, -852, 784, -844, 788, -1632, 864, -768, 1640, -852, 784, -1624, 1640, -852, 776, -852, 788, -1636, 1632, -852, 784, -852, 776, -856, 780, -852, 784, -848, 784, -852, 780, -856, 776, -776, 780 };


// This array can be filled with output from signalduino
//id=738, channel=2, temp=12.3 :  unknown Protocol
//uint8_t signal_Stream[]= {  0,3,0,2,0,1,0,2,0,2,0,2,0,1,0,1,0,1,0,2,0,1,0,1,0,2,0,1,0,1,0,1,0,1,0,1,0,2,0,2,0,2,0,2,0,1,0,2,0,2,0,2,0,2,0,2,0,2,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,3};
//int patternData[]=  { 476, -980, -1956, -4016,100  };//{-100,-988,488,-1960,-4016};

// Kaku Swirtch Protocol bits: 00111000111000111111111010001001
//uint8_t signal_Stream[]={1,5,1,0,1,3,1,0,1,3,1,2,1,0,1,2,1,0,1,2,1,0,1,0,1,2,1,0,1,2,1,2,1,0,1,2,1,0,1,2,1,0,1,2,1,0,1,0,1,3,1,0,1,3,1,0,1,2,1,2,1,0,1,2,1,0,1,2,1,0,1,2,1,0,1,2,1,0,1,2,1,0,1,2,1,0,1,2,1,0,1,2,1,0,1,0,1,3,1,2,1,0,1,0,1,3,1,0,1,3,1,0,1,3,1,2,1,0,1,0,1,3,1,0,1,3,1,2,1,0,1,4};
//int patternData[]={-232,308,-904,-1092,-9464,-2548,1589};

// Demo Signal like from einhell example
//int patternData[]={390,-1184,-401,1122,-20012};
//uint8_t signal_Stream []={0,2,3,2,3,2,3,1,0,1,0,2,3,2,3,1,0,2,3,1,0,1,0,1,0,1,0,1,0,2,3,2,3,2,3,1,0,1,0,2,3,1,0,1,0,1,0,2,3,2,3,2,3,2,3,2,3,2,3,1,0,2,0,4};

// Demo Signal like from RF803E

//int patternData[]={291,-506,0,-926,695,-1758};
//uint8_t signal_Stream []={0,1,0,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,1,0,1,0,1,0,1,0,1,0,1,0,1,0,3,4,3,4,1,0,1,0,3,4,1,4,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,5,0,1,0,1,0,3,4,3,4,3,0,1,4,3,0,1,0,1,4,3,0,1,4,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,0,1,4,1,0,1,0,1,0,1,0,3,4,1,0,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,1,0,1,0,1,0,1,0,1,0,1,0,1,0,3,4,3,4,1,0,1,0,3,4,1,4,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,5,0,1,0,1,0,3,4,3,4,3,0,1,4,3,0,1,0,1,4,3,0,1};

// Demo Signal like from Oregon NR868
/*
int patternData[] = {
	1139,-1494,-504,596,
};

uint8_t signal_Stream[] = {
	0,1,0,2,3,2,3,1,3,2,3,2,0,1,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,0,1,3,2,0,1,0,1,0,1,0,2,3,2,3,1,3,2,3,2,0,1,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,0,1,3,2,0,1,0,1,0,1,0,2,3,2,3,1,3,2,3,2,0,1,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,0,1,3,2,0,1,0,1,0,1,0,2,3,2,3,1,3,2,3,2,0,1,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,0,1,3,2,0,1,0,1,0,1,0,2,3,2,3,1,3,2,3,2,0,1,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,2,3,0,

};
*/


//uint8_t signal_Stream []={ 0,1,0,1,0,2,0,1,0,1,0,1,0,1,0,1,0,1,0,1,3,1,0,1,0,1,0,1,0,1,0,1,0,1,3,1,3,1,3,2,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,3,1,0,1,0,1,0,1,0,1,0,1,0,1,0,2,0,1,0,1,0,1,0,1,0,1,0,1,0,1,3,1,0,1,0,1,0,1,0,1,0,1,0,1,3,1,3,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,};

//MS;P0=390;P1=-1184;P2=-401;P3=1122;P4=12754;P5=-20012;P6=1371;D=02323232310232310231010101010231010102310101010232323232323102302305023232323102323102310101010102310101023101010102323232323231023023;CP=0;SP=5;

/* OSV2 Protocol*/
int patternData[] = {
	908,-1061,-569,392,143,105
};

uint8_t signal_Stream[]={ 
	5,4,3,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,2,3,1,3,2,0,2,3,1,3,2,0,2,3,1,3,2,0,1,0,1,0,2,3,1,0,1,3,2,0,1,0,2,3,1,0,1,3,2,0,2,3,1,0,1,0,1,0,1,0,1,3,2,0,2,3,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,3,2,0,1,0,2,3,1,0,1,0,1,0,1,0,1,3,2,0,1,0,2,3,1,3,2,0,2,3,1,0,1,3,2,0,2,3,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,3,2,0,2,3,1,3,2,0,1,0,1,0,2,3,1,0,1,0,1,3,2,0,2,3,1,0,1,3,2,0,1,0,2,3,1,3,2,0,
};



int *pulsedata = NULL;
uint16_t lendata = 0;


uint32_t signalduration = 0;

void init_random_data()
{

	for (uint8_t i = 0; i<rand_data_size; i++)
	{
		random_data[i] = rand() >> 5;//random(1,800);
		if (i % 2 == 0)
		{
			random_data[i] = random_data[i] * -1;
		}

		//delay(1);
		//  Serial.print(',');Serial.print(random_data[i]);
	}

	//Serial.println("");

}


/*
void detect_onoff()
{
	static bool state;
	for (uint16_t i = 0; i <= lendata; ++i)
	{
		//Serial.print("#");Serial.println(i);
		state = ooDetect.detect(&pulsedata[i]);
		//delay(100);
		if (state)
		{
			Serial.println("Message dekodiert");
			//ooDetect.reset();
			delay(1000);
		}
	}
	if (!state)
	{
		ooDetect.processMessage();
		Serial.println("Message nicht dekodiert");
		delay(1000);
	}

}
*/

void decode_onoff()
{
	bool state;
	signalduration = 0;

	for (uint8_t i = 0; i<rand_data_size; i++)
	{
		state = ooDecode.decode(&random_data[i]);  // simulate some noise
	}

	uint16_t i = 0;

	for (uint8_t repeat = 0; repeat < 2; repeat++)
	{
		i = 0;
		if (repeat == 0) i = 20;      // Simulate that we have not retrieved all from the first message
		for (; i < lendata; ++i)
		{
			//Serial.print("#");Serial.println(i);
			state = ooDecode.decode(&pulsedata[i]);
			//delay(100);
			signalduration += abs(pulsedata[i]);
			if (state)
			{
				Serial.println("Message dekodiert");
				//ooDecode.reset();
				//delay(1000);
			}
		}
	}
	for (uint8_t i = 0; i<rand_data_size; i++)
	{
		state = ooDecode.decode(&random_data[i]);
	}


}


void decode_signalstream()
{
	uint16_t len = sizeof(signal_Stream) / sizeof(signal_Stream[0]);
	init_random_data();
	signalduration = 0;
	bool state;

	for (uint8_t i = 0; i<rand_data_size; i++)
	{
		//state =  ooDecode.decode(&random_data[i]);
	}

	for (uint8_t j = 0; j<1; ++j) {

		uint16_t i = 0;

		for (; i<lendata; ++i)
		{
			//Serial.print("#");Serial.println(i);
			state = ooDecode.decode(&patternData[signal_Stream[i]]);
			//delay(100);
			signalduration += abs(patternData[signal_Stream[i]]);
		}
	}

	init_random_data();
	for (uint8_t i = 0; i<rand_data_size; i++)
	{
		state = ooDecode.decode(&random_data[i]);
	}

	if (!state)
	{
		ooDecode.processMessage();
		Serial.println("Message nicht dekodiert");
		//delay(1000);
	}

}


uint8_t msg = 0;




void setup() {
	randomSeed(A0);
	Serial.begin(BAUDRATE);
	init_random_data();
	delay(2000);
	Serial.println("Startup:");
	// Simple ON OFF Data
	//detect_onoff();
	uint32_t start_time = 0;
	uint32_t end_time = 0;
	uint32_t duration = 0;


	//   Oregon Scientific V2 protocol regression test



	ooDecode.MCenabled = true;
	ooDecode.MSenabled = true;
	ooDecode.MUenabled = true;

	/*
	pulsedata = sample_OSV2_data;
	lendata = sizeof(sample_OSV2_data)/sizeof(sample_OSV2_data[0]);
	Serial.println("");
	Serial.println("--------------------------------------------------------");
	Serial.print("Len Input data (OSV2 Manchester): ");
	Serial.println(lendata);
	Serial.println("Detecting pattern ");
	init_random_data(); signalduration=0;
	start_time=micros();
	decode_onoff();
	end_time=micros();
	duration= end_time-start_time;
	Serial.print("Detection Time =");  Serial.print(duration);  Serial.println(" micro seconds");
	Serial.print("Signal Time is=");  Serial.print(signalduration);  Serial.println(" micro seconds");
	Serial.println("--------------------------------------------------------");
	*/

	//   regression test, working with Signaldata and not pulsedata
	Serial.println("");
	Serial.println("--------------------------------------------------------");
	Serial.print("Len Input data (signal data protocol): ");
	lendata = sizeof(signal_Stream) / sizeof(signal_Stream[0]);
	Serial.println(lendata);
	Serial.println("Detecting pattern ");
	init_random_data(); signalduration = 0;
	start_time = micros();
	decode_signalstream();
	end_time = micros();
	duration = end_time - start_time;
	Serial.print("Detection Time =");  Serial.print(duration);  Serial.println(" micro seconds");
	Serial.print("Signal Time is=");  Serial.print(signalduration);  Serial.println(" micro seconds");
	Serial.println("--------------------------------------------------------");
	Serial.println("");


	return;
	
	//   M0 Logilink protocol puls pause regression test
	pulsedata = sample_onoff_data;
	lendata = sizeof(sample_onoff_data) / sizeof(sample_onoff_data[0]);
	Serial.println("");
	Serial.println("--------------------------------------------------------");
	Serial.print("Len Input data (Logilink protocol): ");
	Serial.println(lendata);
	Serial.println("Detecting pattern ");
	Serial.println("");
	init_random_data(); signalduration = 0;
	start_time = micros();
	decode_onoff();
	end_time = micros();
	duration = end_time - start_time;
	Serial.print("Detection Time =");  Serial.print(duration);  Serial.println(" micro seconds");
	Serial.print("Signal Time is=");  Serial.print(signalduration);  Serial.println(" micro seconds");
	Serial.println("--------------------------------------------------------");
	Serial.println("");






}

void loop() {
	//}
	delay(1);

	int v1 = -20000;
	int v2 = -20083;
	int avg;
	v1 = (long(v1) + v2) >>1;
	

}




