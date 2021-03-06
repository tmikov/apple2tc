/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/a2symbols.h"

#include <cstdlib>

// Adapted from: https://github.com/whscullin/apple2js/blob/master/js/symbols.ts
// With the following license:
/*
The MIT License (MIT)

Copyright (c) 2013-2019 Will Scullin

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

struct SymDef {
  unsigned addr;
  const char *name;
};

static const SymDef s_symbols[] = {
  {0x0000, "LOC0"},
  {0x0001, "LOC1"},
  {0x0002, "LOC2"},
  {0x0003, "LOC3"},
  {0x0004, "LOC4"},
  {0x000a, "BAS_USRVEC"},
  {0x000b, "BAS_USRPTR"},
  {0x000d, "CHARAC"},
  {0x000e, "ENDCHR"},
  {0x000f, "TKNCNTR"},
  {0x0010, "DIMFLG"},
  {0x0011, "VALTYP"},
  {0x0012, "INTFLG"},
  {0x0013, "DATAFLG"},
  {0x0014, "SUBFLG"},
  {0x0015, "INPUTFLG"},
  {0x0016, "SIGNFLG"},
  {0x001a, "HSHAPE"},
  {0x001c, "HCOLOR1"},
  {0x001d, "HCOUNT"},
  {0x0020, "WNDLFT"},
  {0x0021, "WNDWDTH"},
  {0x0022, "WNDTOP"},
  {0x0023, "WNDBTM"},
  {0x0024, "BIT_ZP"},
  {0x0025, "CV"},
  {0x0026, "GBASL"},
  {0x0027, "GBASH"},
  {0x0028, "BASL"},
  {0x0029, "BASH"},
  {0x002a, "BAS2L"},
  {0x002b, "BAS2H"},
  {0x002c, "LMNEM"},
  {0x002d, "V2"},
  {0x002e, "FORMAT"},
  {0x002f, "LENGTH"},
  {0x0030, "HMASK"},
  {0x0031, "MODE"},
  {0x0032, "INVFLG"},
  {0x0033, "PROMPT"},
  {0x0034, "YSAV"},
  {0x0035, "YSAV1"},
  {0x0036, "CSWL"},
  {0x0038, "KSWL"},
  {0x003a, "PCL"},
  {0x003b, "PCH"},
  {0x003c, "IWMBITS"},
  {0x003d, "A1H"},
  {0x003e, "A2L"},
  {0x003f, "A2H"},
  {0x0040, "IWMTRKFND"},
  {0x0041, "IWMTRACK"},
  {0x0042, "A4L"},
  {0x0043, "A4H"},
  {0x0044, "A5L"},
  {0x0045, "ERR_OVERFLOW"},
  {0x0046, "XREG"},
  {0x0047, "YREG"},
  {0x0048, "STATUS"},
  {0x0049, "SPNT"},
  {0x004a, "SAVE6502"},
  {0x004d, "ERR_MEMFULL"},
  {0x004e, "RNDL"},
  {0x004f, "RNDH"},
  {0x0050, "LINNUM"},
  {0x0052, "TEMPPT"},
  {0x0053, "LASTPT"},
  {0x0055, "TEMPST"},
  {0x005a, "ERR_UNDEFSTAT"},
  {0x005e, "SRCPTR"},
  {0x0060, "DSTPTR"},
  {0x0062, "RESULT"},
  {0x0064, "P_REL"},
  {0x0067, "TXTTAB"},
  {0x0069, "VARTAB"},
  {0x006b, "ARYTAB"},
  {0x006d, "STREND"},
  {0x006f, "FRETOP"},
  {0x0071, "FRESPC"},
  {0x0073, "MEMSIZ"},
  {0x0075, "CURLIN"},
  {0x0077, "OLDLIN"},
  {0x0078, "ERR_REDIMD"},
  {0x0079, "OLDTEXT"},
  {0x007b, "DATLIN"},
  {0x007d, "P_PWR"},
  {0x007f, "INPTR"},
  {0x0081, "TOK_FOR"},
  {0x0083, "VARPTR"},
  {0x0085, "ERR_ZERODIV"},
  {0x0087, "TXPSV"},
  {0x0088, "BSH"},
  {0x0089, "CPRTYP"},
  {0x008a, "TEMP3"},
  {0x008c, "DSCPTR"},
  {0x008d, "RTNH"},
  {0x008f, "DSCLEN"},
  {0x0090, "BCC"},
  {0x0091, "GARLEN"},
  {0x0092, "ARGEXT"},
  {0x0093, "TEMP1"},
  {0x0094, "ARYPTR"},
  {0x0095, "ERR_ILLDIR"},
  {0x0096, "HIGHTR"},
  {0x0098, "CTRLXH"},
  {0x0099, "TMPEXP"},
  {0x009a, "EXPON"},
  {0x009b, "LOWTR"},
  {0x009c, "EXPSGN"},
  {0x009d, "FAC"},
  {0x009e, "DSCTMPL"},
  {0x009f, "DSCTMPH"},
  {0x00a0, "TMPVPTR"},
  {0x00a2, "FACSIGN"},
  {0x00a3, "ERR_BADTYPE"},
  {0x00a4, "SHFTSGNX"},
  {0x00a5, "ARG"},
  {0x00a8, "ARGVPTR"},
  {0x00aa, "ARGSIGN"},
  {0x00ab, "SGNCPR"},
  {0x00ac, "FACEXT"},
  {0x00ad, "STRNG2"},
  {0x00af, "PRGEND"},
  {0x00b0, "TOK_GOSUB"},
  {0x00b1, "CHRGET"},
  {0x00b2, "TOK_REM"},
  {0x00b7, "CHRGOT"},
  {0x00b8, "TXTPTR"},
  {0x00ba, "TOK_PRINT"},
  {0x00bf, "ERR_FRMCPX"},
  {0x00c0, "HGRHIGH"},
  {0x00c1, "TOK_TO"},
  {0x00c2, "TOK_FN"},
  {0x00c3, "TOK_SPC"},
  {0x00c4, "TOK_THEN"},
  {0x00c5, "TOK_AT"},
  {0x00c6, "TOK_NOT"},
  {0x00c7, "TOK_STEP"},
  {0x00c8, "TOK_PLUS"},
  {0x00c9, "TOK_MINUS"},
  {0x00cf, "TOK_GREATER"},
  {0x00d0, "HGRDX"},
  {0x00d2, "HGRDY"},
  {0x00d3, "HGRQUAD"},
  {0x00d4, "HGRE"},
  {0x00d6, "AUTORUN"},
  {0x00d7, "TOK_SCRN"},
  {0x00d8, "ERRFLG"},
  {0x00da, "ERRLIN"},
  {0x00dc, "ERRPOS"},
  {0x00de, "ERRNUM"},
  {0x00df, "ERRSTK"},
  {0x00e0, "ERR_UNDEFFUNC"},
  {0x00e2, "HGRY"},
  {0x00e4, "HGRCOLOR"},
  {0x00e5, "HGRHORIZ"},
  {0x00e6, "HGRPAGE"},
  {0x00e7, "HGRSCALE"},
  {0x00e8, "HGRSHPTR"},
  {0x00ea, "HGRCLSN"},
  {0x00f0, "FIRST"},
  {0x00f1, "SPDBYT"},
  {0x00f2, "TRCFLG"},
  {0x00f3, "FLASHBIT"},
  {0x00f4, "TXTPSV"},
  {0x00f6, "CURLSV"},
  {0x00f8, "REMSTK"},
  {0x00f9, "HGRROT"},
  {0x00fd, "CIOPG"},
  {0x00fe, "ERR_BADRESP"},
  {0x00ff, "DELETE"},
  {0x0100, "STACK"},
  {0x0118, "HGRWIDE"},
  {0x01fb, "LINEIMAGE"},
  {0x01fc, "IMGNXLNPTR"},
  {0x01fe, "IMGLINNUM"},
  {0x0200, "INBUFF"},
  {0x0300, "TWOSBUFF"},
  {0x0356, "CNVTBL"},
  {0x03ef, "GOBRKV"},
  {0x03f0, "BRKV"},
  {0x03f2, "SOFTEV"},
  {0x03f4, "PWREDUP"},
  {0x03f5, "AMPERV"},
  {0x03f8, "USRADDR"},
  {0x03fb, "NMI"},
  {0x03fe, "IRQADDR"},
  {0x0400, "LINE1"},
  {0x07f8, "MSLOT"},
  {0x0800, "PRGMEM"},
  {0x2000, "HGR1SCRN"},
  {0x4000, "HGR2SCRN"},
  {0xb000, "MTSPC1"},
  {0xb600, "DISK2RAM"},
  {0xb700, "MTSPC2"},
  {0xc000, "KBD"},
  {0xc010, "KBDSTRB"},
  {0xc020, "TAPEOUT"},
  {0xc030, "SPKR"},
  {0xc040, "STROBE"},
  {0xc050, "TXTCLR"},
  {0xc051, "TXTSET"},
  {0xc052, "MIXCLR"},
  {0xc053, "MIXSET"},
  {0xc054, "TXTPAGE1"},
  {0xc055, "TXTPAGE2"},
  {0xc056, "LORES"},
  {0xc057, "HIRES"},
  {0xc058, "AN0OFF"},
  {0xc059, "AN0ON"},
  {0xc05a, "AN1OFF"},
  {0xc05b, "AN1ON"},
  {0xc05c, "AN2OFF"},
  {0xc05d, "AN2ON"},
  {0xc05e, "AN3OFF"},
  {0xc05f, "AN3ON"},
  {0xc060, "TAPEIN"},
  {0xc061, "PB0"},
  {0xc062, "PB1"},
  {0xc063, "PB2"},
  {0xc064, "PADDL0"},
  {0xc065, "PADDL1"},
  {0xc066, "PADDL2"},
  {0xc067, "PADDL3"},
  {0xc068, "TAPEIN_X"},
  {0xc069, "PB0_X"},
  {0xc06a, "PB1_X"},
  {0xc06b, "PB2_X"},
  {0xc06c, "PADDL0_X"},
  {0xc06d, "PADDL1_X"},
  {0xc06e, "PADDL2_X"},
  {0xc06f, "PADDL3_X"},
  {0xc070, "PTRIG"},
  {0xc080, "IWMPH0OFF"},
  {0xc081, "IWMPH0ON"},
  {0xc082, "MTSPC3"},
  {0xc089, "IWMMOTORON"},
  {0xc08a, "IWMSELDRV1"},
  {0xc08b, "MTSPC4"},
  {0xc08c, "IWMQ6OFF"},
  {0xc08d, "MTSPC5"},
  {0xc08e, "IWMQ7OFF"},
  {0xc08f, "MTSPC6"},
  {0xc100, "SLOT1"},
  {0xc200, "SLOT2"},
  {0xc300, "SLOT3"},
  {0xc400, "SLOT4"},
  {0xc500, "SLOT5"},
  {0xc600, "DISK2ROM"},
  {0xc606, "MKDCTBL"},
  {0xc614, "CHKDBL0S"},
  {0xc61e, "REJECT"},
  {0xc63d, "FINDT0S0"},
  {0xc65c, "RDSECT"},
  {0xc65d, "RDSECT0"},
  {0xc65e, "RDSECT1"},
  {0xc663, "RDSECT2"},
  {0xc667, "RDSECT3"},
  {0xc671, "RDSECT4"},
  {0xc683, "RDSAD"},
  {0xc685, "RDSAD1"},
  {0xc687, "RDSAD2"},
  {0xc68f, "RDSAD3"},
  {0xc6a6, "RDF62ESD"},
  {0xc6a8, "RDTWOS"},
  {0xc6aa, "RDTWOS1"},
  {0xc6ba, "RDSIXES"},
  {0xc6bc, "RDSIXES1"},
  {0xc6cb, "RDCHKSUM"},
  {0xc6d3, "RDF62ESDX"},
  {0xc6d7, "DCD62ENC"},
  {0xc6d9, "DCD62ENC1"},
  {0xc700, "SLOT7"},
  {0xc800, "ALLSLOTS"},
  {0xcdfe, "SLTVTAB"},
  {0xcf53, "SLTGOERR"},
  {0xcfdc, "JMPADRTBL"},
  {0xcfff, "CLRROM"},
  {0xd000, "TKADTBL"},
  {0xd002, "TA_FOR"},
  {0xd004, "TA_NEXT"},
  {0xd006, "TA_DATA"},
  {0xd008, "TA_INPUT"},
  {0xd00a, "TA_DEL"},
  {0xd00c, "TA_DIM"},
  {0xd00e, "TA_READ"},
  {0xd010, "TA_GR"},
  {0xd012, "TA_TEXT"},
  {0xd014, "TA_PR"},
  {0xd016, "TA_IN"},
  {0xd018, "TA_CALL"},
  {0xd01a, "TA_PLOT"},
  {0xd01c, "TA_HLIN"},
  {0xd01e, "TA_VLIN"},
  {0xd020, "TA_HGR2"},
  {0xd022, "TA_HGR"},
  {0xd024, "TA_HCOLOR"},
  {0xd026, "TA_HPLOT"},
  {0xd028, "TA_DRAW"},
  {0xd02a, "TA_XDRAW"},
  {0xd02c, "TA_HTAB"},
  {0xd02e, "TA_HOME"},
  {0xd030, "TA_ROT"},
  {0xd032, "TA_SCALE"},
  {0xd034, "TA_SHLOAD"},
  {0xd036, "TA_TRACE"},
  {0xd038, "TA_NOTRACE"},
  {0xd03a, "TA_NORMAL"},
  {0xd03c, "TA_INVERSE"},
  {0xd03e, "TA_FLASH"},
  {0xd040, "TA_COLOR"},
  {0xd042, "TA_POP"},
  {0xd044, "TA_VTAB"},
  {0xd046, "TA_HIMEM"},
  {0xd048, "TA_LOMEM"},
  {0xd04a, "TA_ONERR"},
  {0xd04c, "TA_RESUME"},
  {0xd04e, "TA_RECALL"},
  {0xd050, "TA_STORE"},
  {0xd052, "TA_SPEED"},
  {0xd054, "TA_LET"},
  {0xd056, "TA_GOTO"},
  {0xd058, "TA_RUN"},
  {0xd05a, "TA_IF"},
  {0xd05c, "TA_RESTORE"},
  {0xd05e, "TA_AMPERV"},
  {0xd060, "TA_GOSUB"},
  {0xd062, "TA_RETURN"},
  {0xd064, "TA_REM"},
  {0xd066, "TA_STOP"},
  {0xd068, "TA_ONGOTO"},
  {0xd06a, "TA_WAIT"},
  {0xd06c, "TA_LOAD"},
  {0xd06e, "TA_SAVE"},
  {0xd070, "TA_DEF"},
  {0xd072, "TA_POKE"},
  {0xd074, "TA_PRINT"},
  {0xd076, "TA_CONT"},
  {0xd078, "TA_LIST"},
  {0xd07a, "TA_CLEAR"},
  {0xd07c, "TA_GET"},
  {0xd07e, "TA_NEW"},
  {0xd080, "UNFNC"},
  {0xd082, "TA_INT"},
  {0xd084, "TA_ABS"},
  {0xd086, "TA_USR"},
  {0xd088, "TA_FRE"},
  {0xd08a, "TA_ERROR"},
  {0xd08c, "TA_PDL"},
  {0xd08e, "TA_POS"},
  {0xd090, "TA_SQR"},
  {0xd092, "TA_RND"},
  {0xd094, "TA_LOG"},
  {0xd096, "TA_EXP"},
  {0xd098, "TA_COS"},
  {0xd09a, "TA_SIN"},
  {0xd09c, "TA_TAN"},
  {0xd09e, "TA_ATN"},
  {0xd0a0, "TA_PEEK"},
  {0xd0a2, "TA_LEN"},
  {0xd0a4, "TA_STR"},
  {0xd0a6, "TA_VAL"},
  {0xd0a8, "TA_ASC"},
  {0xd0aa, "TA_CHR"},
  {0xd0ac, "TA_LEFT"},
  {0xd0ae, "TA_RIGHT"},
  {0xd0b0, "TA_MID"},
  {0xd0b2, "MO_ADD"},
  {0xd0b3, "TA_ADD"},
  {0xd0b5, "MO_SUB"},
  {0xd0b6, "TA_SUB"},
  {0xd0b8, "MO_MUL"},
  {0xd0b9, "TA_MUL"},
  {0xd0bb, "MO_DIV"},
  {0xd0bc, "TA_DIV"},
  {0xd0be, "MO_PWR"},
  {0xd0bf, "TA_PWR"},
  {0xd0c1, "MO_AND"},
  {0xd0c2, "TA_AND"},
  {0xd0c4, "MO_OR"},
  {0xd0c5, "TA_OR"},
  {0xd0c7, "MO_UMNS_GTLT"},
  {0xd0c8, "TA_UMNS_GTLT"},
  {0xd0ca, "MO_UNOT_EQUL"},
  {0xd0cb, "TA_UNOT_EQUL"},
  {0xd0cd, "MO_UPLS_LTGT"},
  {0xd0ce, "TA_UPLS_LTGT"},
  {0xd0d0, "TN_END"},
  {0xd0d3, "TN_FOR"},
  {0xd0d6, "TN_NEXT"},
  {0xd0da, "TN_DATA"},
  {0xd0de, "TN_INPUT"},
  {0xd0e3, "TN_DEL"},
  {0xd0e6, "TN_DIM"},
  {0xd0e9, "TN_READ"},
  {0xd0ed, "TN_GR"},
  {0xd0ef, "TN_TEXT"},
  {0xd0f3, "TN_PR"},
  {0xd0f6, "TN_IN"},
  {0xd0f9, "TN_CALL"},
  {0xd0fd, "TN_PLOT"},
  {0xd101, "TN_HLIN"},
  {0xd105, "TN_VLIN"},
  {0xd109, "TN_HGR2"},
  {0xd10d, "TN_HGR"},
  {0xd110, "TN_HCOLOR"},
  {0xd117, "TN_HPLOT"},
  {0xd11c, "TN_DRAW"},
  {0xd120, "TN_XDRAW"},
  {0xd125, "TN_HTAB"},
  {0xd129, "TN_HOME"},
  {0xd12d, "TN_ROT"},
  {0xd131, "TN_SCALE"},
  {0xd137, "TN_SHLOAD"},
  {0xd13d, "TN_TRACE"},
  {0xd142, "TN_NOTRACE"},
  {0xd149, "TN_NORMAL"},
  {0xd14f, "TN_INVERSE"},
  {0xd156, "TN_FLASH"},
  {0xd15b, "TN_COLOR"},
  {0xd161, "TN_POP"},
  {0xd164, "TN_VTAB"},
  {0xd168, "TN_HIMEM"},
  {0xd16e, "TN_LOMEM"},
  {0xd174, "TN_ONERR"},
  {0xd179, "TN_RESUME"},
  {0xd17f, "TN_RECALL"},
  {0xd185, "TN_STORE"},
  {0xd18a, "TN_SPEED"},
  {0xd190, "TN_LET"},
  {0xd193, "TN_GOTO"},
  {0xd197, "TN_RUN"},
  {0xd19a, "TN_IF"},
  {0xd19c, "TN_RESTORE"},
  {0xd1a3, "TN_AMPERSAND"},
  {0xd1a4, "TN_GOSUB"},
  {0xd1a9, "TN_RETURN"},
  {0xd1af, "TN_REM"},
  {0xd1b2, "TN_STOP"},
  {0xd1b6, "TN_ON"},
  {0xd1b8, "TN_WAIT"},
  {0xd1bc, "TN_LOAD"},
  {0xd1c0, "TN_SAVE"},
  {0xd1c4, "TN_DEF"},
  {0xd1c7, "TN_POKE"},
  {0xd1cb, "TN_PRINT"},
  {0xd1d0, "TN_CONT"},
  {0xd1d4, "TN_LIST"},
  {0xd1d8, "TN_CLEAR"},
  {0xd1dd, "TN_GET"},
  {0xd1e0, "TN_NEW"},
  {0xd1e3, "TN_TAB"},
  {0xd1e7, "TN_TO"},
  {0xd1e9, "TN_FN"},
  {0xd1eb, "TN_SPC"},
  {0xd1ef, "TN_THEN"},
  {0xd1f3, "TN_AT"},
  {0xd1f5, "TN_NOT"},
  {0xd1f8, "TN_STEP"},
  {0xd1fc, "TN_ADD"},
  {0xd1fd, "TN_SUB"},
  {0xd1fe, "TN_MUL"},
  {0xd1ff, "TN_DIV"},
  {0xd200, "TN_PWR"},
  {0xd201, "TN_AND"},
  {0xd204, "TN_OR"},
  {0xd206, "TN_NEG"},
  {0xd207, "TN_EQU"},
  {0xd208, "TN_REL"},
  {0xd209, "TN_SGN"},
  {0xd20c, "TN_INT"},
  {0xd20f, "TN_ABS"},
  {0xd212, "TN_USR"},
  {0xd215, "TN_FRE"},
  {0xd218, "TN_SCRN"},
  {0xd21d, "TN_PDL"},
  {0xd220, "TN_POS"},
  {0xd223, "TN_SQR"},
  {0xd226, "TN_RND"},
  {0xd229, "TN_LOG"},
  {0xd22c, "TN_EXP"},
  {0xd22f, "TN_COS"},
  {0xd232, "TN_SIN"},
  {0xd235, "TN_TAN"},
  {0xd238, "TN_ATN"},
  {0xd23b, "TN_PEEK"},
  {0xd23f, "TN_LEN"},
  {0xd242, "TN_STR"},
  {0xd246, "TN_VAL"},
  {0xd249, "TN_ASC"},
  {0xd24c, "TN_CHR"},
  {0xd250, "TN_LEFT"},
  {0xd255, "TN_RIGHT"},
  {0xd25b, "TN_MID"},
  {0xd25f, "TOKNAMEND"},
  {0xd260, "NOFOR_ERR"},
  {0xd270, "SYNTAXERR"},
  {0xd276, "NOGOSUBERR"},
  {0xd28a, "NODATAERR"},
  {0xd295, "ILLQTYERR"},
  {0xd2a5, "OVERFLOWERR"},
  {0xd2ad, "MEMFULLERR"},
  {0xd2ba, "UNDEFSTATERR"},
  {0xd2cb, "BADSUBSERR"},
  {0xd2d8, "REDIMDERR"},
  {0xd2e5, "ZERODIVERR"},
  {0xd2f5, "ILLDIRERR"},
  {0xd303, "BADTYPEERR"},
  {0xd310, "STRLONGERR"},
  {0xd31f, "FRMCPXERR"},
  {0xd332, "CANTCONTERR"},
  {0xd340, "UNDEFFUNCERR"},
  {0xd350, "QT_ERROR"},
  {0xd358, "QT_IN"},
  {0xd35d, "QT_BREAK"},
  {0xd365, "GETFORPTR"},
  {0xd36a, "FORFRAME"},
  {0xd37f, "SAMEVARPTR"},
  {0xd38b, "NEXTFRAME"},
  {0xd392, "BAS_RTS00"},
  {0xd393, "MVBLKUP1"},
  {0xd39a, "MVBLKUP2"},
  {0xd3ab, "MVBKPHSA"},
  {0xd3b7, "MVBKPHDA"},
  {0xd3c3, "DOMVBLKUP1"},
  {0xd3c7, "DOMVBLKUP2"},
  {0xd3ce, "NXTMVBLKUP"},
  {0xd3d6, "CHKMEM"},
  {0xd3e3, "REASON"},
  {0xd3ed, "MAXFRESPC"},
  {0xd3f1, "PUSHNINE"},
  {0xd3fc, "PULLNINE"},
  {0xd40f, "BAS_RTS01"},
  {0xd410, "MEMERR"},
  {0xd412, "ERROR"},
  {0xd419, "DOERRMSG"},
  {0xd41f, "PRERRMSG"},
  {0xd431, "PRERRLINO"},
  {0xd43c, "RESTART"},
  {0xd45c, "NMBRDLN"},
  {0xd464, "NMBRDLN2"},
  {0xd49f, "DECSRCPTR"},
  {0xd4a7, "DOMVBLKDWN"},
  {0xd4b5, "NEWLINE"},
  {0xd4d1, "MVPRGM"},
  {0xd4ea, "INSERTLINE"},
  {0xd4f2, "FIXLINKS"},
  {0xd4fe, "NXTLNK"},
  {0xd50f, "PUTLNK"},
  {0xd511, "FNDEOL"},
  {0xd52c, "INLIN"},
  {0xd52e, "INLIN2"},
  {0xd539, "LENGTHOK"},
  {0xd541, "STRIP"},
  {0xd54c, "NOINPUT"},
  {0xd553, "INCHR"},
  {0xd559, "PARSELINE"},
  {0xd56c, "PARSE"},
  {0xd56d, "NXCHR"},
  {0xd578, "DATASTATE"},
  {0xd588, "ISITATOKEN"},
  {0xd590, "ISATOKEN"},
  {0xd5a2, "INC_Y"},
  {0xd5a7, "INC_X"},
  {0xd5a8, "LIN"},
  {0xd5cb, "PUTTOK"},
  {0xd5cd, "PUTIN"},
  {0xd5e0, "SDF"},
  {0xd5e2, "ISITAREM"},
  {0xd5e9, "COPYCHARS"},
  {0xd5f2, "QUOTATION"},
  {0xd5f9, "NXTTOKNAM"},
  {0xd5fd, "SKIPOVER"},
  {0xd604, "CHKTNCHR"},
  {0xd610, "LINEND"},
  {0xd61a, "FNDLIN"},
  {0xd61e, "FNDLIN2"},
  {0xd635, "FL_CHKLO"},
  {0xd63e, "FL_NXTLIN"},
  {0xd647, "FL_NOTFND"},
  {0xd648, "BAS_RTS02"},
  {0xd649, "NEW"},
  {0xd64b, "SCRTCH"},
  {0xd665, "SETPTRS"},
  {0xd66a, "CLEAR"},
  {0xd66c, "CLEARC"},
  {0xd683, "STKINI"},
  {0xd696, "BAS_RTS03"},
  {0xd697, "CLRTXTPTR"},
  {0xd6a5, "LIST"},
  {0xd6b1, "STRTRNG"},
  {0xd6c4, "ENDRNG"},
  {0xd6cc, "MAINLST"},
  {0xd6da, "LSTNXTLIN"},
  {0xd6f5, "LSTD"},
  {0xd6f7, "LST1"},
  {0xd6fe, "LISTLINE"},
  {0xd702, "SNDCHR"},
  {0xd712, "NOCRTAB"},
  {0xd724, "LISTED"},
  {0xd72c, "GETCHAR"},
  {0xd731, "GC1"},
  {0xd734, "TOKEN"},
  {0xd746, "SKPTK"},
  {0xd749, "TOKL"},
  {0xd750, "PT"},
  {0xd755, "TOKLP"},
  {0xd75f, "TOKDUN"},
  {0xd766, "FOR"},
  {0xd777, "FOR2"},
  {0xd7af, "STEP"},
  {0xd7c3, "ONESTEP"},
  {0xd7d2, "NEWSTT"},
  {0xd7e5, "DIR"},
  {0xd805, "TRACEQ"},
  {0xd81d, "EXECUTE"},
  {0xd826, "GOEND"},
  {0xd828, "EXECSTMNT"},
  {0xd82a, "EXECSTMNT1"},
  {0xd83f, "ASSIGNMENT"},
  {0xd842, "COLONQ"},
  {0xd846, "SYNERR"},
  {0xd849, "RESTORE"},
  {0xd853, "SETDA"},
  {0xd857, "BAS_RTS05"},
  {0xd858, "ISCTRLCH"},
  {0xd860, "GETLOWKEY"},
  {0xd863, "ISONERRON"},
  {0xd86c, "ISCTRLCL"},
  {0xd86e, "STOP"},
  {0xd870, "END"},
  {0xd871, "END2"},
  {0xd888, "END3"},
  {0xd88a, "END4"},
  {0xd893, "GOSTART"},
  {0xd896, "CONT"},
  {0xd8a1, "CANCONT"},
  {0xd8af, "BAS_RTS06"},
  {0xd8b0, "BAS_SAVE"},
  {0xd8c9, "BAS_LOAD"},
  {0xd8ed, "BAS_LOADED"},
  {0xd8f0, "VARTIO"},
  {0xd901, "PROGIO"},
  {0xd912, "RUN"},
  {0xd91b, "RUNLIN"},
  {0xd921, "GOSUB"},
  {0xd935, "GO_TO_LINE"},
  {0xd93e, "GOTO"},
  {0xd955, "GOTO_1"},
  {0xd959, "GOTO_2"},
  {0xd96a, "BAS_RTS07"},
  {0xd96b, "POPRTN"},
  {0xd979, "NOGSBERR"},
  {0xd97c, "UNDFSTERR"},
  {0xd981, "SYNERR2"},
  {0xd984, "RETURN"},
  {0xd995, "DATA"},
  {0xd998, "ADDON"},
  {0xd9a2, "BAS_RTS08"},
  {0xd9a3, "DATA_END"},
  {0xd9a6, "REM_END"},
  {0xd9ae, "END_SWAP"},
  {0xd9b6, "END_SCAN"},
  {0xd9c5, "PULL3"},
  {0xd9c9, "IF"},
  {0xd9d8, "QTF"},
  {0xd9dc, "REM"},
  {0xd9e1, "IF_TRUE"},
  {0xd9e9, "JGOCMD"},
  {0xd9ec, "ONGOTO"},
  {0xd9f4, "ONGOTOK"},
  {0xd9f8, "ONGOCASE"},
  {0xda00, "ONGOFIND"},
  {0xda0b, "BAS_RTS09"},
  {0xda0c, "LINGET"},
  {0xda12, "ASCHEX"},
  {0xda40, "NXTDGT"},
  {0xda46, "LET"},
  {0xda63, "LETINT"},
  {0xda77, "LETREAL"},
  {0xda7a, "LETSTR"},
  {0xda7b, "PUTSTR"},
  {0xda8c, "STRDSC"},
  {0xda9a, "COPSTRDSC"},
  {0xdaa1, "NEWSTRDSC"},
  {0xdab7, "SAVSTRDSC"},
  {0xdacf, "PRSTR"},
  {0xdad5, "PRINT"},
  {0xdad7, "PRINT2"},
  {0xdafb, "CRDO"},
  {0xdb00, "NEGATE"},
  {0xdb02, "BAS_RTS10"},
  {0xdb03, "PRCOMMA"},
  {0xdb0e, "PRCMMA"},
  {0xdb16, "PRTABSPC"},
  {0xdb21, "PRTAB"},
  {0xdb2b, "PRSPC"},
  {0xdb2c, "NXTSPC"},
  {0xdb2f, "PRNXTCHR"},
  {0xdb35, "DOSPC"},
  {0xdb3a, "STROUT"},
  {0xdb3d, "STRPRT"},
  {0xdb44, "STRPRT_1"},
  {0xdb57, "OUTSP"},
  {0xdb5a, "OUTQUES"},
  {0xdb5c, "OUTDO"},
  {0xdb64, "OUTDOCTRL"},
  {0xdb71, "INPUTERR"},
  {0xdb7b, "READERR"},
  {0xdb7f, "ERRLINE"},
  {0xdb86, "INPERR"},
  {0xdb87, "RESPERR"},
  {0xdb8b, "BADRESPERR"},
  {0xdb90, "DOREENTRY"},
  {0xdba0, "GET"},
  {0xdbb2, "INPUT"},
  {0xdbc4, "QOUT"},
  {0xdbc7, "DIRINERR"},
  {0xdbdc, "NEXTIN"},
  {0xdbe2, "BAS_READ"},
  {0xdbe9, "INFLAG0"},
  {0xdbeb, "PRCSINLST"},
  {0xdbf1, "PRCSINITM"},
  {0xdc1f, "PRCNOTGET"},
  {0xdc27, "PRCTXTPTR"},
  {0xdc2b, "INSTART"},
  {0xdc3f, "PUTCHR"},
  {0xdc4b, "PPENDCHR"},
  {0xdc4c, "PUTENDCHR"},
  {0xdc57, "SKIPHIGH"},
  {0xdc63, "NUMIN"},
  {0xdc69, "DATIN"},
  {0xdc72, "INMORE"},
  {0xdc7e, "SWAPPTRS"},
  {0xdc99, "INPFIN"},
  {0xdca0, "FINDATA"},
  {0xdcb9, "NXTSTT"},
  {0xdcc6, "INPDONE"},
  {0xdcd1, "INDNSKIP"},
  {0xdcde, "BAS_RTS11"},
  {0xdcdf, "ERR_EXTRA"},
  {0xdcef, "ERR_REENTRY"},
  {0xdcf9, "NEXT"},
  {0xdcff, "VARNXT"},
  {0xdd02, "SKPV"},
  {0xdd0b, "NOFORERR"},
  {0xdd0d, "GERR"},
  {0xdd0f, "GOTFOR"},
  {0xdd52, "GONEWST"},
  {0xdd55, "ENDFOR"},
  {0xdd67, "FRMNUM"},
  {0xdd6a, "CHKNUM"},
  {0xdd6c, "CHKSTR"},
  {0xdd6d, "CHKVAL"},
  {0xdd73, "BAS_RTS12"},
  {0xdd74, "NOTNUMSTR"},
  {0xdd76, "MISMATCH"},
  {0xdd78, "JERROR"},
  {0xdd7b, "FRMEVL"},
  {0xdd81, "FESKPHI"},
  {0xdd86, "FRMEVL1"},
  {0xdd95, "FRMEVL2"},
  {0xdd98, "FE2LOOP"},
  {0xddb4, "FE2CONT"},
  {0xddc5, "FE2NUM"},
  {0xddcd, "PREFTEST"},
  {0xddd6, "NXTOP"},
  {0xddd7, "SAVOP"},
  {0xdde4, "FRMREL"},
  {0xddee, "FRSKPGR"},
  {0xddf6, "PREFNC"},
  {0xddfd, "FRM_RECURSE"},
  {0xde0d, "SNTXERR"},
  {0xde10, "PSHFAC"},
  {0xde15, "PSHFACX"},
  {0xde20, "PUSHFAC"},
  {0xde35, "NOTMATH"},
  {0xde38, "GOEX"},
  {0xde3a, "DOMATHNOW"},
  {0xde41, "SKPNUMCHK"},
  {0xde43, "DOMATH"},
  {0xde5d, "EXIT"},
  {0xde60, "GETVAL"},
  {0xde64, "SKIP"},
  {0xde69, "NUMBER"},
  {0xde6c, "VARIABLE"},
  {0xde81, "STRTEXT"},
  {0xde8a, "TEXTSTR"},
  {0xde90, "TOKNOT"},
  {0xde98, "EQUOP"},
  {0xde9f, "EQUOP1"},
  {0xdea4, "TOKFN"},
  {0xdeab, "TOKSGN"},
  {0xdeb2, "PARCHK"},
  {0xdeb8, "CHKCLS"},
  {0xdebb, "CHKOPN"},
  {0xdebe, "CHKCOM"},
  {0xdec0, "SYNCHR"},
  {0xdec9, "SYNERROR"},
  {0xdece, "UMINUS"},
  {0xded0, "EQUL"},
  {0xded5, "FNDNUMVAR"},
  {0xdee5, "FNDINTVAR"},
  {0xdef6, "FNDFPVAR"},
  {0xdef9, "SCREEN"},
  {0xdf0c, "UNARY"},
  {0xdf3a, "UNARY1"},
  {0xdf3f, "UNARY2"},
  {0xdf4f, "OR"},
  {0xdf55, "AND"},
  {0xdf5d, "FALSE"},
  {0xdf60, "TRUE"},
  {0xdf65, "RELOPS"},
  {0xdf7d, "STRCMP"},
  {0xdfa5, "SCFS"},
  {0xdfaa, "CMPLOOP"},
  {0xdfb0, "NUMCMP"},
  {0xdfb5, "DOCMP"},
  {0xdfc1, "CMPDONE"},
  {0xdfca, "CMPFINFLT"},
  {0xdfcd, "PDL"},
  {0xdfd6, "NXDIM"},
  {0xdfd9, "DIM"},
  {0xdfe3, "PTRGET"},
  {0xdfe8, "PTRGET2"},
  {0xdfea, "PTRGET3"},
  {0xdff4, "BADNAM"},
  {0xdff7, "NAMOK"},
  {0xe000, "BASIC"},
  {0xe003, "BASIC2"},
  {0xe007, "PTRGETMOR"},
  {0xe011, "PTRGETNUM"},
  {0xe012, "PTRGETNLP"},
  {0xe01c, "PTRGETSTR"},
  {0xe026, "PTRGETINT"},
  {0xe036, "PTRGETNIN"},
  {0xe03d, "PTRGET2ND"},
  {0xe046, "PTRGETARY"},
  {0xe049, "PTRGETNVC"},
  {0xe04f, "PTRGETIVP"},
  {0xe059, "PTRGETLP1"},
  {0xe05b, "PTRGETLP2"},
  {0xe065, "PTRGETFND"},
  {0xe073, "PTRGET4_13"},
  {0xe07d, "ISLETC"},
  {0xe086, "BAS_RTS13"},
  {0xe087, "NAMNOTFND"},
  {0xe09a, "CON_ZERO"},
  {0xe09c, "MKNEWVAR"},
  {0xe0b2, "MNVSPA"},
  {0xe0de, "SETVARPTRYA"},
  {0xe0e8, "SETVARPTR"},
  {0xe0ed, "GETARY"},
  {0xe0ef, "GETARY2"},
  {0xe0f9, "GETARY3"},
  {0xe0fe, "NEG32768"},
  {0xe102, "MAKINT"},
  {0xe108, "MKINT"},
  {0xe10c, "AYINT"},
  {0xe119, "IQERROR"},
  {0xe11b, "CNVRT2INT"},
  {0xe11e, "ARRAY"},
  {0xe12c, "NXTDIM"},
  {0xe169, "FNDARY"},
  {0xe16d, "ARYLOOP"},
  {0xe179, "ARYNAME"},
  {0xe188, "NXTARY"},
  {0xe196, "SUBERR"},
  {0xe199, "IQERR"},
  {0xe19b, "JMPERR"},
  {0xe19e, "ARYVARFND"},
  {0xe1aa, "CHKDIM"},
  {0xe1b8, "NEWARYVAR"},
  {0xe1bc, "NODATERR"},
  {0xe1c1, "MAKARY"},
  {0xe1d5, "ARYNOTINT"},
  {0xe1de, "ARYISREAL"},
  {0xe1e7, "SAVDIM"},
  {0xe1f7, "DFLTDIM"},
  {0xe21a, "CHKARYSPC"},
  {0xe229, "CLRARY"},
  {0xe22e, "CLRNXTPG"},
  {0xe24b, "FNDARYELM"},
  {0xe253, "DIMLUP"},
  {0xe269, "GSE"},
  {0xe26c, "GME"},
  {0xe26f, "DIMOK"},
  {0xe270, "DIMOK1"},
  {0xe281, "NEXTDIM"},
  {0xe292, "DIMNOTINT"},
  {0xe298, "DIMISREAL"},
  {0xe2ac, "BAS_RTS14"},
  {0xe2ad, "MULTSUBS"},
  {0xe2b6, "MULTSUBS1"},
  {0xe2c0, "MLTSBSLOOP"},
  {0xe2d9, "MULTSUBS1_2"},
  {0xe2de, "FRE"},
  {0xe2e5, "FREESPACE"},
  {0xe2f2, "GIVAYF"},
  {0xe2ff, "POS"},
  {0xe301, "SNGFLT"},
  {0xe306, "ERRDIR"},
  {0xe30b, "IDERR"},
  {0xe30e, "UNDFNCERR"},
  {0xe313, "DEF"},
  {0xe341, "FN_NAME"},
  {0xe354, "FN_CALL"},
  {0xe378, "SAVOLD"},
  {0xe3a9, "GETOLD"},
  {0xe3af, "FN_DATA"},
  {0xe3c5, "STR"},
  {0xe3d5, "STRINI"},
  {0xe3dd, "STRSPC"},
  {0xe3e7, "STRLTRL"},
  {0xe3ed, "STRLTRL2"},
  {0xe3f7, "STRFNDEND"},
  {0xe404, "STRENDFND"},
  {0xe408, "STRZNDFND"},
  {0xe409, "STRQNDFND"},
  {0xe415, "STRLTRLNC"},
  {0xe41f, "STRLTRLZP"},
  {0xe42a, "PUTNEW"},
  {0xe430, "FRM2CPXERR"},
  {0xe432, "JERR"},
  {0xe435, "PUTEMP"},
  {0xe452, "GETSPACE"},
  {0xe454, "GETSPC"},
  {0xe45f, "GETSPC2"},
  {0xe469, "GOTSPACE"},
  {0xe474, "MEMFULL"},
  {0xe484, "GARBAGE"},
  {0xe488, "FNDVAR"},
  {0xe4a0, "TVAR"},
  {0xe4a9, "SVARS"},
  {0xe4b5, "SVAR"},
  {0xe4bd, "SVARGO"},
  {0xe4c2, "ARYVAR"},
  {0xe4ca, "ARYVAR2"},
  {0xe4ce, "ARYVAR3"},
  {0xe4d9, "ARYVARGO"},
  {0xe50a, "AVGSKPADV"},
  {0xe50c, "ARYSTR"},
  {0xe514, "GOGO"},
  {0xe519, "CHKSMPLVAR"},
  {0xe523, "CHKVAR"},
  {0xe538, "CHKVAR1"},
  {0xe542, "CHKVAR2"},
  {0xe552, "CHKBUMP"},
  {0xe55d, "CHKEXIT"},
  {0xe562, "GRBGPASS"},
  {0xe597, "CAT"},
  {0xe5b2, "STR2LGERR"},
  {0xe5b7, "CAT1"},
  {0xe5d4, "MOVINS"},
  {0xe5e2, "MOVSTR"},
  {0xe5e6, "MOVSTR1"},
  {0xe5ea, "MOVESTR"},
  {0xe5f3, "MOVSTR2"},
  {0xe5fc, "MOVSTR3"},
  {0xe5fd, "FRESTR"},
  {0xe600, "FREFAC"},
  {0xe604, "FRETMP"},
  {0xe62f, "FRETMP1"},
  {0xe630, "FRETMP2"},
  {0xe635, "FRETMS"},
  {0xe645, "FRETMS1"},
  {0xe646, "CHRSTR"},
  {0xe65a, "LEFTSTR"},
  {0xe660, "RGHTSTR"},
  {0xe666, "INSTR"},
  {0xe667, "MIDSTR2"},
  {0xe668, "MIDSTR3"},
  {0xe67f, "INSTRSKP"},
  {0xe686, "RIGHTSTR"},
  {0xe691, "MIDSTR"},
  {0xe6a2, "MIDSTR1"},
  {0xe6b9, "INSTRING"},
  {0xe6d6, "LEN"},
  {0xe6dc, "GETSTR"},
  {0xe6e5, "ASC"},
  {0xe6f2, "GOIQ"},
  {0xe6f5, "GTBYTC"},
  {0xe6f8, "GETBYT"},
  {0xe6fb, "CONINT"},
  {0xe707, "VAL"},
  {0xe70f, "VALGO"},
  {0xe727, "VALSKP"},
  {0xe73d, "POINT"},
  {0xe746, "GTNUM"},
  {0xe74c, "COMBYTE"},
  {0xe752, "GETADR"},
  {0xe764, "PEEK"},
  {0xe77b, "POKE"},
  {0xe784, "WAIT"},
  {0xe793, "WAITSKIP"},
  {0xe797, "WAITLOOP"},
  {0xe79f, "BAS_RTS15"},
  {0xe7a0, "FADDH"},
  {0xe7a7, "FSUB"},
  {0xe7aa, "FSUBT"},
  {0xe7b9, "FADD1"},
  {0xe7be, "FADD"},
  {0xe7c1, "FADDT"},
  {0xe7c6, "FADDFAC"},
  {0xe7ce, "FADD2"},
  {0xe7ea, "FARGLESS"},
  {0xe7ee, "FBITSHFT"},
  {0xe7fa, "FADD3"},
  {0xe806, "SUBMANTS"},
  {0xe829, "NRMLZFAC1"},
  {0xe82e, "NRMLZFAC2"},
  {0xe832, "NRMLZLOOP"},
  {0xe84e, "ZFACEXPSGN"},
  {0xe850, "ZFACSGNEXP"},
  {0xe852, "ZFACSGN"},
  {0xe855, "ADDMANTS"},
  {0xe874, "FRLOOP"},
  {0xe880, "FNRMLZFAC"},
  {0xe88d, "FRBCC"},
  {0xe88f, "FROUND"},
  {0xe89d, "BAS_RTS16"},
  {0xe89e, "TWSCMPFAC"},
  {0xe8a4, "TCFACMANT"},
  {0xe8c6, "INCFACMAN"},
  {0xe8d4, "BAS_RTS17"},
  {0xe8d5, "OVERFLOW"},
  {0xe8da, "SHFTRGHT1"},
  {0xe8dc, "SR1LOOP"},
  {0xe8f0, "SHFTRGHT"},
  {0xe8fd, "SRLOOP"},
  {0xe903, "SRSKP"},
  {0xe907, "SHFTRGHT4"},
  {0xe911, "SHFTRGHT5"},
  {0xe913, "CON_ONE"},
  {0xe918, "POLY_LOG"},
  {0xe919, "POLY_LOG_7"},
  {0xe91e, "POLY_LOG_5"},
  {0xe923, "POLY_LOG_3"},
  {0xe928, "POLY_LOG_1"},
  {0xe92d, "CON_SQRHALF"},
  {0xe932, "CON_SQRTWO"},
  {0xe937, "CON_NEGHALF"},
  {0xe93c, "CON_LOGTWO"},
  {0xe941, "LOG"},
  {0xe948, "GIQ"},
  {0xe94b, "LOG2"},
  {0xe97f, "FMULT"},
  {0xe982, "FMULTT"},
  {0xe987, "FMULTT1"},
  {0xe9b0, "MULT1"},
  {0xe9b5, "MULT2"},
  {0xe9b8, "M2ADDLOOP"},
  {0xe9d4, "MULT2SHFT"},
  {0xe9e2, "BAS_RTS18"},
  {0xe9e3, "UPAY2ARG"},
  {0xea0e, "ADDEXPS"},
  {0xea10, "ADDEXPS1"},
  {0xea1b, "ADDEXPS2"},
  {0xea26, "ADDEXPS3"},
  {0xea28, "ADDEXPS4"},
  {0xea2b, "OUTOFRNG"},
  {0xea31, "ZERO"},
  {0xea36, "JOV"},
  {0xea39, "MUL10"},
  {0xea4f, "MUL10RTN"},
  {0xea50, "CON_TEN"},
  {0xea55, "DIV10"},
  {0xea5e, "DIV"},
  {0xea60, "NON_SEQUITUR"},
  {0xea66, "FDIV"},
  {0xea69, "FDIVT"},
  {0xea80, "FDIVT1"},
  {0xea96, "FDIVT2"},
  {0xeaa3, "FDIVT3"},
  {0xeaa6, "FDIVT4"},
  {0xeab4, "FDIVT5"},
  {0xead1, "FDIVT6"},
  {0xead5, "FDIVT7"},
  {0xeae1, "DOZDIVERR"},
  {0xeae6, "CPY2FAC"},
  {0xeaf9, "UPAY2FAC"},
  {0xeb1e, "FACRND2TMP2"},
  {0xeb21, "FACRND2TMP1"},
  {0xeb27, "SETFOR"},
  {0xeb2b, "FACRND2XY"},
  {0xeb53, "CPYARG2FAC"},
  {0xeb55, "CPYARGFAC"},
  {0xeb59, "CA2FLOOP"},
  {0xeb63, "FACRND2ARG"},
  {0xeb66, "FAC2ARG"},
  {0xeb68, "F2ALOOP"},
  {0xeb71, "BAS_RTS19"},
  {0xeb72, "ROUND_FAC"},
  {0xeb7a, "INCMAN"},
  {0xeb82, "SIGN"},
  {0xeb86, "SIGN1"},
  {0xeb88, "SIGN2"},
  {0xeb8f, "BAS_RTS20"},
  {0xeb90, "SGN"},
  {0xeb93, "FLOAT"},
  {0xeb9b, "FLOAT1"},
  {0xeba0, "FLOAT2"},
  {0xebaf, "ABS"},
  {0xebb2, "FCOMP"},
  {0xebb4, "FCOMP2"},
  {0xebe9, "FCFINISH"},
  {0xebef, "FCDONE"},
  {0xebf2, "QINT"},
  {0xec06, "QINT1"},
  {0xec11, "BAS_RTS21"},
  {0xec12, "QINT2"},
  {0xec23, "INT"},
  {0xec40, "QINT3"},
  {0xec49, "BAS_RTS22"},
  {0xec4a, "FIN"},
  {0xec4e, "FINLOOP"},
  {0xec5d, "FINPLUS?"},
  {0xec61, "FIN1EVAL"},
  {0xec64, "FIN2NMRL"},
  {0xec66, "FIN3PDP?"},
  {0xec85, "FIN3NEGX"},
  {0xec87, "FIN4POSX"},
  {0xec8a, "FIN5NMRL"},
  {0xec8c, "FIN6SGNX"},
  {0xec98, "FINADJX"},
  {0xec9e, "FIN7POSX"},
  {0xeca0, "FIN8ADJX"},
  {0xeca9, "FIN8DPL"},
  {0xecb2, "FIN8DPR"},
  {0xecb9, "FIN8EVDN?"},
  {0xecbe, "FIN8EVD"},
  {0xecc1, "FIN9NMRL"},
  {0xecc8, "FIN9X10"},
  {0xecd5, "ADDACC"},
  {0xece8, "GETEXP"},
  {0xecf7, "GETEXP1"},
  {0xed05, "GETEXP2"},
  {0xed0a, "CON_1E8M1"},
  {0xed0f, "CON_1E9M1"},
  {0xed14, "CON_1E9"},
  {0xed19, "INPRT"},
  {0xed24, "LINPRT"},
  {0xed2e, "PRINTFAC"},
  {0xed31, "PRINTSTR"},
  {0xed34, "FOUT"},
  {0xed36, "FOUT1"},
  {0xed41, "FOUTPOS"},
  {0xed4f, "FOUTNOT0"},
  {0xed57, "FOUTIS1"},
  {0xed60, "FOUTMORE"},
  {0xed62, "FOUTADJ1"},
  {0xed6d, "FOUTADJ2"},
  {0xed78, "FOUTSMALL"},
  {0xed7f, "FOUTLARGE"},
  {0xed86, "FOUTINRNG1"},
  {0xed89, "FOUTINRNG2"},
  {0xed9e, "FOUTXF1"},
  {0xed9f, "FOUTXF2"},
  {0xedaa, "FOUTPDP"},
  {0xedbb, "FOUTSVY"},
  {0xedbd, "FOUTMKS"},
  {0xedc1, "FOUT2LOOP"},
  {0xede5, "FOUT2LPND"},
  {0xede7, "FOUTCNTD"},
  {0xedee, "FOUTNMRL"},
  {0xee09, "FOUTSKPDP"},
  {0xee19, "FOUTMVBK"},
  {0xee26, "FOUTNEEDX"},
  {0xee36, "FOUTPUTX"},
  {0xee42, "FOUTWHATX"},
  {0xee57, "FOUTFIN"},
  {0xee5a, "FOUTMKND"},
  {0xee5f, "FOUTPTSTK"},
  {0xee64, "CON_HALF"},
  {0xee69, "DECTBL"},
  {0xee8d, "SQR"},
  {0xee97, "FPWRT"},
  {0xeea0, "FPWRT1"},
  {0xeeba, "FPWRT2"},
  {0xeed0, "NEGOP"},
  {0xeeda, "BAS_RTS23"},
  {0xeedb, "CON_LOG_E"},
  {0xeee0, "POLY_EXP"},
  {0xeeff, "POLY_EXP_L2"},
  {0xef04, "POLY_ONE"},
  {0xef09, "EXP"},
  {0xef19, "EXP1"},
  {0xef24, "EXP2LOOP"},
  {0xef27, "EXP3"},
  {0xef37, "EXP4LOOP"},
  {0xef5c, "POLY_ODD"},
  {0xef72, "POLYNOMIAL"},
  {0xef76, "SERMAIN"},
  {0xef85, "SERMAIN1"},
  {0xef89, "SERMAIN2"},
  {0xef96, "SERMAIN3"},
  {0xefa5, "BAS_RTS24"},
  {0xefa6, "CON_RND1"},
  {0xefaa, "CON_RND2"},
  {0xefae, "RND"},
  {0xefcc, "RNDFACNEG"},
  {0xefe7, "GO_MOVMF"},
  {0xefea, "COS"},
  {0xeff1, "SIN"},
  {0xf023, "SIN1"},
  {0xf026, "SIN2"},
  {0xf033, "SIN3"},
  {0xf03a, "TAN"},
  {0xf062, "TAN1"},
  {0xf066, "PI_HALVED"},
  {0xf06b, "PI_DOUBLED"},
  {0xf070, "QUARTER"},
  {0xf075, "POLY_SIN"},
  {0xf076, "PI2_11"},
  {0xf07b, "PI2_9"},
  {0xf080, "PI2_7"},
  {0xf085, "PI2_5"},
  {0xf08a, "PI2_3"},
  {0xf08f, "PI2_1"},
  {0xf094, "EASTER_EGG1"},
  {0xf099, "EASTER_EGG0"},
  {0xf09e, "ATN"},
  {0xf0a6, "ATN_1"},
  {0xf0b4, "ATN_2"},
  {0xf0c7, "ATN_3"},
  {0xf0cd, "BAS_RTS25"},
  {0xf0ce, "POLYATN"},
  {0xf0cf, "POLYATN_C"},
  {0xf0d4, "POLYATN_B"},
  {0xf0d9, "POLYATN_A"},
  {0xf0de, "POLYATN_9"},
  {0xf0e3, "POLYATN_8"},
  {0xf0e8, "POLYATN_7"},
  {0xf0ed, "POLYATN_6"},
  {0xf0f2, "POLYATN_5"},
  {0xf0f7, "POLYATN_4"},
  {0xf0fc, "POLYATN_3"},
  {0xf101, "POLYATN_2"},
  {0xf106, "POLYATN_1"},
  {0xf10b, "GENCHRGET"},
  {0xf111, "GENCHRGOT"},
  {0xf122, "GENCGRTS"},
  {0xf123, "RND_SEED"},
  {0xf128, "COLD_START"},
  {0xf152, "CPYCHRGET"},
  {0xf181, "FNDMEM"},
  {0xf195, "MEMFND"},
  {0xf1b8, "INITPRGM"},
  {0xf1d5, "CALL"},
  {0xf1de, "IN_NUMBER"},
  {0xf1e5, "PR_NUMBER"},
  {0xf1ec, "PLOTFNS"},
  {0xf206, "BAS_GOERR"},
  {0xf209, "LINCOOR"},
  {0xf218, "LINAT"},
  {0xf225, "BAS_PLOT"},
  {0xf232, "HLIN"},
  {0xf241, "VLIN"},
  {0xf24f, "COLOR"},
  {0xf256, "VTAB"},
  {0xf262, "SPEED"},
  {0xf26d, "TRACE"},
  {0xf26f, "NOTRACE"},
  {0xf273, "NORMAL"},
  {0xf277, "INVERSE"},
  {0xf279, "NRMINV"},
  {0xf27b, "NRMINVFLSH"},
  {0xf280, "FLASH"},
  {0xf286, "HIMEM"},
  {0xf296, "JMM"},
  {0xf299, "SETHI"},
  {0xf2a6, "LOMEM"},
  {0xf2cb, "ONERR"},
  {0xf2e9, "ERRHNDLR"},
  {0xf318, "RESUME"},
  {0xf32e, "JSYN"},
  {0xf331, "DEL"},
  {0xf357, "DELSKIP1"},
  {0xf365, "DELMVDWN"},
  {0xf367, "DELMVLOOP"},
  {0xf371, "DELMVSKP1"},
  {0xf377, "DELMVSKP2"},
  {0xf388, "DELMVSKP3"},
  {0xf390, "GR"},
  {0xf399, "TEXT"},
  {0xf39f, "STORE"},
  {0xf3af, "STORESKP1"},
  {0xf3bc, "RECALL"},
  {0xf3d2, "RECALLDATA"},
  {0xf3d8, "HGR2"},
  {0xf3e2, "HGR"},
  {0xf3ea, "SETHPG"},
  {0xf3f2, "HCLR"},
  {0xf3fe, "BGLOOP"},
  {0xf411, "HPOSN"},
  {0xf441, "HPOSN1"},
  {0xf442, "HPOSN2"},
  {0xf457, "HPLOT0"},
  {0xf465, "MVLFTRGT"},
  {0xf467, "MVLEFT"},
  {0xf46e, "MVLFTRGT1"},
  {0xf471, "MVLEFT1"},
  {0xf476, "MVLEFT2"},
  {0xf478, "MVLEFT3"},
  {0xf47e, "COLORSHIFT"},
  {0xf489, "BAS_RTS26"},
  {0xf48a, "MVRIGHT"},
  {0xf49c, "LRUDX1"},
  {0xf49d, "LRUDX2"},
  {0xf4b3, "LRUD1"},
  {0xf4b4, "LRUD2"},
  {0xf4c4, "LRUD3"},
  {0xf4c8, "LRUD4"},
  {0xf4cc, "CON3"},
  {0xf4d3, "MVUPDWN0"},
  {0xf4eb, "MVUP1"},
  {0xf4f6, "MVUP2"},
  {0xf4fb, "MVUP3"},
  {0xf4fd, "MVUP4"},
  {0xf4ff, "MVUP5"},
  {0xf501, "MVUPDWN1"},
  {0xf505, "MVDWN"},
  {0xf507, "CON4"},
  {0xf524, "MVDWN1"},
  {0xf52a, "MVDWN2"},
  {0xf52c, "MVDWN3"},
  {0xf530, "HLINRL"},
  {0xf53a, "HGLIN"},
  {0xf550, "HGLIN1"},
  {0xf568, "HGLIN2"},
  {0xf57c, "MOVEX1L1"},
  {0xf581, "MOVEX2"},
  {0xf58b, "MOVEX3L2"},
  {0xf59e, "MOVEX4"},
  {0xf5b2, "MSKTBL"},
  {0xf5b9, "CON_1C"},
  {0xf5ba, "COSTBL"},
  {0xf5cb, "HFIND"},
  {0xf5f0, "HFNDLOOP"},
  {0xf5fe, "HFNDFIN"},
  {0xf600, "BAS_RTS27"},
  {0xf601, "DRAW0"},
  {0xf605, "DRAW1"},
  {0xf626, "DRWLP2"},
  {0xf630, "DRWLP3"},
  {0xf63d, "DRW4"},
  {0xf648, "DRW5"},
  {0xf658, "DRW6"},
  {0xf65d, "XDRAW0"},
  {0xf661, "XDRAW1"},
  {0xf682, "XDRWLP2"},
  {0xf68c, "XDRWLP3"},
  {0xf699, "XDRW4"},
  {0xf6a4, "XDRW5"},
  {0xf6b4, "XDRW6"},
  {0xf6b9, "HFNS"},
  {0xf6cd, "HFNS1"},
  {0xf6e6, "GGERR"},
  {0xf6e9, "HCOLOR"},
  {0xf6f5, "BAS_RTS28"},
  {0xf6f6, "COLORTBL"},
  {0xf6fe, "HPLOT"},
  {0xf708, "HPLOTLOOP"},
  {0xf70f, "HPLOT2"},
  {0xf721, "ROT"},
  {0xf727, "SCALE"},
  {0xf72d, "DRWPTR"},
  {0xf741, "DRWPTR1"},
  {0xf747, "DRWPTR2"},
  {0xf766, "DRWPTR3"},
  {0xf769, "DRAW"},
  {0xf76f, "XDRAW"},
  {0xf775, "SHLOAD"},
  {0xf796, "SHLOAD1"},
  {0xf7a0, "SHLOAD2"},
  {0xf7a3, "SHLOAD3"},
  {0xf7bc, "TAPEPTR"},
  {0xf7d9, "GETARYPTR"},
  {0xf7e7, "HTAB"},
  {0xf7ec, "HTABLOOP"},
  {0xf7fa, "HTABEXIT"},
  {0xf7fd, "KRW"},
  {0xf800, "MON_PLOT"},
  {0xf80c, "RTMASK"},
  {0xf80e, "PLOT1"},
  {0xf819, "HLINE"},
  {0xf81c, "HLINE1"},
  {0xf826, "VLINEZ"},
  {0xf828, "VLINE"},
  {0xf82d, "VLINE2"},
  {0xf831, "MON_RTS1"},
  {0xf832, "CLRSCR"},
  {0xf836, "CLRTOP"},
  {0xf838, "CLRSCR2"},
  {0xf83a, "CLRSCR4"},
  {0xf83c, "CLRSCR3"},
  {0xf840, "CLRSCR5"},
  {0xf847, "GBASCALC"},
  {0xf856, "GBCALC"},
  {0xf85f, "NEXTCOLOR"},
  {0xf864, "SETCOL"},
  {0xf868, "NIBLOHI"},
  {0xf871, "SCRN"},
  {0xf879, "SCRN2"},
  {0xf87b, "NIBHILO"},
  {0xf87f, "RTMSKZ"},
  {0xf882, "INSDS1"},
  {0xf88e, "INSDS2"},
  {0xf890, "GET816LEN"},
  {0xf89b, "IEVEN"},
  {0xf8a5, "ERR"},
  {0xf8a9, "GETFMT"},
  {0xf8be, "MNNDX1"},
  {0xf8c2, "MNNDX2"},
  {0xf8c9, "MNNDX3"},
  {0xf8d0, "INSTDSP"},
  {0xf8d4, "PRNTOP"},
  {0xf8db, "PRNTBL"},
  {0xf8f5, "NXTCOL"},
  {0xf8f9, "PRMN2"},
  {0xf910, "PRADR1"},
  {0xf914, "PRADR2"},
  {0xf926, "PRADR3"},
  {0xf92a, "PRADR4"},
  {0xf930, "PRADR5"},
  {0xf938, "RELADR"},
  {0xf940, "PRNTYX"},
  {0xf941, "PRNTAX"},
  {0xf944, "PRNTX"},
  {0xf948, "PRBLNK"},
  {0xf94a, "PRBL2"},
  {0xf94c, "PRBL3"},
  {0xf953, "PCADJ"},
  {0xf954, "PCADJ2"},
  {0xf956, "PCADJ3"},
  {0xf95c, "PCADJ4"},
  {0xf961, "MON_RTS2"},
  {0xf962, "FMT1"},
  {0xf9a6, "FMT2"},
  {0xf9b4, "CHAR1"},
  {0xf9ba, "CHAR2"},
  {0xf9c0, "MNEML"},
  {0xfa00, "MNEMR"},
  {0xfa40, "IRQ"},
  {0xfa48, "XLTBASE"},
  {0xfa4c, "BREAK"},
  {0xfa59, "OLDBRK"},
  {0xfa62, "RESET"},
  {0xfa6f, "INITAN"},
  {0xfa81, "NEWMON"},
  {0xfa9b, "FIXSEV"},
  {0xfaa3, "NOFIX"},
  {0xfaa6, "PWRUP"},
  {0xfaa9, "SETPG3"},
  {0xfaab, "SETPLP"},
  {0xfaba, "SLOOP"},
  {0xfac7, "NXTBYT"},
  {0xfad5, "MON_NOP1"},
  {0xfad6, "MON_NOP2"},
  {0xfad7, "REGDSP"},
  {0xfada, "RGDSP1"},
  {0xfae4, "RDSP1"},
  {0xfafd, "PWRCON"},
  {0xfb02, "DISKID"},
  {0xfb09, "TITLE"},
  {0xfb11, "XLTBL"},
  {0xfb19, "RTBL"},
  {0xfb1e, "PREAD"},
  {0xfb21, "PREAD4"},
  {0xfb25, "PREAD2"},
  {0xfb2e, "MON_RTS2D"},
  {0xfb2f, "INIT"},
  {0xfb33, "TEXT01"},
  {0xfb36, "TEXT02"},
  {0xfb39, "SETTXT"},
  {0xfb40, "SETGR"},
  {0xfb4b, "SETWND"},
  {0xfb51, "SETWND2"},
  {0xfb5b, "TABV"},
  {0xfb60, "APPLEII"},
  {0xfb65, "STITLE"},
  {0xfb6f, "SETPWRC"},
  {0xfb78, "VIDWAIT"},
  {0xfb88, "KBDWAIT"},
  {0xfb94, "NOWAIT"},
  {0xfb97, "ESCOLD"},
  {0xfb9b, "ESCNOW"},
  {0xfba5, "ESCNEW"},
  {0xfbb3, "VERSION"},
  {0xfbbf, "ZIDBYTE2"},
  {0xfbc0, "ZIDBYTE"},
  {0xfbc1, "BASCALC"},
  {0xfbd0, "BASCALC2"},
  {0xfbd9, "BELL1"},
  {0xfbdd, "BELLA"},
  {0xfbe2, "BELLB"},
  {0xfbe4, "BELL2"},
  {0xfbef, "MON_RTS2B"},
  {0xfbf0, "STORADV"},
  {0xfbf4, "ADVANCE"},
  {0xfbfc, "MON_RTS3"},
  {0xfbfd, "VIDOUT"},
  {0xfc10, "BACKSPACE"},
  {0xfc1a, "CURSORUP"},
  {0xfc22, "MON_VTAB"},
  {0xfc24, "VTABZ"},
  {0xfc2b, "MON_RTS4"},
  {0xfc2c, "ESC1"},
  {0xfc42, "CLREOP"},
  {0xfc46, "CLEOP1"},
  {0xfc58, "HOME"},
  {0xfc62, "CARRETURN"},
  {0xfc66, "LINEFEED"},
  {0xfc70, "SCROLL"},
  {0xfc76, "SCRL1"},
  {0xfc8c, "SCRL2"},
  {0xfc95, "SCRL3"},
  {0xfc9c, "CLREOL"},
  {0xfc9e, "CLREOLZ"},
  {0xfca0, "CLEOL2"},
  {0xfca8, "MON_WAIT"},
  {0xfca9, "MON_WAIT2"},
  {0xfcaa, "MON_WAIT3"},
  {0xfcb4, "NXTA4"},
  {0xfcba, "NXTA1"},
  {0xfcc8, "MON_RTS4B"},
  {0xfcc9, "HEADER"},
  {0xfcd6, "WRBIT"},
  {0xfcdb, "ZERODLY"},
  {0xfce2, "ONEDLY"},
  {0xfce5, "WRTAPE"},
  {0xfcec, "RDBYTE"},
  {0xfcee, "RDBYT2"},
  {0xfcfa, "RD2BIT"},
  {0xfcfd, "RDBIT"},
  {0xfd0c, "RDKEY"},
  {0xfd10, "RDKEY0"},
  {0xfd18, "RDKEY1"},
  {0xfd1b, "KEYIN"},
  {0xfd21, "KEYIN2"},
  {0xfd2f, "ESC"},
  {0xfd35, "RDCHAR"},
  {0xfd3d, "NOTCR"},
  {0xfd4d, "NOTCR5"},
  {0xfd58, "NOTCR4"},
  {0xfd5a, "NOTCR3"},
  {0xfd5c, "NOTCR2"},
  {0xfd5f, "NOTCR1"},
  {0xfd62, "CANCEL"},
  {0xfd67, "GETLNZ"},
  {0xfd6a, "GETLN"},
  {0xfd6c, "GETLN0"},
  {0xfd6f, "GETLN1"},
  {0xfd71, "BCKSPC"},
  {0xfd75, "NXTCHAR"},
  {0xfd7e, "CAPTST"},
  {0xfd84, "ADDINP"},
  {0xfd8b, "CROUT1"},
  {0xfd8e, "CROUT"},
  {0xfd92, "PRA1"},
  {0xfd96, "PRYX2"},
  {0xfd99, "PRYX3"},
  {0xfda3, "XAM8"},
  {0xfdad, "MOD8CHK"},
  {0xfdb3, "XAM"},
  {0xfdb6, "DATAOUT"},
  {0xfdc6, "XAMPM"},
  {0xfdd1, "ADD"},
  {0xfdda, "PRBYTE"},
  {0xfde3, "PRHEX"},
  {0xfde5, "PRHEXZ"},
  {0xfded, "COUT"},
  {0xfdf0, "COUT1"},
  {0xfdf6, "COUTZ"},
  {0xfdfc, "COUTA"},
  {0xfe00, "BL1"},
  {0xfe04, "BLANK"},
  {0xfe17, "MON_RTS5"},
  {0xfe18, "SETMODE"},
  {0xfe1d, "SETMDZ"},
  {0xfe1f, "IDROUTINE"},
  {0xfe20, "LT"},
  {0xfe22, "LT2"},
  {0xfe2c, "MOVE"},
  {0xfe36, "VFY"},
  {0xfe58, "VFYOK"},
  {0xfe5e, "MON_LIST"},
  {0xfe63, "MON_LIST2"},
  {0xfe75, "A1PC"},
  {0xfe78, "A1PCLP"},
  {0xfe7f, "A1PCRTS"},
  {0xfe80, "SETINV"},
  {0xfe84, "SETNORM"},
  {0xfe86, "SETIFLG"},
  {0xfe89, "SETKBD"},
  {0xfe8b, "INPORT"},
  {0xfe8d, "MON_INPRT"},
  {0xfe93, "SETVID"},
  {0xfe95, "OUTPORT"},
  {0xfe97, "OUTPRT"},
  {0xfe9b, "IOPRT"},
  {0xfea7, "IOPRT1"},
  {0xfea9, "IOPRT2"},
  {0xfeae, "MON_NOP3"},
  {0xfeaf, "MON_NOP4"},
  {0xfeb0, "XBASIC"},
  {0xfeb3, "BASCONT"},
  {0xfeb6, "GO"},
  {0xfeb9, "GO1"},
  {0xfebc, "GO2"},
  {0xfebf, "REGZ"},
  {0xfec2, "TRACE_RTS"},
  {0xfec4, "STEPZ_RTS"},
  {0xfec5, "STEPZ_USR"},
  {0xfeca, "USR"},
  {0xfecd, "MON_WRITE"},
  {0xfed4, "WR1"},
  {0xfeed, "WRBYTE"},
  {0xfeef, "WRBYT2"},
  {0xfef6, "CRMON"},
  {0xfefd, "MON_READ"},
  {0xff02, "MON_READ2"},
  {0xff0a, "MON_RD2"},
  {0xff16, "MON_RD3"},
  {0xff2d, "MON_RDERR"},
  {0xff3a, "BELL"},
  {0xff3f, "MON_RESTORE"},
  {0xff4a, "MON_SAVE"},
  {0xff4c, "SAV1"},
  {0xff58, "IORTS"},
  {0xff59, "OLDRST"},
  {0xff65, "MON"},
  {0xff69, "MONZ"},
  {0xff70, "SCANIDX"},
  {0xff73, "NXTITM"},
  {0xff7a, "CHRSRCH"},
  {0xff8a, "DIGIT"},
  {0xff90, "NXTBIT"},
  {0xff98, "NXTBAS"},
  {0xffa2, "NXTBS2"},
  {0xffa7, "GETNUM"},
  {0xffad, "NXTCHR"},
  {0xffbe, "TOSUB"},
  {0xffc7, "ZMODE"},
  {0xffcc, "CHRTBL"},
  {0xffe3, "SUBTBL"},
};

const char *findApple2Symbol(unsigned offset) {
  static_assert(sizeof(uintptr_t) >= sizeof(unsigned), "unsigned must fit in const void*");

  const auto *elem = (const SymDef *)bsearch(
      (const void *)(uintptr_t)offset,
      s_symbols,
      sizeof(s_symbols) / sizeof(s_symbols[0]),
      sizeof(s_symbols[0]),
      [](const void *_key, const void *_elem) {
        auto key = (unsigned)(uintptr_t)_key;
        const auto *elem = (const SymDef *)_elem;
        return key < elem->addr ? -1 : key == elem->addr ? 0 : 1;
      });

  return elem ? elem->name : nullptr;
}

std::string apple2SymbolResolver(CPUInst inst) {
  if (cpuAddrModeHasOperand(inst.addrMode) && inst.addrMode != CPUAddrMode::Imm) {
    if (const char *name = findApple2Symbol(inst.operand))
      return name;
  }
  return {};
}
