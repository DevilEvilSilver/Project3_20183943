//#include "pch.h"
#include "EditorLayer.h"

#include <imgui.h>

namespace Silver {

	EditorLayer::EditorLayer()
		:Layer("EditorLayer")
        //, m_EditorCameraController(16.0f / 9.0f)
	{
        m_SceneHierarchyPanel = std::make_shared<SceneHierarchyPanel>();
	}

	void EditorLayer::OnAttach()
	{
		FramebufferSpec spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = std::make_shared<Framebuffer>(spec);

        m_Scene = std::make_shared<Scene>();

        m_Entity = m_Scene->CreateEntity("3D Model Entity");
        m_Entity->AddComponent<AnimatedModelComponent>(std::make_shared<AnimatedModel>("assets/models/originAnimModel.dae"));
        //m_Entity->AddComponent<AnimatedModelComponent>(std::make_shared<AnimatedModel>("assets/models/AnimModel.dae"))
        //    .BindAnimation("action_container-Armature");
        m_Entity->AddComponent<Texture2DComponent>(std::make_shared<Texture2D>("assets/textures/animTexture.png"));
        m_Entity->AddComponent<ShaderComponent>(std::make_shared<Shader>("assets/shaders/AnimModel.glsl"));

        m_CameraEntity = m_Scene->CreateEntity("Camera Entity");
        auto& cam = m_CameraEntity->AddComponent<CameraComponent>(std::make_shared<CameraLookAt>(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f));
        cam.Primary = true;
        cam.FixedAspectRatio = true;

        class CameraController : public ScriptableEntity
        {
        public:
            CameraController(const std::shared_ptr<Entity>& entity)
                :ScriptableEntity(entity) {}

            void OnAttach() override
            {
                m_Camera = std::static_pointer_cast<CameraLookAt>(GetComponent<CameraComponent>().m_Camera);
            }

            void OnDetach() override
            {

            }

            void OnUpdate(float deltaTime) override
            {
                if (Input::IsKeyPressed(KEY_W))
                    m_Camera->RotationUp(m_CameraRotationSpeed * deltaTime);
                else if (Input::IsKeyPressed(KEY_S))
                    m_Camera->RotationDown(m_CameraRotationSpeed * deltaTime);
                if (Input::IsKeyPressed(KEY_A))
                    m_Camera->RotationLeft(m_CameraRotationSpeed * deltaTime);
                else if (Input::IsKeyPressed(KEY_D))
                    m_Camera->RotationRight(m_CameraRotationSpeed * deltaTime);
            }

            void OnEvent(Event& e) override
            {
                EventDispatcher dispatcher(e);
                dispatcher.Dispatch<MouseScrolledEvent>(BIND_FN(CameraController::OnMouseScrolled));
            }

            bool OnMouseScrolled(MouseScrolledEvent& e)
            {
                m_Camera->MoveForward(m_CameraZoomSpeed * e.GetYOffset());
                return false;
            }

        private:
            std::shared_ptr<CameraLookAt> m_Camera;
            float m_CameraRotationSpeed = 90.0f;
            float m_CameraZoomSpeed = 0.5f;
        };
        m_CameraEntity->AddComponent<ScriptComponent>().Bind(std::make_shared<CameraController>(m_CameraEntity));

        m_SceneHierarchyPanel->SetContext(m_Scene);
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(float deltaTime)
	{
        // Resize
        if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
            (m_Framebuffer->GetSpecification().Width != m_ViewportSize.x || m_Framebuffer->GetSpecification().Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((unsigned int)m_ViewportSize.x, (unsigned int)m_ViewportSize.y);
            //m_EditorCameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

            m_Scene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
        }

        //Update
        //if (m_ViewportFocused)
            //m_EditorCameraController.OnUpdate(deltaTime);

        // Render
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		RenderCommand::Clear();

        //Update Scene
        m_Scene->OnUpdate(deltaTime);

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		static bool dockspaceOpen = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);

				if (ImGui::MenuItem("Exit")) { Application::GetInstance().Close(); }

                ImGui::EndMenu();
            }


            ImGui::EndMenuBar();
        }
        ImGui::End();

        // View port
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("View port");

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::GetInstance().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		
		ImTextureID textureID = (void*)(uint64_t)m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2{ 0,1 }, ImVec2{ 1,0 }); // extra param for ImGui weird behavior with uv
		ImGui::End();
		ImGui::PopStyleVar();

        // DEMO panel
        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        // Scene Hierarchy Panel
        m_SceneHierarchyPanel->OnImGuiRender();
	}

	void EditorLayer::OnEvent(Event& e)
	{
        //m_EditorCameraController.OnEvent(e);

        // Update Scene
        m_Scene->OnEvent(e);
	}

}