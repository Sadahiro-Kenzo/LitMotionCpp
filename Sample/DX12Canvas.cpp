#include "pch.h"
#include "DX12Canvas.h"

namespace LitMotionCpp::Sample
{
    DX12TextLabel::DX12TextLabel(TextLabelSpec& spec)
        :m_text(spec.text)
        , m_position{ spec.position.x,spec.position.y }
        , m_color{spec.color.x,spec.color.y,spec.color.z,spec.color.w}
        , m_hAlign(spec.hAlign)
        , m_vAlign(spec.vAlign)
        , m_pivot(spec.pivot)
		, m_size(spec.size)
    {

    }

    std::weak_ptr<ITextLabel> LitMotionCpp::Sample::DX12Canvas::PushBackTextLabel(TextLabelSpec& spec)
    {
        return m_textLabels.emplace_back(std::make_shared<DX12TextLabel>(spec));
    }

    std::weak_ptr<ISprite> LitMotionCpp::Sample::DX12Canvas::PushBackSprite()
    {
		return m_sprites.emplace_back(std::make_shared<DX12Sprite>());
    }

    void LitMotionCpp::Sample::DX12Canvas::Clear()
    {
        m_textLabels.clear();
		m_sprites.clear();
    }

}// namespace LitMotionCpp::Sample

