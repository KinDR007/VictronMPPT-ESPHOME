#include "victron.h"
#include "esphome/core/log.h"
#include <algorithm>  // std::min
#include "esphome/core/helpers.h"
#include <Arduino.h>

namespace esphome {
namespace victron {

static const char *const TAG = "victron";

static const uint8_t OFF_REASONS_SIZE = 16;
static const char OFF_REASONS_0[] PROGMEM = "No input power";
static const char OFF_REASONS_1[] PROGMEM = "Switched off (power switch)";
static const char OFF_REASONS_2[] PROGMEM = "Switched off (device mode register)";
static const char OFF_REASONS_3[] PROGMEM = "Remote input";
static const char OFF_REASONS_4[] PROGMEM = "Protection active";
static const char OFF_REASONS_5[] PROGMEM = "Paygo";
static const char OFF_REASONS_6[] PROGMEM = "BMS";
static const char OFF_REASONS_7[] PROGMEM = "Engine shutdown detection";
static const char OFF_REASONS_8[] PROGMEM = "Analysing input voltage";
static const char OFF_REASONS_9[] PROGMEM = "Unknown: Bit 10";
static const char OFF_REASONS_10[] PROGMEM = "Unknown: Bit 11";
static const char OFF_REASONS_11[] PROGMEM = "Unknown: Bit 12";
static const char OFF_REASONS_12[] PROGMEM = "Unknown: Bit 13";
static const char OFF_REASONS_13[] PROGMEM = "Unknown: Bit 14";
static const char OFF_REASONS_14[] PROGMEM = "Unknown: Bit 15";
static const char OFF_REASONS_15[] PROGMEM = "Unknown: Bit 16";
static const char *const OFF_REASONS[OFF_REASONS_SIZE] PROGMEM = {
    OFF_REASONS_0,
    OFF_REASONS_1,
    OFF_REASONS_2,
    OFF_REASONS_3,
    OFF_REASONS_4,
    OFF_REASONS_5,
    OFF_REASONS_6,
    OFF_REASONS_7,
    OFF_REASONS_8,
    OFF_REASONS_9,
    OFF_REASONS_10,
    OFF_REASONS_11,
    OFF_REASONS_12,
    OFF_REASONS_13,
    OFF_REASONS_14,
    OFF_REASONS_15
};

static char buffer_off_reason[36];

static const char ERROR_CODE_0[] PROGMEM = "No error";
static const char ERROR_CODE_2[] PROGMEM = "Battery voltage too high";
static const char ERROR_CODE_17[] PROGMEM = "Charger temperature too high";
static const char ERROR_CODE_18[] PROGMEM = "Charger over current";
static const char ERROR_CODE_19[] PROGMEM = "Charger current reversed";
static const char ERROR_CODE_20[] PROGMEM = "Bulk time limit exceeded";
static const char ERROR_CODE_21[] PROGMEM = "Current sensor issue";
static const char ERROR_CODE_26[] PROGMEM = "Terminals overheated";
static const char ERROR_CODE_28[] PROGMEM = "Converter issue";
static const char ERROR_CODE_33[] PROGMEM = "Input voltage too high (solar panel)";
static const char ERROR_CODE_34[] PROGMEM = "Input current too high (solar panel)";
static const char ERROR_CODE_38[] PROGMEM = "Input shutdown (excessive battery voltage)";
static const char ERROR_CODE_39[] PROGMEM = "Input shutdown (due to current flow during off mode)";
static const char ERROR_CODE_65[] PROGMEM = "Lost communication with one of devices";
static const char ERROR_CODE_66[] PROGMEM = "Synchronised charging device configuration issue";
static const char ERROR_CODE_67[] PROGMEM = "BMS connection lost";
static const char ERROR_CODE_68[] PROGMEM = "Network misconfigured";
static const char ERROR_CODE_116[] PROGMEM = "Factory calibration data lost";
static const char ERROR_CODE_117[] PROGMEM = "Invalid/incompatible firmware";
static const char ERROR_CODE_119[] PROGMEM = "User settings invalid";
static const char ERROR_CODE_UNKNOWN[] PROGMEM = "Unknown";

static char buffer_error_code[53];

struct ErrorCodeEntry {
  int code;
  const char *msg;
};

static const char CHARGING_MODE_0[] PROGMEM = "Off";
static const char CHARGING_MODE_1[] PROGMEM = "Low power";
static const char CHARGING_MODE_2[] PROGMEM = "Fault";
static const char CHARGING_MODE_3[] PROGMEM = "Bulk";
static const char CHARGING_MODE_4[] PROGMEM = "Absorption";
static const char CHARGING_MODE_5[] PROGMEM = "Float";
static const char CHARGING_MODE_6[] PROGMEM = "Storage";
static const char CHARGING_MODE_7[] PROGMEM = "Equalize (manual)";
static const char CHARGING_MODE_9[] PROGMEM = "Inverting";
static const char CHARGING_MODE_11[] PROGMEM = "Power supply";
static const char CHARGING_MODE_245[] PROGMEM = "Starting-up";
static const char CHARGING_MODE_246[] PROGMEM = "Repeated absorption";
static const char CHARGING_MODE_247[] PROGMEM = "Auto equalize / Recondition";
static const char CHARGING_MODE_248[] PROGMEM = "BatterySafe";
static const char CHARGING_MODE_252[] PROGMEM = "External control";
static const char CHARGING_MODE_UNKNOWN[] PROGMEM = "Unknown";

static char buffer_charging_mode[30];

// Device type strings in PROGMEM
static const char DEVICE_TYPE_BMV_700[] PROGMEM = "BMV-700";
static const char DEVICE_TYPE_BMV_702[] PROGMEM = "BMV-702";
static const char DEVICE_TYPE_BMV_700H[] PROGMEM = "BMV-700H";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_70_15[] PROGMEM = "BlueSolar MPPT 70|15";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_75_50[] PROGMEM = "BlueSolar MPPT 75|50";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_150_35[] PROGMEM = "BlueSolar MPPT 150|35";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_75_15[] PROGMEM = "BlueSolar MPPT 75|15";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_100_15[] PROGMEM = "BlueSolar MPPT 100|15";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_100_30[] PROGMEM = "BlueSolar MPPT 100|30";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_100_50[] PROGMEM = "BlueSolar MPPT 100|50";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_150_70[] PROGMEM = "BlueSolar MPPT 150|70";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_150_100[] PROGMEM = "BlueSolar MPPT 150|100";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_100_50_REV2[] PROGMEM = "BlueSolar MPPT 100|50 rev2";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_100_30_REV2[] PROGMEM = "BlueSolar MPPT 100|30 rev2";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_150_35_REV2[] PROGMEM = "BlueSolar MPPT 150|35 rev2";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_75_10[] PROGMEM = "BlueSolar MPPT 75|10";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_150_45[] PROGMEM = "BlueSolar MPPT 150|45";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_150_60[] PROGMEM = "BlueSolar MPPT 150|60";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_150_85[] PROGMEM = "BlueSolar MPPT 150|85";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_250_100[] PROGMEM = "SmartSolar MPPT 250|100";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_150_100[] PROGMEM = "SmartSolar MPPT 150|100";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_150_85[] PROGMEM = "SmartSolar MPPT 150|85";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_75_15[] PROGMEM = "SmartSolar MPPT 75|15";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_75_15_REV2[] PROGMEM = "SmartSolar MPPT 75|15 rev2";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_75_10[] PROGMEM = "SmartSolar MPPT 75|10";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_75_10_REV2[] PROGMEM = "SmartSolar MPPT 75|10 rev2";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_100_15[] PROGMEM = "SmartSolar MPPT 100|15";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_100_30[] PROGMEM = "SmartSolar MPPT 100|30";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_150_45_REV3[] PROGMEM = "SmartSolar MPPT 150|45 rev3";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_100_50[] PROGMEM = "SmartSolar MPPT 100|50";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_150_35[] PROGMEM = "SmartSolar MPPT 150|35";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_150_100_REV2[] PROGMEM = "SmartSolar MPPT 150|100 rev2";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_150_85_REV2[] PROGMEM = "SmartSolar MPPT 150|85 rev2";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_250_70[] PROGMEM = "SmartSolar MPPT 250|70";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_250_85[] PROGMEM = "SmartSolar MPPT 250|85";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_250_60[] PROGMEM = "SmartSolar MPPT 250|60";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_250_45[] PROGMEM = "SmartSolar MPPT 250|45";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_100_20[] PROGMEM = "SmartSolar MPPT 100|20";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_100_20_48V[] PROGMEM = "SmartSolar MPPT 100|20 48V";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_150_45[] PROGMEM = "SmartSolar MPPT 150|45";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_150_60[] PROGMEM = "SmartSolar MPPT 150|60";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_150_70[] PROGMEM = "SmartSolar MPPT 150|70";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_250_85_REV2[] PROGMEM = "SmartSolar MPPT 250|85 rev2";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_250_100_REV2[] PROGMEM = "SmartSolar MPPT 250|100 rev2";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_100_20[] PROGMEM = "BlueSolar MPPT 100|20";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_100_20_48V[] PROGMEM = "BlueSolar MPPT 100|20 48V";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_250_60_REV2[] PROGMEM = "SmartSolar MPPT 250|60 rev2";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_250_70_REV2[] PROGMEM = "SmartSolar MPPT 250|70 rev2";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_150_45_REV2[] PROGMEM = "SmartSolar MPPT 150|45 rev2";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_150_60_REV2[] PROGMEM = "SmartSolar MPPT 150|60 rev2";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_150_70_REV2[] PROGMEM = "SmartSolar MPPT 150|70 rev2";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_150_85_REV3[] PROGMEM = "SmartSolar MPPT 150|85 rev3";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_150_100_REV3[] PROGMEM = "SmartSolar MPPT 150|100 rev3";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_150_45_REV2[] PROGMEM = "BlueSolar MPPT 150|45 rev2";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_150_60_REV2[] PROGMEM = "BlueSolar MPPT 150|60 rev2";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_150_70_REV2[] PROGMEM = "BlueSolar MPPT 150|70 rev2";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_75_15_REV3[] PROGMEM = "BlueSolar MPPT 75|15 rev3";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_150_70[] PROGMEM = "SmartSolar MPPT VE.Can 150/70";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_150_45[] PROGMEM = "SmartSolar MPPT VE.Can 150/45";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_150_60[] PROGMEM = "SmartSolar MPPT VE.Can 150/60";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_150_85[] PROGMEM = "SmartSolar MPPT VE.Can 150/85";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_150_100[] PROGMEM = "SmartSolar MPPT VE.Can 150/100";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_250_45[] PROGMEM = "SmartSolar MPPT VE.Can 250/45";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_250_60[] PROGMEM = "SmartSolar MPPT VE.Can 250/60";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_250_70[] PROGMEM = "SmartSolar MPPT VE.Can 250/70";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_250_85[] PROGMEM = "SmartSolar MPPT VE.Can 250/85";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_250_100[] PROGMEM = "SmartSolar MPPT VE.Can 250/100";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_150_70_REV2[] PROGMEM = "SmartSolar MPPT VE.Can 150/70 rev2";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_150_85_REV2[] PROGMEM = "SmartSolar MPPT VE.Can 150/85 rev2";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_150_100_REV2[] PROGMEM = "SmartSolar MPPT VE.Can 150/100 rev2";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_VE_CAN_150_100[] PROGMEM = "BlueSolar MPPT VE.Can 150/100";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_VE_CAN_250_70[] PROGMEM = "BlueSolar MPPT VE.Can 250/70";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_VE_CAN_250_100[] PROGMEM = "BlueSolar MPPT VE.Can 250/100";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_250_70_REV2[] PROGMEM = "SmartSolar MPPT VE.Can 250/70 rev2";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_250_100_REV2[] PROGMEM = "SmartSolar MPPT VE.Can 250/100 rev2";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_250_85_REV2[] PROGMEM = "SmartSolar MPPT VE.Can 250/85 rev2";
static const char DEVICE_TYPE_PHOENIX_INVERTER_12V_250VA_230V[] PROGMEM = "Phoenix Inverter 12V 250VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_24V_250VA_230V[] PROGMEM = "Phoenix Inverter 24V 250VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_48V_250VA_230V[] PROGMEM = "Phoenix Inverter 48V 250VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_12V_375VA_230V[] PROGMEM = "Phoenix Inverter 12V 375VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_24V_375VA_230V[] PROGMEM = "Phoenix Inverter 24V 375VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_48V_375VA_230V[] PROGMEM = "Phoenix Inverter 48V 375VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_12V_500VA_230V[] PROGMEM = "Phoenix Inverter 12V 500VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_24V_500VA_230V[] PROGMEM = "Phoenix Inverter 24V 500VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_48V_500VA_230V[] PROGMEM = "Phoenix Inverter 48V 500VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_12V_250VA_120V[] PROGMEM = "Phoenix Inverter 12V 250VA 120V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_24V_250VA_120V[] PROGMEM = "Phoenix Inverter 24V 250VA 120V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_48V_250VA_120V[] PROGMEM = "Phoenix Inverter 48V 250VA 120V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_12V_375VA_120V[] PROGMEM = "Phoenix Inverter 12V 375VA 120V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_24V_375VA_120V[] PROGMEM = "Phoenix Inverter 24V 375VA 120V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_48V_375VA_120V[] PROGMEM = "Phoenix Inverter 48V 375VA 120V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_12V_500VA_120V[] PROGMEM = "Phoenix Inverter 12V 500VA 120V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_24V_500VA_120V[] PROGMEM = "Phoenix Inverter 24V 500VA 120V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_48V_500VA_120V[] PROGMEM = "Phoenix Inverter 48V 500VA 120V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_12V_800VA_230V[] PROGMEM = "Phoenix Inverter 12V 800VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_24V_800VA_230V[] PROGMEM = "Phoenix Inverter 24V 800VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_48V_800VA_230V[] PROGMEM = "Phoenix Inverter 48V 800VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_12V_800VA_120V[] PROGMEM = "Phoenix Inverter 12V 800VA 120V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_24V_800VA_120V[] PROGMEM = "Phoenix Inverter 24V 800VA 120V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_48V_800VA_120V[] PROGMEM = "Phoenix Inverter 48V 800VA 120V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_12V_1200VA_230V[] PROGMEM = "Phoenix Inverter 12V 1200VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_24V_1200VA_230V[] PROGMEM = "Phoenix Inverter 24V 1200VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_48V_1200VA_230V[] PROGMEM = "Phoenix Inverter 48V 1200VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_12V_1200VA_120V[] PROGMEM = "Phoenix Inverter 12V 1200VA 120V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_24V_1200VA_120V[] PROGMEM = "Phoenix Inverter 24V 1200VA 120V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_48V_1200VA_120V[] PROGMEM = "Phoenix Inverter 48V 1200VA 120V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_12V_1600VA_230V[] PROGMEM = "Phoenix Inverter 12V 1600VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_24V_1600VA_230V[] PROGMEM = "Phoenix Inverter 24V 1600VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_48V_1600VA_230V[] PROGMEM = "Phoenix Inverter 48V 1600VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_12V_2000VA_230V[] PROGMEM = "Phoenix Inverter 12V 2000VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_24V_2000VA_230V[] PROGMEM = "Phoenix Inverter 24V 2000VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_48V_2000VA_230V[] PROGMEM = "Phoenix Inverter 48V 2000VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_12V_3000VA_230V[] PROGMEM = "Phoenix Inverter 12V 3000VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_24V_3000VA_230V[] PROGMEM = "Phoenix Inverter 24V 3000VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_48V_3000VA_230V[] PROGMEM = "Phoenix Inverter 48V 3000VA 230V";
static const char DEVICE_TYPE_BLUE_SMART_IP65_CHARGER_12_25[] PROGMEM = "Blue Smart IP65 Charger 12|25";
static const char DEVICE_TYPE_BLUE_SMART_IP22_CHARGER_24_8[] PROGMEM = "Blue Smart IP22 Charger 24|8";
static const char DEVICE_TYPE_BLUE_SMART_IP22_CHARGER_24_12[] PROGMEM = "Blue Smart IP22 Charger 24|12";
static const char DEVICE_TYPE_BLUE_SMART_IP22_CHARGER_24_16[] PROGMEM = "Blue Smart IP22 Charger 24|16";
static const char DEVICE_TYPE_PHOENIX_SMART_IP43_CHARGER_12_50_1_1[] PROGMEM = "Phoenix Smart IP43 Charger 12|50 (1+1)";
static const char DEVICE_TYPE_PHOENIX_SMART_IP43_CHARGER_12_50_3[] PROGMEM = "Phoenix Smart IP43 Charger 12|50 (3)";
static const char DEVICE_TYPE_PHOENIX_SMART_IP43_CHARGER_24_25_1_1[] PROGMEM = "Phoenix Smart IP43 Charger 24|25 (1+1)";
static const char DEVICE_TYPE_PHOENIX_SMART_IP43_CHARGER_24_25_3[] PROGMEM = "Phoenix Smart IP43 Charger 24|25 (3)";
static const char DEVICE_TYPE_PHOENIX_SMART_IP43_CHARGER_12_30_1_1[] PROGMEM = "Phoenix Smart IP43 Charger 12|30 (1+1)";
static const char DEVICE_TYPE_PHOENIX_SMART_IP43_CHARGER_12_30_3[] PROGMEM = "Phoenix Smart IP43 Charger 12|30 (3)";
static const char DEVICE_TYPE_PHOENIX_SMART_IP43_CHARGER_24_16_1_1[] PROGMEM = "Phoenix Smart IP43 Charger 24|16 (1+1)";
static const char DEVICE_TYPE_PHOENIX_SMART_IP43_CHARGER_24_16_3[] PROGMEM = "Phoenix Smart IP43 Charger 24|16 (3)";
static const char DEVICE_TYPE_BMV_712_SMART[] PROGMEM = "BMV-712 Smart";
static const char DEVICE_TYPE_BMV_710H_SMART[] PROGMEM = "BMV-710H Smart";
static const char DEVICE_TYPE_BMV_712_SMART_REV2[] PROGMEM = "BMV-712 Smart Rev2";
static const char DEVICE_TYPE_SMARTSHUNT_500A_50MV[] PROGMEM = "SmartShunt 500A/50mV";
static const char DEVICE_TYPE_SMARTSHUNT_1000A_50MV[] PROGMEM = "SmartShunt 1000A/50mV";
static const char DEVICE_TYPE_SMARTSHUNT_2000A_50MV[] PROGMEM = "SmartShunt 2000A/50mV";
static const char DEVICE_TYPE_MULTI_RS_SOLAR_48V_6000VA_230V[] PROGMEM = "Multi RS Solar 48V 6000VA 230V";
static const char DEVICE_TYPE_PHOENIX_INVERTER_SMART_12V_5000VA_230VAC_64K[] PROGMEM = "Phoenix Inverter Smart 12V 5000VA 230Vac 64k";
static const char DEVICE_TYPE_PHOENIX_INVERTER_SMART_24V_5000VA_230VAC_64K[] PROGMEM = "Phoenix Inverter Smart 24V 5000VA 230Vac 64k";
static const char DEVICE_TYPE_PHOENIX_INVERTER_SMART_48V_5000VA_230VAC_64K[] PROGMEM = "Phoenix Inverter Smart 48V 5000VA 230Vac 64k";
static const char DEVICE_TYPE_PHOENIX_INVERTER_12V_800VA_230VAC_64K_HS[] PROGMEM = "Phoenix Inverter 12V 800VA 230Vac 64k HS";
static const char DEVICE_TYPE_PHOENIX_INVERTER_24V_800VA_230VAC_64K_HS[] PROGMEM = "Phoenix Inverter 24V 800VA 230Vac 64k HS";
static const char DEVICE_TYPE_PHOENIX_INVERTER_48V_800VA_230VAC_64K_HS[] PROGMEM = "Phoenix Inverter 48V 800VA 230Vac 64k HS";
static const char DEVICE_TYPE_PHOENIX_INVERTER_12V_800VA_120VAC_64K_HS[] PROGMEM = "Phoenix Inverter 12V 800VA 120Vac 64k HS";
static const char DEVICE_TYPE_PHOENIX_INVERTER_24V_800VA_120VAC_64K_HS[] PROGMEM = "Phoenix Inverter 24V 800VA 120Vac 64k HS";
static const char DEVICE_TYPE_PHOENIX_INVERTER_48V_800VA_120VAC_64K_HS[] PROGMEM = "Phoenix Inverter 48V 800VA 120Vac 64k HS";
static const char DEVICE_TYPE_PHOENIX_INVERTER_12V_1200VA_230VAC_64K_HS[] PROGMEM = "Phoenix Inverter 12V 1200VA 230Vac 64k HS";
static const char DEVICE_TYPE_PHOENIX_INVERTER_24V_1200VA_230VAC_64K_HS[] PROGMEM = "Phoenix Inverter 24V 1200VA 230Vac 64k HS";
static const char DEVICE_TYPE_PHOENIX_INVERTER_48V_1200VA_230VAC_64K_HS[] PROGMEM = "Phoenix Inverter 48V 1200VA 230Vac 64k HS";
static const char DEVICE_TYPE_PHOENIX_INVERTER_24V_1200VA_120VAC_64K_HS[] PROGMEM = "Phoenix Inverter 24V 1200VA 120Vac 64k HS";
static const char DEVICE_TYPE_PHOENIX_INVERTER_48V_1200VA_120VAC_64K_HS[] PROGMEM = "Phoenix Inverter 48V 1200VA 120Vac 64k HS";
static const char DEVICE_TYPE_ORION_XS_12V_12V_50A[] PROGMEM = "Orion XS 12V/12V-50A";
static const char DEVICE_TYPE_ORION_XS_1400[] PROGMEM = "Orion XS 1400";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_75_50_REV2[] PROGMEM = "BlueSolar MPPT 75|50 rev2";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_150_45_REV3[] PROGMEM = "BlueSolar MPPT 150|45 rev3";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_100_30_REV3[] PROGMEM = "BlueSolar MPPT 100|30 rev3";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_100_50_REV3[] PROGMEM = "BlueSolar MPPT 100|50 rev3";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_75_10_REV2[] PROGMEM = "BlueSolar MPPT 75|10 rev2";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_75_15_REV2[] PROGMEM = "BlueSolar MPPT 75|15 rev2";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_100_15_REV2[] PROGMEM = "BlueSolar MPPT 100|15 rev2";
static const char DEVICE_TYPE_BLUESOLAR_MPPT_75_10_REV3[] PROGMEM = "BlueSolar MPPT 75/10 rev3";
static const char DEVICE_TYPE_SMARTSOLAR_CHARGER_MPPT_100_30[] PROGMEM = "SmartSolar Charger MPPT 100/30";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_RS_450_100[] PROGMEM = "SmartSolar MPPT RS 450/100";
static const char DEVICE_TYPE_SMARTSOLAR_MPPT_RS_450_200[] PROGMEM = "SmartSolar MPPT RS 450/200";

static const char DEVICE_TYPE_UNKNOWN[] PROGMEM = "Unknown";

static char buffer_device_type[48];

void VictronComponent::dump_config() {  // NOLINT(google-readability-function-size,readability-function-size)
static const char *prefix = "  ";
  ESP_LOGCONFIG(TAG, "Victron:");
  LOG_BINARY_SENSOR(prefix, "Load state", load_state_binary_sensor_);
  LOG_BINARY_SENSOR(prefix, "Relay state", relay_state_binary_sensor_);
  LOG_SENSOR(prefix, "Max Power Yesterday", max_power_yesterday_sensor_);
  LOG_SENSOR(prefix, "Max Power Today", max_power_today_sensor_);
  LOG_SENSOR(prefix, "Yield Total", yield_total_sensor_);
  LOG_SENSOR(prefix, "Yield Yesterday", yield_yesterday_sensor_);
  LOG_SENSOR(prefix, "Yield Today", yield_today_sensor_);
  LOG_SENSOR(prefix, "Panel Voltage", panel_voltage_sensor_);
  LOG_SENSOR(prefix, "Panel Power", panel_power_sensor_);
  LOG_SENSOR(prefix, "Battery Voltage", battery_voltage_sensor_);
  LOG_SENSOR(prefix, "Battery Voltage 2", battery_voltage_2_sensor_);
  LOG_SENSOR(prefix, "Battery Voltage 3", battery_voltage_3_sensor_);
  LOG_SENSOR(prefix, "Battery Current", battery_current_sensor_);
  LOG_SENSOR(prefix, "Battery Current", battery_current_2_sensor_);
  LOG_SENSOR(prefix, "Battery Current", battery_current_3_sensor_);
  LOG_SENSOR(prefix, "AC Out Voltage", ac_out_voltage_sensor_);
  LOG_SENSOR(prefix, "AC Out Current", ac_out_current_sensor_);
  LOG_SENSOR(prefix, "Load Current", load_current_sensor_);
  LOG_SENSOR(prefix, "Day Number", day_number_sensor_);
  LOG_SENSOR(prefix, "Charging Mode ID", charging_mode_id_sensor_);
  LOG_SENSOR(prefix, "Error Code", error_code_sensor_);
  LOG_SENSOR(prefix, "Warning Code", warning_code_sensor_);
  LOG_SENSOR(prefix, "Tracking Mode ID", tracking_mode_id_sensor_);
  LOG_SENSOR(prefix, "Device Mode ID", device_mode_id_sensor_);
  LOG_SENSOR(prefix, "Off Reason Bitmask", off_reason_bitmask_sensor_);
  LOG_TEXT_SENSOR(prefix, "Charging Mode", charging_mode_text_sensor_);
  LOG_TEXT_SENSOR(prefix, "Error Text", error_text_sensor_);
  LOG_TEXT_SENSOR(prefix, "Warning Text", warning_text_sensor_);
  LOG_TEXT_SENSOR(prefix, "Tracking Mode", tracking_mode_text_sensor_);
  LOG_TEXT_SENSOR(prefix, "Device Mode", device_mode_text_sensor_);
  LOG_TEXT_SENSOR(prefix, "Firmware Version", firmware_version_text_sensor_);
  LOG_TEXT_SENSOR(prefix, "Firmware Version 24bit", firmware_version_24bit_text_sensor_);
  LOG_TEXT_SENSOR(prefix, "Device Type", device_type_text_sensor_);
  LOG_TEXT_SENSOR(prefix, "Off Reason", off_reason_text_sensor_);

  LOG_SENSOR(prefix, "Battery Temperature ", battery_temperature_sensor_);
  LOG_SENSOR(prefix, "Instantaneous Power", instantaneous_power_sensor_);
  LOG_SENSOR(prefix, "Consumed Amp Hours", consumed_amp_hours_sensor_);
  LOG_SENSOR(prefix, "State Of Charge", state_of_charge_sensor_);
  LOG_SENSOR(prefix, "Time To Go", time_to_go_sensor_);
  LOG_SENSOR(prefix, "Depth Of The Deepest Discharge", depth_of_the_deepest_discharge_sensor_);
  LOG_SENSOR(prefix, "Depth Of The Last Discharge", depth_of_the_last_discharge_sensor_);
  LOG_SENSOR(prefix, "Number Of Charge Cycles", number_of_charge_cycles_sensor_);
  LOG_SENSOR(prefix, "Number Of Full Discharges", number_of_full_discharges_sensor_);
  LOG_SENSOR(prefix, "Min Battery Voltage", min_battery_voltage_sensor_);
  LOG_SENSOR(prefix, "Max Battery Voltage", max_battery_voltage_sensor_);
  LOG_SENSOR(prefix, "Last Full Charge", last_full_charge_sensor_);
  LOG_SENSOR(prefix, "Amount Of Discharged Energy", amount_of_discharged_energy_sensor_);
  LOG_SENSOR(prefix, "Amount Of Charged Energy", amount_of_charged_energy_sensor_);
  LOG_TEXT_SENSOR(prefix, "Alarm Condition Active", alarm_condition_active_text_sensor_);
  LOG_TEXT_SENSOR(prefix, "Alarm Reason", alarm_reason_text_sensor_);
  LOG_TEXT_SENSOR(prefix, "Model Description", model_description_text_sensor_);

  check_uart_settings(19200);
}

void VictronComponent::loop() {
  const uint32_t now = millis();
  const uint32_t elapsed_time = now - last_transmission_;
  bool available_data = false;
  if ((state_ > 0) && (elapsed_time >= 200)) {
    // last transmission too long ago. Reset RX index.
    ESP_LOGE(TAG, "Too old data: %ldms", elapsed_time);
    state_ = 0;
  }
  last_transmission_ = now;

  if (!available())
    return;

  while (available()) {
    available_data = true;
    uint8_t c;
    read_byte(&c);
    if (state_ == 0) {
      if (c == '\r' || c == '\n') {
        continue;
      }
      label_.clear();
      value_.clear();
      state_ = 1;
    }
    if (state_ == 1) {
      // Start of a ve.direct hex frame
      if (c == ':') {
        state_ = 3;
        continue;
      }
      if (c == '\t') {
        state_ = 2;
      } else {
        label_.push_back(c);
      }
      continue;
    }
    if (state_ == 2) {
      if (label_ == "Checksum") {
        state_ = 0;
        // The checksum is used as end of frame indicator
        if (now - this->last_publish_ >= this->throttle_) {
          this->last_publish_ = now;
          this->publishing_ = true;
        } else {
          this->publishing_ = false;
        }
        continue;
      }
      if (c == '\r' || c == '\n') {
        if (this->publishing_) {
          handle_value_();
        }
        state_ = 0;
      } else {
        value_.push_back(c);
      }
    }
    // Discard ve.direct hex frame
    if (state_ == 3) {
      if (c == '\r' || c == '\n') {
        state_ = 0;
      }
    }
  }
    uint32_t loop_time = millis() - now;
    if (available_data && loop_time > 10){
        ESP_LOGD(TAG, "Loop: %ldms", loop_time);
    }
}

static const char *charging_mode_text(int value) {
  const char *result;
  switch (value) {
    case 0:
      result = CHARGING_MODE_0;
      break;
    case 1:
      result = CHARGING_MODE_1;
      break;
    case 2:
      result = CHARGING_MODE_2;
      break;
    case 3:
      result = CHARGING_MODE_3;
      break;
    case 4:
      result = CHARGING_MODE_4;
      break;
    case 5:
      result = CHARGING_MODE_5;
      break;
    case 6:
      result = CHARGING_MODE_6;
      break;
    case 7:
      result = CHARGING_MODE_7;
      break;
    case 9:
      result = CHARGING_MODE_9;
      break;
    case 11:
      result = CHARGING_MODE_11;
      break;
    case 245:
      result = CHARGING_MODE_245;
      break;
    case 246:
      result = CHARGING_MODE_246;
      break;
    case 247:
      result = CHARGING_MODE_247;
      break;
    case 248:
      result = CHARGING_MODE_248;
      break;
    case 252:
      result = CHARGING_MODE_252;
      break;
    default:
      result = CHARGING_MODE_UNKNOWN;
      break;
  }
  strcpy_P(buffer_charging_mode, result);
  return buffer_charging_mode;
}

static const char *error_code_text(int value) {
  const char *result;
  switch (value) {
    case 0:
      result = ERROR_CODE_0;
      break;
    case 2:
      result = ERROR_CODE_2;
      break;
    case 17:
      result = ERROR_CODE_17;
      break;
    case 18:
      result = ERROR_CODE_18;
      break;
    case 19:
      result = ERROR_CODE_19;
      break;
    case 20:
      result = ERROR_CODE_20;
      break;
    case 21:
      result = ERROR_CODE_21;
      break;
    case 26:
      result = ERROR_CODE_26;
      break;
    case 28:
      result = ERROR_CODE_28;
      break;
    case 33:
      result = ERROR_CODE_33;
      break;
    case 34:
      result = ERROR_CODE_34;
      break;
    case 38:
      result = ERROR_CODE_38;
      break;
    case 39:
      result = ERROR_CODE_39;
      break;
    case 65:
      result = ERROR_CODE_65;
      break;
    case 66:
      result = ERROR_CODE_66;
      break;
    case 67:
      result = ERROR_CODE_67;
      break;
    case 68:
      result = ERROR_CODE_68;
      break;
    case 116:
      result = ERROR_CODE_116;
      break;
    case 117:
      result = ERROR_CODE_117;
      break;
    case 119:
      result = ERROR_CODE_119;
      break;
    default:
      result = ERROR_CODE_UNKNOWN;
      break;
  }
  strcpy_P(buffer_error_code, result);
  return buffer_error_code;
}

static const char *warning_code_text(int value) {
  switch (value) {
    case 0:
      return "No warning";
    case 1:
      return "Low Voltage";
    case 2:
      return "High Voltage";
    case 4:
      return "Low SOC";
    case 8:
      return "Low Starter Voltage";
    case 16:
      return "High Starter Voltage";
    case 32:
      return "Low Temperature";
    case 64:
      return "High Temperature";
    case 128:
      return "Mid Voltage";
    case 256:
      return "Overload";
    case 512:
      return "DC-ripple";
    case 1024:
      return "Low V AC out";
    case 2048:
      return "High V AC out";
    default:
      return "Multiple warnings";
  }
}

static const char *tracking_mode_text(int value) {
  switch (value) {
    case 0:
      return "Off";
    case 1:
      return "Limited";
    case 2:
      return "Active";
    default:
      return "Unknown";
  }
}

static const char *device_mode_text(int value) {
  switch (value) {
    case 0:
      return "Off";
    case 2:
      return "On";
    case 4:
      return "Off";
    case 5:
      return "Eco";
    default:
      return "Unknown";
  }
}

static std::string dc_monitor_mode_text(int value) {
  switch (value) {
    case -9:
      return "Solar charger";
    case -8:
      return "Wind turbine";
    case -7:
      return "Shaft generator";
    case -6:
      return "Alternator";
    case -5:
      return "Fuel cell";
    case -4:
      return "Water generator";
    case -3:
      return "DC/DC charger";
    case -2:
      return "AC charger";
    case -1:
      return "Generic source";
    case 0:
      return "Battery monitor (BMV)";
    case 1:
      return "Generic load";
    case 2:
      return "Electric drive";
    case 3:
      return "Fridge";
    case 4:
      return "Water pump";
    case 5:
      return "Bilge pump";
    case 6:
      return "DC system";
    case 7:
      return "Inverter";
    case 8:
      return "Water heater";
    default:
      return "Unknown";
  }
}

static const char *device_type_text(int value) {
  const char *result;
  switch (value) {
    case 0x203:
      result = DEVICE_TYPE_BMV_700;
      break;
    case 0x204:
      result = DEVICE_TYPE_BMV_702;
      break;
    case 0x205:
      result = DEVICE_TYPE_BMV_700H;
      break;
    case 0x0300:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_70_15;
      break;
    case 0xA040:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_75_50;
      break;
    case 0xA041:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_150_35;
      break;
    case 0xA042:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_75_15;
      break;
    case 0xA043:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_100_15;
      break;
    case 0xA044:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_100_30;
      break;
    case 0xA045:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_100_50;
      break;
    case 0xA046:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_150_70;
      break;
    case 0xA047:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_150_100;
      break;
    case 0xA049:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_100_50_REV2;
      break;
    case 0xA04A:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_100_30_REV2;
      break;
    case 0xA04B:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_150_35_REV2;
      break;
    case 0xA04C:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_75_10;
      break;
    case 0xA04D:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_150_45;
      break;
    case 0xA04E:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_150_60;
      break;
    case 0xA04F:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_150_85;
      break;
    case 0xA050:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_250_100;
      break;
    case 0xA051:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_150_100;
      break;
    case 0xA052:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_150_85;
      break;
    case 0xA053:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_75_15;
      break;
    case 0xA075:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_75_15_REV2;
      break;
    case 0xA054:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_75_10;
      break;
    case 0xA074:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_75_10_REV2;
      break;
    case 0xA055:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_100_15;
      break;
    case 0xA056:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_100_30;
      break;
    case 0xA073:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_150_45_REV3;
      break;
    case 0xA057:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_100_50;
      break;
    case 0xA058:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_150_35;
      break;
    case 0xA059:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_150_100_REV2;
      break;
    case 0xA05A:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_150_85_REV2;
      break;
    case 0xA05B:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_250_70;
      break;
    case 0xA05C:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_250_85;
      break;
    case 0xA05D:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_250_60;
      break;
    case 0xA05E:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_250_45;
      break;
    case 0xA05F:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_100_20;
      break;
    case 0xA060:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_100_20_48V;
      break;
    case 0xA061:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_150_45;
      break;
    case 0xA062:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_150_60;
      break;
    case 0xA063:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_150_70;
      break;
    case 0xA064:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_250_85_REV2;
      break;
    case 0xA065:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_250_100_REV2;
      break;
    case 0xA066:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_100_20;
      break;
    case 0xA067:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_100_20_48V;
      break;
    case 0xA068:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_250_60_REV2;
      break;
    case 0xA069:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_250_70_REV2;
      break;
    case 0xA06A:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_150_45_REV2;
      break;
    case 0xA06B:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_150_60_REV2;
      break;
    case 0xA06C:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_150_70_REV2;
      break;
    case 0xA06D:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_150_85_REV3;
      break;
    case 0xA06E:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_150_100_REV3;
      break;
    case 0xA06F:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_150_45_REV2;
      break;
    case 0xA070:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_150_60_REV2;
      break;
    case 0xA071:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_150_70_REV2;
      break;
    case 0xA07D:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_75_15_REV3;
      break;
    case 0xA102:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_150_70;
      break;
    case 0xA103:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_150_45;
      break;
    case 0xA104:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_150_60;
      break;
    case 0xA105:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_150_85;
      break;
    case 0xA106:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_150_100;
      break;
    case 0xA107:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_250_45;
      break;
    case 0xA108:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_250_60;
      break;
    case 0xA109:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_250_70;
      break;
    case 0xA10A:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_250_85;
      break;
    case 0xA10B:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_250_100;
      break;
    case 0xA10C:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_150_70_REV2;
      break;
    case 0xA10D:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_150_85_REV2;
      break;
    case 0xA10E:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_150_100_REV2;
      break;
    case 0xA10F:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_VE_CAN_150_100;
      break;
    case 0xA112:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_VE_CAN_250_70;
      break;
    case 0xA113:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_VE_CAN_250_100;
      break;
    case 0xA114:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_250_70_REV2;
      break;
    case 0xA115:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_250_100_REV2;
      break;
    case 0xA116:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_VE_CAN_250_85_REV2;
      break;
    case 0xA201:
      result = DEVICE_TYPE_PHOENIX_INVERTER_12V_250VA_230V;
      break;
    case 0xA202:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_250VA_230V;
      break;
    case 0xA204:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_250VA_230V;
      break;
    case 0xA211:
      result = DEVICE_TYPE_PHOENIX_INVERTER_12V_375VA_230V;
      break;
    case 0xA212:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_375VA_230V;
      break;
    case 0xA214:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_375VA_230V;
      break;
    case 0xA221:
      result = DEVICE_TYPE_PHOENIX_INVERTER_12V_500VA_230V;
      break;
    case 0xA222:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_500VA_230V;
      break;
    case 0xA224:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_500VA_230V;
      break;
    case 0xA231:
      result = DEVICE_TYPE_PHOENIX_INVERTER_12V_250VA_230V;
      break;
    case 0xA232:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_250VA_230V;
      break;
    case 0xA234:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_250VA_230V;
      break;
    case 0xA239:
      result = DEVICE_TYPE_PHOENIX_INVERTER_12V_250VA_120V;
      break;
    case 0xA23A:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_250VA_120V;
      break;
    case 0xA23C:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_250VA_120V;
      break;
    case 0xA241:
      result = DEVICE_TYPE_PHOENIX_INVERTER_12V_375VA_230V;
      break;
    case 0xA242:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_375VA_230V;
      break;
    case 0xA244:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_375VA_230V;
      break;
    case 0xA249:
      result = DEVICE_TYPE_PHOENIX_INVERTER_12V_375VA_120V;
      break;
    case 0xA24A:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_375VA_120V;
      break;
    case 0xA24C:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_375VA_120V;
      break;
    case 0xA251:
      result = DEVICE_TYPE_PHOENIX_INVERTER_12V_500VA_230V;
      break;
    case 0xA252:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_500VA_230V;
      break;
    case 0xA254:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_500VA_230V;
      break;
    case 0xA259:
      result = DEVICE_TYPE_PHOENIX_INVERTER_12V_500VA_120V;
      break;
    case 0xA25A:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_500VA_120V;
      break;
    case 0xA25C:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_500VA_120V;
      break;
    case 0xA261:
      result = DEVICE_TYPE_PHOENIX_INVERTER_12V_800VA_230V;
      break;
    case 0xA262:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_800VA_230V;
      break;
    case 0xA264:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_800VA_230V;
      break;
    case 0xA269:
      result = DEVICE_TYPE_PHOENIX_INVERTER_12V_800VA_120V;
      break;
    case 0xA26A:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_800VA_120V;
      break;
    case 0xA26C:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_800VA_120V;
      break;
    case 0xA271:
      result = DEVICE_TYPE_PHOENIX_INVERTER_12V_1200VA_230V;
      break;
    case 0xA272:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_1200VA_230V;
      break;
    case 0xA274:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_1200VA_230V;
      break;
    case 0xA279:
    case 0xA2F9:
      result = DEVICE_TYPE_PHOENIX_INVERTER_12V_1200VA_120V;
      break;
    case 0xA27A:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_1200VA_120V;
      break;
    case 0xA27C:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_1200VA_120V;
      break;
    case 0xA281:
      result = DEVICE_TYPE_PHOENIX_INVERTER_12V_1600VA_230V;
      break;
    case 0xA282:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_1600VA_230V;
      break;
    case 0xA284:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_1600VA_230V;
      break;
    case 0xA291:
      result = DEVICE_TYPE_PHOENIX_INVERTER_12V_2000VA_230V;
      break;
    case 0xA292:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_2000VA_230V;
      break;
    case 0xA294:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_2000VA_230V;
      break;
    case 0xA2A1:
      result = DEVICE_TYPE_PHOENIX_INVERTER_12V_3000VA_230V;
      break;
    case 0xA2A2:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_3000VA_230V;
      break;
    case 0xA2A4:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_3000VA_230V;
      break;
    case 0xA30A:
      result = DEVICE_TYPE_BLUE_SMART_IP65_CHARGER_12_25;
      break;
    case 0xA332:
      result = DEVICE_TYPE_BLUE_SMART_IP22_CHARGER_24_8;
      break;
    case 0xA334:
      result = DEVICE_TYPE_BLUE_SMART_IP22_CHARGER_24_12;
      break;
    case 0xA336:
      result = DEVICE_TYPE_BLUE_SMART_IP22_CHARGER_24_16;
      break;
    case 0xA340:
      result = DEVICE_TYPE_PHOENIX_SMART_IP43_CHARGER_12_50_1_1;
      break;
    case 0xA341:
      result = DEVICE_TYPE_PHOENIX_SMART_IP43_CHARGER_12_50_3;
      break;
    case 0xA342:
      result = DEVICE_TYPE_PHOENIX_SMART_IP43_CHARGER_24_25_1_1;
      break;
    case 0xA343:
      result = DEVICE_TYPE_PHOENIX_SMART_IP43_CHARGER_24_25_3;
      break;
    case 0xA344:
      result = DEVICE_TYPE_PHOENIX_SMART_IP43_CHARGER_12_30_1_1;
      break;
    case 0xA345:
      result = DEVICE_TYPE_PHOENIX_SMART_IP43_CHARGER_12_30_3;
      break;
    case 0xA346:
      result = DEVICE_TYPE_PHOENIX_SMART_IP43_CHARGER_24_16_1_1;
      break;
    case 0xA347:
      result = DEVICE_TYPE_PHOENIX_SMART_IP43_CHARGER_24_16_3;
      break;
    case 0xA381:
      result = DEVICE_TYPE_BMV_712_SMART;
      break;
    case 0xA382:
      result = DEVICE_TYPE_BMV_710H_SMART;
      break;
    case 0xA383:
      result = DEVICE_TYPE_BMV_712_SMART_REV2;
      break;
    case 0xA389:
      result = DEVICE_TYPE_SMARTSHUNT_500A_50MV;
      break;
    case 0xA38A:
      result = DEVICE_TYPE_SMARTSHUNT_1000A_50MV;
      break;
    case 0xA38B:
      result = DEVICE_TYPE_SMARTSHUNT_2000A_50MV;
      break;
    case 0xA442:
      result = DEVICE_TYPE_MULTI_RS_SOLAR_48V_6000VA_230V;
      break;

    // Additional PIDs mentioned in VE.Direct-HEX-Protocol specifications
    case 0xA2B1:
      result = DEVICE_TYPE_PHOENIX_INVERTER_SMART_12V_5000VA_230VAC_64K;
      break;
    case 0xA2B2:
      result = DEVICE_TYPE_PHOENIX_INVERTER_SMART_24V_5000VA_230VAC_64K;
      break;
    case 0xA2B4:
      result = DEVICE_TYPE_PHOENIX_INVERTER_SMART_48V_5000VA_230VAC_64K;
      break;
    case 0xA2E1:
      result = DEVICE_TYPE_PHOENIX_INVERTER_12V_800VA_230VAC_64K_HS;
      break;
    case 0xA2E2:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_800VA_230VAC_64K_HS;
      break;
    case 0xA2E4:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_800VA_230VAC_64K_HS;
      break;
    case 0xA2E9:
      result = DEVICE_TYPE_PHOENIX_INVERTER_12V_800VA_120VAC_64K_HS;
      break;
    case 0xA2EA:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_800VA_120VAC_64K_HS;
      break;
    case 0xA2EC:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_800VA_120VAC_64K_HS;
      break;
    case 0xA2F1:
      result = DEVICE_TYPE_PHOENIX_INVERTER_12V_1200VA_230VAC_64K_HS;
      break;
    case 0xA2F2:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_1200VA_230VAC_64K_HS;
      break;
    case 0xA2F4:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_1200VA_230VAC_64K_HS;
      break;
    case 0xA2FA:
      result = DEVICE_TYPE_PHOENIX_INVERTER_24V_1200VA_120VAC_64K_HS;
      break;
    case 0xA2FC:
      result = DEVICE_TYPE_PHOENIX_INVERTER_48V_1200VA_120VAC_64K_HS;
      break;
    case 0xA3F0:
      result = DEVICE_TYPE_ORION_XS_12V_12V_50A;
      break;
    case 0xA3F1:
      result = DEVICE_TYPE_ORION_XS_1400;
      break;
    case 0xA048:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_75_50_REV2;
      break;
    case 0xA072:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_150_45_REV3;
      break;
    case 0xA076:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_100_30_REV3;
      break;
    case 0xA077:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_100_50_REV3;
      break;
    case 0xA078:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_150_35_REV2;
      break;
    case 0xA079:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_75_10_REV2;
      break;
    case 0xA07A:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_75_15_REV2;
      break;
    case 0xA07B:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_100_15_REV2;
      break;
    case 0xA07C:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_75_10_REV3;
      break;
    case 0xA07E:
      result = DEVICE_TYPE_SMARTSOLAR_CHARGER_MPPT_100_30;
      break;
    case 0xA110:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_RS_450_100;
      break;
    case 0xA111:
      result = DEVICE_TYPE_SMARTSOLAR_MPPT_RS_450_200;
      break;
    case 0xA117:
      result = DEVICE_TYPE_BLUESOLAR_MPPT_VE_CAN_150_100;
      break;

    default:
      result = DEVICE_TYPE_UNKNOWN;
      break;
  }
  strcpy_P(buffer_device_type, result);
  return buffer_device_type;
}

static std::string off_reason_text(uint32_t mask) {
  bool first = true;
  std::string value_list = "";
  if (mask) {
    for (uint8_t i = 0; i < OFF_REASONS_SIZE; i++) {
      if (mask & (1 << i)) {
        if (first) {
          first = false;
        } else {
          value_list.append(";");
        }
        strcpy_P(buffer_off_reason, (PGM_P)pgm_read_ptr(&OFF_REASONS[i]));
        value_list.append(buffer_off_reason);
      }
    }
  }

  return value_list;
}

void VictronComponent::handle_value_() {
  int value;

  if (label_ == "V") {
    this->publish_state_(battery_voltage_sensor_, atoi(value_.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "V2") {
    // mV to V
    this->publish_state_(battery_voltage_2_sensor_, atoi(value_.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "V3") {
    // mV to V
    this->publish_state_(battery_voltage_3_sensor_, atoi(value_.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "VS") {
    // mV to V
    this->publish_state_(auxiliary_battery_voltage_sensor_, atoi(value_.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "VM") {
    // mV to V
    this->publish_state_(midpoint_voltage_of_the_battery_bank_sensor_,
                         atoi(value_.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "DM") {
    // Per mill to %
    this->publish_state_(midpoint_deviation_of_the_battery_bank_sensor_,
                         atoi(value_.c_str()) * 0.10f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "VPV") {
    // mV to V
    this->publish_state_(panel_voltage_sensor_, atoi(value_.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "PPV") {
    this->publish_state_(panel_power_sensor_, atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "I") {
    // mA to A
    this->publish_state_(battery_current_sensor_, atoi(value_.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "I2") {
    // mA to A
    this->publish_state_(battery_current_2_sensor_, atoi(value_.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "I3") {
    // mA to A
    this->publish_state_(battery_current_3_sensor_, atoi(value_.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "IL") {
    this->publish_state_(load_current_sensor_, atoi(value_.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "LOAD") {
    this->publish_state_(load_state_binary_sensor_, value_ == "ON" || value_ == "On");
    return;
  }

  if (label_ == "T") {
    if (value_ == "---") {
      this->publish_state_(battery_temperature_sensor_, NAN);
      return;
    }

    this->publish_state_(battery_temperature_sensor_, atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "P") {
    this->publish_state_(instantaneous_power_sensor_, atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "CE") {
    // mAh -> Ah
    this->publish_state_(consumed_amp_hours_sensor_, atoi(value_.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "SOC") {
    // Per mill to %
    this->publish_state_(state_of_charge_sensor_, atoi(value_.c_str()) * 0.10f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "TTG") {
    this->publish_state_(time_to_go_sensor_, atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "Alarm") {
    this->publish_state_(alarm_condition_active_text_sensor_, value_);
    return;
  }

  if (label_ == "Relay") {
    this->publish_state_(relay_state_binary_sensor_, value_ == "ON" || value_ == "On");
    return;
  }

  if (label_ == "AR") {
    static uint16_t last_error = UINT16_MAX;
    uint16_t value = atoi(value_.c_str());  // NOLINT(cert-err34-c)
    if( value != last_error ) {
      last_error = value;
      this->publish_state_(alarm_reason_text_sensor_, error_code_text(value));  // NOLINT(cert-err34-c)
    }
    return;
  }

  if (label_ == "OR") {
    auto off_reason_bitmask = parse_hex<uint32_t>(value_.substr(2, value_.size() - 2));
    if (off_reason_bitmask) {
    static uint32_t last_off_mask = UINT32_MAX;
      this->publish_state_(off_reason_bitmask_sensor_, *off_reason_bitmask);
      // Load the string only if it has changed.
      if (*off_reason_bitmask != last_off_mask){
        last_off_mask = *off_reason_bitmask;
        this->publish_state_(off_reason_text_sensor_, off_reason_text(*off_reason_bitmask));
      }
    }
    return;
  }

  if (label_ == "H1") {
    // mAh -> Ah
    this->publish_state_(depth_of_the_deepest_discharge_sensor_,
                         atoi(value_.c_str()) / 1000.0);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H2") {
    // mAh -> Ah
    this->publish_state_(depth_of_the_last_discharge_sensor_, atoi(value_.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H3") {
    // mAh -> Ah
    this->publish_state_(depth_of_the_average_discharge_sensor_,
                         atoi(value_.c_str()) / 1000.0);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H4") {
    this->publish_state_(number_of_charge_cycles_sensor_, atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H5") {
    this->publish_state_(number_of_full_discharges_sensor_, atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H6") {
    if (value_ == "---") {
      this->publish_state_(cumulative_amp_hours_drawn_sensor_, NAN);
      return;
    }

    // mAh -> Ah
    this->publish_state_(cumulative_amp_hours_drawn_sensor_, atoi(value_.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H7") {
    // mV to V
    this->publish_state_(min_battery_voltage_sensor_, atoi(value_.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H8") {
    // mV to V
    this->publish_state_(max_battery_voltage_sensor_, atoi(value_.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H9") {
    if (value_ == "---") {
      this->publish_state_(last_full_charge_sensor_, NAN);
      return;
    }

    // sec -> min
    this->publish_state_(last_full_charge_sensor_, (float) atoi(value_.c_str()) / 60.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H10") {
    if (value_ == "---") {
      this->publish_state_(number_of_automatic_synchronizations_sensor_, NAN);
      return;
    }

    this->publish_state_(number_of_automatic_synchronizations_sensor_, atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H11") {
    this->publish_state_(number_of_low_main_voltage_alarms_sensor_, atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H12") {
    this->publish_state_(number_of_high_main_voltage_alarms_sensor_, atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H13") {
    this->publish_state_(number_of_low_auxiliary_voltage_alarms_sensor_, atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H14") {
    this->publish_state_(number_of_high_auxiliary_voltage_alarms_sensor_,
                         atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H15") {
    // mV to V
    this->publish_state_(min_auxiliary_battery_voltage_sensor_,
                         atoi(value_.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H16") {
    // mV to V
    this->publish_state_(max_auxiliary_battery_voltage_sensor_,
                         atoi(value_.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  // "H17"    0.01 kWh   Amount of discharged energy (BMV) / Amount of produced energy (DC monitor)
  if (label_ == "H17") {
    // Wh
    this->publish_state_(amount_of_discharged_energy_sensor_, atoi(value_.c_str()) * 10.0f);  // NOLINT(cert-err34-c)
    return;
  }

  // "H18"    0.01 kWh   Amount of charged energy (BMV) / Amount of consumed energy (DC monitor)
  if (label_ == "H18") {
    // Wh
    this->publish_state_(amount_of_charged_energy_sensor_, atoi(value_.c_str()) * 10.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H19") {
    this->publish_state_(yield_total_sensor_, atoi(value_.c_str()) * 10.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H20") {
    this->publish_state_(yield_today_sensor_, atoi(value_.c_str()) * 10.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H21") {
    this->publish_state_(max_power_today_sensor_, atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H22") {
    this->publish_state_(yield_yesterday_sensor_, atoi(value_.c_str()) * 10.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H23") {
    this->publish_state_(max_power_yesterday_sensor_, atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "ERR") {
    static uint16_t last_error = UINT16_MAX;
    uint16_t value = atoi(value_.c_str());  // NOLINT(cert-err34-c)
    this->publish_state_(error_code_sensor_, value);
    if (value != last_error){
        last_error = value;
        this->publish_state_(error_text_sensor_, error_code_text(value));
    }
    return;
  }

  if (label_ == "CS") {
  static uint16_t last_charging_mode = UINT16_MAX;
    value = static_cast<uint16_t>(atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    this->publish_state_(charging_mode_id_sensor_, (float) value);
    if( value != last_charging_mode ){
        last_charging_mode = value;
        this->publish_state_(charging_mode_text_sensor_, charging_mode_text(value));
    }
    return;
  }

  // "BMV"               Model description (deprecated)
  if (label_ == "BMV") {
    this->publish_state_(model_description_text_sensor_, value_);
    return;
  }

  if (label_ == "FW") {
    this->publish_state_once_(firmware_version_text_sensor_, value_.insert(value_.size() - 2, "."));
    return;
  }

  if (label_ == "FWE") {
    if (this->firmware_version_24bit_text_sensor_ == nullptr || this->firmware_version_24bit_text_sensor_->has_state())
      return;

    if (value_.size() > 4) {
      std::string release_type = value_.substr(value_.size() - 2, 2);
      std::string version_number = value_.substr(0, value_.size() - 2);
      version_number = version_number.insert(version_number.size() - 2, ".");
      release_type = (release_type == "FF") ? "-official" : "-beta-" + release_type;

      this->publish_state_once_(firmware_version_24bit_text_sensor_, version_number + release_type);
      return;
    }

    this->publish_state_once_(firmware_version_24bit_text_sensor_, value_);
    return;
  }

  if (label_ == "PID") {
    static uint32_t last_pid = UINT32_MAX;
    uint32_t value = strtol(value_.c_str(), nullptr, 0);
    if (value != last_pid) {
      last_pid = value;
      this->publish_state_(device_type_text_sensor_, device_type_text(value));
    }
    return;
  }

  if (label_ == "SER#") {
    this->publish_state_once_(serial_number_text_sensor_, value_);
    return;
  }

  if (label_ == "HC#") {
    this->publish_state_once_(hardware_revision_text_sensor_, value_);
    return;
  }

  if (label_ == "HSDS") {
    this->publish_state_(day_number_sensor_, atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "MODE") {
    value = atoi(value_.c_str());  // NOLINT(cert-err34-c)
    this->publish_state_(device_mode_id_sensor_, (float) value);
    this->publish_state_(device_mode_text_sensor_, device_mode_text(value));
    return;
  }

  if (label_ == "AC_OUT_V") {
    this->publish_state_(ac_out_voltage_sensor_, atoi(value_.c_str()) / 100.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "AC_OUT_I") {
    this->publish_state_(ac_out_current_sensor_, std::max(0.0f, atoi(value_.c_str()) / 10.0f));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "AC_OUT_S") {
    this->publish_state_(ac_out_apparent_power_sensor_, atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "WARN") {
    value = atoi(value_.c_str());  // NOLINT(cert-err34-c)
    this->publish_state_(warning_code_sensor_, value);
    this->publish_state_(warning_text_sensor_, warning_code_text(value));
    return;
  }

  if (label_ == "MPPT") {
    value = atoi(value_.c_str());  // NOLINT(cert-err34-c)
    this->publish_state_(tracking_mode_id_sensor_, (float) value);
    this->publish_state_(tracking_mode_text_sensor_, tracking_mode_text(value));
    return;
  }

  if (label_ == "MON") {
    value = atoi(value_.c_str());  // NOLINT(cert-err34-c)
    this->publish_state_(dc_monitor_mode_id_sensor_, (float) value);
    this->publish_state_(dc_monitor_mode_text_sensor_, dc_monitor_mode_text(value));
    return;
  }

  ESP_LOGD(TAG, "Unhandled property: %s %s", label_.c_str(), value_.c_str());
}

void VictronComponent::publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state) {
  if (binary_sensor == nullptr)
    return;

  binary_sensor->publish_state(state);
}

void VictronComponent::publish_state_(sensor::Sensor *sensor, float value) {
  if (sensor == nullptr)
    return;

  sensor->publish_state(value);
}

void VictronComponent::publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state) {
    publish_state_(text_sensor, state.c_str());
}

void VictronComponent::publish_state_once_(text_sensor::TextSensor *text_sensor, const std::string &state) {
    publish_state_once_(text_sensor, state.c_str());
}

void VictronComponent::publish_state_(text_sensor::TextSensor *text_sensor, const char *state) {
  if (text_sensor == nullptr)
    return;

  text_sensor->publish_state(state);
}

void VictronComponent::publish_state_once_(text_sensor::TextSensor *text_sensor, const char *state) {
  if (text_sensor == nullptr)
    return;

  if (text_sensor->has_state())
    return;

  text_sensor->publish_state(state);
}

}  // namespace victron
}  // namespace esphome
