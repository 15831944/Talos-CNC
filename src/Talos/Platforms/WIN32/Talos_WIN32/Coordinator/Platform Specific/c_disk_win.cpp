/*
* c_core_avr_2560.cpp
*
* Created: 2/27/2019 3:47:52 PM
* Author: jeff_d
*/


#include "c_disk_win.h"
#include "../../../../../NGC_RS274/NGC_G_Groups.h"
#include "../../../../../NGC_RS274/NGC_M_Groups.h"
#include<iostream>
#include<fstream>
using namespace std;

uint8_t Hardware_Abstraction_Layer::Disk::initialize()
{
	return 0;
}

uint8_t Hardware_Abstraction_Layer::Disk::load_configuration()
{
	return 1;

}

uint8_t Hardware_Abstraction_Layer::Disk::load_initialize_block(BinaryRecords::s_ngc_block * initial_block)
{
	//default the motion state to canceled
	initial_block->g_group[NGC_RS274::Groups::G::Motion] = NGC_RS274::G_codes::MOTION_CANCELED;
	//default plane selection
	initial_block->g_group[NGC_RS274::Groups::G::PLANE_SELECTION] = NGC_RS274::G_codes::XY_PLANE_SELECTION;
	//default the machines distance mode to absolute
	initial_block->g_group[NGC_RS274::Groups::G::DISTANCE_MODE] = NGC_RS274::G_codes::ABSOLUTE_DISANCE_MODE;
	//default feed rate mode
	initial_block->g_group[NGC_RS274::Groups::G::Feed_rate_mode] = NGC_RS274::G_codes::FEED_RATE_UNITS_PER_MINUTE_MODE;
	//default the machines units to inches
	initial_block->g_group[NGC_RS274::Groups::G::Units] = NGC_RS274::G_codes::MILLIMETER_SYSTEM_SELECTION;
	//default the machines cutter comp to off
	initial_block->g_group[NGC_RS274::Groups::G::Cutter_radius_compensation] = NGC_RS274::G_codes::CANCEL_CUTTER_RADIUS_COMPENSATION;
	//default tool length offset
	initial_block->g_group[NGC_RS274::Groups::G::Tool_length_offset] = NGC_RS274::G_codes::CANCEL_TOOL_LENGTH_OFFSET;
	//default tool length offset
	initial_block->g_group[NGC_RS274::Groups::G::RETURN_MODE_CANNED_CYCLE] = NGC_RS274::G_codes::CANNED_CYCLE_RETURN_TO_Z;
	//default coordinate system selection
	initial_block->g_group[NGC_RS274::Groups::G::COORDINATE_SYSTEM_SELECTION] = NGC_RS274::G_codes::MOTION_IN_MACHINE_COORDINATE_SYSTEM;
	//default path control mode
	initial_block->g_group[NGC_RS274::Groups::G::PATH_CONTROL_MODE] = NGC_RS274::G_codes::PATH_CONTROL_EXACT_PATH;
	//default coordinate system type
	initial_block->g_group[NGC_RS274::Groups::G::RECTANGLAR_POLAR_COORDS_SELECTION] = NGC_RS274::G_codes::RECTANGULAR_COORDINATE_SYSTEM;
	//default canned cycle return mode
	initial_block->g_group[NGC_RS274::Groups::G::RETURN_MODE_CANNED_CYCLE] = NGC_RS274::G_codes::CANNED_CYCLE_RETURN_TO_R;
	//default spindle mode
	initial_block->m_group[NGC_RS274::Groups::M::SPINDLE] = NGC_RS274::M_codes::SPINDLE_STOP;
	//default coolant mode
	initial_block->m_group[NGC_RS274::Groups::M::COOLANT] = NGC_RS274::M_codes::COOLANT_OFF;
	return 0;
}

uint8_t Hardware_Abstraction_Layer::Disk::put_block(BinaryRecords::s_ngc_block * write_block)
{
	char stream[sizeof(BinaryRecords::s_ngc_block)];
	memcpy(stream, write_block, sizeof(BinaryRecords::s_ngc_block));
	write("test.dat", stream, e_file_modes::OpenCreate, sizeof(BinaryRecords::s_ngc_block));
	return 1;
}

uint8_t Hardware_Abstraction_Layer::Disk::get_block(BinaryRecords::s_ngc_block * read_block)
{
	char stream[sizeof(BinaryRecords::s_ngc_block)];
	
	read("test.dat", stream, e_file_modes::OpenCreate, sizeof(BinaryRecords::s_ngc_block));

	memcpy(read_block, stream, sizeof(BinaryRecords::s_ngc_block));
	
	return 1;
}


uint8_t Hardware_Abstraction_Layer::Disk::write(const char * filename, char * buffer, e_file_modes mode, uint16_t size)
{
	ofstream wf(filename, ios::out | ios::binary);
	if (!wf) {
		cout << "Cannot open file!" << endl;
		return 1;
	}

	wf.write(buffer, size);
	wf.close();
	if (!wf.good()) {
		cout << "Error occurred at writing time!" << endl;
		return 1;
	}
}

uint8_t Hardware_Abstraction_Layer::Disk::read(const char * filename, char * buffer, e_file_modes mode, uint16_t size)
{

	ifstream rf(filename, ios::out | ios::binary);
	if (!rf) {
		cout << "Cannot open file!" << endl;
		return 1;
	}
	rf.read(buffer, size);
	rf.close();

	
}

/*
------------------------------------------------SAM3X8------------------------------

///
//
//
// Here are several macros which should be used when configuring a SPI
// peripheral.
//
// \section spi_configuration_macros SPI Configuration Macros
// - \ref SPI_PCS
// - \ref SPI_SCBR
// - \ref SPI_DLYBS
// - \ref SPI_DLYBCT
///

// Calculate the PCS field value given the chip select NPCS value 
#define SPI_PCS(npcs)       ((~(1 << (npcs)) & 0xF) << 16)

// Calculates the value of the CSR SCBR field given the baudrate and MCK.
#define SPI_SCBR(baudrate, masterClock) ((uint32_t) ((masterClock) / (baudrate)) << 8)

// Calculates the value of the CSR DLYBS field given the desired delay (in ns) 
#define SPI_DLYBS(delay, masterClock) ((uint32_t) ((((masterClock) / 1000000) * (delay)) / 1000) << 16)

// Calculates the value of the CSR DLYBCT field given the desired delay (in ns)
#define SPI_DLYBCT(delay, masterClock) ((uint32_t) ((((masterClock) / 1000000) * (delay)) / 32000) << 24)



void SPIClass::init() {
if (initialized)
return;
interruptMode = 0;
interruptSave = 0;
interruptMask[0] = 0;
interruptMask[1] = 0;
interruptMask[2] = 0;
interruptMask[3] = 0;
initCb();
SPI_Configure(spi, id, SPI_MR_MSTR | SPI_MR_PS | SPI_MR_MODFDIS);
SPI_Enable(spi);
initialized = true;
}

extern void SPI_Configure(Spi* spi, uint32_t dwId, uint32_t dwConfiguration)
{
	pmc_enable_periph_clk(dwId);
	spi->SPI_CR = SPI_CR_SPIDIS;

	// Execute a software reset of the SPI twice
	spi->SPI_CR = SPI_CR_SWRST;
	spi->SPI_CR = SPI_CR_SWRST;
	spi->SPI_MR = dwConfiguration;
}
extern void SPI_Enable( Spi* spi )
{
spi->SPI_CR = SPI_CR_SPIEN ;
}

------------------------------------------------AVR-----------------------------
uint8_t Sd2Card::init(uint8_t sckRateID, uint8_t chipSelectPin) {
errorCode_ = inBlock_ = partialBlockRead_ = type_ = 0;
chipSelectPin_ = chipSelectPin;
// 16-bit init start time allows over a minute
uint16_t t0 = (uint16_t)millis();
uint32_t arg;

// set pin modes
pinMode(chipSelectPin_, OUTPUT); //set CS to ouput
digitalWrite(chipSelectPin_, HIGH); //make cs high
#ifndef USE_SPI_LIB
pinMode(SPI_MISO_PIN, INPUT); //make MISO input
pinMode(SPI_MOSI_PIN, OUTPUT); //make MOST output
pinMode(SPI_SCK_PIN, OUTPUT); //make SCK output
#endif

#ifndef SOFTWARE_SPI
#ifndef USE_SPI_LIB
// SS must be in output mode even it is not chip select
pinMode(SS_PIN, OUTPUT);
digitalWrite(SS_PIN, HIGH); // disable any SPI device using hardware SS pin
// Enable SPI, Master, clock rate f_osc/128
SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
// clear double speed
SPSR &= ~(1 << SPI2X);
#else // USE_SPI_LIB
SDCARD_SPI.begin();
settings = SPISettings(250000, MSBFIRST, SPI_MODE0);
#endif // USE_SPI_LIB
#endif // SOFTWARE_SPI

// must supply min of 74 clock cycles with CS high.
#ifdef USE_SPI_LIB
SDCARD_SPI.beginTransaction(settings);
#endif
for (uint8_t i = 0; i < 10; i++) spiSend(0XFF);
#ifdef USE_SPI_LIB
SDCARD_SPI.endTransaction();
#endif

chipSelectLow();

// command to go idle in SPI mode
while ((status_ = cardCommand(CMD0, 0)) != R1_IDLE_STATE) {
if (((uint16_t)(millis() - t0)) > SD_INIT_TIMEOUT) {
error(SD_CARD_ERROR_CMD0);
goto fail;
}
}
// check SD version
if ((cardCommand(CMD8, 0x1AA) & R1_ILLEGAL_COMMAND)) {
type(SD_CARD_TYPE_SD1);
} else {
// only need last byte of r7 response
for (uint8_t i = 0; i < 4; i++) status_ = spiRec();
if (status_ != 0XAA) {
error(SD_CARD_ERROR_CMD8);
goto fail;
}
type(SD_CARD_TYPE_SD2);
}
// initialize card and send host supports SDHC if SD2
arg = type() == SD_CARD_TYPE_SD2 ? 0X40000000 : 0;

while ((status_ = cardAcmd(ACMD41, arg)) != R1_READY_STATE) {
// check for timeout
if (((uint16_t)(millis() - t0)) > SD_INIT_TIMEOUT) {
error(SD_CARD_ERROR_ACMD41);
goto fail;
}
}
// if SD2 read OCR register to check for SDHC card
if (type() == SD_CARD_TYPE_SD2) {
if (cardCommand(CMD58, 0)) {
error(SD_CARD_ERROR_CMD58);
goto fail;
}
if ((spiRec() & 0XC0) == 0XC0) type(SD_CARD_TYPE_SDHC);
// discard rest of ocr - contains allowed voltage range
for (uint8_t i = 0; i < 3; i++) spiRec();
}
chipSelectHigh();

#ifndef SOFTWARE_SPI
return setSckRate(sckRateID);
#else  // SOFTWARE_SPI
return true;
#endif  // SOFTWARE_SPI

fail:
chipSelectHigh();
return false;
}


*/