/**
 * @file HundredYenSAT.h
 * @brief Hundred Yen SATライブラリの全公開APIを読み込む。
 *
 * 機能単位でincludeしたい場合は、hundred_yen_sat/devices/Imu.hなどを
 * 直接includeすることもできる。
 */
#pragma once

#include "hundred_yen_sat/devices/Camera.h"
#include "hundred_yen_sat/devices/Gnss.h"
#include "hundred_yen_sat/devices/Imu.h"
#include "hundred_yen_sat/devices/Leds.h"
#include "hundred_yen_sat/devices/LightSensor.h"
#include "hundred_yen_sat/devices/Motor.h"
#include "hundred_yen_sat/devices/Motors.h"
#include "hundred_yen_sat/devices/Speaker.h"
#include "hundred_yen_sat/devices/Watchdog.h"
#include "hundred_yen_sat/fsm/Fsm.h"
#include "hundred_yen_sat/storage/IniStore.h"
#include "hundred_yen_sat/storage/Recorder.h"
#include "hundred_yen_sat/storage/Storage.h"
