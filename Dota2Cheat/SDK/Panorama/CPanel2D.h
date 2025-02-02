#pragma once
#include "../Base/VClass.h"
#include "../Base/Vector.h"
#include "../Base/Definitions.h"
#include "CSource2UITexture.h"

#include "CUIPanel.h"

// CPanel2D and its descendants

class CPanel2D : public VClass {
public:
	struct CImageData : public VClass {
		GETTER(CSource2UITexture*, GetTexture, 0x30);
	};
	struct CImageProxySource : public VClass {
		GETTER(CImageData*, GetImageData, 0x10);
	};

	Vector2D GetPositionWithinWindow() const {
		Vector2D result{ 0, 0 };
		GetVFunc(VMI::CPanel2D::GetPositionWithinWindow)(0, &result.x, &result.y);
		return result;
	}

	GETTER(CUIPanel*, GetUIPanel, 0x8);
	GETTER(CImageProxySource*, GetImageProxySource, 0xC8);
};

class CDOTA_UI_HeroImage : public CPanel2D {
public:
	GETTER(const char*, GetSrc, 0x98);
	GETTER(int, GetHeroID, 0xDC); // xref: "heroimagestyle", lesser offset
};

// discovered by me and rebuilt by Morphling
class CDOTA_Hud_ErrorMsg : public CPanel2D {
	PAD(0x30);
public:
	float m_flTotalTime; // 0x38
	float m_flErrorDurationTime; // 0x3c

	void SetDialogVariable(const char* variable, const char* value) {
		GetUIPanel()->CallVFunc<293>(variable, value);
	}

	void ShowErrorMessage(const char* text) {
		auto panel = GetUIPanel();

		panel->RemoveClass("PopOutEffect");
		panel->ApplyStyles();
		panel->AddClass("ShowErrorMsg");

		panel->RemoveClass("Hidden");
		SetDialogVariable("error_msg", text);
		panel->AddClass("PopOutEffect");

		m_flTotalTime = Plat_FloatTime() + m_flErrorDurationTime;
	}
};