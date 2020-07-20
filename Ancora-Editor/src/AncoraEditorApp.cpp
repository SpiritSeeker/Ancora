#include <Ancora.h>
#include <Ancora/Core/EntryPoint.h>

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"

namespace Ancora {

	class AncoraEditor : public Application
	{
	public:
		AncoraEditor()
			: Application("Ancora Editor")
		{
			PushLayer(new EditorLayer());
		}

		~AncoraEditor()
		{
		}

	};

	Application* CreateApplication()
	{
		return new AncoraEditor();
	}

}
