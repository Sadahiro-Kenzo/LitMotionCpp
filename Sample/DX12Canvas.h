#pragma once
#include <string>
#include <vector>
#include <ranges>
#include "ICanvas.h"
#include <DirectXMath.h>

using namespace DirectX;

namespace LitMotionCpp::Sample
{
	class DX12TextLabel : public ITextLabel
	{
	private:
		std::string m_text;
		XMFLOAT2 m_position;
		XMFLOAT4 m_color;
		HorizontalAlign m_hAlign;
		VerticalAlign m_vAlign;
		Point2f m_pivot;
		Point2f m_size;
	public:
		DX12TextLabel() = delete;
		DX12TextLabel(TextLabelSpec&);
		virtual ~DX12TextLabel() = default;
		virtual void SetText(const char* text) override { m_text = text; }
		virtual const char* GetText() const override { return m_text.c_str(); }
		virtual void SetX(const float x) override { m_position.x = x; }
		virtual void SetY(const float y) override { m_position.y = y; }
		virtual void SetPosition(const float x, const float y) override { m_position.x = x; m_position.y = y; }
		virtual void SetColor(const Color4f& color) override { m_color.x = color.x; m_color.y = color.y; m_color.z = color.z; m_color.w = color.w; }
		virtual void SetHorizontalAlign(HorizontalAlign hAlign) override { m_hAlign = hAlign; }
		virtual void SetVerticalAlign(VerticalAlign vAlign) override { m_vAlign = vAlign; }
		virtual void SetPivotX(const float x) override { m_pivot.x = x; }
		virtual void SetPivotY(const float y) override { m_pivot.y = y; }
		virtual void SetPivot(const float x, const float y) override { m_pivot.x = x; m_pivot.y = y; }
		virtual float GetX() const override { return m_position.x; }
		virtual float GetY() const override { return m_position.y; }
		virtual Point2f GetPosition() const override { return Point2f{ m_position.x, m_position.y }; }
		XMFLOAT2 DX12GetPosition() const { return m_position; }
		const XMFLOAT4* DX12GetColor() const { return &m_color; }
		virtual HorizontalAlign GetHorizontalAlign() const override { return m_hAlign; }
		virtual VerticalAlign GetVerticalAlign() const override { return m_vAlign; }
		virtual float GetPivotX() const override { return m_pivot.x; }
		virtual float GetPivotY() const override { return m_pivot.y; }
		virtual float GetWidth() const override { return m_size.x; }
		virtual float GetHeight() const override { return m_size.y; }
	};

	class DX12Sprite :public ISprite
	{
	private:
		XMFLOAT2 m_position;
		XMFLOAT4 m_color;
	public:
		DX12Sprite() = default;
		virtual ~DX12Sprite() = default;
		virtual void SetX(const float x) override { m_position.x = x; }
		virtual void SetY(const float y) override { m_position.y = y; }
		virtual void SetPosition(const float x, const float y) override { m_position.x = x; m_position.y = y; }
		virtual float GetX() const override { return m_position.x; }
		virtual float GetY() const override { return m_position.y; }
		virtual Point2f GetPosition() const override { return Point2f{ m_position.x, m_position.y }; }
		XMFLOAT2 DX12GetPosition() const { return m_position; }
		virtual void SetColor(const Color4f& color) override { m_color.x=color.x; m_color.y = color.y; m_color.z = color.z; m_color.w = color.w;}
		const XMFLOAT4* DX12GetColor() const { return &m_color; }
	};

	class DX12Canvas : public ICanvas
	{
	private:
		std::vector<std::shared_ptr<DX12TextLabel>> m_textLabels;
		std::vector<std::shared_ptr<DX12Sprite>> m_sprites;
	public:
		DX12Canvas() = default;
		virtual ~DX12Canvas() = default;
		// ICanvas ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
		std::weak_ptr<ITextLabel> PushBackTextLabel(TextLabelSpec&) override;
		std::weak_ptr<ISprite> PushBackSprite() override;
		void Clear() override;

		const auto GetTextLabels() const { return m_textLabels | std::views::all; }
		const auto GetSprites() const { return m_sprites | std::views::all; }
	};

}// namespace LitMotionCpp::Sample

