#pragma once
#include <memory>
#include <vecmath.h>

namespace LitMotionCpp::Sample
{
	enum class HorizontalAlign
	{
		Left,
		Center,
		Right
	};

	enum class VerticalAlign
	{
		Top,
		Middle,
		Bottom
	};

	struct TextLabelSpec
	{
		const char* text = "";
		Color4f color = { 0.0f,0.0f,0.0f,1.0f };
		Point2f position = { 0.0f, 0.0f };
		HorizontalAlign hAlign = HorizontalAlign::Left;
		VerticalAlign vAlign = VerticalAlign::Top;
		Point2f pivot = { 0.5f, 0.5f };
		Point2f size = { 800.0f,50.0f };
	};

	class ITextLabel
	{
	public:
		ITextLabel() = default;
		virtual ~ITextLabel() = default;
		virtual void SetText(const char* text) = 0;
		virtual const char* GetText() const = 0;
		virtual void SetColor(const Color4f& color) = 0; 
		virtual void SetX(const float x) = 0;
		virtual void SetY(const float y) =0;
		virtual void SetPosition(const float x, const float y) = 0;
		void SetPosition(const Point2f& p) { SetPosition(p.x, p.y); }
		virtual void SetHorizontalAlign(HorizontalAlign hAlign) = 0;
		virtual void SetVerticalAlign(VerticalAlign vAlign) = 0;
		virtual void SetPivotX(const float x) = 0;
		virtual void SetPivotY(const float y) = 0;
		virtual void SetPivot(const float x, const float y) = 0;
		void SetPivot(const Point2f& p) { SetPivot(p.x, p.y); }
		virtual float GetX() const = 0;
		virtual float GetY() const = 0;
		virtual Point2f GetPosition() const = 0;
		virtual HorizontalAlign GetHorizontalAlign() const = 0;
		virtual VerticalAlign GetVerticalAlign() const = 0;
		virtual float GetPivotX() const = 0;
		virtual float GetPivotY() const = 0;
		virtual float GetWidth() const = 0;
		virtual float GetHeight() const = 0;
	};

	class ISprite
	{
	public:
		ISprite() = default;
		virtual ~ISprite() = default;
		virtual void SetX(float x) = 0;
		virtual void SetY(float y) = 0;
		virtual void SetPosition(float x, float y) = 0;
		virtual float GetX() const = 0;
		virtual float GetY() const = 0;
		virtual Point2f GetPosition() const = 0;
		virtual void SetColor(const Color4f& color) = 0;
	};

	class ICanvas
	{
	public:
		ICanvas() = default;
		virtual ~ICanvas() = default;
		virtual std::weak_ptr<ITextLabel> PushBackTextLabel(TextLabelSpec&) = 0;
		virtual std::weak_ptr<ISprite> PushBackSprite() = 0;
		virtual void Clear() = 0;
	};

}// namespace LitMotionCpp::Sample
