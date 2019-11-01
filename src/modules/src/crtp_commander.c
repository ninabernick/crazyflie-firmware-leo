/**
 *    ||          ____  _ __
 * +------+      / __ )(_) /_______________ _____  ___
 * | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *  ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * Crazyflie Firmware
 *
 * Copyright (C) 2011-2017 Bitcraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */
#include <stdbool.h>

#include "crtp_commander.h"

#include "commander.h"
#include "crtp.h"
#include "log.h"
#include "debug.h"


static bool isInit;

static void commanderCrtpCB(CRTPPacket* pk);

void crtpCommanderInit(void)
{
  if(isInit) {
    return;
  }

  crtpInit();
  crtpRegisterPortCB(CRTP_PORT_SETPOINT, commanderCrtpCB);
  crtpRegisterPortCB(CRTP_PORT_SETPOINT_GENERIC, commanderCrtpCB);
  isInit = true;
}

static void commanderCrtpCB(CRTPPacket* pk)
{
  static setpoint_t setpoint;

  if(pk->port == CRTP_PORT_SETPOINT && pk->channel == 0) {
    // leonana: the _cf.commander.send_setpoint(0, 0, 0, 0) will go through here
    // DEBUG_PRINT("Enter PORT_SETPOINT\n");
    crtpCommanderRpytDecodeSetpoint(&setpoint, pk);
    commanderSetSetpoint(&setpoint, COMMANDER_PRIORITY_CRTP);
  } else if (pk->port == CRTP_PORT_SETPOINT_GENERIC && pk->channel == 0) {
    // leonana: packets like altHold or hover go through here
    crtpCommanderGenericDecodeSetpoint(&setpoint, pk);
    commanderSetSetpoint(&setpoint, COMMANDER_PRIORITY_CRTP);
  }
}
