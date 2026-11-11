/*
 * SPDX-License-Identifier: GPL-3.0
 * Copyright (c) 2026 Karol Trykacz
 */

#pragma once

#include "main.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// Input ports number
#define MIDI_IN_PORTS_NUM 0x01
// Output ports number
#define MIDI_OUT_PORTS_NUM 0x01

typedef enum {
  MIDI_IDLE = 0,
  MIDI_BUSY,
} MIDI_StateTypeDef;

typedef enum {
  USBD_OK = 0U,
  USBD_BUSY,
  USBD_EMEM,
  USBD_FAIL,
} USBD_StatusTypeDef;

#ifndef MIDI_ENABLE_SYSEX
#define MIDI_ENABLE_SYSEX 0
#endif

#if MIDI_ENABLE_SYSEX
#ifndef MIDI_SYSEX_MAX_DATA
#define MIDI_SYSEX_MAX_DATA 128u
#endif
#endif

typedef enum {
  MIDI_NOTE_OFF = 0x80,
  MIDI_NOTE_ON = 0x90,
  MIDI_POLY_KEY_PRESSURE = 0xA0,
  MIDI_CONTROL_CHANGE = 0xB0,
  MIDI_PROGRAM_CHANGE = 0xC0,
  MIDI_CHANNEL_PRESSURE = 0xD0,
  MIDI_PITCH_BEND = 0xE0,

  MIDI_SYSEX_START = 0xF0,
  MIDI_MTC_QUARTER_FRAME = 0xF1,
  MIDI_SONG_POSITION = 0xF2,
  MIDI_SONG_SELECT = 0xF3,
  MIDI_TUNE_REQUEST = 0xF6,
  MIDI_SYSEX_END = 0xF7,

  MIDI_TIMING_CLOCK = 0xF8,
  MIDI_START = 0xFA,
  MIDI_CONTINUE = 0xFB,
  MIDI_STOP = 0xFC,
  MIDI_ACTIVE_SENSING = 0xFE,
  MIDI_SYSTEM_RESET = 0xFF
} MIDI_MessageType;

typedef struct {
  MIDI_MessageType type;
  uint8_t channel;

  union {
    struct {
      uint8_t note;
      uint8_t velocity;
    } note;

    struct {
      uint8_t note;
      uint8_t pressure;
    } keyPressure;

    struct {
      uint8_t controller;
      uint8_t value;
    } control;

    struct {
      uint8_t program;
    } program;

    struct {
      uint8_t pressure;
    } channelPressure;

    struct {
      uint16_t value;
    } pitchBend;

    struct {
      uint8_t messageType;
      uint8_t value;
    } quarterFrame;

    struct {
      uint16_t position;
    } songPosition;

    struct {
      uint8_t song;
    } songSelect;

#if MIDI_ENABLE_SYSEX
    struct {
      uint16_t length;
      uint8_t data[MIDI_SYSEX_MAX_DATA];
    } sysEx;
#endif

  } data;
} MIDI_Message;

typedef enum {
  MIDI_MODE_ALL_SOUND_OFF = 120,
  MIDI_MODE_RESET_ALL_CONTROLLERS = 121,
  MIDI_MODE_LOCAL_CONTROL = 122,
  MIDI_MODE_ALL_NOTES_OFF = 123,
  MIDI_MODE_OMNI_OFF = 124,
  MIDI_MODE_OMNI_ON = 125,
  MIDI_MODE_MONO_ON = 126,
  MIDI_MODE_POLY_ON = 127
} MIDI_ChannelMode;

typedef enum {
  MIDI_CC_BANK_SELECT = 0,
  MIDI_CC_MODULATION = 1,
  MIDI_CC_BREATH_CONTROLLER = 2,
  MIDI_CC_FOOT_CONTROLLER = 4,
  MIDI_CC_PORTAMENTO_TIME = 5,
  MIDI_CC_DATA_ENTRY_MSB = 6,
  MIDI_CC_CHANNEL_VOLUME = 7,
  MIDI_CC_BALANCE = 8,
  MIDI_CC_PAN = 10,
  MIDI_CC_EXPRESSION = 11,
  MIDI_CC_EFFECT_CONTROL_1 = 12,
  MIDI_CC_EFFECT_CONTROL_2 = 13,
  MIDI_CC_GENERAL_PURPOSE_1 = 16,
  MIDI_CC_GENERAL_PURPOSE_2 = 17,
  MIDI_CC_GENERAL_PURPOSE_3 = 18,
  MIDI_CC_GENERAL_PURPOSE_4 = 19,

  MIDI_CC_LSB_OFFSET = 32,
  MIDI_CC_BANK_SELECT_LSB = 32,
  MIDI_CC_DATA_ENTRY_LSB = 38,

  MIDI_CC_DAMPER_PEDAL = 64,
  MIDI_CC_PORTAMENTO_ON_OFF = 65,
  MIDI_CC_SOSTENUTO = 66,
  MIDI_CC_SOFT_PEDAL = 67,
  MIDI_CC_LEGATO_FOOTSWITCH = 68,
  MIDI_CC_HOLD_2 = 69,
  MIDI_CC_SOUND_CONTROLLER_1 = 70,
  MIDI_CC_SOUND_CONTROLLER_2 = 71,
  MIDI_CC_SOUND_CONTROLLER_3 = 72,
  MIDI_CC_SOUND_CONTROLLER_4 = 73,
  MIDI_CC_SOUND_CONTROLLER_5 = 74,
  MIDI_CC_SOUND_CONTROLLER_6 = 75,
  MIDI_CC_SOUND_CONTROLLER_7 = 76,
  MIDI_CC_SOUND_CONTROLLER_8 = 77,
  MIDI_CC_SOUND_CONTROLLER_9 = 78,
  MIDI_CC_SOUND_CONTROLLER_10 = 79,
  MIDI_CC_GENERAL_PURPOSE_5 = 80,
  MIDI_CC_GENERAL_PURPOSE_6 = 81,
  MIDI_CC_GENERAL_PURPOSE_7 = 82,
  MIDI_CC_GENERAL_PURPOSE_8 = 83,
  MIDI_CC_PORTAMENTO_CONTROL = 84,
  MIDI_CC_EFFECTS_DEPTH_1 = 91,
  MIDI_CC_EFFECTS_DEPTH_2 = 92,
  MIDI_CC_EFFECTS_DEPTH_3 = 93,
  MIDI_CC_EFFECTS_DEPTH_4 = 94,
  MIDI_CC_EFFECTS_DEPTH_5 = 95,
  MIDI_CC_DATA_INCREMENT = 96,
  MIDI_CC_DATA_DECREMENT = 97,
  MIDI_CC_NRPN_LSB = 98,
  MIDI_CC_NRPN_MSB = 99,
  MIDI_CC_RPN_LSB = 100,
  MIDI_CC_RPN_MSB = 101,
  MIDI_CC_CHANNEL_MODE_BASE = 120
} MIDI_Controller;

typedef enum {
  MIDI_RPN_PITCH_BEND_SENSITIVITY = 0x0000,
  MIDI_RPN_FINE_TUNING = 0x0001,
  MIDI_RPN_COARSE_TUNING = 0x0002,
  MIDI_RPN_TUNING_PROGRAM_SELECT = 0x0003,
  MIDI_RPN_TUNING_BANK_SELECT = 0x0004
} MIDI_RegisteredParameter;

#define MIDI_LOCAL_CONTROL_OFF 0x00
#define MIDI_LOCAL_CONTROL_ON 0x7F

#define MIDI_SYSEX_ID_EXTENDED 0x00
#define MIDI_SYSEX_ID_NON_COMMERCIAL 0x7D
#define MIDI_SYSEX_ID_NON_REAL_TIME 0x7E
#define MIDI_SYSEX_ID_REAL_TIME 0x7F
#define MIDI_SYSEX_DEVICE_ID_ALL_CALL 0x7F

typedef enum {
  MIDI_SYSEX_NRT_SAMPLE_DUMP_HEADER = 0x01,
  MIDI_SYSEX_NRT_SAMPLE_DATA_PACKET = 0x02,
  MIDI_SYSEX_NRT_SAMPLE_DUMP_REQUEST = 0x03,
  MIDI_SYSEX_NRT_MTC = 0x04,
  MIDI_SYSEX_NRT_SAMPLE_DUMP_EXTENSIONS = 0x05,
  MIDI_SYSEX_NRT_GENERAL_INFORMATION = 0x06,
  MIDI_SYSEX_NRT_FILE_DUMP = 0x07,
  MIDI_SYSEX_NRT_MIDI_TUNING = 0x08,
  MIDI_SYSEX_NRT_GENERAL_MIDI = 0x09,
  MIDI_SYSEX_NRT_END_OF_FILE = 0x7B,
  MIDI_SYSEX_NRT_WAIT = 0x7C,
  MIDI_SYSEX_NRT_CANCEL = 0x7D,
  MIDI_SYSEX_NRT_NAK = 0x7E,
  MIDI_SYSEX_NRT_ACK = 0x7F
} MIDI_SysExNonRealTimeSubID;

typedef enum {
  MIDI_SYSEX_RT_MTC = 0x01,
  MIDI_SYSEX_RT_MIDI_SHOW_CONTROL = 0x02,
  MIDI_SYSEX_RT_NOTATION_INFO = 0x03,
  MIDI_SYSEX_RT_DEVICE_CONTROL = 0x04,
  MIDI_SYSEX_RT_MTC_CUEING = 0x05,
  MIDI_SYSEX_RT_MMC_COMMAND = 0x06,
  MIDI_SYSEX_RT_MMC_RESPONSE = 0x07,
  MIDI_SYSEX_RT_MIDI_TUNING = 0x08
} MIDI_SysExRealTimeSubID;

typedef enum {
  MIDI_SYSEX_GEN_INFO_IDENTITY_REQUEST = 0x01,
  MIDI_SYSEX_GEN_INFO_IDENTITY_REPLY = 0x02
} MIDI_SysExGeneralInfoSubID;

typedef enum {
  MIDI_SYSEX_FILE_DUMP_HEADER = 0x01,
  MIDI_SYSEX_FILE_DUMP_DATA_PACKET = 0x02,
  MIDI_SYSEX_FILE_DUMP_REQUEST = 0x03
} MIDI_SysExFileDumpSubID;

typedef enum {
  MIDI_SYSEX_TUNING_BULK_DUMP_REQUEST = 0x00,
  MIDI_SYSEX_TUNING_BULK_DUMP = 0x01,
  MIDI_SYSEX_TUNING_NOTE_CHANGE = 0x02
} MIDI_SysExTuningSubID;

typedef enum {
  MIDI_SYSEX_GM_ON = 0x01,
  MIDI_SYSEX_GM_OFF = 0x02
} MIDI_SysExGeneralMidiSubID;

typedef enum {
  MIDI_SYSEX_NOTATION_BAR_NUMBER = 0x01,
  MIDI_SYSEX_NOTATION_TIME_SIGNATURE_IMMEDIATE = 0x02,
  MIDI_SYSEX_NOTATION_TIME_SIGNATURE_DELAYED = 0x42
} MIDI_SysExNotationSubID;

typedef enum {
  MIDI_SYSEX_DEVICE_MASTER_VOLUME = 0x01,
  MIDI_SYSEX_DEVICE_MASTER_BALANCE = 0x02
} MIDI_SysExDeviceControlSubID;

typedef enum {
  MIDI_SYSEX_SDEXT_MULTIPLE_LOOP_POINTS = 0x01,
  MIDI_SYSEX_SDEXT_LOOP_POINTS_REQUEST = 0x02
} MIDI_SysExSampleDumpExtSubID;

typedef enum {
  MIDI_SYSEX_MTC_SPECIAL = 0x00,
  MIDI_SYSEX_MTC_PUNCH_IN_POINTS = 0x01,
  MIDI_SYSEX_MTC_PUNCH_OUT_POINTS = 0x02,
  MIDI_SYSEX_MTC_DELETE_PUNCH_IN = 0x03,
  MIDI_SYSEX_MTC_DELETE_PUNCH_OUT = 0x04,
  MIDI_SYSEX_MTC_EVENT_START_POINT = 0x05,
  MIDI_SYSEX_MTC_EVENT_STOP_POINT = 0x06,
  MIDI_SYSEX_MTC_EVENT_START_INFO = 0x07,
  MIDI_SYSEX_MTC_EVENT_STOP_INFO = 0x08,
  MIDI_SYSEX_MTC_DELETE_EVENT_START = 0x09,
  MIDI_SYSEX_MTC_DELETE_EVENT_STOP = 0x0A,
  MIDI_SYSEX_MTC_CUE_POINTS = 0x0B,
  MIDI_SYSEX_MTC_CUE_POINTS_INFO = 0x0C,
  MIDI_SYSEX_MTC_DELETE_CUE_POINT = 0x0D,
  MIDI_SYSEX_MTC_EVENT_NAME = 0x0E
} MIDI_SysExMTCSubID;

typedef enum {
  MIDI_SYSEX_RT_MTC_FULL_MESSAGE = 0x01,
  MIDI_SYSEX_RT_MTC_USER_BITS = 0x02
} MIDI_SysExRTMTCSubID;

typedef enum {
  MIDI_SYSEX_CUEING_SPECIAL = 0x00,
  MIDI_SYSEX_CUEING_PUNCH_IN_POINTS = 0x01,
  MIDI_SYSEX_CUEING_PUNCH_OUT_POINTS = 0x02,
  MIDI_SYSEX_CUEING_EVENT_START_POINT = 0x05,
  MIDI_SYSEX_CUEING_EVENT_STOP_POINT = 0x06,
  MIDI_SYSEX_CUEING_EVENT_START_INFO = 0x07,
  MIDI_SYSEX_CUEING_EVENT_STOP_INFO = 0x08,
  MIDI_SYSEX_CUEING_CUE_POINTS = 0x0B,
  MIDI_SYSEX_CUEING_CUE_POINTS_INFO = 0x0C
} MIDI_SysExCueingSubID;

#define MIDI_PITCH_BEND_CENTER 0x2000u

static inline int MIDI_IsChannelMessage(MIDI_MessageType t) {
  return (t >= MIDI_NOTE_OFF) && (t <= MIDI_PITCH_BEND);
}

static inline int MIDI_IsSystemCommon(MIDI_MessageType t) {
  return (t >= MIDI_SYSEX_START) && (t <= MIDI_SYSEX_END);
}

static inline int MIDI_IsRealTime(MIDI_MessageType t) {
  return t >= MIDI_TIMING_CLOCK;
}

static inline int MIDI_IsChannelMode(const MIDI_Message *msg) {
  return (msg->type == MIDI_CONTROL_CHANGE) &&
         (msg->data.control.controller >= MIDI_CC_CHANNEL_MODE_BASE);
}

static uint8_t MIDI_DataBytes(MIDI_MessageType t) {
  switch (t) {
  case MIDI_NOTE_OFF:
  case MIDI_NOTE_ON:
  case MIDI_POLY_KEY_PRESSURE:
  case MIDI_CONTROL_CHANGE:
  case MIDI_PITCH_BEND:
  case MIDI_SONG_POSITION:
    return 2;
  case MIDI_PROGRAM_CHANGE:
  case MIDI_CHANNEL_PRESSURE:
  case MIDI_MTC_QUARTER_FRAME:
  case MIDI_SONG_SELECT:
    return 1;
  default:
    return 0;
  }
}

typedef enum {
  MIDI_DECODER_STATUS,
  MIDI_DECODER_DATA,
  MIDI_DECODER_SYSEX
} MIDI_DecoderState;

typedef struct {
  MIDI_DecoderState state;
  uint8_t runningStatus;
  uint8_t status;
  uint8_t data[2];
  uint8_t count;
  uint8_t overflow;
  MIDI_Message message;
} MIDI_Decoder;

static inline void MIDI_DecoderReset(MIDI_Decoder *dec) {
  dec->state = MIDI_DECODER_STATUS;
  dec->runningStatus = 0;
  dec->status = 0;
  dec->data[0] = 0;
  dec->data[1] = 0;
  dec->count = 0;
  dec->overflow = 0;
  dec->message.type = (MIDI_MessageType)0;
  dec->message.channel = 0;
}

static inline size_t encode_midi(const MIDI_Message *msg, uint8_t *out,
                                 size_t outSize) {
  uint8_t status = (uint8_t)msg->type;

  if (MIDI_IsChannelMessage(msg->type))
    status |= (uint8_t)(msg->channel & 0x0F);

  if (msg->type == MIDI_SYSEX_START) {
#if MIDI_ENABLE_SYSEX
    size_t i;
    if (outSize < (size_t)msg->data.sysEx.length + 2u)
      return 0;
    out[0] = MIDI_SYSEX_START;
    for (i = 0; i < msg->data.sysEx.length; i++)
      out[1 + i] = msg->data.sysEx.data[i];
    out[1 + msg->data.sysEx.length] = MIDI_SYSEX_END;
    return (size_t)msg->data.sysEx.length + 2u;
#else
    return 0; /* SysEx disabled */
#endif
  }

  if (outSize < (size_t)1u + MIDI_DataBytes(msg->type))
    return 0;

  out[0] = status;
  switch (msg->type) {
  case MIDI_NOTE_OFF:
  case MIDI_NOTE_ON:
    out[1] = msg->data.note.note & 0x7F;
    out[2] = msg->data.note.velocity & 0x7F;
    break;
  case MIDI_POLY_KEY_PRESSURE:
    out[1] = msg->data.keyPressure.note & 0x7F;
    out[2] = msg->data.keyPressure.pressure & 0x7F;
    break;
  case MIDI_CONTROL_CHANGE:
    out[1] = msg->data.control.controller & 0x7F;
    out[2] = msg->data.control.value & 0x7F;
    break;
  case MIDI_PROGRAM_CHANGE:
    out[1] = msg->data.program.program & 0x7F;
    break;
  case MIDI_CHANNEL_PRESSURE:
    out[1] = msg->data.channelPressure.pressure & 0x7F;
    break;
  case MIDI_PITCH_BEND:
    out[1] = (uint8_t)(msg->data.pitchBend.value & 0x7F);        /* LSB */
    out[2] = (uint8_t)((msg->data.pitchBend.value >> 7) & 0x7F); /* MSB */
    break;
  case MIDI_MTC_QUARTER_FRAME:
    out[1] = (uint8_t)(((msg->data.quarterFrame.messageType & 0x07) << 4) |
                       (msg->data.quarterFrame.value & 0x0F));
    break;
  case MIDI_SONG_POSITION:
    out[1] = (uint8_t)(msg->data.songPosition.position & 0x7F);        /* LSB */
    out[2] = (uint8_t)((msg->data.songPosition.position >> 7) & 0x7F); /* MSB */
    break;
  case MIDI_SONG_SELECT:
    out[1] = msg->data.songSelect.song & 0x7F;
    break;
  default:
    break;
  }
  return (size_t)1u + MIDI_DataBytes(msg->type);
}

static inline uint8_t MIDI_Decode(MIDI_Decoder *dec, uint8_t byte) {
  if (byte >= MIDI_TIMING_CLOCK) {
    if (byte == MIDI_TIMING_CLOCK || byte == MIDI_START ||
        byte == MIDI_CONTINUE || byte == MIDI_STOP ||
        byte == MIDI_ACTIVE_SENSING || byte == MIDI_SYSTEM_RESET) {
      dec->message.type = (MIDI_MessageType)byte;
      dec->message.channel = 0;
      return 1;
    }
    return 0;
  }

  if (byte & 0x80) {
    if (byte == MIDI_SYSEX_END) {
      if (dec->state == MIDI_DECODER_SYSEX) {
        dec->state = MIDI_DECODER_STATUS;
#if MIDI_ENABLE_SYSEX
        dec->message.type = MIDI_SYSEX_START;
        dec->message.channel = 0;
        return 1;
#else
        return 0;
#endif
      }
      return 0;
    }

    dec->state = MIDI_DECODER_STATUS;

    if (byte == MIDI_SYSEX_START) {
      dec->runningStatus = 0;
#if MIDI_ENABLE_SYSEX
      dec->message.data.sysEx.length = 0;
#endif
      dec->overflow = 0;
      dec->state = MIDI_DECODER_SYSEX;
      return 0;
    }
    if (byte == 0xF4 || byte == 0xF5)
      return 0;

    if (byte < 0xF0)
      dec->runningStatus = byte;
    else
      dec->runningStatus = 0;

    if (byte == MIDI_TUNE_REQUEST) {
      dec->message.type = MIDI_TUNE_REQUEST;
      dec->message.channel = 0;
      return 1;
    }

    dec->status = byte;
    dec->count = 0;
    dec->state = MIDI_DECODER_DATA;
    return 0;
  }

  if (dec->state == MIDI_DECODER_SYSEX) {
#if MIDI_ENABLE_SYSEX
    if (dec->message.data.sysEx.length < MIDI_SYSEX_MAX_DATA) {
      dec->message.data.sysEx.data[dec->message.data.sysEx.length] = byte;
      dec->message.data.sysEx.length++;
    } else {
      dec->overflow = 1;
    }
#endif
    return 0;
  }

  if (dec->state == MIDI_DECODER_STATUS) {
    if (dec->runningStatus == 0)
      return 0;
    dec->status = dec->runningStatus;
    dec->count = 0;
    dec->state = MIDI_DECODER_DATA;
  }

  dec->data[dec->count] = byte;
  dec->count++;

  {
    MIDI_MessageType t = (dec->status < 0xF0)
                             ? (MIDI_MessageType)(dec->status & 0xF0)
                             : (MIDI_MessageType)dec->status;
    if (dec->count < MIDI_DataBytes(t))
      return 0;

    dec->message.type = t;
    dec->message.channel = (dec->status < 0xF0) ? (dec->status & 0x0F) : 0;

    switch (t) {
    case MIDI_NOTE_OFF:
    case MIDI_NOTE_ON:
      dec->message.data.note.note = dec->data[0];
      dec->message.data.note.velocity = dec->data[1];
      if (dec->message.type == MIDI_NOTE_ON && dec->data[1] == 0)
        dec->message.type = MIDI_NOTE_OFF;
      break;
    case MIDI_POLY_KEY_PRESSURE:
      dec->message.data.keyPressure.note = dec->data[0];
      dec->message.data.keyPressure.pressure = dec->data[1];
      break;
    case MIDI_CONTROL_CHANGE:
      dec->message.data.control.controller = dec->data[0];
      dec->message.data.control.value = dec->data[1];
      break;
    case MIDI_PROGRAM_CHANGE:
      dec->message.data.program.program = dec->data[0];
      break;
    case MIDI_CHANNEL_PRESSURE:
      dec->message.data.channelPressure.pressure = dec->data[0];
      break;
    case MIDI_PITCH_BEND:
      dec->message.data.pitchBend.value =
          (uint16_t)dec->data[0] | ((uint16_t)dec->data[1] << 7);
      break;
    case MIDI_MTC_QUARTER_FRAME:
      dec->message.data.quarterFrame.messageType = (dec->data[0] >> 4) & 0x07;
      dec->message.data.quarterFrame.value = dec->data[0] & 0x0F;
      break;
    case MIDI_SONG_POSITION:
      dec->message.data.songPosition.position =
          (uint16_t)dec->data[0] | ((uint16_t)dec->data[1] << 7);
      break;
    case MIDI_SONG_SELECT:
      dec->message.data.songSelect.song = dec->data[0];
      break;
    default:
      break;
    }
    dec->state = MIDI_DECODER_STATUS;
    return 1;
  }
}

#define MIDI_EPIN_ADDRESS 0x81
#define MIDI_EPIN_SIZE 0x40
#define MIDI_EPOUT_ADDRESS 0x01
#define MIDI_EPOUT_SIZE 0x40
#define USB_MIDI_CLASS_DESC_SHIFT 18
#define USB_MIDI_DESC_SIZE 7
#define USB_MIDI_INTERFACE_DESC_SIZE                                           \
  (MIDI_IN_PORTS_NUM * 16 + MIDI_OUT_PORTS_NUM * 16 + 33)
#define USB_MIDI_CONFIG_DESC_SIZE                                              \
  (USB_MIDI_INTERFACE_DESC_SIZE + USB_MIDI_CLASS_DESC_SHIFT)
#define MIDI_DESCRIPTOR_TYPE 0x21
#define MIDI_REQ_SET_PROTOCOL 0x0B
#define MIDI_REQ_GET_PROTOCOL 0x03
#define MIDI_REQ_SET_IDLE 0x0A
#define MIDI_REQ_GET_IDLE 0x02
#define MIDI_JACK_1 0x01
#define MIDI_JACK_2 0x02
#define MIDI_JACK_3 0x03
#define MIDI_JACK_4 0x04
#define MIDI_JACK_5 0x05
#define MIDI_JACK_6 0x06
#define MIDI_JACK_7 0x07
#define MIDI_JACK_8 0x08
#define MIDI_JACK_9 0x09
#define MIDI_JACK_10 0x0a
#define MIDI_JACK_11 0x0b
#define MIDI_JACK_12 0x0c
#define MIDI_JACK_13 0x0d
#define MIDI_JACK_14 0x0e
#define MIDI_JACK_15 0x0f
#define MIDI_JACK_16 0x10
#define MIDI_JACK_17 (MIDI_IN_PORTS_NUM * 2 + 0x01)
#define MIDI_JACK_18 (MIDI_IN_PORTS_NUM * 2 + 0x02)
#define MIDI_JACK_19 (MIDI_IN_PORTS_NUM * 2 + 0x03)
#define MIDI_JACK_20 (MIDI_IN_PORTS_NUM * 2 + 0x04)
#define MIDI_JACK_21 (MIDI_IN_PORTS_NUM * 2 + 0x05)
#define MIDI_JACK_22 (MIDI_IN_PORTS_NUM * 2 + 0x06)
#define MIDI_JACK_23 (MIDI_IN_PORTS_NUM * 2 + 0x07)
#define MIDI_JACK_24 (MIDI_IN_PORTS_NUM * 2 + 0x08)
#define MIDI_JACK_25 (MIDI_IN_PORTS_NUM * 2 + 0x09)
#define MIDI_JACK_26 (MIDI_IN_PORTS_NUM * 2 + 0x0a)
#define MIDI_JACK_27 (MIDI_IN_PORTS_NUM * 2 + 0x0b)
#define MIDI_JACK_28 (MIDI_IN_PORTS_NUM * 2 + 0x0c)
#define MIDI_JACK_29 (MIDI_IN_PORTS_NUM * 2 + 0x0d)
#define MIDI_JACK_30 (MIDI_IN_PORTS_NUM * 2 + 0x0e)
#define MIDI_JACK_31 (MIDI_IN_PORTS_NUM * 2 + 0x0f)
#define MIDI_JACK_32 (MIDI_IN_PORTS_NUM * 2 + 0x10)

#define MIDI_EPIN_ADDR 0x81
#define MIDI_EPIN_SIZE 0x40

#define MIDI_EPOUT_ADDR 0x01
#define MIDI_EPOUT_SIZE 0x40

#define USB_MIDI_CLASS_DESC_SHIFT 18
#define USB_MIDI_DESC_SIZE 7
#define USB_MIDI_INTERFACE_DESC_SIZE                                           \
  (MIDI_IN_PORTS_NUM * 16 + MIDI_OUT_PORTS_NUM * 16 + 33)
#define USB_MIDI_CONFIG_DESC_SIZE                                              \
  (USB_MIDI_INTERFACE_DESC_SIZE + USB_MIDI_CLASS_DESC_SHIFT)

#define MIDI_DESCRIPTOR_TYPE 0x21

#define MIDI_REQ_SET_PROTOCOL 0x0B
#define MIDI_REQ_GET_PROTOCOL 0x03

#define MIDI_REQ_SET_IDLE 0x0A
#define MIDI_REQ_GET_IDLE 0x02

#define MIDI_JACK_1 0x01
#define MIDI_JACK_2 0x02
#define MIDI_JACK_3 0x03
#define MIDI_JACK_4 0x04
#define MIDI_JACK_5 0x05
#define MIDI_JACK_6 0x06
#define MIDI_JACK_7 0x07
#define MIDI_JACK_8 0x08
#define MIDI_JACK_9 0x09
#define MIDI_JACK_10 0x0a
#define MIDI_JACK_11 0x0b
#define MIDI_JACK_12 0x0c
#define MIDI_JACK_13 0x0d
#define MIDI_JACK_14 0x0e
#define MIDI_JACK_15 0x0f
#define MIDI_JACK_16 0x10
#define MIDI_JACK_17 (MIDI_IN_PORTS_NUM * 2 + 0x01)
#define MIDI_JACK_18 (MIDI_IN_PORTS_NUM * 2 + 0x02)
#define MIDI_JACK_19 (MIDI_IN_PORTS_NUM * 2 + 0x03)
#define MIDI_JACK_20 (MIDI_IN_PORTS_NUM * 2 + 0x04)
#define MIDI_JACK_21 (MIDI_IN_PORTS_NUM * 2 + 0x05)
#define MIDI_JACK_22 (MIDI_IN_PORTS_NUM * 2 + 0x06)
#define MIDI_JACK_23 (MIDI_IN_PORTS_NUM * 2 + 0x07)
#define MIDI_JACK_24 (MIDI_IN_PORTS_NUM * 2 + 0x08)
#define MIDI_JACK_25 (MIDI_IN_PORTS_NUM * 2 + 0x09)
#define MIDI_JACK_26 (MIDI_IN_PORTS_NUM * 2 + 0x0a)
#define MIDI_JACK_27 (MIDI_IN_PORTS_NUM * 2 + 0x0b)
#define MIDI_JACK_28 (MIDI_IN_PORTS_NUM * 2 + 0x0c)
#define MIDI_JACK_29 (MIDI_IN_PORTS_NUM * 2 + 0x0d)
#define MIDI_JACK_30 (MIDI_IN_PORTS_NUM * 2 + 0x0e)
#define MIDI_JACK_31 (MIDI_IN_PORTS_NUM * 2 + 0x0f)
#define MIDI_JACK_32 (MIDI_IN_PORTS_NUM * 2 + 0x10)

#define MIDI_CIN_NOTE_ON 0x9
#define MIDI_CIN_NOTE_OFF 0x8
#define MIDI_CIN_CONTROL_CHANGE 0xB

#define USB_REQ_GET_STATUS 0x00U
#define USB_REQ_CLEAR_FEATURE 0x01U
#define USB_REQ_SET_FEATURE 0x03U
#define USB_REQ_SET_ADDRESS 0x05U
#define USB_REQ_GET_DESCRIPTOR 0x06U
#define USB_REQ_SET_DESCRIPTOR 0x07U
#define USB_REQ_GET_CONFIGURATION 0x08U
#define USB_REQ_SET_CONFIGURATION 0x09U
#define USB_REQ_GET_INTERFACE 0x0AU
#define USB_REQ_SET_INTERFACE 0x0BU
#define USB_REQ_SYNCH_FRAME 0x0CU

#define USB_DESC_TYPE_DEVICE 0x01U
#define USB_DESC_TYPE_CONFIGURATION 0x02U
#define USB_DESC_TYPE_STRING 0x03U
#define USB_DESC_TYPE_INTERFACE 0x04U
#define USB_DESC_TYPE_ENDPOINT 0x05U
#define USB_DESC_TYPE_DEVICE_QUALIFIER 0x06U
#define USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION 0x07U
#define USB_DESC_TYPE_IAD 0x0BU
#define USB_DESC_TYPE_BOS 0x0FU
#define USB_LEN_DEV_QUALIFIER_DESC 0x0AU
#define USB_LEN_DEV_DESC 0x12U
#define USB_LEN_CFG_DESC 0x09U
#define USB_LEN_IF_DESC 0x09U
#define USB_LEN_EP_DESC 0x07U
#define USB_LEN_OTG_DESC 0x03U
#define USB_LEN_LANGID_STR_DESC 0x04U
#define USB_LEN_OTHER_SPEED_DESC_SIZ 0x09U
#define USBD_IDX_LANGID_STR 0x00U
#define USBD_IDX_MFC_STR 0x01U
#define USBD_IDX_PRODUCT_STR 0x02U
#define USBD_IDX_SERIAL_STR 0x03U
#define USBD_IDX_CONFIG_STR 0x04U
#define USBD_IDX_INTERFACE_STR 0x05U

#define USBD_IDX_JACK_STR_BASE 0x04U
#define MIDI_JACK_NAME_COUNT                                                   \
  ((MIDI_IN_PORTS_NUM > MIDI_OUT_PORTS_NUM) ? MIDI_IN_PORTS_NUM                \
                                            : MIDI_OUT_PORTS_NUM)

typedef struct {
  uint16_t vendor_id;
  uint16_t product_id;
  uint16_t bcd_device;
  const char *manufacturer;
  const char *product;
  const char *serial;
  uint16_t lang_id;
  const char *port_names[8];
} USB_DeviceConfig_t;

static uint8_t USB_BuildStringDesc(uint8_t *buffer, const char *str) {
  if (!str || !buffer)
    return 0;

  size_t char_count = strlen(str);
  if (char_count > 31)
    char_count = 31;

  buffer[0] = (uint8_t)(2 + (char_count * 2));
  buffer[1] = USB_DESC_TYPE_STRING;

  for (size_t i = 0; i < char_count; i++) {
    buffer[2 + (i * 2)] = (uint8_t)str[i];
    buffer[2 + (i * 2) + 1] = 0x00;
  }

  return buffer[0];
}

__ALIGN_BEGIN const uint8_t dev_qual[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END = {
    USB_LEN_DEV_QUALIFIER_DESC,
    USB_DESC_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x01,
    0x00,
};

__ALIGN_BEGIN static uint8_t cfg_desc[USB_MIDI_CONFIG_DESC_SIZE] __ALIGN_END = {
    0x09,                        /* bLength: Configuration Descriptor size */
    USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
    (USB_MIDI_CONFIG_DESC_SIZE & 0xFF),
    (USB_MIDI_CONFIG_DESC_SIZE >> 8), /*wTotalLength: total block, LE16 */
    0x01,                             /*bNumInterfaces: 1 interface*/
    0x01, /*bConfigurationValue: ID of this configuration. */
    0x00, /*iConfiguration: Index of string descriptor describing the
             configuration (Unused.)*/
    0x80, /*bmAttributes: Bus Powered device, not Self Powered, no Remote
             wakeup capability. */
    0xFA, /*MaxPower 500 mA: this current is used for detecting Vbus*/

    /************** MIDI Adapter Standard MS Interface Descriptor
     ****************/
    0x09,                    /*bLength: Interface Descriptor size*/
    USB_DESC_TYPE_INTERFACE, /*bDescriptorType: Interface descriptor type*/
    0x00,                    /*bInterfaceNumber: Index of this interface.*/
    0x00,                    /*bAlternateSetting: Alternate setting*/
    0x02,                    /*bNumEndpoints*/
    0x01,                    /*bInterfaceClass: AUDIO*/
    0x03,                    /*bInterfaceSubClass : MIDISTREAMING*/
    0x00,                    /*nInterfaceProtocol : Unused*/
    0x00,                    /*iInterface: Unused*/

    /******************** MIDI Adapter Class-specific MS Interface
       Descriptor ********************/
    /* USB_MIDI_CLASS_DESC_SHIFT */
    0x07, /*bLength: Descriptor size*/
    0x24, /*bDescriptorType: CS_INTERFACE descriptor*/
    0x01, /*bDescriptorSubtype: MS_HEADER subtype*/
    0x00,
    0x01, /*BcdADC: Revision of this class specification*/
    (USB_MIDI_INTERFACE_DESC_SIZE & 0xFF),
    (USB_MIDI_INTERFACE_DESC_SIZE >>
     8), /*wTotalLength: class-specific total, LE16*/

#if MIDI_IN_PORTS_NUM >= 1
    /******************** MIDI Adapter MIDI IN Jack Descriptor (External)
     ********************/
    0x06,        /*bLength: Size of this descriptor, in bytes*/
    0x24,        /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x02,        /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
    0x02,        /*bJackType: EXTERNAL.*/
    MIDI_JACK_1, /*bJackID: ID of this Jack.*/
    0x00,        /*iJack: Unused.*/

    /******************** MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
     ********************/
    0x09,        /*bLength: Size of this descriptor, in bytes*/
    0x24,        /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x03,        /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
    0x01,        /*bJackType: EMBEDDED*/
    MIDI_JACK_2, /*bJackID: ID of this Jack.*/
    0x01,        /*bNrInputPins: Number of Input Pins of this Jack.*/
    MIDI_JACK_1, /*BaSourceID(1): ID of the Entity to which this Pin is
                    connected.*/
    0x01,        /*BaSourcePin(1): Output Pin number of the Entity to which this
                    Input Pin is connected.*/
    (USBD_IDX_JACK_STR_BASE + 0), /*iJack: Unused.*/
#endif

#if MIDI_IN_PORTS_NUM >= 2
    /******************** MIDI Adapter MIDI IN Jack Descriptor (External)
     ********************/
    0x06,        /*bLength: Size of this descriptor, in bytes*/
    0x24,        /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x02,        /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
    0x02,        /*bJackType: EXTERNAL.*/
    MIDI_JACK_3, /*bJackID: ID of this Jack.*/
    0x00,        /*iJack: Unused.*/

    /******************** MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
     ********************/
    0x09,        /*bLength: Size of this descriptor, in bytes*/
    0x24,        /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x03,        /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
    0x01,        /*bJackType: EMBEDDED*/
    MIDI_JACK_4, /*bJackID: ID of this Jack.*/
    0x01,        /*bNrInputPins: Number of Input Pins of this Jack.*/
    MIDI_JACK_3, /*BaSourceID(1): ID of the Entity to which this Pin is
                    connected.*/
    0x01,        /*BaSourcePin(1): Output Pin number of the Entity to which this
                    Input Pin is connected.*/
    (USBD_IDX_JACK_STR_BASE + 1), /*iJack: Unused.*/
#endif

#if MIDI_IN_PORTS_NUM >= 3
    /******************** MIDI Adapter MIDI IN Jack Descriptor (External)
     ********************/
    0x06,        /*bLength: Size of this descriptor, in bytes*/
    0x24,        /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x02,        /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
    0x02,        /*bJackType: EXTERNAL.*/
    MIDI_JACK_5, /*bJackID: ID of this Jack.*/
    0x00,        /*iJack: Unused.*/

    /******************** MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
     ********************/
    0x09,        /*bLength: Size of this descriptor, in bytes*/
    0x24,        /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x03,        /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
    0x01,        /*bJackType: EMBEDDED*/
    MIDI_JACK_6, /*bJackID: ID of this Jack.*/
    0x01,        /*bNrInputPins: Number of Input Pins of this Jack.*/
    MIDI_JACK_5, /*BaSourceID(1): ID of the Entity to which this Pin is
                    connected.*/
    0x01,        /*BaSourcePin(1): Output Pin number of the Entity to which this
                    Input Pin is connected.*/
    (USBD_IDX_JACK_STR_BASE + 2), /*iJack: Unused.*/
#endif

#if MIDI_IN_PORTS_NUM >= 4
    /******************** MIDI Adapter MIDI IN Jack Descriptor (External)
     ********************/
    0x06,        /*bLength: Size of this descriptor, in bytes*/
    0x24,        /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x02,        /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
    0x02,        /*bJackType: EXTERNAL.*/
    MIDI_JACK_7, /*bJackID: ID of this Jack.*/
    0x00,        /*iJack: Unused.*/

    /******************** MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
     ********************/
    0x09,        /*bLength: Size of this descriptor, in bytes*/
    0x24,        /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x03,        /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
    0x01,        /*bJackType: EMBEDDED*/
    MIDI_JACK_8, /*bJackID: ID of this Jack.*/
    0x01,        /*bNrInputPins: Number of Input Pins of this Jack.*/
    MIDI_JACK_7, /*BaSourceID(1): ID of the Entity to which this Pin is
                    connected.*/
    0x01,        /*BaSourcePin(1): Output Pin number of the Entity to which this
                    Input Pin is connected.*/
    (USBD_IDX_JACK_STR_BASE + 3), /*iJack: Unused.*/
#endif

#if MIDI_IN_PORTS_NUM >= 5
    /******************** MIDI Adapter MIDI IN Jack Descriptor (External)
     ********************/
    0x06,        /*bLength: Size of this descriptor, in bytes*/
    0x24,        /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x02,        /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
    0x02,        /*bJackType: EXTERNAL.*/
    MIDI_JACK_9, /*bJackID: ID of this Jack.*/
    0x00,        /*iJack: Unused.*/

    /******************** MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
     ********************/
    0x09,         /*bLength: Size of this descriptor, in bytes*/
    0x24,         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x03,         /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
    0x01,         /*bJackType: EMBEDDED*/
    MIDI_JACK_10, /*bJackID: ID of this Jack.*/
    0x01,         /*bNrInputPins: Number of Input Pins of this Jack.*/
    MIDI_JACK_9,  /*BaSourceID(1): ID of the Entity to which this Pin is
                     connected.*/
    0x01, /*BaSourcePin(1): Output Pin number of the Entity to which this
             Input Pin is connected.*/
    (USBD_IDX_JACK_STR_BASE + 4), /*iJack: Unused.*/
#endif

#if MIDI_IN_PORTS_NUM >= 6
    /******************** MIDI Adapter MIDI IN Jack Descriptor (External)
     ********************/
    0x06,         /*bLength: Size of this descriptor, in bytes*/
    0x24,         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x02,         /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
    0x02,         /*bJackType: EXTERNAL.*/
    MIDI_JACK_11, /*bJackID: ID of this Jack.*/
    0x00,         /*iJack: Unused.*/

    /******************** MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
     ********************/
    0x09,         /*bLength: Size of this descriptor, in bytes*/
    0x24,         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x03,         /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
    0x01,         /*bJackType: EMBEDDED*/
    MIDI_JACK_12, /*bJackID: ID of this Jack.*/
    0x01,         /*bNrInputPins: Number of Input Pins of this Jack.*/
    MIDI_JACK_11, /*BaSourceID(1): ID of the Entity to which this Pin is
                     connected.*/
    0x01, /*BaSourcePin(1): Output Pin number of the Entity to which this
             Input Pin is connected.*/
    (USBD_IDX_JACK_STR_BASE + 5), /*iJack: Unused.*/
#endif

#if MIDI_IN_PORTS_NUM >= 7
    /******************** MIDI Adapter MIDI IN Jack Descriptor (External)
     ********************/
    0x06,         /*bLength: Size of this descriptor, in bytes*/
    0x24,         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x02,         /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
    0x02,         /*bJackType: EXTERNAL.*/
    MIDI_JACK_13, /*bJackID: ID of this Jack.*/
    0x00,         /*iJack: Unused.*/

    /******************** MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
     ********************/
    0x09,         /*bLength: Size of this descriptor, in bytes*/
    0x24,         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x03,         /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
    0x01,         /*bJackType: EMBEDDED*/
    MIDI_JACK_14, /*bJackID: ID of this Jack.*/
    0x01,         /*bNrInputPins: Number of Input Pins of this Jack.*/
    MIDI_JACK_13, /*BaSourceID(1): ID of the Entity to which this Pin is
                     connected.*/
    0x01, /*BaSourcePin(1): Output Pin number of the Entity to which this
             Input Pin is connected.*/
    (USBD_IDX_JACK_STR_BASE + 6), /*iJack: Unused.*/
#endif

#if MIDI_IN_PORTS_NUM >= 8
    /******************** MIDI Adapter MIDI IN Jack Descriptor (External)
     ********************/
    0x06,         /*bLength: Size of this descriptor, in bytes*/
    0x24,         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x02,         /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
    0x02,         /*bJackType: EXTERNAL.*/
    MIDI_JACK_15, /*bJackID: ID of this Jack.*/
    0x00,         /*iJack: Unused.*/

    /******************** MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
     ********************/
    0x09,         /*bLength: Size of this descriptor, in bytes*/
    0x24,         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x03,         /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
    0x01,         /*bJackType: EMBEDDED*/
    MIDI_JACK_16, /*bJackID: ID of this Jack.*/
    0x01,         /*bNrInputPins: Number of Input Pins of this Jack.*/
    MIDI_JACK_15, /*BaSourceID(1): ID of the Entity to which this Pin is
                     connected.*/
    0x01, /*BaSourcePin(1): Output Pin number of the Entity to which this
             Input Pin is connected.*/
    (USBD_IDX_JACK_STR_BASE + 7), /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 1
    /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded)
     ********************/
    0x06,                         /*bLength: Size of this descriptor, in bytes*/
    0x24,                         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x02,                         /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
    0x01,                         /*bJackType: EMBEDDED*/
    MIDI_JACK_17,                 /*bJackID: ID of this Jack.*/
    (USBD_IDX_JACK_STR_BASE + 0), /*iJack: Unused.*/

    /******************** MIDI Adapter MIDI OUT Jack Descriptor (External)
     ********************/
    0x09,         /*bLength: Size of this descriptor, in bytes*/
    0x24,         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x03,         /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
    0x02,         /*bJackType: EXTERNAL.*/
    MIDI_JACK_18, /*bJackID: ID of this Jack.*/
    0x01,         /*bNrInputPins: Number of Input Pins of this Jack.*/
    MIDI_JACK_17, /*BaSourceID(1): ID of the Entity to which this Pin is
                     connected.*/
    0x01, /*BaSourcePin(1): Output Pin number of the Entity to which this
             Input Pin is connected.*/
    0x00, /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 2
    /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded)
     ********************/
    0x06,                         /*bLength: Size of this descriptor, in bytes*/
    0x24,                         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x02,                         /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
    0x01,                         /*bJackType: EMBEDDED*/
    MIDI_JACK_19,                 /*bJackID: ID of this Jack.*/
    (USBD_IDX_JACK_STR_BASE + 1), /*iJack: Unused.*/

    /******************** MIDI Adapter MIDI OUT Jack Descriptor (External)
     ********************/
    0x09,         /*bLength: Size of this descriptor, in bytes*/
    0x24,         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x03,         /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
    0x02,         /*bJackType: EXTERNAL.*/
    MIDI_JACK_20, /*bJackID: ID of this Jack.*/
    0x01,         /*bNrInputPins: Number of Input Pins of this Jack.*/
    MIDI_JACK_19, /*BaSourceID(1): ID of the Entity to which this Pin is
                     connected.*/
    0x01, /*BaSourcePin(1): Output Pin number of the Entity to which this
             Input Pin is connected.*/
    0x00, /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 3
    /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded)
     ********************/
    0x06,                         /*bLength: Size of this descriptor, in bytes*/
    0x24,                         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x02,                         /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
    0x01,                         /*bJackType: EMBEDDED*/
    MIDI_JACK_21,                 /*bJackID: ID of this Jack.*/
    (USBD_IDX_JACK_STR_BASE + 2), /*iJack: Unused.*/

    /******************** MIDI Adapter MIDI OUT Jack Descriptor (External)
     ********************/
    0x09,         /*bLength: Size of this descriptor, in bytes*/
    0x24,         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x03,         /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
    0x02,         /*bJackType: EXTERNAL.*/
    MIDI_JACK_22, /*bJackID: ID of this Jack.*/
    0x01,         /*bNrInputPins: Number of Input Pins of this Jack.*/
    MIDI_JACK_21, /*BaSourceID(1): ID of the Entity to which this Pin is
                     connected.*/
    0x01, /*BaSourcePin(1): Output Pin number of the Entity to which this
             Input Pin is connected.*/
    0x00, /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 4
    /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded)
     ********************/
    0x06,                         /*bLength: Size of this descriptor, in bytes*/
    0x24,                         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x02,                         /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
    0x01,                         /*bJackType: EMBEDDED*/
    MIDI_JACK_23,                 /*bJackID: ID of this Jack.*/
    (USBD_IDX_JACK_STR_BASE + 3), /*iJack: Unused.*/

    /******************** MIDI Adapter MIDI OUT Jack Descriptor (External)
     ********************/
    0x09,         /*bLength: Size of this descriptor, in bytes*/
    0x24,         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x03,         /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
    0x02,         /*bJackType: EXTERNAL.*/
    MIDI_JACK_24, /*bJackID: ID of this Jack.*/
    0x01,         /*bNrInputPins: Number of Input Pins of this Jack.*/
    MIDI_JACK_23, /*BaSourceID(1): ID of the Entity to which this Pin is
                     connected.*/
    0x01, /*BaSourcePin(1): Output Pin number of the Entity to which this
             Input Pin is connected.*/
    0x00, /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 5
    /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded)
     ********************/
    0x06,                         /*bLength: Size of this descriptor, in bytes*/
    0x24,                         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x02,                         /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
    0x01,                         /*bJackType: EMBEDDED*/
    MIDI_JACK_25,                 /*bJackID: ID of this Jack.*/
    (USBD_IDX_JACK_STR_BASE + 4), /*iJack: Unused.*/

    /******************** MIDI Adapter MIDI OUT Jack Descriptor (External)
     ********************/
    0x09,         /*bLength: Size of this descriptor, in bytes*/
    0x24,         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x03,         /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
    0x02,         /*bJackType: EXTERNAL.*/
    MIDI_JACK_26, /*bJackID: ID of this Jack.*/
    0x01,         /*bNrInputPins: Number of Input Pins of this Jack.*/
    MIDI_JACK_25, /*BaSourceID(1): ID of the Entity to which this Pin is
                     connected.*/
    0x01, /*BaSourcePin(1): Output Pin number of the Entity to which this
             Input Pin is connected.*/
    0x00, /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 6
    /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded)
     ********************/
    0x06,                         /*bLength: Size of this descriptor, in bytes*/
    0x24,                         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x02,                         /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
    0x01,                         /*bJackType: EMBEDDED*/
    MIDI_JACK_27,                 /*bJackID: ID of this Jack.*/
    (USBD_IDX_JACK_STR_BASE + 5), /*iJack: Unused.*/

    /******************** MIDI Adapter MIDI OUT Jack Descriptor (External)
     ********************/
    0x09,         /*bLength: Size of this descriptor, in bytes*/
    0x24,         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x03,         /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
    0x02,         /*bJackType: EXTERNAL.*/
    MIDI_JACK_28, /*bJackID: ID of this Jack.*/
    0x01,         /*bNrInputPins: Number of Input Pins of this Jack.*/
    MIDI_JACK_27, /*BaSourceID(1): ID of the Entity to which this Pin is
                     connected.*/
    0x01, /*BaSourcePin(1): Output Pin number of the Entity to which this
             Input Pin is connected.*/
    0x00, /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 7
    /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded)
     ********************/
    0x06,                         /*bLength: Size of this descriptor, in bytes*/
    0x24,                         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x02,                         /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
    0x01,                         /*bJackType: EMBEDDED*/
    MIDI_JACK_29,                 /*bJackID: ID of this Jack.*/
    (USBD_IDX_JACK_STR_BASE + 6), /*iJack: Unused.*/

    /******************** MIDI Adapter MIDI OUT Jack Descriptor (External)
     ********************/
    0x09,         /*bLength: Size of this descriptor, in bytes*/
    0x24,         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x03,         /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
    0x02,         /*bJackType: EXTERNAL.*/
    MIDI_JACK_30, /*bJackID: ID of this Jack.*/
    0x01,         /*bNrInputPins: Number of Input Pins of this Jack.*/
    MIDI_JACK_29, /*BaSourceID(1): ID of the Entity to which this Pin is
                     connected.*/
    0x01, /*BaSourcePin(1): Output Pin number of the Entity to which this
             Input Pin is connected.*/
    0x00, /*iJack: Unused.*/
#endif

#if MIDI_OUT_PORTS_NUM >= 8
    /******************** MIDI Adapter MIDI IN Jack Descriptor (Embedded)
     ********************/
    0x06,                         /*bLength: Size of this descriptor, in bytes*/
    0x24,                         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x02,                         /*bDescriptorSubtype: MIDI_IN_JACK subtype*/
    0x01,                         /*bJackType: EMBEDDED*/
    MIDI_JACK_31,                 /*bJackID: ID of this Jack.*/
    (USBD_IDX_JACK_STR_BASE + 7), /*iJack: Unused.*/

    /******************** MIDI Adapter MIDI OUT Jack Descriptor (External)
     ********************/
    0x09,         /*bLength: Size of this descriptor, in bytes*/
    0x24,         /*bDescriptorType: CS_INTERFACE descriptor.*/
    0x03,         /*bDescriptorSubtype: MIDI_OUT_JACK subtype*/
    0x02,         /*bJackType: EXTERNAL.*/
    MIDI_JACK_32, /*bJackID: ID of this Jack.*/
    0x01,         /*bNrInputPins: Number of Input Pins of this Jack.*/
    MIDI_JACK_31, /*BaSourceID(1): ID of the Entity to which this Pin is
                     connected.*/
    0x01, /*BaSourcePin(1): Output Pin number of the Entity to which this
             Input Pin is connected.*/
    0x00, /*iJack: Unused.*/
#endif

    /******************** MIDI Adapter Standard Bulk OUT Endpoint Descriptor
     ********************/
    0x09,                   /*bLength: Size of this descriptor, in bytes*/
    USB_DESC_TYPE_ENDPOINT, /*bDescriptorType: ENDPOINT descriptor.*/
    MIDI_EPOUT_ADDR,        /*bEndpointAddress: OUT Endpoint 1.*/
    0x02,                   /*bmAttributes: Bulk, not shared.*/
    MIDI_EPOUT_SIZE,
    0x00, /*wMaxPacketSize*/
    0x00, /*bInterval: Ignored for Bulk. Set to zero.*/
    0x00, /*bRefresh: Unused.*/
    0x00, /*bSynchAddress: Unused.*/

    /******************** MIDI Adapter Class-specific Bulk OUT Endpoint
       Descriptor ********************/
    (4 + MIDI_OUT_PORTS_NUM), /*bLength: Size of this descriptor, in bytes*/
    0x25,                     /*bDescriptorType: CS_ENDPOINT descriptor*/
    0x01,                     /*bDescriptorSubtype: MS_GENERAL subtype.*/
    MIDI_OUT_PORTS_NUM,       /*bNumEmbMIDIJack: Number of embedded MIDI IN
                                 Jacks.*/
#if MIDI_OUT_PORTS_NUM >= 1
    MIDI_JACK_17, /*BaAssocJackID(1): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 2
    MIDI_JACK_19, /*BaAssocJackID(2): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 3
    MIDI_JACK_21, /*BaAssocJackID(3): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 4
    MIDI_JACK_23, /*BaAssocJackID(4): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 5
    MIDI_JACK_25, /*BaAssocJackID(5): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 6
    MIDI_JACK_27, /*BaAssocJackID(6): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 7
    MIDI_JACK_29, /*BaAssocJackID(7): ID of the Embedded MIDI IN Jack.*/
#endif
#if MIDI_OUT_PORTS_NUM >= 8
    MIDI_JACK_31, /*BaAssocJackID(8): ID of the Embedded MIDI IN Jack.*/
#endif

    /******************** MIDI Adapter Standard Bulk IN Endpoint Descriptor
     ********************/
    0x09,                   /*bLength: Size of this descriptor, in bytes*/
    USB_DESC_TYPE_ENDPOINT, /*bDescriptorType: ENDPOINT descriptor.*/
    MIDI_EPIN_ADDR,         /*bEndpointAddress: IN Endpoint 1.*/
    0x02,                   /*bmAttributes: Bulk, not shared.*/
    MIDI_EPIN_SIZE,
    0x00, /*wMaxPacketSize*/
    0x00, /*bInterval: Ignored for Bulk. Set to zero.*/
    0x00, /*bRefresh: Unused.*/
    0x00, /*bSynchAddress: Unused.*/

    /******************** MIDI Adapter Class-specific Bulk IN Endpoint
       Descriptor ********************/
    (4 + MIDI_IN_PORTS_NUM), /*bLength: Size of this descriptor, in bytes*/
    0x25,                    /*bDescriptorType: CS_ENDPOINT descriptor*/
    0x01,                    /*bDescriptorSubtype: MS_GENERAL subtype.*/
    MIDI_IN_PORTS_NUM,       /*bNumEmbMIDIJack: Number of embedded MIDI OUT
                                Jacks.*/
#if MIDI_IN_PORTS_NUM >= 1
    MIDI_JACK_2, /*BaAssocJackID(1): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 2
    MIDI_JACK_4, /*BaAssocJackID(2): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 3
    MIDI_JACK_6, /*BaAssocJackID(3): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 4
    MIDI_JACK_8, /*BaAssocJackID(4): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 5
    MIDI_JACK_10, /*BaAssocJackID(5): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 6
    MIDI_JACK_12, /*BaAssocJackID(6): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 7
    MIDI_JACK_14, /*BaAssocJackID(7): ID of the Embedded MIDI OUT Jack.*/
#endif
#if MIDI_IN_PORTS_NUM >= 8
    MIDI_JACK_16, /*BaAssocJackID(8): ID of the Embedded MIDI OUT Jack.*/
#endif
};

typedef enum {
  USBD_STATE_DEFAULT = 0x01,
  USBD_STATE_ADDRESSED = 0x02,
  USBD_STATE_CONFIGURED = 0x03,
  USBD_STATE_SUSPENDED = 0x04
} USBD_StateTypeDef;

typedef struct {
  struct {
    uint8_t dev_desc[USB_LEN_DEV_DESC];
    uint8_t lang_id_desc[4];
    uint8_t manufacturer_desc[64];
    uint8_t product_desc[64];
    uint8_t serial_desc[64];
    uint8_t jack_desc[8][64];
  } descriptors;

  volatile USBD_StateTypeDef device_state;
  uint32_t Protocol;
  uint32_t IdleState;
  uint32_t AltSetting;
  MIDI_StateTypeDef midi_state;
  PCD_HandleTypeDef *hpcd;

} USBD_MIDI_HandleTypeDef;

void USBD_MIDI_InitDescriptors(USBD_MIDI_HandleTypeDef *hmidi,
                               const USB_DeviceConfig_t *cfg) {
  if (!hmidi)
    return;

  if (cfg == NULL) {
    static const USB_DeviceConfig_t defaults = {
        .vendor_id = 0x1209,    /* pid.codes community test VID */
        .product_id = 0x0001,   /* test PID */
        .bcd_device = 0x0100,   /* release 1.00 */
        .manufacturer = "neo",  /* Manufacturer String */
        .product = "neo synth", /* Product String */
        .serial = "0001",       /* Serial Number String */
        .lang_id = 0x0409,      /* English (US) */
        .port_names = {NULL},
    };
    cfg = &defaults;
  }

  const uint16_t vid = cfg->vendor_id;
  const uint16_t pid = cfg->product_id;
  const uint16_t bcd = cfg->bcd_device;
  const uint16_t lang = cfg->lang_id;

  uint8_t dev_temp[USB_LEN_DEV_DESC] = {
      USB_LEN_DEV_DESC,
      USB_DESC_TYPE_DEVICE,
      0x00,
      0x02, // bcdUSB = 2.00
      0x00,
      0x00,
      0x00, // Class, SubClass, Protocol
      0x40, // bMaxPacketSize0 = 64
      (uint8_t)(vid & 0xFF),
      (uint8_t)((vid >> 8) & 0xFF),
      (uint8_t)(pid & 0xFF),
      (uint8_t)((pid >> 8) & 0xFF),
      (uint8_t)(bcd & 0xFF),
      (uint8_t)((bcd >> 8) & 0xFF),
      USBD_IDX_MFC_STR,
      USBD_IDX_PRODUCT_STR,
      USBD_IDX_SERIAL_STR,
      0x01 // bNumConfigurations
  };
  memcpy(hmidi->descriptors.dev_desc, dev_temp, USB_LEN_DEV_DESC);

  hmidi->descriptors.lang_id_desc[0] = 4;
  hmidi->descriptors.lang_id_desc[1] = USB_DESC_TYPE_STRING;
  hmidi->descriptors.lang_id_desc[2] = (uint8_t)(lang & 0xFF);
  hmidi->descriptors.lang_id_desc[3] = (uint8_t)((lang >> 8) & 0xFF);

  USB_BuildStringDesc(hmidi->descriptors.manufacturer_desc, cfg->manufacturer);
  USB_BuildStringDesc(hmidi->descriptors.product_desc, cfg->product);
  USB_BuildStringDesc(hmidi->descriptors.serial_desc, cfg->serial);

  /* Per-port jack name strings (empty string = jack stays unnamed). */
  for (size_t i = 0; i < 8; i++) {
    const char *pname = (cfg->port_names[i] != NULL) ? cfg->port_names[i] : "";
    USB_BuildStringDesc(hmidi->descriptors.jack_desc[i], pname);
  }
}

typedef struct usb_setup_req {
  uint8_t bmRequest;
  uint8_t bRequest;
  uint16_t wValue;
  uint16_t wIndex;
  uint16_t wLength;
} USBD_SetupReqTypedef;

static uint8_t USBrxBuffer[MIDI_EPOUT_SIZE];

static uint8_t usb_setup_buffer[MIDI_EPOUT_SIZE];

static void USB_PartitionPeripheralBuffer(PCD_HandleTypeDef *hpcd) {

#if defined(USB) || defined(USB_DRD_FS)
  HAL_PCDEx_PMAConfig(hpcd, 0x00, PCD_SNG_BUF, 0x18);
  HAL_PCDEx_PMAConfig(hpcd, 0x80, PCD_SNG_BUF, 0x58);
  HAL_PCDEx_PMAConfig(hpcd, MIDI_EPOUT_ADDRESS, PCD_SNG_BUF, 0x98);
  HAL_PCDEx_PMAConfig(hpcd, MIDI_EPIN_ADDRESS, PCD_SNG_BUF, 0xD8);

#elif defined(USB_OTG_FS) || defined(USB_OTG_HS)
  HAL_PCDEx_SetRxFiFo(hpcd, 0x80);
  HAL_PCDEx_SetTxFiFo(hpcd, 0, 0x40); // EP0
  HAL_PCDEx_SetTxFiFo(hpcd, 1, 0x40); // MIDI IN
#else
#error "No supported USB peripheral found"
#endif
}

HAL_StatusTypeDef init_MIDI_device(USBD_MIDI_HandleTypeDef *hmidi,
                                   PCD_HandleTypeDef *hpcd) {

  if (hmidi->descriptors.dev_desc[0] == 0) {
    const USB_DeviceConfig_t usb_midi_cfg = {.vendor_id = 0x1209,
                                             .product_id = 0x0001,
                                             .bcd_device = 0x0100,
                                             .manufacturer = "neo",
                                             .product = "neo synthesizer",
                                             .serial = "0001",
                                             .lang_id = 0x0409};
    USBD_MIDI_InitDescriptors(hmidi, &usb_midi_cfg);
  }

  hmidi->hpcd = hpcd;
  hmidi->hpcd->pData = hmidi;
  hmidi->Protocol = 0;
  hmidi->IdleState = 0;
  hmidi->AltSetting = 0;
  hmidi->midi_state = MIDI_IDLE;
  hmidi->device_state = USBD_STATE_DEFAULT;

  USB_PartitionPeripheralBuffer(hpcd);

  HAL_PCD_Start(hpcd);

  return HAL_OK;
}

static uint8_t *cfg_desc_ptr = NULL;
static uint16_t cfg_desc_len = 0;
static uint16_t cfg_desc_sent = 0;

static uint8_t usbmidi_decode_packet(const uint8_t pkt[4], MIDI_Message *out) {
  const uint8_t cin = pkt[0] & 0x0F;
  const uint8_t status = pkt[1];

  out->channel = status & 0x0F;

  switch (cin) {
  case 0x2:
    if (status == MIDI_MTC_QUARTER_FRAME) {
      out->type = MIDI_MTC_QUARTER_FRAME;
      out->data.quarterFrame.messageType = (pkt[2] >> 4) & 0x07;
      out->data.quarterFrame.value = pkt[2] & 0x0F;
      return pkt[0] >> 4;
    }
    if (status == MIDI_SONG_SELECT) {
      out->type = MIDI_SONG_SELECT;
      out->data.songSelect.song = pkt[2] & 0x7F;
      return pkt[0] >> 4;
    }
    return 0xFF;
  case 0x3:
    if (status == MIDI_SONG_POSITION) {
      out->type = MIDI_SONG_POSITION;
      out->data.songPosition.position =
          (uint16_t)(pkt[2] & 0x7F) | ((uint16_t)(pkt[3] & 0x7F) << 7);
      return pkt[0] >> 4;
    }
    return 0xFF;
  case 0x5:
    if (status == MIDI_TUNE_REQUEST) {
      out->type = MIDI_TUNE_REQUEST;
      return pkt[0] >> 4;
    }
    return 0xFF;
  case 0x6:
    switch (status & 0xF0) {
    case MIDI_PROGRAM_CHANGE:
      out->type = MIDI_PROGRAM_CHANGE;
      out->data.program.program = pkt[2] & 0x7F;
      return pkt[0] >> 4;
    case MIDI_CHANNEL_PRESSURE:
      out->type = MIDI_CHANNEL_PRESSURE;
      out->data.channelPressure.pressure = pkt[2] & 0x7F;
      return pkt[0] >> 4;
    default:
      return 0xFF;
    }
  case 0x7:
  case 0x8:
  case 0x9:
  case 0xA:
  case 0xB:
  case 0xE:
    switch (status & 0xF0) {
    case MIDI_NOTE_OFF:
      out->type = MIDI_NOTE_OFF;
      out->data.note.note = pkt[2] & 0x7F;
      out->data.note.velocity = pkt[3] & 0x7F;
      return pkt[0] >> 4;
    case MIDI_NOTE_ON:
      out->type = MIDI_NOTE_ON;
      out->data.note.note = pkt[2] & 0x7F;
      out->data.note.velocity = pkt[3] & 0x7F;
      if (out->data.note.velocity == 0)
        out->type = MIDI_NOTE_OFF;
      return pkt[0] >> 4;
    case MIDI_POLY_KEY_PRESSURE:
      out->type = MIDI_POLY_KEY_PRESSURE;
      out->data.keyPressure.note = pkt[2] & 0x7F;
      out->data.keyPressure.pressure = pkt[3] & 0x7F;
      return pkt[0] >> 4;
    case MIDI_CONTROL_CHANGE:
      out->type = MIDI_CONTROL_CHANGE;
      out->data.control.controller = pkt[2] & 0x7F;
      out->data.control.value = pkt[3] & 0x7F;
      return pkt[0] >> 4;
    case MIDI_PITCH_BEND:
      out->type = MIDI_PITCH_BEND;
      out->data.pitchBend.value =
          (uint16_t)(pkt[2] & 0x7F) | ((uint16_t)(pkt[3] & 0x7F) << 7);
      return pkt[0] >> 4;
    default:
      return 0xFF;
    }
  case 0xF:
    if (status >= MIDI_TIMING_CLOCK) {
      out->type = (MIDI_MessageType)status;
      return pkt[0] >> 4;
    }
    return 0xFF;
  default:
    return 0xFF;
  }
}

__weak void on_midi_message(uint8_t cable, const MIDI_Message *message);

static uint8_t usb_rx_buffer[MIDI_EPOUT_SIZE] = {0};

void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum) {
  if (epnum == 0) {

    HAL_PCD_EP_Receive(hpcd, 0x00, (uint8_t *)hpcd->Setup, 8);

  } else if (epnum == (MIDI_EPOUT_ADDRESS & 0x7F)) {

    uint16_t count = (uint16_t)hpcd->OUT_ep[epnum].xfer_count;
    uint16_t i;

    for (i = 0; (uint16_t)(i + 4) <= count; i += 4) {
      MIDI_Message msg;
      uint8_t cable = usbmidi_decode_packet(&usb_rx_buffer[i], &msg);
      if (cable != 0xFF && on_midi_message != 0)
        on_midi_message(cable, &msg);
    }

    HAL_PCD_EP_Receive(hpcd, MIDI_EPOUT_ADDRESS, usb_rx_buffer,
                       MIDI_EPOUT_SIZE);
  }
}

void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum) {
  USBD_MIDI_HandleTypeDef *hmidi = (USBD_MIDI_HandleTypeDef *)hpcd->pData;

  if (epnum == 0) {
    if (cfg_desc_sent < cfg_desc_len) {

      uint16_t remaining = cfg_desc_len - cfg_desc_sent;
      uint16_t send_len = (remaining > 64) ? 64 : remaining;

      HAL_PCD_EP_Transmit(hpcd, 0x80, cfg_desc_ptr + cfg_desc_sent, send_len);
      cfg_desc_sent += send_len;

    } else {

      HAL_PCD_EP_Receive(hpcd, 0x00, (uint8_t *)hpcd->Setup, 8);
    }

  } else if (epnum == (MIDI_EPIN_ADDRESS & 0x7F)) {

    if (hmidi) {
      hmidi->midi_state = MIDI_IDLE;
    }
  }
}

void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd) {

  HAL_PCD_EP_Open(hpcd, 0x00, 64, EP_TYPE_CTRL);
  HAL_PCD_EP_Open(hpcd, 0x80, 64, EP_TYPE_CTRL);

  HAL_PCD_EP_Receive(hpcd, 0x00, (uint8_t *)hpcd->Setup, 8);
}
void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd) {

  USBD_SetupReqTypedef *req = (USBD_SetupReqTypedef *)hpcd->Setup;
  USBD_MIDI_HandleTypeDef *hmidi = (USBD_MIDI_HandleTypeDef *)hpcd->pData;

  if (!hmidi) {
    return;
  }

  if ((req->bmRequest & 0x60) != 0x00) {
    return;
  }

  switch (req->bRequest) {
  case USB_REQ_GET_DESCRIPTOR: {
    uint8_t type = req->wValue >> 8;

    if (type == USB_DESC_TYPE_CONFIGURATION) {
      cfg_desc_ptr = (uint8_t *)cfg_desc;
      cfg_desc_len = sizeof(cfg_desc);
      if (cfg_desc_len > req->wLength) {
        cfg_desc_len = req->wLength;
      }

      cfg_desc_sent = 0;

      uint16_t send_len = (cfg_desc_len > 64) ? 64 : cfg_desc_len;
      HAL_PCD_EP_Transmit(hpcd, 0x80, cfg_desc_ptr, send_len);
      cfg_desc_sent += send_len;

    } else if (type == USB_DESC_TYPE_DEVICE) {

      uint16_t len = sizeof(hmidi->descriptors.dev_desc);

      if (len > req->wLength) {
        len = req->wLength;
      }

      HAL_PCD_EP_Transmit(hpcd, 0x80, hmidi->descriptors.dev_desc, len);

    } else if (type == USB_DESC_TYPE_DEVICE_QUALIFIER) {

      uint16_t len = sizeof(dev_qual);

      if (len > req->wLength) {
        len = req->wLength;
      }

      HAL_PCD_EP_Transmit(hpcd, 0x80, (uint8_t *)dev_qual, len);

    } else if (type == USB_DESC_TYPE_STRING) {
      uint8_t index = req->wValue & 0xFF;
      uint8_t *pbuf = NULL;
      uint16_t len = 0;

      switch (index) {
      case 0:
        pbuf = hmidi->descriptors.lang_id_desc;
        len = hmidi->descriptors.lang_id_desc[0];
        break;
      case USBD_IDX_MFC_STR:
        pbuf = hmidi->descriptors.manufacturer_desc;
        len = hmidi->descriptors.manufacturer_desc[0];
        break;
      case USBD_IDX_PRODUCT_STR:
        pbuf = hmidi->descriptors.product_desc;
        len = hmidi->descriptors.product_desc[0];
        break;
      case USBD_IDX_SERIAL_STR:
        pbuf = hmidi->descriptors.serial_desc;
        len = hmidi->descriptors.serial_desc[0];
        break;
      default:
        if (index >= USBD_IDX_JACK_STR_BASE &&
            index < USBD_IDX_JACK_STR_BASE + MIDI_JACK_NAME_COUNT) {
          uint8_t jack_idx = (uint8_t)(index - USBD_IDX_JACK_STR_BASE);
          pbuf = hmidi->descriptors.jack_desc[jack_idx];
          len = hmidi->descriptors.jack_desc[jack_idx][0];
        } else {
          HAL_PCD_EP_SetStall(hpcd, 0x80);
          return;
        }
        break;
      }

      if (len > req->wLength) {
        len = req->wLength;
      }

      HAL_PCD_EP_Transmit(hpcd, 0x80, pbuf, len);

    } else {
      HAL_PCD_EP_SetStall(hpcd, 0x80);
    }
    break;
  }

  case USB_REQ_SET_ADDRESS: {
    HAL_PCD_EP_Transmit(hpcd, 0x80, NULL, 0);
    hpcd->USB_Address = (uint8_t)(req->wValue & 0x7F);
    HAL_PCD_SetAddress(hpcd, hpcd->USB_Address);
    break;
  }

  case USB_REQ_SET_CONFIGURATION: {
    HAL_PCD_EP_Transmit(hpcd, 0x80, NULL, 0);

    hmidi->device_state = USBD_STATE_CONFIGURED;

    HAL_PCD_EP_Open(hpcd, MIDI_EPOUT_ADDRESS, MIDI_EPOUT_SIZE, EP_TYPE_BULK);
    HAL_PCD_EP_Open(hpcd, MIDI_EPIN_ADDRESS, MIDI_EPIN_SIZE, EP_TYPE_BULK);

    HAL_PCD_EP_Receive(hpcd, MIDI_EPOUT_ADDRESS, usb_rx_buffer,
                       MIDI_EPOUT_SIZE);
    break;
  }

  default:
    HAL_PCD_EP_SetStall(hpcd, 0x80);
    break;
  }
}

void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd) {
  USBD_MIDI_HandleTypeDef *midi = (USBD_MIDI_HandleTypeDef *)hpcd->pData;
  midi->device_state = USBD_STATE_SUSPENDED;
}

void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd) {
  USBD_MIDI_HandleTypeDef *midi = (USBD_MIDI_HandleTypeDef *)hpcd->pData;
  midi->device_state = USBD_STATE_CONFIGURED;
}

void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd) {
  USBD_MIDI_HandleTypeDef *midi = (USBD_MIDI_HandleTypeDef *)hpcd->pData;
  midi->device_state = USBD_STATE_ADDRESSED;
}

void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd) {
  USBD_MIDI_HandleTypeDef *midi = (USBD_MIDI_HandleTypeDef *)hpcd->pData;
  midi->device_state = USBD_STATE_DEFAULT;
}

static uint8_t usb_cin(const MIDI_Message *m) {
  switch (m->type) {
  case MIDI_MTC_QUARTER_FRAME:
  case MIDI_SONG_SELECT:
    return 0x2;
  case MIDI_SONG_POSITION:
    return 0x3;
  case MIDI_TUNE_REQUEST:
    return 0x5;
  case MIDI_TIMING_CLOCK:
  case MIDI_START:
  case MIDI_CONTINUE:
  case MIDI_STOP:
  case MIDI_ACTIVE_SENSING:
  case MIDI_SYSTEM_RESET:
    return 0xF;
  default:
    return (uint8_t)m->type >> 4;
  }
}

USBD_StatusTypeDef send_midi_message_to(USBD_MIDI_HandleTypeDef *hmidi,
                                        const MIDI_Message *message,
                                        uint8_t cable) {
  uint8_t midi[3], buf[4];
  size_t n;

  if (hmidi->device_state != USBD_STATE_CONFIGURED)
    return USBD_FAIL;

  if (cable >= MIDI_IN_PORTS_NUM)
    return USBD_FAIL;

  n = encode_midi(message, midi, sizeof midi);

  if (n == 0)
    return USBD_FAIL;

  buf[0] = (uint8_t)((cable << 4) | usb_cin(message));
  buf[1] = midi[0];
  buf[2] = (n > 1) ? midi[1] : 0;
  buf[3] = (n > 2) ? midi[2] : 0;

  hmidi->midi_state = MIDI_BUSY;

  if (HAL_PCD_EP_Transmit(hmidi->hpcd, MIDI_EPIN_ADDRESS, buf, 4) != HAL_OK) {
    hmidi->midi_state = MIDI_IDLE;
    return USBD_FAIL;
  }
  return USBD_OK;
}

USBD_StatusTypeDef send_midi_message(USBD_MIDI_HandleTypeDef *hmidi,
                                     const MIDI_Message *message) {
  return send_midi_message_to(hmidi, message, 0);
}

#ifdef __cplusplus
}
#endif
