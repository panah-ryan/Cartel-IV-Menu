#pragma once
#include "stdafx.h"

HRESULT HvPeekBytes(QWORD address, void* buffer, int size);
HRESULT HvExpansionInstall();
QWORD HvPeekQWORD(QWORD Address);
QWORD HvGetFuseLine(BYTE fuseIndex);