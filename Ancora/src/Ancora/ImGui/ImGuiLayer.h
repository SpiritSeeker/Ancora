#pragma once

#include "Ancora/Core/Layer.h"

#include "Ancora/Events/KeyEvent.h"
#include "Ancora/Events/MouseEvent.h"
#include "Ancora/Events/ApplicationEvent.h"

namespace Ancora {

	class ANCORA_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};

}
