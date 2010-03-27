#ifndef	__DEVICE_H__
#define	__DEVICE_H__

#pragma once

//*****************************************************************************
//	eDevice
//-----------------------------------------------------------------------------
class eDevice
{
public:
	eDevice() : enabled(true) {}
	virtual ~eDevice() {}
	void Enable(bool e) { enabled = e; }

	virtual void Init() {}
	virtual void Reset() {}
	virtual void FrameStart() {}
	virtual void FrameUpdate() {}
	virtual void FrameEnd(dword tacts) {}

	virtual void IoRead(word port, byte* v, int tact) {}
	virtual void IoWrite(word port, byte v, int tact) {}
protected:
	bool	enabled;
};

enum eDeviceId { D_ROM, D_RAM, D_ULA, D_KEYBOARD, D_KEMPSTON_JOY, D_KEMPSTON_MOUSE, D_BEEPER, D_AY, D_WD1793, D_TAPE, D_COUNT };

//*****************************************************************************
//	eDevices
//-----------------------------------------------------------------------------
class eDevices
{
public:
	eDevices();
	~eDevices();

	void Reset();

	template<class T> void Add(T* d) { _Add(T::Id(), d); }
	template<class T> T* Get() const { return (T*)_Get(T::Id()); }

	byte IoRead(word port, int tact) const;
	void IoWrite(word port, byte v, int tact);

	void FrameStart();
	void FrameUpdate();
	void FrameEnd(dword tacts);

protected:
	void _Add(eDeviceId id, eDevice* d);
	eDevice* _Get(eDeviceId id) const { return items[id]; }
	eDevice* items[D_COUNT];
};

#endif//__DEVICE_H__