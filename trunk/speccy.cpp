#include "std.h"

#include "speccy.h"
#include "devices/device.h"
#include "z80/z80.h"
#include "devices/memory.h"
#include "devices/ula.h"
#include "devices/input/keyboard.h"
#include "devices/input/kempston_joy.h"
#include "devices/input/kempston_mouse.h"
#include "devices/input/tape.h"
#include "devices/sound/beeper.h"
#include "devices/sound/ay.h"
#include "devices/fdd/wd1793.h"

//=============================================================================
//	eSpeccy::eSpeccy
//-----------------------------------------------------------------------------
eSpeccy::eSpeccy() : cpu(NULL), memory(NULL), frame_tacts(0)
	, int_len(0), nmi_pending(0), t_states(0)
{
	// pentagon timings
	frame_tacts = 71680;
	int_len = 32;

	memory = new eMemory;
	devices.Add(new eRom(memory));
	devices.Add(new eRam(memory));
	devices.Add(new eUla(memory));
	devices.Add(new eKeyboard);
	devices.Add(new eKempstonJoy);
	devices.Add(new eKempstonMouse);
	devices.Add(new eBeeper);
	devices.Add(new eAY);
	devices.Add(new eWD1793(this, Device<eRom>()));
	devices.Add(new eTape(this));
	cpu = new xZ80::eZ80(memory, &devices, frame_tacts);

	Reset();
}
//=============================================================================
//	eSpeccy::~eSpeccy
//-----------------------------------------------------------------------------
eSpeccy::~eSpeccy()
{
	delete cpu;
	delete memory;
}
//=============================================================================
//	eSpeccy::Reset
//-----------------------------------------------------------------------------
void eSpeccy::Reset()
{
	cpu->Reset();
	devices.Reset();
}
//=============================================================================
//	eSpeccy::Update
//-----------------------------------------------------------------------------
void eSpeccy::Update()
{
	devices.FrameStart();
	cpu->Update(int_len, &nmi_pending);
	dword t = cpu->FrameTacts() + cpu->T();
	devices.FrameUpdate();
	devices.FrameEnd(t);
	t_states += cpu->FrameTacts();
}