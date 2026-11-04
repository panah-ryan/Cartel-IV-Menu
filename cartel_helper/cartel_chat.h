#pragma once
#include "types.h"
#include "constants.h"
#include "intrinsics.h"
#include "common.h"
#include "cartel_helper.h"


#define MAX_DISPLAY_MESSAGES 8
#define MAX_CHAT_MESSAGES 16
#define MESSAGE_TIMEOUT 8000
//#define JOINING_MESSAGE "~b~has joined the chat." //use CHAT_JOINING_MESSAGE defined in cartels functions.h

void cartel_chat_display();