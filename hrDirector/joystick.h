#pragma once

struct XINPUT_DEVICE_NODE
{
	DWORD dwVidPid;
	XINPUT_DEVICE_NODE* pNext;
};

struct DI_ENUM_CONTEXT
{
	DIJOYCONFIG* pPreferredJoyCfg;
	bool bPreferredJoyCfgValid;
};


class CJoystickDevice
{
public:
	CJoystickDevice();
	~CJoystickDevice();
public:
	static BOOL CALLBACK    EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);
	static BOOL CALLBACK    EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
public:
	HRESULT InitDirectInput(HWND hWnd);
	void FreeDirectInput();
	HRESULT UpdateInputState(DIJOYSTATE2& js);

private:
	HRESULT SetupForIsXInputDevice();
	static bool IsXInputDevice(const GUID* pGuidProductFromDirectInput);
	void CleanupForIsXInputDevice();

protected:
	static bool                    g_bFilterOutXinputDevices;
	static XINPUT_DEVICE_NODE*     g_pXInputDeviceList;

protected:
	static LPDIRECTINPUT8          g_pDI;
	static LPDIRECTINPUTDEVICE8    g_pJoystick;
};