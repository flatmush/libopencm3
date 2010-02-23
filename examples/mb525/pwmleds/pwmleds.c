/*
 * This file is part of the libopenstm32 project.
 *
 * Copyright (C) 2010 Piotr Esden-Tempski <piotr@esden.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopenstm32/common.h>
#include <libopenstm32/rcc.h>
#include <libopenstm32/gpio.h>
#include <libopenstm32/timer.h>

#define COMPARE
//#define MOVING_FADE
// #define KITT

#ifdef COMPARE
#define GAMMA_LINEAR
#define GAMMA_1_3
#define GAMMA_2_5
#define GAMMA_3_0
#endif

#ifdef MOVING_FADE
#define GAMMA_2_5
#define GAMMA_TABLE gamma_table_2_5
#endif

#ifdef KITT
#define GAMMA_2_5
#define GAMMA_TABLE gamma_table_2_5
#endif

/*
 * Gamma correction table
 *
 * The nonlinear tables are calculating with the function:
 * Iout = Iin ** gamma
 */
#ifdef GAMMA_LINEAR
static const u16 gamma_table_linear[] = {
	1,	4,	9,	17,	26,	37,	51,	67,
	84,	104,	126,	149,	175,	203,	233,	265,
	299,	334,	372,	412,	454,	499,	545,	593,
	643,	695,	749,	805,	864,	924,	986,	1050,
	1116,	1185,	1255,	1327,	1402,	1478,	1556,	1637,
	1719,	1803,	1890,	1978,	2068,	2161,	2255,	2352,
	2450,	2550,	2653,	2757,	2864,	2972,	3082,	3195,
	3309,	3426,	3544,	3664,	3787,	3911,	4038,	4166,
	4296,	4429,	4563,	4700,	4838,	4978,	5121,	5265,
	5411,	5560,	5710,	5862,	6017,	6173,	6331,	6492,
	6654,	6818,	6985,	7153,	7323,	7495,	7670,	7846,
	8024,	8204,	8387,	8571,	8757,	8945,	9135,	9327,
	9521,	9718,	9916,	10116,	10318,	10522,	10728,	10936,
	11146,	11358,	11572,	11788,	12006,	12226,	12448,	12672,
	12897,	13125,	13355,	13587,	13821,	14057,	14294,	14534,
	14776,	15020,	15265,	15513,	15763,	16014,	16268,	16524,
	16781,	17041,	17302,	17566,	17831,	18099,	18368,	18640,
	18913,	19189,	19466,	19745,	20027,	20310,	20595,	20883,
	21172,	21463,	21756,	22051,	22349,	22648,	22949,	23252,
	23557,	23864,	24173,	24484,	24797,	25112,	25429,	25748,
	26069,	26391,	26716,	27043,	27372,	27702,	28035,	28370,
	28707,	29045,	29386,	29728,	30073,	30419,	30768,	31118,
	31471,	31825,	32182,	32540,	32900,	33263,	33627,	33993,
	34361,	34731,	35104,	35478,	35854,	36232,	36612,	36994,
	37378,	37764,	38152,	38542,	38933,	39327,	39723,	40121,
	40521,	40922,	41326,	41732,	42139,	42549,	42960,	43374,
	43789,	44207,	44626,	45048,	45471,	45896,	46324,	46753,
	47184,	47617,	48052,	48490,	48929,	49370,	49813,	50258,
	50705,	51154,	51604,	52057,	52512,	52969,	53428,	53888,
	54351,	54816,	55282,	55751,	56222,	56694,	57169,	57645,
	58123,	58604,	59086,	59570,	60057,	60545,	61035,	61527,
	62021,	62517,	63016,	63516,	64018,	64521,	65027,	65535,
};
#endif

#ifdef GAMMA_1_3
static const u16 gamma_table_1_3[] = {
	/* Gamma 1.3 */
	0,	49,	120,	203,	296,	395,	501,	612,
	728,	848,	973,	1101,	1233,	1368,	1506,	1648,
	1792,	1939,	2088,	2241,	2395,	2552,	2711,	2872,
	3036,	3201,	3369,	3538,	3709,	3882,	4057,	4234,
	4412,	4592,	4774,	4957,	5142,	5329,	5517,	5706,
	5897,	6090,	6283,	6479,	6675,	6873,	7072,	7273,
	7475,	7678,	7882,	8088,	8294,	8502,	8711,	8922,
	9133,	9346,	9559,	9774,	9990,	10207,	10425,	10644,
	10864,	11086,	11308,	11531,	11755,	11981,	12207,	12434,
	12662,	12891,	13121,	13352,	13584,	13817,	14051,	14285,
	14521,	14757,	14994,	15233,	15472,	15712,	15952,	16194,
	16436,	16679,	16923,	17168,	17414,	17660,	17908,	18156,
	18405,	18654,	18905,	19156,	19408,	19660,	19914,	20168,
	20423,	20679,	20935,	21192,	21450,	21708,	21968,	22228,
	22488,	22750,	23012,	23275,	23538,	23802,	24067,	24332,
	24599,	24865,	25133,	25401,	25670,	25939,	26209,	26480,
	26751,	27023,	27296,	27569,	27843,	28118,	28393,	28669,
	28945,	29222,	29500,	29778,	30057,	30336,	30616,	30897,
	31178,	31460,	31742,	32025,	32308,	32592,	32877,	33162,
	33448,	33734,	34021,	34309,	34597,	34885,	35175,	35464,
	35754,	36045,	36337,	36628,	36921,	37214,	37507,	37801,
	38096,	38391,	38686,	38982,	39279,	39576,	39874,	40172,
	40471,	40770,	41070,	41370,	41670,	41972,	42273,	42576,
	42878,	43181,	43485,	43789,	44094,	44399,	44705,	45011,
	45317,	45625,	45932,	46240,	46549,	46858,	47167,	47477,
	47787,	48098,	48409,	48721,	49034,	49346,	49659,	49973,
	50287,	50602,	50917,	51232,	51548,	51864,	52181,	52498,
	52816,	53134,	53452,	53771,	54091,	54411,	54731,	55052,
	55373,	55694,	56016,	56339,	56662,	56985,	57309,	57633,
	57957,	58282,	58608,	58933,	59260,	59586,	59913,	60241,
	60569,	60897,	61226,	61555,	61884,	62214,	62545,	62875,
	63206,	63538,	63870,	64202,	64535,	64868,	65201,	65535,
};
#endif

#ifdef GAMMA_2_5
static const u16 gamma_table_2_5[] = {
	/* gamma = 2.5 */
	0,	0,	0,	1,	2,	4,	6,	8,
	11,	15,	20,	25,	31,	38,	46,	55,
	65,	75,	87,	99,	113,	128,	143,	160,
	178,	197,	218,	239,	262,	286,	311,	338,
	366,	395,	425,	457,	491,	526,	562,	599,
	639,	679,	722,	765,	811,	857,	906,	956,
	1007,	1061,	1116,	1172,	1231,	1291,	1352,	1416,
	1481,	1548,	1617,	1688,	1760,	1834,	1910,	1988,
	2068,	2150,	2233,	2319,	2407,	2496,	2587,	2681,
	2776,	2874,	2973,	3075,	3178,	3284,	3391,	3501,
	3613,	3727,	3843,	3961,	4082,	4204,	4329,	4456,
	4585,	4716,	4850,	4986,	5124,	5264,	5407,	5552,
	5699,	5849,	6001,	6155,	6311,	6470,	6632,	6795,
	6962,	7130,	7301,	7475,	7650,	7829,	8009,	8193,
	8379,	8567,	8758,	8951,	9147,	9345,	9546,	9750,
	9956,	10165,	10376,	10590,	10806,	11025,	11247,	11472,
	11699,	11929,	12161,	12397,	12634,	12875,	13119,	13365,
	13614,	13865,	14120,	14377,	14637,	14899,	15165,	15433,
	15705,	15979,	16256,	16535,	16818,	17104,	17392,	17683,
	17978,	18275,	18575,	18878,	19184,	19493,	19805,	20119,
	20437,	20758,	21082,	21409,	21739,	22072,	22407,	22746,
	23089,	23434,	23782,	24133,	24487,	24845,	25206,	25569,
	25936,	26306,	26679,	27055,	27435,	27818,	28203,	28592,
	28985,	29380,	29779,	30181,	30586,	30994,	31406,	31820,
	32239,	32660,	33085,	33513,	33944,	34379,	34817,	35258,
	35702,	36150,	36602,	37056,	37514,	37976,	38441,	38909,
	39380,	39856,	40334,	40816,	41301,	41790,	42282,	42778,
	43277,	43780,	44286,	44795,	45308,	45825,	46345,	46869,
	47396,	47927,	48461,	48999,	49540,	50085,	50634,	51186,
	51742,	52301,	52864,	53431,	54001,	54575,	55153,	55734,
	56318,	56907,	57499,	58095,	58695,	59298,	59905,	60515,
	61130,	61748,	62370,	62995,	63624,	64258,	64894,	65535,
};
#endif

#ifdef GAMMA_3_0
static const u16 gamma_table_3_0[] = {
	/* gamma = 3.0 */
	0,	0,	0,	0,	0,	0,	1,	1,
	2,	3,	4,	5,	7,	9,	11,	13,
	16,	19,	23,	27,	32,	37,	42,	48,
	55,	62,	69,	78,	87,	96,	107,	118,
	130,	142,	155,	169,	184,	200,	217,	234,
	253,	272,	293,	314,	337,	360,	385,	410,
	437,	465,	494,	524,	556,	588,	622,	658,
	694,	732,	771,	812,	854,	897,	942,	988,
	1036,	1085,	1136,	1189,	1243,	1298,	1356,	1415,
	1475,	1538,	1602,	1667,	1735,	1804,	1876,	1949,
	2024,	2100,	2179,	2260,	2343,	2427,	2514,	2603,
	2693,	2786,	2881,	2978,	3078,	3179,	3283,	3389,
	3497,	3607,	3720,	3835,	3952,	4072,	4194,	4319,
	4446,	4575,	4707,	4842,	4979,	5118,	5261,	5405,
	5553,	5703,	5856,	6011,	6169,	6330,	6494,	6660,
	6830,	7002,	7177,	7355,	7536,	7719,	7906,	8096,
	8289,	8484,	8683,	8885,	9090,	9298,	9510,	9724,
	9942,	10163,	10387,	10614,	10845,	11079,	11317,	11557,
	11802,	12049,	12300,	12555,	12813,	13074,	13339,	13608,
	13880,	14156,	14435,	14718,	15005,	15295,	15589,	15887,
	16189,	16494,	16803,	17117,	17433,	17754,	18079,	18408,
	18740,	19077,	19418,	19762,	20111,	20464,	20821,	21182,
	21547,	21917,	22290,	22668,	23050,	23436,	23827,	24222,
	24621,	25025,	25433,	25845,	26262,	26683,	27109,	27539,
	27974,	28413,	28857,	29306,	29759,	30217,	30680,	31147,
	31619,	32095,	32577,	33063,	33554,	34050,	34551,	35056,
	35567,	36082,	36602,	37128,	37658,	38194,	38734,	39280,
	39830,	40386,	40947,	41513,	42084,	42661,	43243,	43830,
	44422,	45019,	45622,	46231,	46844,	47463,	48088,	48718,
	49353,	49994,	50641,	51293,	51950,	52614,	53282,	53957,
	54637,	55323,	56014,	56712,	57415,	58123,	58838,	59558,
	60285,	61017,	61755,	62499,	63249,	64005,	64767,	65535,
};
#endif

/* Set STM32 to 72 MHz. */
void clock_setup(void)
{
	/* Select HSI as SYSCLK source. */
	rcc_set_sysclk_source(SW_SYSCLKSEL_HSICLK);

	/* Set the PLL multiplication factor to 9. */
	rcc_set_pll_multiplication_factor(PLLMUL_PLL_CLK_MUL9);

	/* Select HSI/2 as PLL source. */
	rcc_set_pll_source(PLLSRC_HSI_CLK_DIV2);

	rcc_set_pllxtpre(PLLXTPRE_HSE_CLK_DIV2);

	/* Enable PLL oscillator and wait for it to stabilize. */
	rcc_osc_on(PLL);
	rcc_wait_for_osc_ready(PLL);

	/* Select PLL as SYSCLK source. */
	rcc_set_sysclk_source(SW_SYSCLKSEL_PLLCLK);

	/* Enable TIM3 clock. */
	rcc_peripheral_enable_clock(&RCC_APB1ENR, TIM3EN);

	/* Enable GPIOC, Alternate Function clocks. */
	rcc_peripheral_enable_clock(&RCC_APB2ENR, IOPCEN | AFIOEN);
}

void gpio_setup(void)
{
	/*
	 * Set GPIO6 (in GPIO port C) to
	 * 'output alternate function push-pull'.
	 */
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
		      GPIO6 | GPIO7 | GPIO8 | GPIO9);

	/* Remap TIM3:
	 * CH1 -> PC6
	 * CH2 -> PC7
	 * CH3 -> PC8
	 * CH4 -> PC9
	 */
	AFIO_MAPR |= AFIO_MAPR_TIM3_REMAP_FULL_REMAP;
}
void tim_setup(void)
{
	/* Clock division and mode */
	TIM3_CR1 = TIM_CR1_CKD_CK_INT | TIM_CR1_CMS_EDGE;
	/* Period */
	TIM3_ARR = 65535;
	/* Prescaler */
	TIM3_PSC = 0;
	TIM3_EGR = TIM_EGR_UG;

	/* ---- */
	/* Output compare 1 mode and preload */
	TIM3_CCMR1 |= TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE;

	/* Polarity and state */
	// TIM3_CCER = TIM_CCER_CC1P | TIM_CCER_CC1E;
	TIM3_CCER |= TIM_CCER_CC1E;

	/* Capture compare value */
	TIM3_CCR1 = 0;

	/* ---- */
	/* Output compare 2 mode and preload */
	TIM3_CCMR1 |= TIM_CCMR1_OC2M_PWM1 | TIM_CCMR1_OC2PE;

	/* Polarity and state */
	// TIM3_CCER = TIM_CCER_CC1P | TIM_CCER_CC1E;
	TIM3_CCER |= TIM_CCER_CC2E;

	/* Capture compare value */
	TIM3_CCR2 = 0;

	/* ---- */
	/* Output compare 3 mode and preload */
	TIM3_CCMR2 |= TIM_CCMR2_OC3M_PWM1 | TIM_CCMR2_OC3PE;

	/* Polarity and state */
	// TIM3_CCER = TIM_CCER_CC1P | TIM_CCER_CC1E;
	TIM3_CCER |= TIM_CCER_CC3E;

	/* Capture compare value */
	TIM3_CCR3 = 0;

	/* ---- */
	/* Output compare 4 mode and preload */
	TIM3_CCMR2 |= TIM_CCMR2_OC4M_PWM1 | TIM_CCMR2_OC4PE;

	/* Polarity and state */
	// TIM3_CCER = TIM_CCER_CC1P | TIM_CCER_CC1E;
	TIM3_CCER |= TIM_CCER_CC4E;

	/* Capture compare value */
	TIM3_CCR4 = 0;

	/* ---- */
	/* ARR reload enable */
	TIM3_CR1 |= TIM_CR1_ARPE;

	/* Counter enable */
	TIM3_CR1 |= TIM_CR1_CEN;
}

int main(void)
{
	int i, j0, j1, j2, j3, d0, d1, d2, d3, j, k, kd;

	clock_setup();
	gpio_setup();
	tim_setup();

#ifdef COMPARE
	j0 = 0;
	d0 = 1;
	j1 = 0;
	d1 = 1;
	j2 = 0;
	d2 = 1;
	j3 = 0;
	d3 = 1;
	while (1) {
		TIM3_CCR1 = gamma_table_linear[j0];
		j0 += d0;
		if (j0 == 255)
			d0 =- 1;
		if (j0 == 0)
			d0 = 1;
		TIM3_CCR2 = gamma_table_1_3[j1];
		j1 += d1;
		if (j1 == 255)
			d1 =- 1;
		if (j1 == 0)
			d1 = 1;
		TIM3_CCR3 = gamma_table_2_5[j2];
		j2 += d2;
		if (j2 == 255)
			d2 =- 1;
		if (j2 == 0)
			d2 = 1;
		TIM3_CCR4= gamma_table_3_0[j3];
		j3 += d3;
		if (j3 == 255)
			d3 =- 1;
		if (j3 == 0)
			d3 = 1;
		for (i = 0; i < 50000; i++);
	}
#endif

#ifdef MOVING_FADE
	j0 = 0;
	d0 = 1;
	j1 = 128;
	d1 = 1;
	j2 = 255;
	d2 = -1;
	j3 = 128;
	d3 = -1;
	while (1) {
		TIM3_CCR1 = GAMMA_TABLE[j0];
		j0 += d0;
		if (j0 == 255)
			d0 =- 1;
		if (j0 == 0)
			d0 = 1;
		TIM3_CCR2 = GAMMA_TABLE[j1];
		j1 += d1;
		if (j1 == 255)
			d1 =- 1;
		if (j1 == 0)
			d1 = 1;
		TIM3_CCR3 = GAMMA_TABLE[j2];
		j2 += d2;
		if (j2 == 255)
			d2 =- 1;
		if (j2 == 0)
			d2 = 1;
		TIM3_CCR4 = GAMMA_TABLE[j3];
		j3 += d3;
		if (j3 == 255)
			d3 =- 1;
		if (j3 == 0)
			d3 = 1;
		for (i = 0; i < 10000; i++);
	}
#endif

#ifdef KITT
	j0 = 255;
	d0 = -1;
	j1 = 20;
	d1 = -1;
	j2 = 20;
	d2 = -1;
	j3 = 20;
	d3 = -1;
	j = 0;
	k = 0;
	kd = 1;
	while (1) {
		TIM3_CCR1 = GAMMA_TABLE[j0];
		j0 += d0;
		if (j0 == 255)
			d0 =- 1;
		if (j0 == 19)
			j0 = 20;
		TIM3_CCR2 = GAMMA_TABLE[j1];
		j1 += d1;
		if (j1 == 255)
			d1 =- 1;
		if (j1 == 19)
			j1 = 20;
		TIM3_CCR3 = GAMMA_TABLE[j2];
		j2 += d2;
		if (j2 == 255)
			d2 =- 1;
		if (j2 == 19)
			j2 = 20;
		TIM3_CCR4 = GAMMA_TABLE[j3];
		j3 += d3;
		if (j3 == 255)
			d3 =- 1;
		if (j3 == 19)
			j3 = 20;
		for (i = 0; i < 5000; i++);
		j++;
		if (j == 100) {
			j = 0;
			switch (k += kd) {
			case 0:
				j0 = 255;
				break;
			case 1:
				j1 = 255;
				break;
			case 2:
				j2 = 255;
				break;
			case 3:
				j3 = 255;
				break;
			}
			if (k == 3)
				kd =- 1;
			if (k == 0)
				kd = 1;
		}
	}
#endif

	return 0;
}
