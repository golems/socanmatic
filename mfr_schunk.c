#include "socanmatic/eds.h"
const canmat_dict_t canmat_dict402 = {
	.length=472,
	.btree_name=(canmat_dict_name_tree_t[]){
		{.parameter_name="Abort connection option code",.i=374},
		{.parameter_name="Acceleration dimension index",.i=416},
		{.parameter_name="Acceleration factor",.i=449},
		{.parameter_name="Acceleration factor/Divisor",.i=452},
		{.parameter_name="Acceleration factor/number of entries",.i=450},
		{.parameter_name="Acceleration factor/Numerator",.i=451},
		{.parameter_name="Acceleration notation index",.i=415},
		{.parameter_name="actual pcb temperature",.i=355},
		{.parameter_name="COB-ID emergency message",.i=21},
		{.parameter_name="COB-ID SYNC",.i=4},
		{.parameter_name="COB-ID time stamp message",.i=19},
		{.parameter_name="Communication cycle period",.i=5},
		{.parameter_name="communication mode",.i=352},
		{.parameter_name="Consumer heartbeat time",.i=23},
		{.parameter_name="Controlword",.i=376},
		{.parameter_name="debug message",.i=336},
		{.parameter_name="debug value float 1",.i=332},
		{.parameter_name="debug value float 2",.i=333},
		{.parameter_name="debug value float 3",.i=334},
		{.parameter_name="debug value float 4",.i=335},
		{.parameter_name="debug value int32 1",.i=326},
		{.parameter_name="debug value int32 2",.i=327},
		{.parameter_name="debug value int32 3",.i=328},
		{.parameter_name="debug value int32 4",.i=329},
		{.parameter_name="debug values float",.i=331},
		{.parameter_name="debug values int32",.i=325},
		{.parameter_name="Delta Position",.i=372},
		{.parameter_name="detail",.i=349},
		{.parameter_name="Device type",.i=0},
		{.parameter_name="Digital inputs",.i=454},
		{.parameter_name="Digital outputs",.i=455},
		{.parameter_name="Digital outputs/bit mask",.i=457},
		{.parameter_name="Digital outputs/number of entries",.i=456},
		{.parameter_name="Disconnect Reset",.i=346},
		{.parameter_name="Drive catalog number",.i=467},
		{.parameter_name="Drive data",.i=471},
		{.parameter_name="Drive data/number of entries",.i=470},
		{.parameter_name="Drive manufacturer",.i=468},
		{.parameter_name="EEPROM Parameters",.i=342},
		{.parameter_name="Emergency consumer",.i=40},
		{.parameter_name="End velocity",.i=406},
		{.parameter_name="Error behavior",.i=41},
		{.parameter_name="Error behavior/communication error",.i=42},
		{.parameter_name="Error Code",.i=375},
		{.parameter_name="error details",.i=348},
		{.parameter_name="Error register",.i=1},
		{.parameter_name="extended status",.i=373},
		{.parameter_name="Fault reaction option code",.i=386},
		{.parameter_name="Feed constant",.i=429},
		{.parameter_name="Feed constant/Feed",.i=431},
		{.parameter_name="Feed constant/number of entries",.i=430},
		{.parameter_name="Feed constant/Shaft revolutions",.i=432},
		{.parameter_name="file",.i=350},
		{.parameter_name="Gear ratio",.i=425},
		{.parameter_name="Gear Ratio/Motor revolutions",.i=427},
		{.parameter_name="Gear Ratio/number of entries",.i=426},
		{.parameter_name="Gear Ratio/Shaft revolutions",.i=428},
		{.parameter_name="Guard time",.i=9},
		{.parameter_name="Halt option code",.i=385},
		{.parameter_name="High resolution time stamp",.i=20},
		{.parameter_name="Home offset",.i=399},
		{.parameter_name="http drive catalog address",.i=469},
		{.parameter_name="http motor catalog address",.i=462},
		{.parameter_name="Identity object",.i=25},
		{.parameter_name="Identity object/Product code",.i=27},
		{.parameter_name="Identity object/Revision number",.i=28},
		{.parameter_name="Identity object/Serial number",.i=29},
		{.parameter_name="Identity object/Vendor-ID",.i=26},
		{.parameter_name="Inhibit time EMCY",.i=22},
		{.parameter_name="KC Current",.i=361},
		{.parameter_name="KC Pos",.i=370},
		{.parameter_name="KC Speed",.i=365},
		{.parameter_name="KP FeedForward",.i=371},
		{.parameter_name="KR Current",.i=358},
		{.parameter_name="KR Pos",.i=367},
		{.parameter_name="KR Speed",.i=362},
		{.parameter_name="KS FeedForward",.i=366},
		{.parameter_name="Life time favtor",.i=10},
		{.parameter_name="line",.i=351},
		{.parameter_name="logging",.i=338},
		{.parameter_name="logging state",.i=339},
		{.parameter_name="logging trigger time",.i=340},
		{.parameter_name="manufacturer debug flags",.i=323},
		{.parameter_name="Manufacturer device name",.i=7},
		{.parameter_name="Manufacturer software version",.i=8},
		{.parameter_name="Manufacturer status register",.i=2},
		{.parameter_name="Max deceleration",.i=453},
		{.parameter_name="Modes of operation",.i=387},
		{.parameter_name="Modes of operation display",.i=388},
		{.parameter_name="Motion profile type",.i=410},
		{.parameter_name="Motor calibrate date",.i=463},
		{.parameter_name="Motor catalog number",.i=460},
		{.parameter_name="Motor data",.i=465},
		{.parameter_name="Motor data/number of entries",.i=464},
		{.parameter_name="Motor manufacturer",.i=461},
		{.parameter_name="Motor Type",.i=459},
		{.parameter_name="nb debug values float",.i=330},
		{.parameter_name="nb debug values int32",.i=324},
		{.parameter_name="nb logging",.i=337},
		{.parameter_name="Number of entries",.i=341},
		{.parameter_name="Number of entries",.i=347},
		{.parameter_name="Number of entries",.i=353},
		{.parameter_name="OS command",.i=36},
		{.parameter_name="OS command mode",.i=39},
		{.parameter_name="OS command/reply",.i=38},
		{.parameter_name="OS command/status",.i=37},
		{.parameter_name="Password",.i=344},
		{.parameter_name="pcb temperature",.i=354},
		{.parameter_name="pcb temperature update period ms",.i=356},
		{.parameter_name="Polarity",.i=404},
		{.parameter_name="Position actual value",.i=389},
		{.parameter_name="Position dimension index",.i=412},
		{.parameter_name="Position encoder resolution",.i=418},
		{.parameter_name="Position encoder resolution/Encoder increments",.i=419},
		{.parameter_name="Position encoder resolution/Motor revolutions",.i=420},
		{.parameter_name="Position encoder resolution/number of entries",.i=417},
		{.parameter_name="Position factor",.i=433},
		{.parameter_name="Position factor/Feed constant",.i=436},
		{.parameter_name="Position factor/number of entries",.i=434},
		{.parameter_name="Position factor/Numerator",.i=435},
		{.parameter_name="Position notation index",.i=411},
		{.parameter_name="Position range limit",.i=395},
		{.parameter_name="Position range limit/Max position range limit",.i=398},
		{.parameter_name="Position range limit/Min position range limit",.i=397},
		{.parameter_name="Position range limit/number of entries",.i=396},
		{.parameter_name="Pre-defined error field",.i=3},
		{.parameter_name="Producer heartbeat time",.i=24},
		{.parameter_name="Profile acceleration",.i=407},
		{.parameter_name="Profile deceleration",.i=408},
		{.parameter_name="Profile velocity",.i=405},
		{.parameter_name="Quick stop deceleration",.i=409},
		{.parameter_name="Restore default parameters",.i=15},
		{.parameter_name="Restore default parameters/restore all default parameters",.i=16},
		{.parameter_name="Restore default parameters/restore application default parameters",.i=18},
		{.parameter_name="Restore default parameters/restore communication default parameters",.i=17},
		{.parameter_name="RPDO00 communication parameter/COB-ID",.i=44},
		{.parameter_name="RPDO00 communication parameter/compatibility entry",.i=47},
		{.parameter_name="RPDO00 communication parameter/event timer",.i=48},
		{.parameter_name="RPDO00 communication parameter/highest sub-index supported",.i=43},
		{.parameter_name="RPDO00 communication parameter/inhibit time",.i=46},
		{.parameter_name="RPDO00 communication parameter/SYNC start value",.i=49},
		{.parameter_name="RPDO00 communication parameter/Transmission type",.i=45},
		{.parameter_name="RPDO00 mapping parameter/1st object",.i=114},
		{.parameter_name="RPDO00 mapping parameter/2nd object",.i=115},
		{.parameter_name="RPDO00 mapping parameter/3rd object",.i=116},
		{.parameter_name="RPDO00 mapping parameter/4th object",.i=117},
		{.parameter_name="RPDO00 mapping parameter/5th object",.i=118},
		{.parameter_name="RPDO00 mapping parameter/6th object",.i=119},
		{.parameter_name="RPDO00 mapping parameter/7th object",.i=120},
		{.parameter_name="RPDO00 mapping parameter/8th object",.i=121},
		{.parameter_name="RPDO00 mapping parameter/number of mapped objects",.i=113},
		{.parameter_name="RPDO01 communication parameter/COB-ID",.i=51},
		{.parameter_name="RPDO01 communication parameter/compatibility entry",.i=54},
		{.parameter_name="RPDO01 communication parameter/event timer",.i=55},
		{.parameter_name="RPDO01 communication parameter/highest sub-index supported",.i=50},
		{.parameter_name="RPDO01 communication parameter/inhibit time",.i=53},
		{.parameter_name="RPDO01 communication parameter/SYNC start value",.i=56},
		{.parameter_name="RPDO01 communication parameter/Transmission type",.i=52},
		{.parameter_name="RPDO01 mapping parameter/1st object",.i=123},
		{.parameter_name="RPDO01 mapping parameter/2nd object",.i=124},
		{.parameter_name="RPDO01 mapping parameter/3rd object",.i=125},
		{.parameter_name="RPDO01 mapping parameter/4th object",.i=126},
		{.parameter_name="RPDO01 mapping parameter/5th object",.i=127},
		{.parameter_name="RPDO01 mapping parameter/6th object",.i=128},
		{.parameter_name="RPDO01 mapping parameter/7th object",.i=129},
		{.parameter_name="RPDO01 mapping parameter/8th object",.i=130},
		{.parameter_name="RPDO01 mapping parameter/number of mapped objects",.i=122},
		{.parameter_name="RPDO02 communication parameter/COB-ID",.i=58},
		{.parameter_name="RPDO02 communication parameter/compatibility entry",.i=61},
		{.parameter_name="RPDO02 communication parameter/event timer",.i=62},
		{.parameter_name="RPDO02 communication parameter/highest sub-index supported",.i=57},
		{.parameter_name="RPDO02 communication parameter/inhibit time",.i=60},
		{.parameter_name="RPDO02 communication parameter/SYNC start value",.i=63},
		{.parameter_name="RPDO02 communication parameter/Transmission type",.i=59},
		{.parameter_name="RPDO02 mapping parameter/1st object",.i=132},
		{.parameter_name="RPDO02 mapping parameter/2nd object",.i=133},
		{.parameter_name="RPDO02 mapping parameter/3rd object",.i=134},
		{.parameter_name="RPDO02 mapping parameter/4th object",.i=135},
		{.parameter_name="RPDO02 mapping parameter/5th object",.i=136},
		{.parameter_name="RPDO02 mapping parameter/6th object",.i=137},
		{.parameter_name="RPDO02 mapping parameter/7th object",.i=138},
		{.parameter_name="RPDO02 mapping parameter/8th object",.i=139},
		{.parameter_name="RPDO02 mapping parameter/number of mapped objects",.i=131},
		{.parameter_name="RPDO03 communication parameter/COB-ID",.i=65},
		{.parameter_name="RPDO03 communication parameter/compatibility entry",.i=68},
		{.parameter_name="RPDO03 communication parameter/event timer",.i=69},
		{.parameter_name="RPDO03 communication parameter/highest sub-index supported",.i=64},
		{.parameter_name="RPDO03 communication parameter/inhibit time",.i=67},
		{.parameter_name="RPDO03 communication parameter/SYNC start value",.i=70},
		{.parameter_name="RPDO03 communication parameter/Transmission type",.i=66},
		{.parameter_name="RPDO03 mapping parameter/1st object",.i=141},
		{.parameter_name="RPDO03 mapping parameter/2nd object",.i=142},
		{.parameter_name="RPDO03 mapping parameter/3rd object",.i=143},
		{.parameter_name="RPDO03 mapping parameter/4th object",.i=144},
		{.parameter_name="RPDO03 mapping parameter/5th object",.i=145},
		{.parameter_name="RPDO03 mapping parameter/6th object",.i=146},
		{.parameter_name="RPDO03 mapping parameter/7th object",.i=147},
		{.parameter_name="RPDO03 mapping parameter/8th object",.i=148},
		{.parameter_name="RPDO03 mapping parameter/number of mapped objects",.i=140},
		{.parameter_name="RPDO04 communication parameter/COB-ID",.i=72},
		{.parameter_name="RPDO04 communication parameter/compatibility entry",.i=75},
		{.parameter_name="RPDO04 communication parameter/event timer",.i=76},
		{.parameter_name="RPDO04 communication parameter/highest sub-index supported",.i=71},
		{.parameter_name="RPDO04 communication parameter/inhibit time",.i=74},
		{.parameter_name="RPDO04 communication parameter/SYNC start value",.i=77},
		{.parameter_name="RPDO04 communication parameter/Transmission type",.i=73},
		{.parameter_name="RPDO04 mapping parameter/1st object",.i=150},
		{.parameter_name="RPDO04 mapping parameter/2nd object",.i=151},
		{.parameter_name="RPDO04 mapping parameter/3rd object",.i=152},
		{.parameter_name="RPDO04 mapping parameter/4th object",.i=153},
		{.parameter_name="RPDO04 mapping parameter/5th object",.i=154},
		{.parameter_name="RPDO04 mapping parameter/6th object",.i=155},
		{.parameter_name="RPDO04 mapping parameter/7th object",.i=156},
		{.parameter_name="RPDO04 mapping parameter/8th object",.i=157},
		{.parameter_name="RPDO04 mapping parameter/number of mapped objects",.i=149},
		{.parameter_name="RPDO05 communication parameter/COB-ID",.i=79},
		{.parameter_name="RPDO05 communication parameter/compatibility entry",.i=82},
		{.parameter_name="RPDO05 communication parameter/event timer",.i=83},
		{.parameter_name="RPDO05 communication parameter/highest sub-index supported",.i=78},
		{.parameter_name="RPDO05 communication parameter/inhibit time",.i=81},
		{.parameter_name="RPDO05 communication parameter/SYNC start value",.i=84},
		{.parameter_name="RPDO05 communication parameter/Transmission type",.i=80},
		{.parameter_name="RPDO05 mapping parameter/1st object",.i=159},
		{.parameter_name="RPDO05 mapping parameter/2nd object",.i=160},
		{.parameter_name="RPDO05 mapping parameter/3rd object",.i=161},
		{.parameter_name="RPDO05 mapping parameter/4th object",.i=162},
		{.parameter_name="RPDO05 mapping parameter/5th object",.i=163},
		{.parameter_name="RPDO05 mapping parameter/6th object",.i=164},
		{.parameter_name="RPDO05 mapping parameter/7th object",.i=165},
		{.parameter_name="RPDO05 mapping parameter/8th object",.i=166},
		{.parameter_name="RPDO05 mapping parameter/number of mapped objects",.i=158},
		{.parameter_name="RPDO06 communication parameter/COB-ID",.i=86},
		{.parameter_name="RPDO06 communication parameter/compatibility entry",.i=89},
		{.parameter_name="RPDO06 communication parameter/event timer",.i=90},
		{.parameter_name="RPDO06 communication parameter/highest sub-index supported",.i=85},
		{.parameter_name="RPDO06 communication parameter/inhibit time",.i=88},
		{.parameter_name="RPDO06 communication parameter/SYNC start value",.i=91},
		{.parameter_name="RPDO06 communication parameter/Transmission type",.i=87},
		{.parameter_name="RPDO06 mapping parameter/1st object",.i=168},
		{.parameter_name="RPDO06 mapping parameter/2nd object",.i=169},
		{.parameter_name="RPDO06 mapping parameter/3rd object",.i=170},
		{.parameter_name="RPDO06 mapping parameter/4th object",.i=171},
		{.parameter_name="RPDO06 mapping parameter/5th object",.i=172},
		{.parameter_name="RPDO06 mapping parameter/6th object",.i=173},
		{.parameter_name="RPDO06 mapping parameter/7th object",.i=174},
		{.parameter_name="RPDO06 mapping parameter/8th object",.i=175},
		{.parameter_name="RPDO06 mapping parameter/number of mapped objects",.i=167},
		{.parameter_name="RPDO07 communication parameter/COB-ID",.i=93},
		{.parameter_name="RPDO07 communication parameter/compatibility entry",.i=96},
		{.parameter_name="RPDO07 communication parameter/event timer",.i=97},
		{.parameter_name="RPDO07 communication parameter/highest sub-index supported",.i=92},
		{.parameter_name="RPDO07 communication parameter/inhibit time",.i=95},
		{.parameter_name="RPDO07 communication parameter/SYNC start value",.i=98},
		{.parameter_name="RPDO07 communication parameter/Transmission type",.i=94},
		{.parameter_name="RPDO07 mapping parameter/1st object",.i=177},
		{.parameter_name="RPDO07 mapping parameter/2nd object",.i=178},
		{.parameter_name="RPDO07 mapping parameter/3rd object",.i=179},
		{.parameter_name="RPDO07 mapping parameter/4th object",.i=180},
		{.parameter_name="RPDO07 mapping parameter/5th object",.i=181},
		{.parameter_name="RPDO07 mapping parameter/6th object",.i=182},
		{.parameter_name="RPDO07 mapping parameter/7th object",.i=183},
		{.parameter_name="RPDO07 mapping parameter/8th object",.i=184},
		{.parameter_name="RPDO07 mapping parameter/number of mapped objects",.i=176},
		{.parameter_name="RPDO08 communication parameter/COB-ID",.i=100},
		{.parameter_name="RPDO08 communication parameter/compatibility entry",.i=103},
		{.parameter_name="RPDO08 communication parameter/event timer",.i=104},
		{.parameter_name="RPDO08 communication parameter/highest sub-index supported",.i=99},
		{.parameter_name="RPDO08 communication parameter/inhibit time",.i=102},
		{.parameter_name="RPDO08 communication parameter/SYNC start value",.i=105},
		{.parameter_name="RPDO08 communication parameter/Transmission type",.i=101},
		{.parameter_name="RPDO08 mapping parameter/1st object",.i=186},
		{.parameter_name="RPDO08 mapping parameter/2nd object",.i=187},
		{.parameter_name="RPDO08 mapping parameter/3rd object",.i=188},
		{.parameter_name="RPDO08 mapping parameter/4th object",.i=189},
		{.parameter_name="RPDO08 mapping parameter/5th object",.i=190},
		{.parameter_name="RPDO08 mapping parameter/6th object",.i=191},
		{.parameter_name="RPDO08 mapping parameter/7th object",.i=192},
		{.parameter_name="RPDO08 mapping parameter/8th object",.i=193},
		{.parameter_name="RPDO08 mapping parameter/number of mapped objects",.i=185},
		{.parameter_name="RPDO09 communication parameter/COB-ID",.i=107},
		{.parameter_name="RPDO09 communication parameter/compatibility entry",.i=110},
		{.parameter_name="RPDO09 communication parameter/event timer",.i=111},
		{.parameter_name="RPDO09 communication parameter/highest sub-index supported",.i=106},
		{.parameter_name="RPDO09 communication parameter/inhibit time",.i=109},
		{.parameter_name="RPDO09 communication parameter/SYNC start value",.i=112},
		{.parameter_name="RPDO09 communication parameter/Transmission type",.i=108},
		{.parameter_name="RPDO09 mapping parameter/1st object",.i=195},
		{.parameter_name="RPDO09 mapping parameter/2nd object",.i=196},
		{.parameter_name="RPDO09 mapping parameter/3rd object",.i=197},
		{.parameter_name="RPDO09 mapping parameter/4th object",.i=198},
		{.parameter_name="RPDO09 mapping parameter/5th object",.i=199},
		{.parameter_name="RPDO09 mapping parameter/6th object",.i=200},
		{.parameter_name="RPDO09 mapping parameter/7th object",.i=201},
		{.parameter_name="RPDO09 mapping parameter/8th object",.i=202},
		{.parameter_name="RPDO09 mapping parameter/number of mapped objects",.i=194},
		{.parameter_name="SECT CONFIG",.i=343},
		{.parameter_name="Software position limit",.i=400},
		{.parameter_name="Software position limit/Max position range limit",.i=403},
		{.parameter_name="Software position limit/Min position range limit",.i=402},
		{.parameter_name="Software position limit/number of entries",.i=401},
		{.parameter_name="Statusword",.i=377},
		{.parameter_name="Store EDS",.i=34},
		{.parameter_name="Store format",.i=35},
		{.parameter_name="Store parameters",.i=11},
		{.parameter_name="Store parameters/save all parameters",.i=12},
		{.parameter_name="Store parameters/save application parameters",.i=14},
		{.parameter_name="Store parameters/save communication parameters",.i=13},
		{.parameter_name="Supported drive modes",.i=466},
		{.parameter_name="sync timeout factor",.i=357},
		{.parameter_name="Synchronous counter overflow value",.i=30},
		{.parameter_name="Synchronous window length",.i=6},
		{.parameter_name="Target position",.i=394},
		{.parameter_name="Target velocity",.i=458},
		{.parameter_name="TD Current",.i=360},
		{.parameter_name="TD Pos",.i=369},
		{.parameter_name="TD Speed",.i=364},
		{.parameter_name="TN Current",.i=359},
		{.parameter_name="TN Pos",.i=368},
		{.parameter_name="TN Speed",.i=363},
		{.parameter_name="TPDO00 communication parameter/COB-ID",.i=203},
		{.parameter_name="TPDO00 communication parameter/Event timer",.i=205},
		{.parameter_name="TPDO00 communication parameter/Transmission type",.i=204},
		{.parameter_name="TPDO00 mapping parameter/1st object",.i=234},
		{.parameter_name="TPDO00 mapping parameter/2nd object",.i=235},
		{.parameter_name="TPDO00 mapping parameter/3rd object",.i=236},
		{.parameter_name="TPDO00 mapping parameter/4th object",.i=237},
		{.parameter_name="TPDO00 mapping parameter/5th object",.i=238},
		{.parameter_name="TPDO00 mapping parameter/6th object",.i=239},
		{.parameter_name="TPDO00 mapping parameter/7th object",.i=240},
		{.parameter_name="TPDO00 mapping parameter/8th object",.i=241},
		{.parameter_name="TPDO00 mapping parameter/number of mapped objects",.i=233},
		{.parameter_name="TPDO01 communication parameter/COB-ID",.i=206},
		{.parameter_name="TPDO01 communication parameter/Event timer",.i=208},
		{.parameter_name="TPDO01 communication parameter/Transmission type",.i=207},
		{.parameter_name="TPDO01 mapping parameter/1st object",.i=243},
		{.parameter_name="TPDO01 mapping parameter/2nd object",.i=244},
		{.parameter_name="TPDO01 mapping parameter/3rd object",.i=245},
		{.parameter_name="TPDO01 mapping parameter/4th object",.i=246},
		{.parameter_name="TPDO01 mapping parameter/5th object",.i=247},
		{.parameter_name="TPDO01 mapping parameter/6th object",.i=248},
		{.parameter_name="TPDO01 mapping parameter/7th object",.i=249},
		{.parameter_name="TPDO01 mapping parameter/8th object",.i=250},
		{.parameter_name="TPDO01 mapping parameter/number of mapped objects",.i=242},
		{.parameter_name="TPDO02 communication parameter/COB-ID",.i=209},
		{.parameter_name="TPDO02 communication parameter/Event timer",.i=211},
		{.parameter_name="TPDO02 communication parameter/Transmission type",.i=210},
		{.parameter_name="TPDO02 mapping parameter/1st object",.i=252},
		{.parameter_name="TPDO02 mapping parameter/2nd object",.i=253},
		{.parameter_name="TPDO02 mapping parameter/3rd object",.i=254},
		{.parameter_name="TPDO02 mapping parameter/4th object",.i=255},
		{.parameter_name="TPDO02 mapping parameter/5th object",.i=256},
		{.parameter_name="TPDO02 mapping parameter/6th object",.i=257},
		{.parameter_name="TPDO02 mapping parameter/7th object",.i=258},
		{.parameter_name="TPDO02 mapping parameter/8th object",.i=259},
		{.parameter_name="TPDO02 mapping parameter/number of mapped objects",.i=251},
		{.parameter_name="TPDO03 communication parameter/COB-ID",.i=212},
		{.parameter_name="TPDO03 communication parameter/Event timer",.i=214},
		{.parameter_name="TPDO03 communication parameter/Transmission type",.i=213},
		{.parameter_name="TPDO03 mapping parameter/1st object",.i=261},
		{.parameter_name="TPDO03 mapping parameter/2nd object",.i=262},
		{.parameter_name="TPDO03 mapping parameter/3rd object",.i=263},
		{.parameter_name="TPDO03 mapping parameter/4th object",.i=264},
		{.parameter_name="TPDO03 mapping parameter/5th object",.i=265},
		{.parameter_name="TPDO03 mapping parameter/6th object",.i=266},
		{.parameter_name="TPDO03 mapping parameter/7th object",.i=267},
		{.parameter_name="TPDO03 mapping parameter/8th object",.i=268},
		{.parameter_name="TPDO03 mapping parameter/number of mapped objects",.i=260},
		{.parameter_name="TPDO04 communication parameter/COB-ID",.i=215},
		{.parameter_name="TPDO04 communication parameter/Event timer",.i=217},
		{.parameter_name="TPDO04 communication parameter/Transmission type",.i=216},
		{.parameter_name="TPDO04 mapping parameter/1st object",.i=270},
		{.parameter_name="TPDO04 mapping parameter/2nd object",.i=271},
		{.parameter_name="TPDO04 mapping parameter/3rd object",.i=272},
		{.parameter_name="TPDO04 mapping parameter/4th object",.i=273},
		{.parameter_name="TPDO04 mapping parameter/5th object",.i=274},
		{.parameter_name="TPDO04 mapping parameter/6th object",.i=275},
		{.parameter_name="TPDO04 mapping parameter/7th object",.i=276},
		{.parameter_name="TPDO04 mapping parameter/8th object",.i=277},
		{.parameter_name="TPDO04 mapping parameter/number of mapped objects",.i=269},
		{.parameter_name="TPDO05 communication parameter/COB-ID",.i=218},
		{.parameter_name="TPDO05 communication parameter/Event timer",.i=220},
		{.parameter_name="TPDO05 communication parameter/Transmission type",.i=219},
		{.parameter_name="TPDO05 mapping parameter/1st object",.i=279},
		{.parameter_name="TPDO05 mapping parameter/2nd object",.i=280},
		{.parameter_name="TPDO05 mapping parameter/3rd object",.i=281},
		{.parameter_name="TPDO05 mapping parameter/4th object",.i=282},
		{.parameter_name="TPDO05 mapping parameter/5th object",.i=283},
		{.parameter_name="TPDO05 mapping parameter/6th object",.i=284},
		{.parameter_name="TPDO05 mapping parameter/7th object",.i=285},
		{.parameter_name="TPDO05 mapping parameter/8th object",.i=286},
		{.parameter_name="TPDO05 mapping parameter/number of mapped objects",.i=278},
		{.parameter_name="TPDO06 communication parameter/COB-ID",.i=221},
		{.parameter_name="TPDO06 communication parameter/Event timer",.i=223},
		{.parameter_name="TPDO06 communication parameter/Transmission type",.i=222},
		{.parameter_name="TPDO06 mapping parameter/1st object",.i=288},
		{.parameter_name="TPDO06 mapping parameter/2nd object",.i=289},
		{.parameter_name="TPDO06 mapping parameter/3rd object",.i=290},
		{.parameter_name="TPDO06 mapping parameter/4th object",.i=291},
		{.parameter_name="TPDO06 mapping parameter/5th object",.i=292},
		{.parameter_name="TPDO06 mapping parameter/6th object",.i=293},
		{.parameter_name="TPDO06 mapping parameter/7th object",.i=294},
		{.parameter_name="TPDO06 mapping parameter/8th object",.i=295},
		{.parameter_name="TPDO06 mapping parameter/number of mapped objects",.i=287},
		{.parameter_name="TPDO07 communication parameter/COB-ID",.i=224},
		{.parameter_name="TPDO07 communication parameter/Event timer",.i=226},
		{.parameter_name="TPDO07 communication parameter/Transmission type",.i=225},
		{.parameter_name="TPDO07 mapping parameter/1st object",.i=297},
		{.parameter_name="TPDO07 mapping parameter/2nd object",.i=298},
		{.parameter_name="TPDO07 mapping parameter/3rd object",.i=299},
		{.parameter_name="TPDO07 mapping parameter/4th object",.i=300},
		{.parameter_name="TPDO07 mapping parameter/5th object",.i=301},
		{.parameter_name="TPDO07 mapping parameter/6th object",.i=302},
		{.parameter_name="TPDO07 mapping parameter/7th object",.i=303},
		{.parameter_name="TPDO07 mapping parameter/8th object",.i=304},
		{.parameter_name="TPDO07 mapping parameter/number of mapped objects",.i=296},
		{.parameter_name="TPDO08 communication parameter/COB-ID",.i=227},
		{.parameter_name="TPDO08 communication parameter/Event timer",.i=229},
		{.parameter_name="TPDO08 communication parameter/Transmission type",.i=228},
		{.parameter_name="TPDO08 mapping parameter/1st object",.i=306},
		{.parameter_name="TPDO08 mapping parameter/2nd object",.i=307},
		{.parameter_name="TPDO08 mapping parameter/3rd object",.i=308},
		{.parameter_name="TPDO08 mapping parameter/4th object",.i=309},
		{.parameter_name="TPDO08 mapping parameter/5th object",.i=310},
		{.parameter_name="TPDO08 mapping parameter/6th object",.i=311},
		{.parameter_name="TPDO08 mapping parameter/7th object",.i=312},
		{.parameter_name="TPDO08 mapping parameter/8th object",.i=313},
		{.parameter_name="TPDO08 mapping parameter/number of mapped objects",.i=305},
		{.parameter_name="TPDO09 communication parameter/COB-ID",.i=230},
		{.parameter_name="TPDO09 communication parameter/Event timer",.i=232},
		{.parameter_name="TPDO09 communication parameter/Transmission type",.i=231},
		{.parameter_name="TPDO09 mapping parameter/1st object",.i=315},
		{.parameter_name="TPDO09 mapping parameter/2nd object",.i=316},
		{.parameter_name="TPDO09 mapping parameter/3rd object",.i=317},
		{.parameter_name="TPDO09 mapping parameter/4th object",.i=318},
		{.parameter_name="TPDO09 mapping parameter/5th object",.i=319},
		{.parameter_name="TPDO09 mapping parameter/6th object",.i=320},
		{.parameter_name="TPDO09 mapping parameter/7th object",.i=321},
		{.parameter_name="TPDO09 mapping parameter/8th object",.i=322},
		{.parameter_name="TPDO09 mapping parameter/number of mapped objects",.i=314},
		{.parameter_name="User",.i=345},
		{.parameter_name="Velocity actual value",.i=391},
		{.parameter_name="Velocity demand value",.i=390},
		{.parameter_name="Velocity dimension index",.i=414},
		{.parameter_name="Velocity encoder factor",.i=437},
		{.parameter_name="Velocity encoder factor/Divisor",.i=440},
		{.parameter_name="Velocity encoder factor/number of entries",.i=438},
		{.parameter_name="Velocity encoder factor/Numerator",.i=439},
		{.parameter_name="Velocity encoder resolution",.i=421},
		{.parameter_name="Velocity encoder resolution/Encoder increments per second",.i=423},
		{.parameter_name="Velocity encoder resolution/Motor revolutions per second",.i=424},
		{.parameter_name="Velocity encoder resolution/number of entries",.i=422},
		{.parameter_name="Velocity factor 1",.i=441},
		{.parameter_name="Velocity factor 1/Divisor",.i=444},
		{.parameter_name="Velocity factor 1/number of entries",.i=442},
		{.parameter_name="Velocity factor 1/Numerator",.i=443},
		{.parameter_name="Velocity factor 2",.i=445},
		{.parameter_name="Velocity factor 2/Divisor",.i=448},
		{.parameter_name="Velocity factor 2/number of entries",.i=446},
		{.parameter_name="Velocity factor 2/Numerator",.i=447},
		{.parameter_name="Velocity notation index",.i=413},
		{.parameter_name="Velocity threshold",.i=392},
		{.parameter_name="Velocity threshold time",.i=393},
		{.parameter_name="Verify configuration",.i=31},
		{.parameter_name="Verify configuration/Configuration date",.i=32},
		{.parameter_name="Verify configuration/Configuration time",.i=33},
		{.parameter_name="vl actual percentage",.i=383},
		{.parameter_name="vl control effort",.i=380},
		{.parameter_name="vl manipulated percentage",.i=384},
		{.parameter_name="vl percentage demand",.i=382},
		{.parameter_name="vl target velocity",.i=378},
		{.parameter_name="vl velocity demand",.i=379},
		{.parameter_name="vl velocity reference",.i=381},
	},
	.obj=(canmat_obj_t[]){
	{
		.index=0x1000,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Device type"
	},
	{
		.index=0x1001,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.mask_descriptor = (struct canmat_code_descriptor[]){
			{.name = "temp", .value=(1 << 3), .description="Temperature Error"},
			{.name = "dev_prof", .value=(1 << 5), .description="Device profile specific"},
			{.name = "current", .value=(1 << 1), .description="Current Error"},
			{.name = "mfr", .value=(1 << 7), .description="manufacturer-specific"},
			{.name = "comm", .value=(1 << 4), .description="Communication error (overrun, error state)"},
			{.name = "voltage", .value=(1 << 2), .description="Voltage Error"},
			{.name = "error", .value=(1 << 0), .description="Generic Error"},
			{.name = NULL, .value=0, .description=NULL}
		},
		.parameter_name="Error register"
	},
	{
		.index=0x1002,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Manufacturer status register"
	},
	{
		.index=0x1003,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.parameter_name="Pre-defined error field"
	},
	{
		.index=0x1005,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="COB-ID SYNC"
	},
	{
		.index=0x1006,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Communication cycle period"
	},
	{
		.index=0x1007,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Synchronous window length"
	},
	{
		.index=0x1008,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_VISIBLE_STRING,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Manufacturer device name"
	},
	{
		.index=0x1009,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_VISIBLE_STRING,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Manufacturer software version"
	},
	{
		.index=0x100c,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Guard time"
	},
	{
		.index=0x100d,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Life time favtor"
	},
	{
		.index=0x1010,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Store parameters"
	},
	{
		.index=0x1010,
		.subindex=0x01,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Store parameters/save all parameters"
	},
	{
		.index=0x1010,
		.subindex=0x02,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Store parameters/save communication parameters"
	},
	{
		.index=0x1010,
		.subindex=0x03,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Store parameters/save application parameters"
	},
	{
		.index=0x1011,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Restore default parameters"
	},
	{
		.index=0x1011,
		.subindex=0x01,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Restore default parameters/restore all default parameters"
	},
	{
		.index=0x1011,
		.subindex=0x02,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Restore default parameters/restore communication default parameters"
	},
	{
		.index=0x1011,
		.subindex=0x03,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Restore default parameters/restore application default parameters"
	},
	{
		.index=0x1012,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="COB-ID time stamp message"
	},
	{
		.index=0x1013,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="High resolution time stamp"
	},
	{
		.index=0x1014,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="COB-ID emergency message"
	},
	{
		.index=0x1015,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Inhibit time EMCY"
	},
	{
		.index=0x1016,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Consumer heartbeat time"
	},
	{
		.index=0x1017,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Producer heartbeat time"
	},
	{
		.index=0x1018,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_IDENTITY,
		.object_type=CANMAT_OBJECT_TYPE_RECORD,
		.parameter_name="Identity object"
	},
	{
		.index=0x1018,
		.subindex=0x01,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Identity object/Vendor-ID"
	},
	{
		.index=0x1018,
		.subindex=0x02,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Identity object/Product code"
	},
	{
		.index=0x1018,
		.subindex=0x03,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Identity object/Revision number"
	},
	{
		.index=0x1018,
		.subindex=0x04,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Identity object/Serial number"
	},
	{
		.index=0x1019,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Synchronous counter overflow value"
	},
	{
		.index=0x1020,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.parameter_name="Verify configuration"
	},
	{
		.index=0x1020,
		.subindex=0x01,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Verify configuration/Configuration date"
	},
	{
		.index=0x1020,
		.subindex=0x02,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Verify configuration/Configuration time"
	},
	{
		.index=0x1021,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_DOMAIN,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Store EDS"
	},
	{
		.index=0x1022,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Store format"
	},
	{
		.index=0x1023,
		.subindex=0x00,
		.pdo_mapping=0,
		.object_type=CANMAT_OBJECT_TYPE_RECORD,
		.parameter_name="OS command"
	},
	{
		.index=0x1023,
		.subindex=0x02,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="OS command/status"
	},
	{
		.index=0x1023,
		.subindex=0x03,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="OS command/reply"
	},
	{
		.index=0x1024,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.parameter_name="OS command mode"
	},
	{
		.index=0x1028,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Emergency consumer"
	},
	{
		.index=0x1029,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Error behavior"
	},
	{
		.index=0x1029,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Error behavior/communication error"
	},
	{
		.index=0x1400,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="RPDO00 communication parameter/highest sub-index supported"
	},
	{
		.index=0x1400,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO00 communication parameter/COB-ID"
	},
	{
		.index=0x1400,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO00 communication parameter/Transmission type"
	},
	{
		.index=0x1400,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO00 communication parameter/inhibit time"
	},
	{
		.index=0x1400,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO00 communication parameter/compatibility entry"
	},
	{
		.index=0x1400,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO00 communication parameter/event timer"
	},
	{
		.index=0x1400,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO00 communication parameter/SYNC start value"
	},
	{
		.index=0x1401,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="RPDO01 communication parameter/highest sub-index supported"
	},
	{
		.index=0x1401,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO01 communication parameter/COB-ID"
	},
	{
		.index=0x1401,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO01 communication parameter/Transmission type"
	},
	{
		.index=0x1401,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO01 communication parameter/inhibit time"
	},
	{
		.index=0x1401,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO01 communication parameter/compatibility entry"
	},
	{
		.index=0x1401,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO01 communication parameter/event timer"
	},
	{
		.index=0x1401,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO01 communication parameter/SYNC start value"
	},
	{
		.index=0x1402,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="RPDO02 communication parameter/highest sub-index supported"
	},
	{
		.index=0x1402,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO02 communication parameter/COB-ID"
	},
	{
		.index=0x1402,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO02 communication parameter/Transmission type"
	},
	{
		.index=0x1402,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO02 communication parameter/inhibit time"
	},
	{
		.index=0x1402,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO02 communication parameter/compatibility entry"
	},
	{
		.index=0x1402,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO02 communication parameter/event timer"
	},
	{
		.index=0x1402,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO02 communication parameter/SYNC start value"
	},
	{
		.index=0x1403,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="RPDO03 communication parameter/highest sub-index supported"
	},
	{
		.index=0x1403,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO03 communication parameter/COB-ID"
	},
	{
		.index=0x1403,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO03 communication parameter/Transmission type"
	},
	{
		.index=0x1403,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO03 communication parameter/inhibit time"
	},
	{
		.index=0x1403,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO03 communication parameter/compatibility entry"
	},
	{
		.index=0x1403,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO03 communication parameter/event timer"
	},
	{
		.index=0x1403,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO03 communication parameter/SYNC start value"
	},
	{
		.index=0x1404,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="RPDO04 communication parameter/highest sub-index supported"
	},
	{
		.index=0x1404,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO04 communication parameter/COB-ID"
	},
	{
		.index=0x1404,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO04 communication parameter/Transmission type"
	},
	{
		.index=0x1404,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO04 communication parameter/inhibit time"
	},
	{
		.index=0x1404,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO04 communication parameter/compatibility entry"
	},
	{
		.index=0x1404,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO04 communication parameter/event timer"
	},
	{
		.index=0x1404,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO04 communication parameter/SYNC start value"
	},
	{
		.index=0x1405,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="RPDO05 communication parameter/highest sub-index supported"
	},
	{
		.index=0x1405,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO05 communication parameter/COB-ID"
	},
	{
		.index=0x1405,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO05 communication parameter/Transmission type"
	},
	{
		.index=0x1405,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO05 communication parameter/inhibit time"
	},
	{
		.index=0x1405,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO05 communication parameter/compatibility entry"
	},
	{
		.index=0x1405,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO05 communication parameter/event timer"
	},
	{
		.index=0x1405,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO05 communication parameter/SYNC start value"
	},
	{
		.index=0x1406,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="RPDO06 communication parameter/highest sub-index supported"
	},
	{
		.index=0x1406,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO06 communication parameter/COB-ID"
	},
	{
		.index=0x1406,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO06 communication parameter/Transmission type"
	},
	{
		.index=0x1406,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO06 communication parameter/inhibit time"
	},
	{
		.index=0x1406,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO06 communication parameter/compatibility entry"
	},
	{
		.index=0x1406,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO06 communication parameter/event timer"
	},
	{
		.index=0x1406,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO06 communication parameter/SYNC start value"
	},
	{
		.index=0x1407,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="RPDO07 communication parameter/highest sub-index supported"
	},
	{
		.index=0x1407,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO07 communication parameter/COB-ID"
	},
	{
		.index=0x1407,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO07 communication parameter/Transmission type"
	},
	{
		.index=0x1407,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO07 communication parameter/inhibit time"
	},
	{
		.index=0x1407,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO07 communication parameter/compatibility entry"
	},
	{
		.index=0x1407,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO07 communication parameter/event timer"
	},
	{
		.index=0x1407,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO07 communication parameter/SYNC start value"
	},
	{
		.index=0x1408,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="RPDO08 communication parameter/highest sub-index supported"
	},
	{
		.index=0x1408,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO08 communication parameter/COB-ID"
	},
	{
		.index=0x1408,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO08 communication parameter/Transmission type"
	},
	{
		.index=0x1408,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO08 communication parameter/inhibit time"
	},
	{
		.index=0x1408,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO08 communication parameter/compatibility entry"
	},
	{
		.index=0x1408,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO08 communication parameter/event timer"
	},
	{
		.index=0x1408,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO08 communication parameter/SYNC start value"
	},
	{
		.index=0x1409,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="RPDO09 communication parameter/highest sub-index supported"
	},
	{
		.index=0x1409,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO09 communication parameter/COB-ID"
	},
	{
		.index=0x1409,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO09 communication parameter/Transmission type"
	},
	{
		.index=0x1409,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO09 communication parameter/inhibit time"
	},
	{
		.index=0x1409,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO09 communication parameter/compatibility entry"
	},
	{
		.index=0x1409,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO09 communication parameter/event timer"
	},
	{
		.index=0x1409,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO09 communication parameter/SYNC start value"
	},
	{
		.index=0x1600,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO00 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1600,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO00 mapping parameter/1st object"
	},
	{
		.index=0x1600,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO00 mapping parameter/2nd object"
	},
	{
		.index=0x1600,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO00 mapping parameter/3rd object"
	},
	{
		.index=0x1600,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO00 mapping parameter/4th object"
	},
	{
		.index=0x1600,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO00 mapping parameter/5th object"
	},
	{
		.index=0x1600,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO00 mapping parameter/6th object"
	},
	{
		.index=0x1600,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO00 mapping parameter/7th object"
	},
	{
		.index=0x1600,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO00 mapping parameter/8th object"
	},
	{
		.index=0x1601,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO01 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1601,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO01 mapping parameter/1st object"
	},
	{
		.index=0x1601,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO01 mapping parameter/2nd object"
	},
	{
		.index=0x1601,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO01 mapping parameter/3rd object"
	},
	{
		.index=0x1601,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO01 mapping parameter/4th object"
	},
	{
		.index=0x1601,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO01 mapping parameter/5th object"
	},
	{
		.index=0x1601,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO01 mapping parameter/6th object"
	},
	{
		.index=0x1601,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO01 mapping parameter/7th object"
	},
	{
		.index=0x1601,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO01 mapping parameter/8th object"
	},
	{
		.index=0x1602,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO02 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1602,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO02 mapping parameter/1st object"
	},
	{
		.index=0x1602,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO02 mapping parameter/2nd object"
	},
	{
		.index=0x1602,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO02 mapping parameter/3rd object"
	},
	{
		.index=0x1602,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO02 mapping parameter/4th object"
	},
	{
		.index=0x1602,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO02 mapping parameter/5th object"
	},
	{
		.index=0x1602,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO02 mapping parameter/6th object"
	},
	{
		.index=0x1602,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO02 mapping parameter/7th object"
	},
	{
		.index=0x1602,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO02 mapping parameter/8th object"
	},
	{
		.index=0x1603,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO03 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1603,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO03 mapping parameter/1st object"
	},
	{
		.index=0x1603,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO03 mapping parameter/2nd object"
	},
	{
		.index=0x1603,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO03 mapping parameter/3rd object"
	},
	{
		.index=0x1603,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO03 mapping parameter/4th object"
	},
	{
		.index=0x1603,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO03 mapping parameter/5th object"
	},
	{
		.index=0x1603,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO03 mapping parameter/6th object"
	},
	{
		.index=0x1603,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO03 mapping parameter/7th object"
	},
	{
		.index=0x1603,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO03 mapping parameter/8th object"
	},
	{
		.index=0x1604,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO04 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1604,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO04 mapping parameter/1st object"
	},
	{
		.index=0x1604,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO04 mapping parameter/2nd object"
	},
	{
		.index=0x1604,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO04 mapping parameter/3rd object"
	},
	{
		.index=0x1604,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO04 mapping parameter/4th object"
	},
	{
		.index=0x1604,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO04 mapping parameter/5th object"
	},
	{
		.index=0x1604,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO04 mapping parameter/6th object"
	},
	{
		.index=0x1604,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO04 mapping parameter/7th object"
	},
	{
		.index=0x1604,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO04 mapping parameter/8th object"
	},
	{
		.index=0x1605,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO05 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1605,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO05 mapping parameter/1st object"
	},
	{
		.index=0x1605,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO05 mapping parameter/2nd object"
	},
	{
		.index=0x1605,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO05 mapping parameter/3rd object"
	},
	{
		.index=0x1605,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO05 mapping parameter/4th object"
	},
	{
		.index=0x1605,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO05 mapping parameter/5th object"
	},
	{
		.index=0x1605,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO05 mapping parameter/6th object"
	},
	{
		.index=0x1605,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO05 mapping parameter/7th object"
	},
	{
		.index=0x1605,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO05 mapping parameter/8th object"
	},
	{
		.index=0x1606,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO06 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1606,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO06 mapping parameter/1st object"
	},
	{
		.index=0x1606,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO06 mapping parameter/2nd object"
	},
	{
		.index=0x1606,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO06 mapping parameter/3rd object"
	},
	{
		.index=0x1606,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO06 mapping parameter/4th object"
	},
	{
		.index=0x1606,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO06 mapping parameter/5th object"
	},
	{
		.index=0x1606,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO06 mapping parameter/6th object"
	},
	{
		.index=0x1606,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO06 mapping parameter/7th object"
	},
	{
		.index=0x1606,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO06 mapping parameter/8th object"
	},
	{
		.index=0x1607,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO07 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1607,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO07 mapping parameter/1st object"
	},
	{
		.index=0x1607,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO07 mapping parameter/2nd object"
	},
	{
		.index=0x1607,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO07 mapping parameter/3rd object"
	},
	{
		.index=0x1607,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO07 mapping parameter/4th object"
	},
	{
		.index=0x1607,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO07 mapping parameter/5th object"
	},
	{
		.index=0x1607,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO07 mapping parameter/6th object"
	},
	{
		.index=0x1607,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO07 mapping parameter/7th object"
	},
	{
		.index=0x1607,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO07 mapping parameter/8th object"
	},
	{
		.index=0x1608,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO08 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1608,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO08 mapping parameter/1st object"
	},
	{
		.index=0x1608,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO08 mapping parameter/2nd object"
	},
	{
		.index=0x1608,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO08 mapping parameter/3rd object"
	},
	{
		.index=0x1608,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO08 mapping parameter/4th object"
	},
	{
		.index=0x1608,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO08 mapping parameter/5th object"
	},
	{
		.index=0x1608,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO08 mapping parameter/6th object"
	},
	{
		.index=0x1608,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO08 mapping parameter/7th object"
	},
	{
		.index=0x1608,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO08 mapping parameter/8th object"
	},
	{
		.index=0x1609,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO09 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1609,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO09 mapping parameter/1st object"
	},
	{
		.index=0x1609,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO09 mapping parameter/2nd object"
	},
	{
		.index=0x1609,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO09 mapping parameter/3rd object"
	},
	{
		.index=0x1609,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO09 mapping parameter/4th object"
	},
	{
		.index=0x1609,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO09 mapping parameter/5th object"
	},
	{
		.index=0x1609,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO09 mapping parameter/6th object"
	},
	{
		.index=0x1609,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO09 mapping parameter/7th object"
	},
	{
		.index=0x1609,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="RPDO09 mapping parameter/8th object"
	},
	{
		.index=0x1800,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO00 communication parameter/COB-ID"
	},
	{
		.index=0x1800,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO00 communication parameter/Transmission type"
	},
	{
		.index=0x1800,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO00 communication parameter/Event timer"
	},
	{
		.index=0x1801,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO01 communication parameter/COB-ID"
	},
	{
		.index=0x1801,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO01 communication parameter/Transmission type"
	},
	{
		.index=0x1801,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO01 communication parameter/Event timer"
	},
	{
		.index=0x1802,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO02 communication parameter/COB-ID"
	},
	{
		.index=0x1802,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO02 communication parameter/Transmission type"
	},
	{
		.index=0x1802,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO02 communication parameter/Event timer"
	},
	{
		.index=0x1803,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO03 communication parameter/COB-ID"
	},
	{
		.index=0x1803,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO03 communication parameter/Transmission type"
	},
	{
		.index=0x1803,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO03 communication parameter/Event timer"
	},
	{
		.index=0x1804,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO04 communication parameter/COB-ID"
	},
	{
		.index=0x1804,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO04 communication parameter/Transmission type"
	},
	{
		.index=0x1804,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO04 communication parameter/Event timer"
	},
	{
		.index=0x1805,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO05 communication parameter/COB-ID"
	},
	{
		.index=0x1805,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO05 communication parameter/Transmission type"
	},
	{
		.index=0x1805,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO05 communication parameter/Event timer"
	},
	{
		.index=0x1806,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO06 communication parameter/COB-ID"
	},
	{
		.index=0x1806,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO06 communication parameter/Transmission type"
	},
	{
		.index=0x1806,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO06 communication parameter/Event timer"
	},
	{
		.index=0x1807,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO07 communication parameter/COB-ID"
	},
	{
		.index=0x1807,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO07 communication parameter/Transmission type"
	},
	{
		.index=0x1807,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO07 communication parameter/Event timer"
	},
	{
		.index=0x1808,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO08 communication parameter/COB-ID"
	},
	{
		.index=0x1808,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO08 communication parameter/Transmission type"
	},
	{
		.index=0x1808,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO08 communication parameter/Event timer"
	},
	{
		.index=0x1809,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO09 communication parameter/COB-ID"
	},
	{
		.index=0x1809,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO09 communication parameter/Transmission type"
	},
	{
		.index=0x1809,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO09 communication parameter/Event timer"
	},
	{
		.index=0x1a00,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO00 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1a00,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO00 mapping parameter/1st object"
	},
	{
		.index=0x1a00,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO00 mapping parameter/2nd object"
	},
	{
		.index=0x1a00,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO00 mapping parameter/3rd object"
	},
	{
		.index=0x1a00,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO00 mapping parameter/4th object"
	},
	{
		.index=0x1a00,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO00 mapping parameter/5th object"
	},
	{
		.index=0x1a00,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO00 mapping parameter/6th object"
	},
	{
		.index=0x1a00,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO00 mapping parameter/7th object"
	},
	{
		.index=0x1a00,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO00 mapping parameter/8th object"
	},
	{
		.index=0x1a01,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO01 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1a01,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO01 mapping parameter/1st object"
	},
	{
		.index=0x1a01,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO01 mapping parameter/2nd object"
	},
	{
		.index=0x1a01,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO01 mapping parameter/3rd object"
	},
	{
		.index=0x1a01,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO01 mapping parameter/4th object"
	},
	{
		.index=0x1a01,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO01 mapping parameter/5th object"
	},
	{
		.index=0x1a01,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO01 mapping parameter/6th object"
	},
	{
		.index=0x1a01,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO01 mapping parameter/7th object"
	},
	{
		.index=0x1a01,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO01 mapping parameter/8th object"
	},
	{
		.index=0x1a02,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO02 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1a02,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO02 mapping parameter/1st object"
	},
	{
		.index=0x1a02,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO02 mapping parameter/2nd object"
	},
	{
		.index=0x1a02,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO02 mapping parameter/3rd object"
	},
	{
		.index=0x1a02,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO02 mapping parameter/4th object"
	},
	{
		.index=0x1a02,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO02 mapping parameter/5th object"
	},
	{
		.index=0x1a02,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO02 mapping parameter/6th object"
	},
	{
		.index=0x1a02,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO02 mapping parameter/7th object"
	},
	{
		.index=0x1a02,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO02 mapping parameter/8th object"
	},
	{
		.index=0x1a03,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO03 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1a03,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO03 mapping parameter/1st object"
	},
	{
		.index=0x1a03,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO03 mapping parameter/2nd object"
	},
	{
		.index=0x1a03,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO03 mapping parameter/3rd object"
	},
	{
		.index=0x1a03,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO03 mapping parameter/4th object"
	},
	{
		.index=0x1a03,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO03 mapping parameter/5th object"
	},
	{
		.index=0x1a03,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO03 mapping parameter/6th object"
	},
	{
		.index=0x1a03,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO03 mapping parameter/7th object"
	},
	{
		.index=0x1a03,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO03 mapping parameter/8th object"
	},
	{
		.index=0x1a04,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO04 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1a04,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO04 mapping parameter/1st object"
	},
	{
		.index=0x1a04,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO04 mapping parameter/2nd object"
	},
	{
		.index=0x1a04,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO04 mapping parameter/3rd object"
	},
	{
		.index=0x1a04,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO04 mapping parameter/4th object"
	},
	{
		.index=0x1a04,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO04 mapping parameter/5th object"
	},
	{
		.index=0x1a04,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO04 mapping parameter/6th object"
	},
	{
		.index=0x1a04,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO04 mapping parameter/7th object"
	},
	{
		.index=0x1a04,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO04 mapping parameter/8th object"
	},
	{
		.index=0x1a05,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO05 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1a05,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO05 mapping parameter/1st object"
	},
	{
		.index=0x1a05,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO05 mapping parameter/2nd object"
	},
	{
		.index=0x1a05,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO05 mapping parameter/3rd object"
	},
	{
		.index=0x1a05,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO05 mapping parameter/4th object"
	},
	{
		.index=0x1a05,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO05 mapping parameter/5th object"
	},
	{
		.index=0x1a05,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO05 mapping parameter/6th object"
	},
	{
		.index=0x1a05,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO05 mapping parameter/7th object"
	},
	{
		.index=0x1a05,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO05 mapping parameter/8th object"
	},
	{
		.index=0x1a06,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO06 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1a06,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO06 mapping parameter/1st object"
	},
	{
		.index=0x1a06,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO06 mapping parameter/2nd object"
	},
	{
		.index=0x1a06,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO06 mapping parameter/3rd object"
	},
	{
		.index=0x1a06,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO06 mapping parameter/4th object"
	},
	{
		.index=0x1a06,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO06 mapping parameter/5th object"
	},
	{
		.index=0x1a06,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO06 mapping parameter/6th object"
	},
	{
		.index=0x1a06,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO06 mapping parameter/7th object"
	},
	{
		.index=0x1a06,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO06 mapping parameter/8th object"
	},
	{
		.index=0x1a07,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO07 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1a07,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO07 mapping parameter/1st object"
	},
	{
		.index=0x1a07,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO07 mapping parameter/2nd object"
	},
	{
		.index=0x1a07,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO07 mapping parameter/3rd object"
	},
	{
		.index=0x1a07,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO07 mapping parameter/4th object"
	},
	{
		.index=0x1a07,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO07 mapping parameter/5th object"
	},
	{
		.index=0x1a07,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO07 mapping parameter/6th object"
	},
	{
		.index=0x1a07,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO07 mapping parameter/7th object"
	},
	{
		.index=0x1a07,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO07 mapping parameter/8th object"
	},
	{
		.index=0x1a08,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO08 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1a08,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO08 mapping parameter/1st object"
	},
	{
		.index=0x1a08,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO08 mapping parameter/2nd object"
	},
	{
		.index=0x1a08,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO08 mapping parameter/3rd object"
	},
	{
		.index=0x1a08,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO08 mapping parameter/4th object"
	},
	{
		.index=0x1a08,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO08 mapping parameter/5th object"
	},
	{
		.index=0x1a08,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO08 mapping parameter/6th object"
	},
	{
		.index=0x1a08,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO08 mapping parameter/7th object"
	},
	{
		.index=0x1a08,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO08 mapping parameter/8th object"
	},
	{
		.index=0x1a09,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO09 mapping parameter/number of mapped objects"
	},
	{
		.index=0x1a09,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO09 mapping parameter/1st object"
	},
	{
		.index=0x1a09,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO09 mapping parameter/2nd object"
	},
	{
		.index=0x1a09,
		.subindex=0x03,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO09 mapping parameter/3rd object"
	},
	{
		.index=0x1a09,
		.subindex=0x04,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO09 mapping parameter/4th object"
	},
	{
		.index=0x1a09,
		.subindex=0x05,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO09 mapping parameter/5th object"
	},
	{
		.index=0x1a09,
		.subindex=0x06,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO09 mapping parameter/6th object"
	},
	{
		.index=0x1a09,
		.subindex=0x07,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO09 mapping parameter/7th object"
	},
	{
		.index=0x1a09,
		.subindex=0x08,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TPDO09 mapping parameter/8th object"
	},
	{
		.index=0x2000,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="manufacturer debug flags"
	},
	{
		.index=0x2001,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_CONST,
		.parameter_name="nb debug values int32"
	},
	{
		.index=0x2001,
		.subindex=0x00,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.parameter_name="debug values int32"
	},
	{
		.index=0x2001,
		.subindex=0x01,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RWR,
		.parameter_name="debug value int32 1"
	},
	{
		.index=0x2001,
		.subindex=0x02,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RWR,
		.parameter_name="debug value int32 2"
	},
	{
		.index=0x2001,
		.subindex=0x03,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RWR,
		.parameter_name="debug value int32 3"
	},
	{
		.index=0x2001,
		.subindex=0x04,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RWR,
		.parameter_name="debug value int32 4"
	},
	{
		.index=0x2002,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_CONST,
		.parameter_name="nb debug values float"
	},
	{
		.index=0x2002,
		.subindex=0x00,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.parameter_name="debug values float"
	},
	{
		.index=0x2002,
		.subindex=0x01,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RWR,
		.parameter_name="debug value float 1"
	},
	{
		.index=0x2002,
		.subindex=0x02,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RWR,
		.parameter_name="debug value float 2"
	},
	{
		.index=0x2002,
		.subindex=0x03,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RWR,
		.parameter_name="debug value float 3"
	},
	{
		.index=0x2002,
		.subindex=0x04,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RWR,
		.parameter_name="debug value float 4"
	},
	{
		.index=0x2003,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_VISIBLE_STRING,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="debug message"
	},
	{
		.index=0x2004,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_CONST,
		.parameter_name="nb logging"
	},
	{
		.index=0x2004,
		.subindex=0x00,
		.object_type=CANMAT_OBJECT_TYPE_RECORD,
		.parameter_name="logging"
	},
	{
		.index=0x2004,
		.subindex=0x01,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="logging state"
	},
	{
		.index=0x2004,
		.subindex=0x02,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="logging trigger time"
	},
	{
		.index=0x2007,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Number of entries"
	},
	{
		.index=0x2007,
		.subindex=0x00,
		.object_type=CANMAT_OBJECT_TYPE_RECORD,
		.parameter_name="EEPROM Parameters"
	},
	{
		.index=0x2007,
		.subindex=0x01,
		.pdo_mapping=0,
		.data_type=0X000A,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="SECT CONFIG"
	},
	{
		.index=0x2008,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_VISIBLE_STRING,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_WO,
		.parameter_name="Password"
	},
	{
		.index=0x2009,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="User"
	},
	{
		.index=0x200a,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_WO,
		.parameter_name="Disconnect Reset"
	},
	{
		.index=0x200b,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Number of entries"
	},
	{
		.index=0x200b,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_RECORD,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="error details"
	},
	{
		.index=0x200b,
		.subindex=0x01,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="detail"
	},
	{
		.index=0x200b,
		.subindex=0x02,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_VISIBLE_STRING,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="file"
	},
	{
		.index=0x200b,
		.subindex=0x03,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="line"
	},
	{
		.index=0x200c,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="communication mode"
	},
	{
		.index=0x200d,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Number of entries"
	},
	{
		.index=0x200d,
		.subindex=0x00,
		.object_type=CANMAT_OBJECT_TYPE_RECORD,
		.parameter_name="pcb temperature"
	},
	{
		.index=0x200d,
		.subindex=0x01,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="actual pcb temperature"
	},
	{
		.index=0x200d,
		.subindex=0x02,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="pcb temperature update period ms"
	},
	{
		.index=0x200e,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="sync timeout factor"
	},
	{
		.index=0x2010,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="KR Current"
	},
	{
		.index=0x2011,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TN Current"
	},
	{
		.index=0x2012,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TD Current"
	},
	{
		.index=0x2013,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="KC Current"
	},
	{
		.index=0x2020,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="KR Speed"
	},
	{
		.index=0x2021,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TN Speed"
	},
	{
		.index=0x2022,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TD Speed"
	},
	{
		.index=0x2023,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="KC Speed"
	},
	{
		.index=0x2024,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="KS FeedForward"
	},
	{
		.index=0x2030,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="KR Pos"
	},
	{
		.index=0x2031,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TN Pos"
	},
	{
		.index=0x2032,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="TD Pos"
	},
	{
		.index=0x2033,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="KC Pos"
	},
	{
		.index=0x2034,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="KP FeedForward"
	},
	{
		.index=0x2041,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_REAL32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Delta Position"
	},
	{
		.index=0x2050,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="extended status"
	},
	{
		.index=0x6007,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.value_descriptor = (struct canmat_code_descriptor[]){
			{.name = "malfunction", .value=(1), .description="malfunction"},
			{.name = "quick_stop", .value=(3), .description="quick_stop"},
			{.name = "disable_voltage", .value=(2), .description="disable_voltage"},
			{.name = "no_action", .value=(0), .description="no_action"},
			{.name = NULL, .value=0, .description=NULL}
		},
		.parameter_name="Abort connection option code"
	},
	{
		.index=0x603f,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Error Code"
	},
	{
		.index=0x6040,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.mask_descriptor = (struct canmat_code_descriptor[]){
			{.name = "op_mode_specific1", .value=(1 <<  5), .description="op_mode_specific1"},
			{.name = "op_mode_specific0", .value=(1 <<  4), .description="op_mode_specific0"},
			{.name = "op_mode_specific2", .value=(1 <<  6), .description="op_mode_specific2"},
			{.name = "quick_stop", .value=(1 <<  2), .description="quick_stop"},
			{.name = "enable_voltage", .value=(1 <<  1), .description="enable_voltage"},
			{.name = "mfr_specific3", .value=(1 << 14), .description="mfr_specific3"},
			{.name = "mfr_specific1", .value=(1 << 12), .description="mfr_specific1"},
			{.name = "mfr_specific0", .value=(1 << 11), .description="mfr_specific0"},
			{.name = "mfr_specific2", .value=(1 << 13), .description="mfr_specific2"},
			{.name = "enable_operation", .value=(1 <<  3), .description="enable_operation"},
			{.name = "switch_on", .value=(1 <<  0), .description="switch_on"},
			{.name = "reset_fault", .value=(1 <<  7), .description="reset_fault"},
			{.name = "reserved1", .value=(1 << 10), .description="reserved1"},
			{.name = "reserved0", .value=(1 <<  9), .description="reserved0"},
			{.name = "mfr_specific4", .value=(1 << 15), .description="mfr_specific4"},
			{.name = "halt", .value=(1 <<  8), .description="halt"},
			{.name = NULL, .value=0, .description=NULL}
		},
		.parameter_name="Controlword"
	},
	{
		.index=0x6041,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.mask_descriptor = (struct canmat_code_descriptor[]){
			{.name = "internal_limit_active", .value=(1 << 11), .description="internal_limit_active"},
			{.name = "ready_to_switch_on", .value=(1 <<  0), .description="ready_to_switch_on"},
			{.name = "op_mode_specific0", .value=(1 << 12), .description="op_mode_specific0"},
			{.name = "op_enabled", .value=(1 <<  2), .description="op_enabled"},
			{.name = "quick_stop", .value=(1 <<  5), .description="quick_stop"},
			{.name = "switched_on", .value=(1 <<  1), .description="switched_on"},
			{.name = "mfr_specific1", .value=(1 << 14), .description="mfr_specific1"},
			{.name = "mfr_specific0", .value=(1 <<  8), .description="mfr_specific0"},
			{.name = "mfr_specific2", .value=(1 << 15), .description="mfr_specific2"},
			{.name = "warning", .value=(1 <<  7), .description="warning"},
			{.name = "target_reached", .value=(1 << 10), .description="target_reached"},
			{.name = "fault", .value=(1 <<  3), .description="fault"},
			{.name = "switch_on_disabled", .value=(1 <<  6), .description="switch_on_disabled"},
			{.name = "remote", .value=(1 <<  9), .description="remote"},
			{.name = "op_mode_specific1", .value=(1 << 13), .description="op_mode_specific1"},
			{.name = "voltage_enabled", .value=(1 <<  4), .description="voltage_enabled"},
			{.name = NULL, .value=0, .description=NULL}
		},
		.parameter_name="Statusword"
	},
	{
		.index=0x6042,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="vl target velocity"
	},
	{
		.index=0x6043,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="vl velocity demand"
	},
	{
		.index=0x6044,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="vl control effort"
	},
	{
		.index=0x604e,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="vl velocity reference"
	},
	{
		.index=0x6053,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="vl percentage demand"
	},
	{
		.index=0x6054,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="vl actual percentage"
	},
	{
		.index=0x6055,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="vl manipulated percentage"
	},
	{
		.index=0x605d,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_INTEGER16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.value_descriptor = (struct canmat_code_descriptor[]){
			{.name = "voltage_limit", .value=(4), .description="voltage_limit"},
			{.name = "quick_stop", .value=(2), .description="quick_stop"},
			{.name = "disable", .value=(0), .description="disable"},
			{.name = "current_limit", .value=(3), .description="current_limit"},
			{.name = "slow_down_ramp", .value=(1), .description="slow_down_ramp"},
			{.name = NULL, .value=0, .description=NULL}
		},
		.parameter_name="Halt option code"
	},
	{
		.index=0x605e,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.value_descriptor = (struct canmat_code_descriptor[]){
			{.name = "voltage_limit", .value=(4), .description="voltage_limit"},
			{.name = "current_limit", .value=(3), .description="current_limit"},
			{.name = "disable", .value=(0), .description="disable"},
			{.name = "quick_stop_ramp", .value=(2), .description="quick_stop_ramp"},
			{.name = "slow_down_ramp", .value=(1), .description="slow_down_ramp"},
			{.name = NULL, .value=0, .description=NULL}
		},
		.parameter_name="Fault reaction option code"
	},
	{
		.index=0x6060,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.value_descriptor = (struct canmat_code_descriptor[]){
			{.name = "pvt", .value=(0x7), .description="Interpolated Position"},
			{.name = "torque", .value=(0x4), .description="torque"},
			{.name = "cyclic_sync_velocity", .value=(0x9), .description="cyclic_sync_velocity"},
			{.name = "custom", .value=(0xFF), .description="custom"},
			{.name = "cyclic_sync_torque", .value=(0x9), .description="cyclic_sync_torque"},
			{.name = "profile_position", .value=(0x1), .description="profile_position"},
			{.name = "homing", .value=(0x6), .description="homing"},
			{.name = "velocity", .value=(0x2), .description="velocity"},
			{.name = "reserved1", .value=(0x5), .description="reserved1"},
			{.name = "cyclic_sync_position", .value=(0x8), .description="cyclic_sync_position"},
			{.name = "profile_velocity", .value=(0x3), .description="profile_velocity"},
			{.name = NULL, .value=0, .description=NULL}
		},
		.parameter_name="Modes of operation"
	},
	{
		.index=0x6061,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Modes of operation display"
	},
	{
		.index=0x6064,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Position actual value"
	},
	{
		.index=0x606b,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Velocity demand value"
	},
	{
		.index=0x606c,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Velocity actual value"
	},
	{
		.index=0x606f,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Velocity threshold"
	},
	{
		.index=0x6070,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Velocity threshold time"
	},
	{
		.index=0x607a,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Target position"
	},
	{
		.index=0x607b,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_INTEGER32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.parameter_name="Position range limit"
	},
	{
		.index=0x607b,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.parameter_name="Position range limit/number of entries"
	},
	{
		.index=0x607b,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_INTEGER32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.parameter_name="Position range limit/Min position range limit"
	},
	{
		.index=0x607b,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_INTEGER32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.parameter_name="Position range limit/Max position range limit"
	},
	{
		.index=0x607c,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_INTEGER32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Home offset"
	},
	{
		.index=0x607d,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_INTEGER32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.parameter_name="Software position limit"
	},
	{
		.index=0x607d,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.parameter_name="Software position limit/number of entries"
	},
	{
		.index=0x607d,
		.subindex=0x01,
		.data_type=CANMAT_DATA_TYPE_INTEGER32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Software position limit/Min position range limit"
	},
	{
		.index=0x607d,
		.subindex=0x02,
		.data_type=CANMAT_DATA_TYPE_INTEGER32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Software position limit/Max position range limit"
	},
	{
		.index=0x607e,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Polarity"
	},
	{
		.index=0x6081,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Profile velocity"
	},
	{
		.index=0x6082,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="End velocity"
	},
	{
		.index=0x6083,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Profile acceleration"
	},
	{
		.index=0x6084,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Profile deceleration"
	},
	{
		.index=0x6085,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Quick stop deceleration"
	},
	{
		.index=0x6086,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.value_descriptor = (struct canmat_code_descriptor[]){
			{.name = "linear_ramp", .value=(0), .description="Trapezoidal profile"},
			{.name = "sin2_ramp", .value=(1), .description="sin2_ramp"},
			{.name = NULL, .value=0, .description=NULL}
		},
		.parameter_name="Motion profile type"
	},
	{
		.index=0x6089,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Position notation index"
	},
	{
		.index=0x608a,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Position dimension index"
	},
	{
		.index=0x608b,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Velocity notation index"
	},
	{
		.index=0x608c,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Velocity dimension index"
	},
	{
		.index=0x608d,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Acceleration notation index"
	},
	{
		.index=0x608e,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Acceleration dimension index"
	},
	{
		.index=0x608f,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Position encoder resolution/number of entries"
	},
	{
		.index=0x608f,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Position encoder resolution"
	},
	{
		.index=0x608f,
		.subindex=0x01,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Position encoder resolution/Encoder increments"
	},
	{
		.index=0x608f,
		.subindex=0x02,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Position encoder resolution/Motor revolutions"
	},
	{
		.index=0x6090,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Velocity encoder resolution"
	},
	{
		.index=0x6090,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Velocity encoder resolution/number of entries"
	},
	{
		.index=0x6090,
		.subindex=0x01,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Velocity encoder resolution/Encoder increments per second"
	},
	{
		.index=0x6090,
		.subindex=0x02,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Velocity encoder resolution/Motor revolutions per second"
	},
	{
		.index=0x6091,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Gear ratio"
	},
	{
		.index=0x6091,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Gear Ratio/number of entries"
	},
	{
		.index=0x6091,
		.subindex=0x01,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Gear Ratio/Motor revolutions"
	},
	{
		.index=0x6091,
		.subindex=0x02,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Gear Ratio/Shaft revolutions"
	},
	{
		.index=0x6092,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Feed constant"
	},
	{
		.index=0x6092,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Feed constant/number of entries"
	},
	{
		.index=0x6092,
		.subindex=0x01,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Feed constant/Feed"
	},
	{
		.index=0x6092,
		.subindex=0x02,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Feed constant/Shaft revolutions"
	},
	{
		.index=0x6093,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Position factor"
	},
	{
		.index=0x6093,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Position factor/number of entries"
	},
	{
		.index=0x6093,
		.subindex=0x01,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Position factor/Numerator"
	},
	{
		.index=0x6093,
		.subindex=0x02,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Position factor/Feed constant"
	},
	{
		.index=0x6094,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Velocity encoder factor"
	},
	{
		.index=0x6094,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Velocity encoder factor/number of entries"
	},
	{
		.index=0x6094,
		.subindex=0x01,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Velocity encoder factor/Numerator"
	},
	{
		.index=0x6094,
		.subindex=0x02,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Velocity encoder factor/Divisor"
	},
	{
		.index=0x6095,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Velocity factor 1"
	},
	{
		.index=0x6095,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Velocity factor 1/number of entries"
	},
	{
		.index=0x6095,
		.subindex=0x01,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Velocity factor 1/Numerator"
	},
	{
		.index=0x6095,
		.subindex=0x02,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Velocity factor 1/Divisor"
	},
	{
		.index=0x6096,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Velocity factor 2"
	},
	{
		.index=0x6096,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Velocity factor 2/number of entries"
	},
	{
		.index=0x6096,
		.subindex=0x01,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Velocity factor 2/Numerator"
	},
	{
		.index=0x6096,
		.subindex=0x02,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Velocity factor 2/Divisor"
	},
	{
		.index=0x6097,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Acceleration factor"
	},
	{
		.index=0x6097,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Acceleration factor/number of entries"
	},
	{
		.index=0x6097,
		.subindex=0x01,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Acceleration factor/Numerator"
	},
	{
		.index=0x6097,
		.subindex=0x02,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Acceleration factor/Divisor"
	},
	{
		.index=0x60c5,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Max deceleration"
	},
	{
		.index=0x60fd,
		.subindex=0x00,
		.pdo_mapping=1,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.mask_descriptor = (struct canmat_code_descriptor[]){
			{.name = "home_sw", .value=(1 << 2), .description="home_sw"},
			{.name = "neg_limit_sw", .value=(1 << 0), .description="neg_limit_sw"},
			{.name = "interlock", .value=(1 << 3), .description="interlock"},
			{.name = "pos_limit_sw", .value=(1 << 1), .description="pos_limit_sw"},
			{.name = NULL, .value=0, .description=NULL}
		},
		.parameter_name="Digital inputs"
	},
	{
		.index=0x60fe,
		.subindex=0x00,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_ARRAY,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Digital outputs"
	},
	{
		.index=0x60fe,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Digital outputs/number of entries"
	},
	{
		.index=0x60fe,
		.subindex=0x02,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Digital outputs/bit mask"
	},
	{
		.index=0x60ff,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_INTEGER32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Target velocity"
	},
	{
		.index=0x6402,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED16,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.value_descriptor = (struct canmat_code_descriptor[]){
			{.name = "trapezoidal_pm_bl", .value=(0x000B), .description="trapezoidal_pm_bl"},
			{.name = "phase_modulated_dc", .value=(0x0001), .description="phase_modulated_dc"},
			{.name = "squirrel_cage", .value=(0x0007), .description="squirrel_cage"},
			{.name = "micro_step_stepper", .value=(0x0009), .description="micro_step_stepper"},
			{.name = "stepper", .value=(0x0008), .description="stepper"},
			{.name = "wound_rotor", .value=(0x0006), .description="wound_rotor"},
			{.name = "non_standard", .value=(0x0000), .description="non_standard"},
			{.name = "pm_synchronous", .value=(0x0003), .description="pm_synchronous"},
			{.name = "sinusoidal_pm_bl", .value=(0x000A), .description="sinusoidal_pm_bl"},
			{.name = "switched_reluctance", .value=(0x0005), .description="switched_reluctance"},
			{.name = "fc_synchronous", .value=(0x0004), .description="fc_synchronous"},
			{.name = "freq_controlled_dc", .value=(0x0002), .description="freq_controlled_dc"},
			{.name = NULL, .value=0, .description=NULL}
		},
		.parameter_name="Motor Type"
	},
	{
		.index=0x6403,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_VISIBLE_STRING,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Motor catalog number"
	},
	{
		.index=0x6404,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_VISIBLE_STRING,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Motor manufacturer"
	},
	{
		.index=0x6405,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_VISIBLE_STRING,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="http motor catalog address"
	},
	{
		.index=0x6406,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_TIME_OF_DAY,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Motor calibrate date"
	},
	{
		.index=0x6410,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Motor data/number of entries"
	},
	{
		.index=0x6410,
		.subindex=0x00,
		.object_type=CANMAT_OBJECT_TYPE_RECORD,
		.parameter_name="Motor data"
	},
	{
		.index=0x6502,
		.subindex=0x00,
		.pdo_mapping=1,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED32,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.mask_descriptor = (struct canmat_code_descriptor[]){
			{.name = "pp", .value=(1 << 0), .description="Profile Position"},
			{.name = "pv", .value=(1 << 2), .description="Profile Velocity"},
			{.name = "ip", .value=(1 << 6), .description="Interpolated position"},
			{.name = "vl", .value=(1 << 1), .description="Velocity"},
			{.name = "tq", .value=(1 << 3), .description="Torque"},
			{.name = "hm", .value=(1 << 5), .description="Homing"},
			{.name = NULL, .value=0, .description=NULL}
		},
		.parameter_name="Supported drive modes"
	},
	{
		.index=0x6503,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_VISIBLE_STRING,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Drive catalog number"
	},
	{
		.index=0x6504,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_VISIBLE_STRING,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="Drive manufacturer"
	},
	{
		.index=0x6505,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_VISIBLE_STRING,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RW,
		.parameter_name="http drive catalog address"
	},
	{
		.index=0x6510,
		.subindex=0x00,
		.pdo_mapping=0,
		.data_type=CANMAT_DATA_TYPE_UNSIGNED8,
		.object_type=CANMAT_OBJECT_TYPE_VAR,
		.access_type=CANMAT_ACCESS_RO,
		.parameter_name="Drive data/number of entries"
	},
	{
		.index=0x6510,
		.subindex=0x00,
		.object_type=CANMAT_OBJECT_TYPE_RECORD,
		.parameter_name="Drive data"
	},
	}
};
